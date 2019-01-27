#include "StdAfx.h"
#include "PIN.h"
#include "HTMLCache.h"
#include "DB.h"
#include "AccountDB.h"
#include "CliExt.h"
#include "License.h"


using namespace License;

CPINSystem g_PINSystem;

CPINSystem::CPINSystem() : m_Enabled(FALSE)
{
}

CPINSystem::~CPINSystem()
{
}

#pragma optimize("", off)
void CPINSystem::Init()
{
	VIRTUALIZER_TIGER_BLACK_START;
	if(CLicense::GetInstance().CanUse(LicenseHtmlPIN))
	{
		m_Enabled = FALSE;
		LoadINI();
		if(m_Enabled)
		{
			g_Log.Add(CLog::Blue, "[%s] System enabled.", __FUNCTION__);
			g_DB.RequestLoadCharacterPINS();
		}
	}
	VIRTUALIZER_TIGER_BLACK_END;
}

/*
[PINSystem]
Enabled=0
;How many times player can enter invalid PIN before he get ban
AttemptLimit=3
;HWID ban time in seconds
BanTime=900
*/

void CPINSystem::LoadINI()
{
	VIRTUALIZER_TIGER_BLACK_START;
	const TCHAR* section = _T("PINSystem");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), FALSE, g_ConfigFile);
	m_AttemptLimit = GetPrivateProfileInt(section, _T("AttemptLimit"), 3, g_ConfigFile);
	m_BanTime = GetPrivateProfileInt(section, _T("BanTime"), 900, g_ConfigFile);
	m_UserCommand = GetPrivateProfileInt(section, _T("UserCommand"), 0, g_ConfigFile);
	m_ForceChangeTime = GetPrivateProfileInt(section, _T("ForceChangeTime"), 0, g_ConfigFile);
	VIRTUALIZER_TIGER_BLACK_END;
}

#pragma optimize("", on)

bool CPINSystem::DBLoad(UINT charId, UINT pin, UINT timestamp)
{
	CTL;
	m_Lock.Enter();
	map<UINT, PINData*>::iterator it = m_Pins.find(charId);
	if(it != m_Pins.end())
	{
		it->second->baseCode.set(pin);
		it->second->timestamp = timestamp;
	}else
	{
		PINData* pPD = new PINData();
		pPD->charId = charId;
		pPD->baseCode.set(pin);
		pPD->timestamp = timestamp;
		m_Pins.insert(pair<UINT, PINData*>(charId, pPD));
	}
	m_Lock.Leave();

	return false;
}

void CPINSystem::UpdatePINData(UINT charId, PINData* pPD)
{
	CTL;
	m_Lock.Enter();
	m_Pins[charId] = pPD;
	m_Lock.Leave();

}

void CPINSystem::ResetTempPIN(UINT charId)
{
	CTL;
	m_Lock.Enter();
	map<UINT, PINData*>::iterator it = m_Pins.find(charId);
	if(it != m_Pins.end())
	{
		it->second->currentCode.clear();
		it->second->newCode.clear();
		it->second->confirmNewCode.clear();
	}else
	{
		m_Pins[charId] = new PINData();
	}
	m_Lock.Leave();

}

