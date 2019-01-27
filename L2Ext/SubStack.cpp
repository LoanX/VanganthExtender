#include "stdafx.h"
#include "SubStack.h"
#include "HtmlCache.h"
#include "ClassManager.h"
#include "DB.h"
#include "PlayerAction.h"
#include "License.h"
#include "SkillAcquireDB.h"

using namespace License;

CSubStack g_SubStack;

#pragma optimize("", off)

void CSubStack::Init()
{
	m_Enabled = FALSE;
	VIRTUALIZER_TIGER_BLACK_START;
	if(CLicense::GetInstance().CanUse(LicenseSubStack))
	{
		m_Enabled = FALSE;
		LoadINI();
		if(m_Enabled)
		{
			g_Log.Add(CLog::Blue, "[%s] System enabled.", __FUNCTION__);
			for(UINT n=88;n<119;n++)
			{
				m_AvailableClasses.push_back(n);
			}
		}
	}
	VIRTUALIZER_TIGER_BLACK_END;
};

#pragma optimize("", on)

void CSubStack::LoadINI()
{
	/*
	Enabled=0
	;format item_id;amount
	StackPrice=57;1000000
	;level that will be set after getting new stack
	Delevel=40
	;price for _ex bypsas - no delevel
	StackPriceEx=57;10000000
	;price for switching the class in the stack
	SwitchPrice=57;100000
	;price for removing the stack
	RemovePrice=57;1000000
	*/
	const TCHAR* section = _T("SubStack");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	m_Delevel = GetPrivateProfileInt(section, _T("Delevel"), 0, g_ConfigFile);
	m_RequiredLevel = GetPrivateProfileInt(section, _T("RequiredLevel"), 0, g_ConfigFile);
	m_Require3rdClassChange = GetPrivateProfileInt(section, _T("Require3rdClassChange"), 1, g_ConfigFile);
	TCHAR temp[8190];
	if(GetPrivateProfileString(section, _T("StackPrice"), 0, temp, 8190, g_ConfigFile))
	{
		wstringstream dataStream(Parser::Replace(temp, L';', L' '));
		dataStream >> m_StackPrice.itemId >> m_StackPrice.amount;
	}
	if(GetPrivateProfileString(section, _T("StackPriceEx"), 0, temp, 8190, g_ConfigFile))
	{
		wstringstream dataStream(Parser::Replace(temp, L';', L' '));
		dataStream >> m_StackPriceEx.itemId >> m_StackPriceEx.amount;
	}
	if(GetPrivateProfileString(section, _T("SwitchPrice"), 0, temp, 8190, g_ConfigFile))
	{
		wstringstream dataStream(Parser::Replace(temp, L';', L' '));
		dataStream >> m_SwitchPrice.itemId >> m_SwitchPrice.amount;
	}
	if(GetPrivateProfileString(section, _T("RemovePrice"), 0, temp, 8190, g_ConfigFile))
	{
		wstringstream dataStream(Parser::Replace(temp, L';', L' '));
		dataStream >> m_RemovePrice.itemId >> m_RemovePrice.amount;
	}
}

void CSubStack::ShowAvailable(User *pUser, bool ex)
{
	guard;
	if(m_Enabled)
	{
		if(pUser->pED->subStackInfo.timestamp < GetTickCount())
		{
			pUser->pED->subStackInfo.timestamp = GetTickCount() + 1000;
			//check level
			if(m_RequiredLevel == 0 || m_RequiredLevel <= pUser->pSD->nLevel)
			{
				//check if current class is third class
				if(m_Require3rdClassChange == 0 || CCategoryDataDB::IsInCategory(pUser->pSD->nClass, CCategoryDataDB::FOURTH_CLASS_GROUP))
				{
					//check if already added
					if(pUser->pED->subStackInfo.classId[pUser->pSD->nSubClassIndex][1] == UINT_MAX)
					{
						if(pUser->pSD->nAlive)
						{
							const WCHAR* wHtml = 0;
							if(ex)
							{
								wHtml = g_HtmlCache.Get(L"substack_show_ex.htm");
							}else
							{
								wHtml = g_HtmlCache.Get(L"substack_show.htm");
							}
							if(wHtml)
							{
								//<template><a action="bypass -h substack_add?class=$class_id">$class_name</a><br></template>
								wstring html(wHtml);
								wstring templateLine;
								size_t startPos = html.find(L"<template>");
								if(startPos != wstring::npos)
								{
									size_t endPos = html.find(L"</template>", startPos);
									if(endPos != wstring::npos)
									{
										templateLine = html.substr(startPos + 10, (endPos - startPos - 10));
										wstringstream lines;
										for(UINT n=0;n<m_AvailableClasses.size();n++)
										{
											int newClass = m_AvailableClasses[n];
											if(newClass != pUser->pSD->nClass)
											{
												wstring line = templateLine;
												line = Utils::ReplaceString(line, L"$class_name", g_ClassManager.GetName(newClass), true);
												WCHAR temp[32];
												temp[0] = 0;
												_itow_s(newClass, temp, 31, 10);
												line = Utils::ReplaceString(line, L"$class_id", temp, true);
												lines << line << endl;
											}
										}
										
										html = html.replace(startPos, (endPos + 11 - startPos), lines.str());
									}
								}
								if(ex)
								{
									pUser->ShowHTML(L"substack_show_ex.htm", html.c_str(), 0);
								}else
								{
									pUser->ShowHTML(L"substack_show.htm", html.c_str(), 0);
								}
							}else
							{
								g_Log.Add(CLog::Error, "[%s] Missing substack_show.htm!", __FUNCTION__);
							}
						}else
						{
							//2567	1	a,You have to be alive to continue!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->SendSystemMessage(2567);
						}
					}else
					{
						//2568	1	a,You have already made a stacked class!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->SendSystemMessage(2568);
					}
				}else
				{
					//2569	1	a,You have to finish 3rd class quest to be able to stack another class!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->SendSystemMessage(2569);
				}
			}else
			{
				//2570	1	a,You've got too low level to be able to stack the class!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->SendSystemMessage(2570);
			}
		}
	}
	unguard;
}

