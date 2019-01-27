#include "stdafx.h"
#include "Rebirth.h"
#include "HtmlCache.h"
#include "DB.h"
#include "License.h"

using namespace License;
CRebirthSystem g_RebirthSystem;

#pragma optimize("", off)


void CRebirthSystem::Init()
{
	m_Enabled = 0;
	VIRTUALIZER_TIGER_BLACK_START;
	if(CLicense::GetInstance().CanUse(LicenseRebirth))
	{
		LoadINI();
		if(m_Enabled)
		{
			g_Log.Add(CLog::Blue, "[%s] system is enabled.", __FUNCTION__);
		}
	}
	VIRTUALIZER_TIGER_BLACK_END;
}

void CRebirthSystem::LoadINI()
{
	VIRTUALIZER_TIGER_BLACK_START;
	const TCHAR* section = _T("Rebirth");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile) ? true : false;
	m_EnableCommand = GetPrivateProfileInt(section, _T("EnableCommand"), 0, g_ConfigFile) ? true : false;
	m_Price.itemId = GetPrivateProfileInt(section, _T("PriceId"), 0, g_ConfigFile);
	m_Price.amount = GetPrivateProfileInt(section, _T("PriceAmount"), 0, g_ConfigFile);
	m_Reward.itemId = GetPrivateProfileInt(section, _T("RewardId"), 0, g_ConfigFile);
	m_Reward.amount = GetPrivateProfileInt(section, _T("RewardAmount"), 0, g_ConfigFile);
	m_MinLevel = GetPrivateProfileInt(section, _T("MinLevel"), 80, g_ConfigFile);
	m_RebirthLevel = GetPrivateProfileInt(section, _T("RebirthLevel"), 1, g_ConfigFile);
	m_StatPointBonus = GetPrivateProfileInt(section, _T("StatPointBonus"), 1, g_ConfigFile);
	m_MaxBonusPerStat = GetPrivateProfileInt(section, _T("MaxBonusPerStat"), 10, g_ConfigFile);
	m_RebirthLimit = GetPrivateProfileInt(section, _T("RebirthLimit"), 10, g_ConfigFile);
	VIRTUALIZER_TIGER_BLACK_END;
}

#pragma optimize("", on)

void CRebirthSystem::OnIncreaseStats(User *pUser, UINT statType)
{
	guard;

	if(m_Enabled)
	{
		RebirthUser& ru = pUser->pED->rebirthUser;
		bool save = false;
		if(ru.availablePoints > 0)
		{
			switch(statType)
			{
			case StatStr:
				{
					if(ru.bonusStr < m_MaxBonusPerStat)
					{
						ru.bonusStr++;
						ru.availablePoints--;
						save = true;
					}else
					{
						//2519	1	a,You cannot add any more points to this attribute.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->pSocket->SendSystemMessage(2519);
					}
					break;
				}
			case StatInt:
				{
					if(ru.bonusInt < m_MaxBonusPerStat)
					{
						ru.bonusInt++;
						ru.availablePoints--;
						save = true;
					}else
					{
						//2519	1	a,You cannot add any more points to this attribute.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->pSocket->SendSystemMessage(2519);
					}
					break;
				}
			case StatCon:
				{
					if(ru.bonusCon < m_MaxBonusPerStat)
					{
						ru.bonusCon++;
						ru.availablePoints--;
						save = true;
					}else
					{
						//2519	1	a,You cannot add any more points to this attribute.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->pSocket->SendSystemMessage(2519);
					}
					break;
				}
			case StatMen:
				{
					if(ru.bonusMen < m_MaxBonusPerStat)
					{
						ru.bonusMen++;
						ru.availablePoints--;
						save = true;
					}else
					{
						//2519	1	a,You cannot add any more points to this attribute.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->pSocket->SendSystemMessage(2519);
					}
					break;
				}
			case StatDex:
				{
					if(ru.bonusDex < m_MaxBonusPerStat)
					{
						ru.bonusDex++;
						ru.availablePoints--;
						save = true;
					}else
					{
						//2519	1	a,You cannot add any more points to this attribute.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->pSocket->SendSystemMessage(2519);
					}
					break;
				}
			case StatWit:
				{
					if(ru.bonusWit < m_MaxBonusPerStat)
					{
						ru.bonusWit++;
						ru.availablePoints--;
						save = true;
					}else
					{
						//2519	1	a,You cannot add any more points to this attribute.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->pSocket->SendSystemMessage(2519);
					}
					break;
				}
			default:
				g_Log.Add(CLog::Error, "[%s] Invalid stat type[%d] user[%S]", __FUNCTION__, statType, pUser->pSD->wszName);
				break;
			}
			if(save)
			{
				//2520	1	a,You've increased one of your base attribute.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2520);
				pUser->SetValidateParametersRequest();
				g_DB.RequestSaveRebirth(pUser->nDBID, pUser->pSD->nSubClassIndex, ru.rebirthCount, ru.availablePoints, ru.bonusInt, ru.bonusStr, ru.bonusCon, ru.bonusMen, ru.bonusDex, ru.bonusWit);
				OnDistributeStats(pUser);
			}
		}else
		{
			//2521	1	a,You don't have any bonus points to distribute.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->pSocket->SendSystemMessage(2521);
		}
	}

	unguard;
}