bool CPINSystem::CanEnter(CUserSocket *pSocket, bool& kick)
{
	CTL;

	UINT charId = pSocket->pED->selectedCharId;
	PINData* pPD = pSocket->pED->lpPD;
	if(pSocket->pED->lpPD == 0 || pSocket->pED->lpPD->loaded == false || charId != pSocket->pED->lpPD->charId)
	{
		m_Lock.Enter();
		map<UINT, PINData*>::iterator it = m_Pins.find(charId);
		if(it!=m_Pins.end())
		{
			pPD = pSocket->pED->lpPD = it->second;
			m_Lock.Leave();
		}else
		{
			pPD = new PINData();
			m_Pins[charId] = pPD;
			m_Lock.Leave();
			pPD->charId = charId;
			pSocket->pED->lpPD = pPD;
		}
		
		pPD->currentCode.clear();
		pPD->newCode.clear();
		pPD->confirmNewCode.clear();
		pSocket->pED->lpPD->loaded = true;
		pSocket->pED->lpPD->charId = charId;
	}

	if(pPD->baseCode.is_set())
	{
		if(m_ForceChangeTime == 0 || (pPD->timestamp + m_ForceChangeTime > time(0)))
		{
			if(pPD->baseCode == pPD->currentCode)
			{
				pPD->loaded = false;
			
				return true;
			}else if(pPD->currentCode.is_set())
			{
				pPD->enterAttempts++;
				if(pPD->enterAttempts < m_AttemptLimit)
				{
					RequestEnterPIN(pSocket, L"Incorrect code!");
				
					return false;
				}else
				{
					pPD->enterAttempts = 0;
					kick = true;
					g_CliExt.Ban(pSocket->pED->longHWID.hash, pSocket->accountName, L"PINSystem", static_cast<UINT>(m_BanTime+time(0)));
				
					return false;
				}
			}else
			{
				RequestEnterPIN(pSocket);
			
				return false;
			}
		}else
		{
			//force change pin
			RequestChangePIN(pSocket, L"Your current PIN is outdated, please change!");
		
			return false;
		}
	}else
	{
		RequestCreatePIN(pSocket);
	}

	return false;
}

bool CPINSystem::RequestHandleBypass(CUserSocket *pSocket, std::wstring& wTxt)
{
	CTL;
	bool ret = false;

	if(wTxt.find(L"pin_act?") == 0)
	{
		UINT dig = Parser::GetBypassValueInt(wTxt.c_str(), L"dig", 0);
		UINT type = Parser::GetBypassValueInt(wTxt.c_str(), L"type", 0);
		UINT lang = Parser::GetBypassValueInt(wTxt.c_str(), L"lang", 0);
		if(type < 6 && dig < 12)
		{
			ret = RequestAddDig(pSocket, dig, type, lang);
		}else
		{
			CIPAddress ip(pSocket->addr);
			g_Log.Add(CLog::Error, "[%s] Invalid dig[%d] or type[%d] from ip[%s]", __FUNCTION__, dig, type, ip.ToString().c_str());
		}
	}else if(wTxt == L"pin_create" || wTxt.find(L"pin_create?") == 0)
	{
		UINT lang = Parser::GetBypassValueInt(wTxt.c_str(), L"lang", 0);
		ret = OnCreatePIN(pSocket, lang);
	}else if(wTxt == L"pin_create_confirm" || wTxt.find(L"pin_create_confirm?") == 0)
	{
		UINT lang = Parser::GetBypassValueInt(wTxt.c_str(), L"lang", 0);
		ret = OnCreateConfirmPIN(pSocket, lang);
	}else if(wTxt == L"pin_enter" || wTxt.find(L"pin_enter?") == 0)
	{
		UINT lang = Parser::GetBypassValueInt(wTxt.c_str(), L"lang", 0);
		ret = OnEnterPIN(pSocket, lang);
	}else if(wTxt == L"pin_request_change" || wTxt.find(L"pin_request_change?") == 0)
	{
		UINT lang = Parser::GetBypassValueInt(wTxt.c_str(), L"lang", 0);
		ret = RequestChangePIN(pSocket, L"", lang);
	}else if(wTxt == L"pin_change" || wTxt.find(L"pin_change?") == 0)
	{
		UINT lang = Parser::GetBypassValueInt(wTxt.c_str(), L"lang", 0);
		ret = OnChangePIN(pSocket, lang);
	}else if(wTxt == L"pin_change_new" || wTxt.find(L"pin_change_new?") == 0)
	{
		UINT lang = Parser::GetBypassValueInt(wTxt.c_str(), L"lang", 0);
		ret = OnChangeNewPIN(pSocket, lang);
	}else if(wTxt == L"pin_change_new_confirm" || wTxt.find(L"pin_change_new_confirm?") == 0)
	{
		UINT lang = Parser::GetBypassValueInt(wTxt.c_str(), L"lang", 0);
		ret = OnChangeNewConfirmPIN(pSocket, lang);
	}

	return ret;
}