void CSubStack::RequestStack(User *pUser, UINT classId, bool ex)
{
	guard;
	if(m_Enabled)
	{
		if(pUser->pED->subStackInfo.timestamp < GetTickCount())
		{
			pUser->pED->subStackInfo.timestamp = GetTickCount() + 2000;
			if(m_RequiredLevel == 0 || m_RequiredLevel <= pUser->pSD->nLevel)
			{
				//check if current class is third class
				if(m_Require3rdClassChange == 0 || CCategoryDataDB::IsInCategory(pUser->pSD->nClass, CCategoryDataDB::FOURTH_CLASS_GROUP))
				{
					//check if already added
					if(pUser->pED->subStackInfo.classId[pUser->pSD->nSubClassIndex][1] == UINT_MAX)
					{
						if(pUser->pSD->nAlive)
						{
							//check price
							bool paid = true;
							if(ex)
							{
								if(m_StackPriceEx.itemId > 0 && m_StackPriceEx.amount > 0)
								{
									if(!pUser->DeleteItemInInventory(m_StackPriceEx.itemId, m_StackPriceEx.amount))
									{
										paid = false;
									}
								}
							}else
							{
								if(m_StackPrice.itemId > 0 && m_StackPrice.amount > 0)
								{
									if(!pUser->DeleteItemInInventory(m_StackPrice.itemId, m_StackPrice.amount))
									{
										paid = false;
									}
								}
							}
							if(paid)
							{
								//save base class and add new sub
								pUser->pED->subStackInfo.classId[pUser->pSD->nSubClassIndex][0] = pUser->pSD->nClass;
								pUser->pED->subStackInfo.classId[pUser->pSD->nSubClassIndex][1] = classId;
								
								g_Logger.Add(L"User[%s][%d] stacked class[%d] with [%d] - subjob index[%d]!", pUser->pSD->wszName, pUser->nDBID, pUser->pSD->nClass, classId, pUser->pSD->nSubClassIndex);

								if(!ex && m_Delevel != 0 && m_Delevel < pUser->pSD->nLevel)
								{
									pUser->SetLevel(m_Delevel);
								}
								PlayerAction::OnClassChange(pUser, classId);
								pUser->SocialAction(SocialActionLevelUp);
								pUser->ValidateSubjobParameters();
								pUser->SaveAcquiredSkills();

								g_DB.RequestSaveSubStack(pUser->nDBID, pUser->pSD->nSubClassIndex, pUser->pED->subStackInfo.classId[pUser->pSD->nSubClassIndex][0], pUser->pED->subStackInfo.classId[pUser->pSD->nSubClassIndex][1]);
								const WCHAR* wHtml = g_HtmlCache.Get(L"substack_done.htm");
								if(wHtml)
								{
									pUser->ShowHTML(L"substack_done.htm", wHtml, 0);
								}
							}else
							{
								pUser->SendSystemMessage(L"You don't have enough item(s) to pay for the stacked class!");
							}
						}else
						{
							//2567	1	a,You have to be alive to continue!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->SendSystemMessage(2567);
						}
					}else
					{
						//2568	1	a,You have already made a stacked class!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->SendSystemMessage(2568);
					}
				}else
				{
					//2569	1	a,You have to finish 3rd class quest to be able to stack another class!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->SendSystemMessage(2569);
				}
			}else
			{
				//2570	1	a,You've got too low level to be able to stack the class!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->SendSystemMessage(2570);
			}
		}
	}
	unguard;
}