void CRebirthSystem::OnDistributeStats(User *pUser)
{
	guard;

	if(m_Enabled)
	{
		const WCHAR* wHtml = g_HtmlCache.Get(L"rebirth_distribute.htm");
		if(wHtml)
		{
			RebirthUser& ru = pUser->pED->rebirthUser;
			WCHAR tmp[16] = { 0 };
			_itow_s(ru.availablePoints, tmp, 16, 10);
			wstring html = Utils::ReplaceString(wHtml, L"<?available_points?>", tmp, true);
			_itow_s(ru.bonusStr, tmp, 16, 10);
			html = Utils::ReplaceString(html, L"<?bonus_str?>", tmp, true);
			_itow_s(ru.bonusInt, tmp, 16, 10);
			html = Utils::ReplaceString(html, L"<?bonus_int?>", tmp, true);
			_itow_s(ru.bonusCon, tmp, 16, 10);
			html = Utils::ReplaceString(html, L"<?bonus_con?>", tmp, true);
			_itow_s(ru.bonusMen, tmp, 16, 10);
			html = Utils::ReplaceString(html, L"<?bonus_men?>", tmp, true);
			_itow_s(ru.bonusDex, tmp, 16, 10);
			html = Utils::ReplaceString(html, L"<?bonus_dex?>", tmp, true);
			_itow_s(ru.bonusWit, tmp, 16, 10);
			html = Utils::ReplaceString(html, L"<?bonus_wit?>", tmp, true);

			pUser->ShowHTML(L"rebirth_distribute.htm", html.c_str(), 0);
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Missing rebirth_distribute.htm", __FUNCTION__);
		}
	}

	unguard;
}

void CRebirthSystem::OnRebirth(User *pUser)
{
	guard;

	if(m_Enabled)
	{
		if(pUser->pSD->nLevel >= m_MinLevel)
		{
			RebirthUser& ru = pUser->pED->rebirthUser;
			if(ru.rebirthCount < m_RebirthLimit)
			{
				if(m_Price.itemId == 0 || m_Price.amount == 0 || pUser->DeleteItemInInventory(m_Price.itemId, m_Price.amount))
				{
					//perform rebirth
					pUser->SetLevel(m_RebirthLevel);
					if(m_Reward.itemId > 0 && m_Reward.amount > 0)
					{
						pUser->AddItemToInventory(m_Reward.itemId, m_Reward.amount);
					}

					ru.rebirthCount++;
					ru.availablePoints += m_StatPointBonus;
					pUser->SocialAction(SocialActionHero);

					g_DB.RequestSaveRebirth(pUser->nDBID, pUser->pSD->nSubClassIndex, ru.rebirthCount, ru.availablePoints, ru.bonusInt, ru.bonusStr, ru.bonusCon, ru.bonusMen, ru.bonusDex, ru.bonusWit);

					const WCHAR *wHtml = g_HtmlCache.Get(L"rebirth_done.htm");
					if(wHtml)
					{
						WCHAR tmp[16] = { 0 };
						_itow_s(m_RebirthLimit - ru.rebirthCount, tmp, 16, 10);
						wstring html = Utils::ReplaceString(wHtml, L"<?rebirth_left?>", tmp, true);
						_itow_s(ru.availablePoints, tmp, 16, 10);
						html = Utils::ReplaceString(html, L"<?stat_points?>", tmp, true);
						pUser->ShowHTML(L"rebirth_done.htm", html.c_str(), 0);
					}else
					{
						g_Log.Add(CLog::Error, "[%s] Missing rebirth_done.htm!", __FUNCTION__);
					}
				}else
				{
					//2522	1	a,You don't have enough item(s) to pay for rebirth.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->pSocket->SendSystemMessage(2522);
				}
			}else
			{
				//2523	1	a,You have reached rebirths limit.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2523);
			}
		}else
		{
			//2524	1	a,Your level is to low to perform a rebirth.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->pSocket->SendSystemMessage(2524);
		}
	}

	unguard;
}

void CRebirthSystem::ShowMenu(User *pUser)
{
	guard;

	if(m_Enabled)
	{
		const WCHAR* wHtml = g_HtmlCache.Get(L"rebirth.htm");
		if(wHtml)
		{
			pUser->ShowHTML(L"rebirth.htm", wHtml, 0);
		}
	}

	unguard;
}