bool CPINSystem::RequestAddDig(CUserSocket *pSocket, UINT dig, UINT type, UINT lang)
{
	CTL;

	PINData* pPD = pSocket->pED->lpPD;
	if(pPD && pPD->loaded)
	{
		switch(type)
		{
		case DigCreate:
		case DigChangeNew:
			{
				pPD->newCode.add(dig);
				break;
			}
		case DigCreateConfirm:
		case DigChangeNewConfirm:
			{
				pPD->confirmNewCode.add(dig);
				break;
			}
		case DigEnter:
		case DigChangeCurrent:
			{
				pPD->currentCode.add(dig);
				break;
			}
		}
	}

	switch(type)
	{
	case DigCreate:
		{
			RequestCreatePIN(pSocket, L"", lang);
			break;
		}
	case DigCreateConfirm:
		{
			RequestCreateConfirmPIN(pSocket, lang);
			break;
		}
	case DigEnter:
		{
			RequestEnterPIN(pSocket, L"", lang);
			break;
		}
	case DigChangeCurrent:
		{
			RequestChangePIN(pSocket, L"", lang);
			break;
		}
	case DigChangeNew:
		{
			RequestChangeNewPIN(pSocket, L"", lang);
			break;
		}
	case DigChangeNewConfirm:
		{
			RequestChangeNewConfirmPIN(pSocket, L"", lang);
			break;
		}
	}

	return false;
}

bool CPINSystem::RequestCreatePIN(CUserSocket *pSocket, wstring message, UINT lang)
{
	CTL;
	if(m_Enabled)
	{
		const WCHAR *wHtml = 0;
		if(lang == 0)
		{
			wHtml = g_HtmlCache.Get(L"pin_create.htm");
		}else
		{
			wHtml = g_HtmlCache.Get(L"pin_create_ru.htm");
		}
		if(wHtml)
		{
			PINCode code;
			bool canContinue = true;
			PINData* pPD = pSocket->pED->lpPD;
			if(pPD && pPD->loaded)
			{
				if(pPD->baseCode.is_set())
				{
					canContinue = false;
				}else
				{
					code = pPD->newCode;
				}
			}else
			{
				canContinue = false;
			}
			if(canContinue)
			{
				wstring html(wHtml);
				html = Utils::ReplaceString(html, L"<?dig1?>", code.to_string(0), true);
				html = Utils::ReplaceString(html, L"<?dig2?>", code.to_string(1), true);
				html = Utils::ReplaceString(html, L"<?dig3?>", code.to_string(2), true);
				html = Utils::ReplaceString(html, L"<?dig4?>", code.to_string(3), true);
				html = Utils::ReplaceString(html, L"<?dig5?>", code.to_string(4), true);
				html = Utils::ReplaceString(html, L"<?dig6?>", code.to_string(5), true);
				html = Utils::ReplaceString(html, L"<?message?>", message, true);

				pSocket->ShowHTML(html.c_str(), 0);
			}
		}else
		{
			if(lang)
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find pin_create_ru.htm !", __FUNCTION__);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find pin_create.htm !", __FUNCTION__);
			}
		}
	}

	return false;
}

bool CPINSystem::OnCreatePIN(CUserSocket *pSocket, UINT lang)
{
	CTL;
	bool canContinue = false;
	PINData* pPD = pSocket->pED->lpPD;
	if(pPD && pPD->newCode.is_set() && pPD->baseCode.is_set() == false)
	{
		canContinue = true;
		
	}
	if(canContinue)
	{
		RequestCreateConfirmPIN(pSocket, lang);
	}else
	{
		RequestCreatePIN(pSocket, L"", lang);
	}

	return false;
}