void CSubStack::RequestSwitch(User *pUser)
{
	guard;
	if(m_Enabled)
	{
		if(pUser->pED->subStackInfo.timestamp < GetTickCount())
		{
			pUser->pED->subStackInfo.timestamp = GetTickCount() + 3000;
			//check if current class is third class
			if(m_Require3rdClassChange ==0 || CCategoryDataDB::IsInCategory(pUser->pSD->nClass, CCategoryDataDB::FOURTH_CLASS_GROUP))
			{
				//check if already added
				if(pUser->pED->subStackInfo.classId[pUser->pSD->nSubClassIndex][1] != UINT_MAX)
				{
					if(pUser->pSD->nAlive)
					{
						bool paid = true;
						if(m_SwitchPrice.itemId > 0 && m_SwitchPrice.amount > 0)
						{
							if(!pUser->DeleteItemInInventory(m_SwitchPrice.itemId, m_SwitchPrice.amount))
							{
								paid = false;
							}
						}
						if(paid)
						{
							//save base class and add new sub
							UINT baseClass = pUser->pED->subStackInfo.classId[pUser->pSD->nSubClassIndex][0];
							UINT stackClass = pUser->pED->subStackInfo.classId[pUser->pSD->nSubClassIndex][1];
							
							UINT newClass = UINT_MAX;
							if(pUser->pSD->nClass == baseClass)
							{
								newClass = stackClass;
							}else
							{
								newClass = baseClass;
							}

							PlayerAction::OnClassChange(pUser, newClass);
							pUser->SocialAction(SocialActionLevelUp);
							pUser->ValidateSubjobParameters();
							pUser->SaveAcquiredSkills();

							const WCHAR* wHtml = g_HtmlCache.Get(L"substack_switch_done.htm");
							if(wHtml)
							{
								pUser->ShowHTML(L"substack_switch_done.htm", wHtml, 0);
							}
						}else
						{
							//2571	1	a,You don't have enough item(s) to pay for switching the class!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->SendSystemMessage(2571);
						}
					}else
					{
						//2567	1	a,You have to be alive to continue!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->SendSystemMessage(2567);
					}
				}else
				{
					//2572	1	a,You don't have a stacked class!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->SendSystemMessage(2572);
				}
			}else
			{
				//2569	1	a,You have to finish 3rd class quest to be able to stack another class!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->SendSystemMessage(2569);
			}
		}
	}
	unguard;
}

void CSubStack::RequestRemove(User *pUser)
{
	guard;
	if(m_Enabled)
	{
		if(pUser->pED->subStackInfo.timestamp < GetTickCount())
		{
			pUser->pED->subStackInfo.timestamp = GetTickCount() + 3000;
			//check if current class is third class
			if(m_Require3rdClassChange == 0 || CCategoryDataDB::IsInCategory(pUser->pSD->nClass, CCategoryDataDB::FOURTH_CLASS_GROUP))
			{
				//check if already added
				if(pUser->pED->subStackInfo.classId[pUser->pSD->nSubClassIndex][1] != UINT_MAX)
				{
					if(pUser->pSD->nAlive)
					{
						bool paid = true;
						if(m_RemovePrice.itemId > 0 && m_RemovePrice.amount > 0)
						{
							if(!pUser->DeleteItemInInventory(m_RemovePrice.itemId, m_RemovePrice.amount))
							{
								paid = false;
							}
						}
						if(paid)
						{
							//save base class and add new sub
							UINT baseClass = pUser->pED->subStackInfo.classId[pUser->pSD->nSubClassIndex][0];
							UINT stackClass = pUser->pED->subStackInfo.classId[pUser->pSD->nSubClassIndex][1];
							
							UINT newClass = UINT_MAX;
							if(pUser->pSD->nClass != baseClass)
							{
								newClass = baseClass;
							}

							if(newClass != UINT_MAX)
							{
								PlayerAction::OnClassChange(pUser, newClass);							
								pUser->ValidateSubjobParameters();
							}

							pUser->SocialAction(SocialActionLevelUp);
							pUser->ValidateAcquiredSkills(false);

							//remove the stacks
							pUser->pED->subStackInfo.classId[pUser->pSD->nSubClassIndex][1] = UINT_MAX;
							pUser->pED->subStackInfo.classId[pUser->pSD->nSubClassIndex][0] = UINT_MAX;

							g_DB.RequestSaveSubStack(pUser->nDBID, pUser->pSD->nSubClassIndex, UINT_MAX, UINT_MAX);
							g_Logger.Add(L"User[%s] removed stacked class [%d] on subjob index[%d]", pUser->pSD->wszName, pUser->pSD->nSubClassIndex, stackClass);

							//remove old stack class skills
							g_SkillAcquireDB.ValidateSkills(pUser);

							const WCHAR* wHtml = g_HtmlCache.Get(L"substack_remove_done.htm");
							if(wHtml)
							{
								pUser->ShowHTML(L"substack_remove_done.htm", wHtml, 0);
							}
						}else
						{
							//2573	1	a,You don't have enough item(s) to remove the stacked class!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->SendSystemMessage(2573);
						}
					}else
					{
						//2567	1	a,You have to be alive to continue!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->SendSystemMessage(2567);
					}
				}else
				{
					//2572	1	a,You don't have a stacked class!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->SendSystemMessage(2572);
				}
			}else
			{
				//2569	1	a,You have to finish 3rd class quest to be able to stack another class!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->SendSystemMessage(2569);
			}
		}
	}
	unguard;
}