bool CPINSystem::RequestCreateConfirmPIN(CUserSocket *pSocket, UINT lang)
{
	CTL;
	if(m_Enabled)
	{
		const WCHAR *wHtml = 0;
		if(lang)
		{
			wHtml = g_HtmlCache.Get(L"pin_create_confirm_ru.htm");
		}else
		{
			wHtml = g_HtmlCache.Get(L"pin_create_confirm.htm");
		}
		if(wHtml)
		{
			PINCode code;
			bool canCreate = true;
			PINData* pPD = pSocket->pED->lpPD;
			if(pPD && pPD->newCode.is_set() == false)
			{
				canCreate = false;
			}else
			{
				code = pPD->confirmNewCode;
			}
			if(canCreate)
			{
				wstring html(wHtml);
				html = Utils::ReplaceString(html, L"<?dig1?>", code.to_string(0), true);
				html = Utils::ReplaceString(html, L"<?dig2?>", code.to_string(1), true);
				html = Utils::ReplaceString(html, L"<?dig3?>", code.to_string(2), true);
				html = Utils::ReplaceString(html, L"<?dig4?>", code.to_string(3), true);
				html = Utils::ReplaceString(html, L"<?dig5?>", code.to_string(4), true);
				html = Utils::ReplaceString(html, L"<?dig6?>", code.to_string(5), true);

				pSocket->ShowHTML(html.c_str(), 0);
			}else
			{
				RequestCreatePIN(pSocket, L"", lang);
			}
		}else
		{
			if(lang)
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find pin_create_confirm_ru.htm !", __FUNCTION__);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find pin_create_confirm.htm !", __FUNCTION__);
			}
		}
	}

	return false;
}

bool CPINSystem::OnCreateConfirmPIN(CUserSocket *pSocket, UINT lang)
{
	CTL;
	bool canContinue = false;
	
	PINData* pPD = pSocket->pED->lpPD;
	if(pPD)
	{
		if(pPD->baseCode.is_set() == false && pPD->newCode.is_set() && pPD->newCode == pPD->confirmNewCode)
		{
			canContinue = true;
		}else
		{
			pPD->newCode.clear();
			pPD->confirmNewCode.clear();
		}
	}
	if(canContinue)
	{
		CreatePINDone(pSocket, lang);
	}else
	{
		RequestCreatePIN(pSocket, L"Incorrect confirmation code!", lang);
	}

	return false;
}

bool CPINSystem::CreatePINDone(CUserSocket *pSocket, UINT lang)
{
	CTL;
	const WCHAR *wHtml = 0;
	if(lang)
	{
		wHtml = g_HtmlCache.Get(L"pin_create_done_ru.htm");
	}else
	{
		wHtml = g_HtmlCache.Get(L"pin_create_done.htm");
	}
	if(wHtml)
	{
		PINCode code;
		bool canContinue = false;
		PINData* pPD = pSocket->pED->lpPD;
		if(pPD)
		{
			if(pPD->baseCode.is_set() == false && pPD->newCode.is_set() && pPD->newCode == pPD->confirmNewCode)
			{
				code = pPD->newCode;
				pPD->baseCode = code;
				pPD->newCode.clear();
				pPD->confirmNewCode.clear();
				pPD->currentCode.clear();
				pPD->timestamp = time(0);
				canContinue = true;
			}else
			{
				pPD->newCode.clear();
				pPD->confirmNewCode.clear();
				pPD->currentCode.clear();
			}
		}
		if(canContinue)
		{
			g_DB.RequestSetCharacterPIN(pSocket->pED->selectedCharId, code.get(), pPD->timestamp);
			CIPAddress ip(pSocket->addr);
			g_Logger.Add(L"Created PIN for char_id[%d] from ip[%S]", pSocket->pED->selectedCharId, ip.ToString().c_str());

			wstring html(wHtml);
			html = Utils::ReplaceString(html, L"<?dig1?>", code.to_string(0), true);
			html = Utils::ReplaceString(html, L"<?dig2?>", code.to_string(1), true);
			html = Utils::ReplaceString(html, L"<?dig3?>", code.to_string(2), true);
			html = Utils::ReplaceString(html, L"<?dig4?>", code.to_string(3), true);
			html = Utils::ReplaceString(html, L"<?dig5?>", code.to_string(4), true);
			html = Utils::ReplaceString(html, L"<?dig6?>", code.to_string(5), true);

			pSocket->ShowHTML(html.c_str(), 0);
		}else
		{
			RequestCreatePIN(pSocket, L"Something went wrong. Try again.", lang);
		}
	}else
	{
		if(lang)
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find pin_create_done_ru.htm !", __FUNCTION__);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find pin_create_done.htm !", __FUNCTION__);
		}
	}

	return false;
}

bool CPINSystem::RequestEnterPIN(CUserSocket *pSocket, wstring message,UINT lang)
{
	CTL;
	if(m_Enabled)
	{
		const WCHAR *wHtml = 0;
		if(lang)
		{
			wHtml = g_HtmlCache.Get(L"pin_form_ru.htm");
		}else
		{
			wHtml = g_HtmlCache.Get(L"pin_form.htm");
		}
		if(wHtml)
		{
			PINCode code;
			bool canContinue = false;
			INT32 attempts = 0;
			PINData* pPD = pSocket->pED->lpPD;
			if(pPD && pPD->baseCode.is_set())
			{
				canContinue = true;
				code = pPD->currentCode;
				attempts = pPD->enterAttempts;
			}
			if(canContinue)
			{
				wstring html(wHtml);
				html = Utils::ReplaceString(html, L"<?dig1?>", code.to_string(0), true);
				html = Utils::ReplaceString(html, L"<?dig2?>", code.to_string(1), true);
				html = Utils::ReplaceString(html, L"<?dig3?>", code.to_string(2), true);
				html = Utils::ReplaceString(html, L"<?dig4?>", code.to_string(3), true);
				html = Utils::ReplaceString(html, L"<?dig5?>", code.to_string(4), true);
				html = Utils::ReplaceString(html, L"<?dig6?>", code.to_string(5), true);
				html = Utils::ReplaceString(html, L"<?message?>", message, true);
				WCHAR temp[32];
				_itow_s(attempts, temp, 31, 10);
				html = Utils::ReplaceString(html, L"<?attempts?>", temp, true);
				_itow_s(m_AttemptLimit, temp, 31, 10);
				html = Utils::ReplaceString(html, L"<?attempts_limit?>", temp, true);

				pSocket->ShowHTML(html.c_str(), 0);
			}
		}else
		{
			if(lang)
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find pin_form_ru.htm !", __FUNCTION__);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find pin_form.htm !", __FUNCTION__);
			}
		}
	}

	return false;
}

bool CPINSystem::OnEnterPIN(CUserSocket *pSocket, UINT lang)
{
	CTL;
	bool kick = false;
	bool canContinue = false;
	
	PINData* pPD = pSocket->pED->lpPD;

	if(pPD)
	{
		if(pPD->baseCode.is_set() && pPD->currentCode == pPD->baseCode)
		{
			canContinue = true;
			pPD->enterAttempts = 0;
		}else
		{
			pPD->currentCode.clear();
			pPD->enterAttempts++;
			if(pPD->enterAttempts >= m_AttemptLimit)
			{
				kick = true;
				pPD->enterAttempts = 0;
			}
		}
	}

	if(canContinue)
	{
		EnterPINDone(pSocket, lang);
	}else if(!kick)
	{
		RequestEnterPIN(pSocket, L"Incorrect code!", lang);
	}

	if(kick)
	{
		g_CliExt.Ban(pSocket->pED->longHWID.hash, pSocket->accountName, L"PINSystem", static_cast<UINT>(m_BanTime+time(0)));
	}

	return kick;
}

bool CPINSystem::EnterPINDone(CUserSocket *pSocket, UINT lang)
{
	CTL;
	const WCHAR *wHtml = 0;
	if(lang)
	{
		wHtml = g_HtmlCache.Get(L"pin_form_done_ru.htm");
	}else
	{
		wHtml = g_HtmlCache.Get(L"pin_form_done.htm");
	}
	if(wHtml)
	{
		pSocket->ShowHTML(wHtml, 0);
	}else
	{
		if(lang)
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find pin_form_done_ru.htm !", __FUNCTION__);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find pin_form_done.htm !", __FUNCTION__);
		}
	}

	return false;
}

bool CPINSystem::RequestChangePIN(CUserSocket *pSocket, wstring message, UINT lang)
{
	CTL;
	if(m_Enabled)
	{
		const WCHAR *wHtml = 0;
		if(lang)
		{
			wHtml = g_HtmlCache.Get(L"pin_change_ru.htm");
		}else
		{
			wHtml = g_HtmlCache.Get(L"pin_change.htm");
		}
		if(wHtml)
		{
			PINCode code;
			bool canContinue = false;
			INT32 attempts = 0;
			PINData* pPD = pSocket->pED->lpPD;
			if(pPD && pPD->baseCode.is_set())
			{
				canContinue = true;
				code = pPD->currentCode;
				attempts = pPD->changeAttempts;
			}
			if(canContinue)
			{
				wstring html(wHtml);
				html = Utils::ReplaceString(html, L"<?dig1?>", code.to_string(0), true);
				html = Utils::ReplaceString(html, L"<?dig2?>", code.to_string(1), true);
				html = Utils::ReplaceString(html, L"<?dig3?>", code.to_string(2), true);
				html = Utils::ReplaceString(html, L"<?dig4?>", code.to_string(3), true);
				html = Utils::ReplaceString(html, L"<?dig5?>", code.to_string(4), true);
				html = Utils::ReplaceString(html, L"<?dig6?>", code.to_string(5), true);
				html = Utils::ReplaceString(html, L"<?message?>", message, true);
				WCHAR temp[32];
				_itow_s(attempts, temp, 31, 10);
				html = Utils::ReplaceString(html, L"<?attempts?>", temp, true);
				_itow_s(m_AttemptLimit, temp, 31, 10);
				html = Utils::ReplaceString(html, L"<?attempts_limit?>", temp, true);

				pSocket->ShowHTML(html.c_str(), 0);
			}
		}else
		{
			if(lang)
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find pin_change_ru.htm !", __FUNCTION__);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find pin_change.htm !", __FUNCTION__);
			}
		}
	}

	return false;
}

bool CPINSystem::OnChangePIN(CUserSocket *pSocket, UINT lang)
{
	CTL;
	bool kick = false;
	bool canContinue = false;
	
	PINData* pPD = pSocket->pED->lpPD;
	if(pPD)
	{
		if(pPD->baseCode.is_set() && pPD->currentCode == pPD->baseCode)
		{
			canContinue = true;
			pPD->changeAttempts = 0;
		}else
		{
			pPD->currentCode.clear();
			pPD->newCode.clear();
			pPD->confirmNewCode.clear();
			pPD->changeAttempts++;
			if(pPD->changeAttempts >= m_AttemptLimit)
			{
				kick = true;
				pPD->changeAttempts = 0;
			}
		}
	}
	
	if(canContinue)
	{
		RequestChangeNewPIN(pSocket, L"", lang);
	}else if(!kick)
	{
		RequestChangePIN(pSocket, L"Incorrect current code!", lang);
	}

	if(kick)
	{
		g_CliExt.Ban(pSocket->pED->longHWID.hash, pSocket->accountName, L"PINSystem", static_cast<UINT>(m_BanTime+time(0)));
	}

	return kick;
}

bool CPINSystem::RequestChangeNewPIN(CUserSocket *pSocket, wstring message, UINT lang)
{
	CTL;
	if(m_Enabled)
	{
		const WCHAR *wHtml = 0;
		if(lang)
		{
			wHtml = g_HtmlCache.Get(L"pin_change_new_ru.htm");
		}else
		{
			wHtml = g_HtmlCache.Get(L"pin_change_new.htm");
		}
		if(wHtml)
		{
			PINCode code;
			bool canContinue = false;
			PINData* pPD = pSocket->pED->lpPD;
			if(pPD)
			{
				if(pPD->baseCode.is_set() && pPD->baseCode == pPD->currentCode)
				{
					canContinue = true;
					code = pPD->newCode;
				}else
				{
					pPD->newCode.clear();
					pPD->currentCode.clear();
					pPD->confirmNewCode.clear();
				}
			}
			if(canContinue)
			{
				wstring html(wHtml);
				html = Utils::ReplaceString(html, L"<?dig1?>", code.to_string(0), true);
				html = Utils::ReplaceString(html, L"<?dig2?>", code.to_string(1), true);
				html = Utils::ReplaceString(html, L"<?dig3?>", code.to_string(2), true);
				html = Utils::ReplaceString(html, L"<?dig4?>", code.to_string(3), true);
				html = Utils::ReplaceString(html, L"<?dig5?>", code.to_string(4), true);
				html = Utils::ReplaceString(html, L"<?dig6?>", code.to_string(5), true);
				html = Utils::ReplaceString(html, L"<?message?>", message, true);

				pSocket->ShowHTML(html.c_str(), 0);
			}
		}else
		{
			if(lang)
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find pin_change_new_ru.htm !", __FUNCTION__);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find pin_change_new.htm !", __FUNCTION__);
			}
		}
	}

	return false;
}

bool CPINSystem::OnChangeNewPIN(CUserSocket *pSocket, UINT lang)
{
	CTL;
	bool canContinue = false;
	bool missingDigs = false;

	PINData* pPD = pSocket->pED->lpPD;
	if(pPD)
	{
		if(pPD->newCode.is_set() && pPD->baseCode.is_set() && pPD->baseCode == pPD->currentCode)
		{
			canContinue = true;
		}else if(pPD->newCode.is_set() == false)
		{
			missingDigs = true;
		}else
		{
			pPD->currentCode.clear();
			pPD->newCode.clear();
			pPD->confirmNewCode.clear();
		}
	}
	if(canContinue)
	{
		RequestChangeNewConfirmPIN(pSocket, L"", lang);
	}else if(missingDigs)
	{
		RequestChangeNewPIN(pSocket, L"You have to enter 6 digits!", lang);
	}else
	{
		RequestChangePIN(pSocket, L"Codes didn't match!", lang);
	}

	return false;
}

bool CPINSystem::RequestChangeNewConfirmPIN(CUserSocket *pSocket, wstring message, UINT lang)
{
	CTL;
	if(m_Enabled)
	{
		const WCHAR *wHtml = 0;
		if(lang)
		{
			wHtml = g_HtmlCache.Get(L"pin_change_new_confirm_ru.htm");
		}else
		{
			wHtml = g_HtmlCache.Get(L"pin_change_new_confirm.htm");
		}
		if(wHtml)
		{
			PINCode code;
			PINData* pPD = pSocket->pED->lpPD;
			bool canCreate = false;
			if(pPD)
			{
				if(pPD->newCode.is_set() && pPD->baseCode.is_set() && pPD->baseCode == pPD->currentCode)
				{
					canCreate = true;
					code = pPD->confirmNewCode;
				}else
				{
					pPD->currentCode.clear();
					pPD->newCode.clear();
					pPD->confirmNewCode.clear();
				}
			}
			if(canCreate)
			{
				wstring html(wHtml);
				html = Utils::ReplaceString(html, L"<?dig1?>", code.to_string(0), true);
				html = Utils::ReplaceString(html, L"<?dig2?>", code.to_string(1), true);
				html = Utils::ReplaceString(html, L"<?dig3?>", code.to_string(2), true);
				html = Utils::ReplaceString(html, L"<?dig4?>", code.to_string(3), true);
				html = Utils::ReplaceString(html, L"<?dig5?>", code.to_string(4), true);
				html = Utils::ReplaceString(html, L"<?dig6?>", code.to_string(5), true);

				pSocket->ShowHTML(html.c_str(), 0);
			}else
			{
				RequestChangePIN(pSocket, L"Codes didn't match!", lang);
			}
		}else
		{
			if(lang)
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find pin_change_new_confirm_ru.htm !", __FUNCTION__);
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find pin_change_new_confirm.htm !", __FUNCTION__);
			}
		}
	}

	return false;
}

bool CPINSystem::OnChangeNewConfirmPIN(CUserSocket *pSocket, UINT lang)
{
	CTL;
	bool canContinue = false;
	PINData* pPD = pSocket->pED->lpPD;
	if(pPD)
	{
		if(pPD->baseCode.is_set() == true && pPD->newCode.is_set() && pPD->newCode == pPD->confirmNewCode && pPD->baseCode == pPD->currentCode)
		{
			canContinue = true;
		}else
		{
			pPD->currentCode.clear();
			pPD->newCode.clear();
			pPD->confirmNewCode.clear();
		}
	}
	if(canContinue)
	{
		ChangePINDone(pSocket, lang);
	}else
	{
		RequestChangePIN(pSocket, L"Incorrect confirmation code!", lang);
	}

	return false;
}

bool CPINSystem::ChangePINDone(CUserSocket *pSocket, UINT lang)
{
	CTL;
	const WCHAR *wHtml = 0;
	if(lang)
	{
		wHtml = g_HtmlCache.Get(L"pin_change_done_ru.htm");
	}else
	{
		wHtml = g_HtmlCache.Get(L"pin_change_done.htm");
	}
	if(wHtml)
	{
		PINCode code;
		PINData* pPD = pSocket->pED->lpPD;
		bool canContinue = false;
		if(pPD)
		{
			if(pPD->baseCode.is_set() && pPD->baseCode == pPD->currentCode && pPD->newCode.is_set() && pPD->newCode == pPD->confirmNewCode)
			{
				code = pPD->newCode;
				pPD->baseCode = code;
				pPD->newCode.clear();
				pPD->confirmNewCode.clear();
				pPD->currentCode.clear();
				pPD->timestamp = time(0);
				canContinue = true;
			}else
			{
				pPD->currentCode.clear();
				pPD->newCode.clear();
				pPD->confirmNewCode.clear();
			}
		}
		if(canContinue)
		{
			g_DB.RequestSetCharacterPIN(pSocket->pED->selectedCharId, code.get(), pPD->timestamp);
			CIPAddress ip(pSocket->addr);
			g_Logger.Add(L"Changed PIN for char_id[%d] from ip[%S]", pSocket->pED->selectedCharId, ip.ToString().c_str());

			wstring html(wHtml);
			html = Utils::ReplaceString(html, L"<?dig1?>", code.to_string(0), true);
			html = Utils::ReplaceString(html, L"<?dig2?>", code.to_string(1), true);
			html = Utils::ReplaceString(html, L"<?dig3?>", code.to_string(2), true);
			html = Utils::ReplaceString(html, L"<?dig4?>", code.to_string(3), true);
			html = Utils::ReplaceString(html, L"<?dig5?>", code.to_string(4), true);
			html = Utils::ReplaceString(html, L"<?dig6?>", code.to_string(5), true);

			pSocket->ShowHTML(html.c_str(), 0);
		}else
		{
			RequestChangePIN(pSocket, L"Codes didn't match!");	//old Create
		}
	}else
	{
		if(lang)
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find pin_create_done_ru.htm !", __FUNCTION__);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find pin_create_done.htm !", __FUNCTION__);
		}
	}

	return false;
}
