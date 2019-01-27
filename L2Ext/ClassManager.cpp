#include "stdafx.h"
#include "ClassManager.h"
#include "PlayerAction.h"
#include "HTMLCacheManager.h"
#include "HtmlCache.h"
#include "DB.h"

CClassManager g_ClassManager;

CClassManager::CClassManager()
{
}

CClassManager::~CClassManager()
{
}

void CClassManager::Init()
{
	/*
	Enabled=0
FirstChangeItemId=57
FirstChangeItemCount=10000
SecondChangeItemId=57
SecondChangeItemCount=100000
ThirdChangeItemId=57
ThirdChangeItemCount=1000000
*/
	const TCHAR* section = _T("ClassManager");
	m_enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	m_changeEnabled[0] = GetPrivateProfileInt(section, _T("FirstEnabled"), 1, g_ConfigFile);
	m_requiredItemId[0] = GetPrivateProfileInt(section, _T("FirstChangeItemId"), 57, g_ConfigFile);
	m_requiredItemCount[0] = GetPrivateProfileInt(section, _T("FirstChangeItemCount"), 10000, g_ConfigFile);
	m_changeEnabled[1] = GetPrivateProfileInt(section, _T("SecondEnabled"), 1, g_ConfigFile);
	m_requiredItemId[1] = GetPrivateProfileInt(section, _T("SecondChangeItemId"), 57, g_ConfigFile);
	m_requiredItemCount[1] = GetPrivateProfileInt(section, _T("SecondChangeItemCount"), 100000, g_ConfigFile);
	m_changeEnabled[2] = GetPrivateProfileInt(section, _T("ThirdEnabled"), 1, g_ConfigFile);
	m_requiredItemId[2] = GetPrivateProfileInt(section, _T("ThirdChangeItemId"), 57, g_ConfigFile);
	m_requiredItemCount[2] = GetPrivateProfileInt(section, _T("ThirdChangeItemCount"), 1000000, g_ConfigFile);
	m_Nobless = GetPrivateProfileInt(section, _T("Nobless"), 0, g_ConfigFile);
	m_NoblessPriceId = GetPrivateProfileInt(section, _T("NoblessPriceId"), 0, g_ConfigFile);
	m_NoblessPriceAmount = GetPrivateProfileInt(section, _T("NoblessPriceAmount"), 0, g_ConfigFile);
	m_SubClass = GetPrivateProfileInt(section, _T("SubClass"), 0, g_ConfigFile);
	m_SubClassPriceId = GetPrivateProfileInt(section, _T("SubClassPriceId"), 0, g_ConfigFile);
	m_SubClassPriceAmount = GetPrivateProfileInt(section, _T("SubClassPriceAmount"), 0, g_ConfigFile);

	m_ChangeReward[0].itemId = GetPrivateProfileInt(section, _T("FirstChangeRewardId"), 0, g_ConfigFile);
	m_ChangeReward[0].amount = GetPrivateProfileInt(section, _T("FirstChangeRewardCount"), 0, g_ConfigFile);
	m_ChangeReward[1].itemId = GetPrivateProfileInt(section, _T("SecondChangeRewardId"), 0, g_ConfigFile);
	m_ChangeReward[1].amount = GetPrivateProfileInt(section, _T("SecondChangeRewardCount"), 0, g_ConfigFile);
	m_ChangeReward[2].itemId = GetPrivateProfileInt(section, _T("ThirdChangeRewardId"), 0, g_ConfigFile);
	m_ChangeReward[2].amount = GetPrivateProfileInt(section, _T("ThirdChangeRewardCount"), 0, g_ConfigFile);
	
	LoadClassName();
}

void CClassManager::LoadClassName()
{
	CTL;

	m_class_name.clear();
	LPBYTE lpFile = 0;
	SIZE_T size = ReadFileBinary(L"..\\Script\\ClassName.txt", lpFile);
	if(lpFile)
	{
		if(size > 2)
		{
			wstringstream file((PWCHAR)&lpFile[2]);
			wstring line;
			while(getline(file, line))
			{
				//class_begin	class_id=13	class_name="Necromancer"	class_end
				if(line[0] == L'/' || line[0] == L';')
				{
					continue;
				}else if(line.find(L"class_begin") == 0)
				{
					INT32 classId = Parser::ParseOptionInt(line, L"class_id", -1);
					wstring className = Parser::ParseOptionMessage(line, L"class_name");
					if(classId >= 0 && className.size() > 0)
					{
						m_class_name[classId] = className;
					}
				}
			}
		}
		delete [] lpFile;
	}
}

wstring CClassManager::GetName(INT32 classId)
{
	map<INT32, wstring>::iterator Iter = m_class_name.find(classId);
	if(Iter!=m_class_name.end())
	{
		return Iter->second;
	}
	return L"MissingName";
}

void CClassManager::GetAvailable(int baseClassId, std::list<int> &lAvailable)
{
	if(baseClassId == 0)
	{
		lAvailable.push_back(1);
		lAvailable.push_back(4);
		lAvailable.push_back(7);
		return;
	}
	if(baseClassId == 1)
	{
		lAvailable.push_back(2);
		lAvailable.push_back(3);
		return;
	}	
	if(baseClassId == 2)
	{
		lAvailable.push_back(88);
		return;
	}
	if(baseClassId == 3)
	{
		lAvailable.push_back(89);
		return;
	}
	if(baseClassId == 4)
	{
		lAvailable.push_back(5);
		lAvailable.push_back(6);
		return;
	}
	if(baseClassId == 5)
	{
		lAvailable.push_back(90);
		return;
	}
	if(baseClassId == 6)
	{
		lAvailable.push_back(91);
		return;
	}
	if(baseClassId == 7)
	{
		lAvailable.push_back(8);
		lAvailable.push_back(9);
		return;
	}
	if(baseClassId == 8)
	{
		lAvailable.push_back(93);
		return;
	}
	if(baseClassId == 9)
	{
		lAvailable.push_back(92);
		return;
	}
	if(baseClassId == 10)
	{
		lAvailable.push_back(11);
		lAvailable.push_back(15);
		return;
	}
	if(baseClassId == 11)
	{
		lAvailable.push_back(12);
		lAvailable.push_back(13);
		lAvailable.push_back(14);
		return;
	}
	if(baseClassId == 12)
	{
		lAvailable.push_back(94);
		return;
	}
	if(baseClassId == 13)
	{
		lAvailable.push_back(95);
		return;
	}
	if(baseClassId == 14)
	{
		lAvailable.push_back(96);
		return;
	}
	if(baseClassId == 15)
	{
		lAvailable.push_back(16);
		lAvailable.push_back(17);
		return;
	}
	if(baseClassId == 16)
	{
		lAvailable.push_back(97);
		return;
	}
	if(baseClassId == 17)
	{
		lAvailable.push_back(98);
		return;
	}
	if(baseClassId == 18)
	{
		lAvailable.push_back(19);
		lAvailable.push_back(22);
		return;
	}
	if(baseClassId == 19)
	{
		lAvailable.push_back(20);
		lAvailable.push_back(21);
		return;
	}
	if(baseClassId == 20)
	{
		lAvailable.push_back(99);
		return;
	}
	if(baseClassId == 21)
	{
		lAvailable.push_back(100);
		return;
	}
	if(baseClassId == 22)
	{
		lAvailable.push_back(23);
		lAvailable.push_back(24);
		return;
	}
	if(baseClassId == 23)
	{
		lAvailable.push_back(101);
		return;
	}
	if(baseClassId == 24)
	{
		lAvailable.push_back(102);
		return;
	}
	if(baseClassId == 25)
	{
		lAvailable.push_back(26);
		lAvailable.push_back(29);
		return;
	}
	if(baseClassId == 26)
	{
		lAvailable.push_back(27);
		lAvailable.push_back(28);
		return;
	}
	if(baseClassId == 27)
	{
		lAvailable.push_back(103);
		return;
	}
	if(baseClassId == 28)
	{
		lAvailable.push_back(104);
		return;
	}
	if(baseClassId == 29)
	{
		lAvailable.push_back(30);
		return;
	}
	if(baseClassId == 30)
	{
		lAvailable.push_back(105);
		return;
	}
	if(baseClassId == 31)
	{
		lAvailable.push_back(32);
		lAvailable.push_back(35);
		return;
	}
	if(baseClassId == 32)
	{
		lAvailable.push_back(33);
		lAvailable.push_back(34);
		return;
	}
	if(baseClassId == 33)
	{
		lAvailable.push_back(106);
		return;
	}
	if(baseClassId == 34)
	{
		lAvailable.push_back(107);
		return;
	}
	if(baseClassId == 35)
	{
		lAvailable.push_back(36);
		lAvailable.push_back(37);
		return;
	}
	if(baseClassId == 36)
	{
		lAvailable.push_back(108);
		return;
	}
	if(baseClassId == 37)
	{
		lAvailable.push_back(109);
		return;
	}
	if(baseClassId == 38)
	{
		lAvailable.push_back(39);
		lAvailable.push_back(42);
		return;
	}
	if(baseClassId == 39)
	{
		lAvailable.push_back(40);
		lAvailable.push_back(41);
		return;
	}
	if(baseClassId == 40)
	{
		lAvailable.push_back(110);
		return;
	}
	if(baseClassId == 41)
	{
		lAvailable.push_back(111);
		return;
	}
	if(baseClassId == 42)
	{
		lAvailable.push_back(43);
		return;
	}
	if(baseClassId == 43)
	{
		lAvailable.push_back(112);
		return;
	}
	if(baseClassId == 44)
	{
		lAvailable.push_back(45);
		lAvailable.push_back(47);
		return;
	}
	if(baseClassId == 45)
	{
		lAvailable.push_back(46);
		return;
	}
	if(baseClassId == 46)
	{
		lAvailable.push_back(113);
		return;
	}
	if(baseClassId == 47)
	{
		lAvailable.push_back(48);
		return;
	}
	if(baseClassId == 48)
	{
		lAvailable.push_back(114);
		return;
	}
	if(baseClassId == 49)
	{
		lAvailable.push_back(50);
		return;
	}
	if(baseClassId == 50)
	{
		lAvailable.push_back(51);
		lAvailable.push_back(52);
		return;
	}
	if(baseClassId == 51)
	{
		lAvailable.push_back(115);
		return;
	}
	if(baseClassId == 52)
	{
		lAvailable.push_back(116);
		return;
	}
	if(baseClassId == 53)
	{
		lAvailable.push_back(54);
		lAvailable.push_back(56);
		return;
	}
	if(baseClassId == 54)
	{
		lAvailable.push_back(55);
		return;
	}
	if(baseClassId == 55)
	{
		lAvailable.push_back(117);
		return;
	}
	if(baseClassId == 56)
	{
		lAvailable.push_back(57);
		return;
	}
	if(baseClassId == 57)
	{
		lAvailable.push_back(118);
		return;
	}
}

bool CClassManager::CanChange(User *pUser, int newClass)
{
	CTL;
	if(CCategoryDataDB::IsInCategory(newClass, CCategoryDataDB::FOURTH_CLASS_GROUP))
	{
		if(m_changeEnabled[2])
		{
			if(pUser->pSD->nLevel > 75)
			{
				list<INT32> lClass;
				GetAvailable(pUser->pSD->nClass, lClass);
				for(list<INT32>::iterator Iter = lClass.begin();Iter!=lClass.end();Iter++)
				{
					if((*Iter) == newClass)
					{
						if(m_requiredItemId[2] > 0 && m_requiredItemCount[2] > 0)
						{
							if(!pUser->DeleteItemInInventory(m_requiredItemId[2], m_requiredItemCount[2]))
							{
								//2538	1	a,You don't have enough item(s) to change the class.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
								pUser->pSocket->SendSystemMessage(2538);
								return false;
							}
						}
						return true;
					}
				}
			}else
			{
				//2539	1	a,Your level is too low. Please come back when your level will be 76 or above.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2539);
				return false;
			}
		}else
		{
			//2540	1	a,This class cannot be acquired through Class Manager.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->pSocket->SendSystemMessage(2540);
			return false;
		}
	}else if(CCategoryDataDB::IsInCategory(newClass, CCategoryDataDB::THIRD_CLASS_GROUP))
	{
		if(m_changeEnabled[1])
		{
			if(pUser->pSD->nLevel > 39)
			{
				list<INT32> lClass;
				GetAvailable(pUser->pSD->nClass, lClass);
				for(list<INT32>::iterator Iter = lClass.begin();Iter!=lClass.end();Iter++)
				{
					if((*Iter) == newClass)
					{
						if(m_requiredItemId[1] > 0 && m_requiredItemCount[1] > 0)
						{
							if(!pUser->DeleteItemInInventory(m_requiredItemId[1], m_requiredItemCount[1]))
							{
								//2538	1	a,You don't have enough item(s) to change the class.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
								pUser->pSocket->SendSystemMessage(2538);
								return false;
							}
						}
						return true;
					}
				}
			}else
			{
				//2541	1	a,Your level is too low. Please come back when your level will be 40 or above.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2541);
				return false;
			}
		}else
		{
			//2540	1	a,This class cannot be acquired through Class Manager.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->pSocket->SendSystemMessage(2540);
			return false;
		}
	}else if(CCategoryDataDB::IsInCategory(newClass, CCategoryDataDB::SECOND_CLASS_GROUP))
	{
		if(m_changeEnabled[0])
		{
			if(pUser->pSD->nLevel > 19)
			{
				list<INT32> lClass;
				GetAvailable(pUser->pSD->nClass, lClass);
				for(list<INT32>::iterator Iter = lClass.begin();Iter!=lClass.end();Iter++)
				{
					if((*Iter) == newClass)
					{
						if(m_requiredItemId[0] > 0 && m_requiredItemCount[0] > 0)
						{
							//g_Log.Add(CLog::Blue, "required item[%d][%d]", m_requiredItemId[0], m_requiredItemCount[0] );

							if(!pUser->DeleteItemInInventory(m_requiredItemId[0], m_requiredItemCount[0]))
							{
								//2538	1	a,You don't have enough item(s) to change the class.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
								pUser->pSocket->SendSystemMessage(2538);
								return false;
							}
						}
						return true;
					}
				}
			}else
			{
				//2542	1	a,Your level is too low. Please come back when your level will be 20 or above.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2542);
				return false;
			}
		}else
		{
			//2540	1	a,This class cannot be acquired through Class Manager.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->pSocket->SendSystemMessage(2540);
			return false;
		}
	}

	g_Log.Add(CLog::Error, "[%s] Invalid new_class[%d] user[%S] base_class[%d]", __FUNCTION__, newClass, pUser->pSD->wszName, pUser->pSD->nClass);

	return false;
}

void CClassManager::RequestShowHtml(User *pUser)
{
	if(m_enabled)
	{
		if(pUser->ValidUser())
		{
			if(!CCategoryDataDB::IsInCategory(pUser->pSD->nClass, CCategoryDataDB::FOURTH_CLASS_GROUP))
			{
				list<INT32> lClass;
				GetAvailable(pUser->pSD->nClass, lClass);

				const WCHAR* pFile = 0;
				if(g_HtmlCache.IsEnabled())
				{
					pFile = g_HtmlCache.Get(L"class_manager_show.htm");
				}else
				{
					pFile = g_HTMLCacheManager.GetHTMLFile( L"class_manager_show.htm", pUser->pSD->bLang);
				}

				if(pFile)
				{
					wstring html(pFile);
					size_t templatePos = html.find(L"<?template");
					if(templatePos != wstring::npos)
					{
						size_t templateEndPos = html.find(L"template?>");
						if(templateEndPos != wstring::npos)
						{
							wstring templateLine = html.substr(templatePos + 10, templateEndPos - templatePos - 10);
							wstringstream classStream;
							if(lClass.size() > 0)
							{
								for(list<INT32>::iterator Iter = lClass.begin();Iter!=lClass.end(); Iter++)
								{
									wstring line = templateLine;
									size_t classIdPos = line.find(L"$class_id");
									if(classIdPos != wstring::npos)
									{
										WCHAR wClassId[8];
										_itow_s((*Iter), wClassId, 8, 10);
										line = line.replace(classIdPos, 9, wClassId);
									}
									size_t classNamePos = line.find(L"$class_name");
									if(classNamePos != wstring::npos)
									{
										line = line.replace(classNamePos, 11, GetName(*Iter));
									}

									classStream << line << endl;
								}
							}else
							{
								classStream << L"...";
							}

							html = html.replace(templatePos, (templateEndPos - templatePos + 10), classStream.str());
							PlayerAction::ShowHTML(pUser, L"class_manager_show.htm", html.c_str(), 0);
						}
					}
				}else
				{
					g_Log.Add(CLog::Error, "[%s] Cannot find class_manager_show.htm!", __FUNCTION__);
				}
			}else
			{
				//2543	1	a,You cannot change class anymore!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2543);
			}
		}
	}
}

void CClassManager::RequestChange(User *pUser, int newClass)
{
	if(m_enabled)
	{
		if(pUser->ValidUser())
		{
			if(CanChange(pUser, newClass))
			{
				UINT oldClass = pUser->pSD->nClass;
				PlayerAction::OnClassChange(pUser, newClass);
				g_DB.RequestClassChangedJob(pUser);
				pUser->SocialAction(SocialActionLevelUp);
				//2544	1	a,Congratulation! You've changed your class!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2544);

				SubStackInfo& ssi = pUser->pED->subStackInfo;
				if(ssi.classId[pUser->pSD->nSubClassIndex][0] != UINT_MAX)
				{
					if(ssi.classId[pUser->pSD->nSubClassIndex][0] == oldClass)
					{
						ssi.classId[pUser->pSD->nSubClassIndex][0] = newClass;
					}else
					{
						ssi.classId[pUser->pSD->nSubClassIndex][1] = newClass;
					}
					g_DB.RequestSaveSubStack(pUser->nDBID, pUser->pSD->nSubClassIndex, ssi.classId[pUser->pSD->nSubClassIndex][0], ssi.classId[pUser->pSD->nSubClassIndex][1]);
				}

				if(m_ChangeReward[0].itemId > 0 && m_ChangeReward[0].amount > 0)
				{
					if(CCategoryDataDB::IsInCategory(newClass, CCategoryDataDB::SECOND_CLASS_GROUP))
					{
						pUser->AddItemToInventory(m_ChangeReward[0].itemId, m_ChangeReward[0].amount);
					}
				}
				if(m_ChangeReward[1].itemId > 0 && m_ChangeReward[1].amount > 0)
				{
					if(CCategoryDataDB::IsInCategory(newClass, CCategoryDataDB::THIRD_CLASS_GROUP))
					{
						pUser->AddItemToInventory(m_ChangeReward[1].itemId, m_ChangeReward[1].amount);
					}
				}
				if(m_ChangeReward[2].itemId > 0 && m_ChangeReward[2].amount > 0)
				{
					if(CCategoryDataDB::IsInCategory(newClass, CCategoryDataDB::FOURTH_CLASS_GROUP))
					{
						pUser->AddItemToInventory(m_ChangeReward[2].itemId, m_ChangeReward[2].amount);
					}
				}
			}
		}
	}
}

void CClassManager::RequestNobless(User *pUser)
{
	guard;
	if(m_enabled && m_Nobless)
	{
		if(pUser->pSD->nNobless == 0)
		{
			if(pUser->pSD->nSubClassIndex > 0)
			{
				if(pUser->pSD->nLevel >= 75)
				{
					bool hasPrice = false;
					if(m_NoblessPriceId > 0 && m_NoblessPriceAmount > 0)
					{
						if(pUser->inventory.HaveItemByClassID(m_NoblessPriceId, m_NoblessPriceAmount))
						{
							pUser->DeleteItemInInventory(m_NoblessPriceId, m_NoblessPriceAmount);
							hasPrice = true;
						}
					}
					if(hasPrice || m_NoblessPriceId == 0)
					{
						g_DB.RequestSaveNoblessType(pUser, 1);
						pUser->AddItemToInventory(7694, 1);	//noblesse tiara
						typedef void(*f)(LPBYTE, INT32, bool);
						f(0x431360L)(pUser->pSD->OneTimeQuestBitMap, 247, true);
						g_DB.SaveQuestInfo(pUser);
						//2545	1	a,Congratulations! You've became a noblesse.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->pSocket->SendSystemMessage(2545);
						pUser->PlaySound(L"ItemSound.quest_finish", 1, pUser->nObjectID, (int)pUser->pSD->Pos.x, (int)pUser->pSD->Pos.y, (int)pUser->pSD->Pos.z);
					}else
					{
						//2546	1	a,You don't have enough item(s) to become a nobless.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->pSocket->SendSystemMessage(2546);
					}
				}else
				{
					//2547	1	a,Your subclass have to be at level 75 or above to become a nobless.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->pSocket->SendSystemMessage(2547);
				}
			}else
			{
				//2548	1	a,In order to become a nobless please switch to your subclass.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2548);
			}
		}else
		{
			//2549	1	a,You already have a nobless status.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->pSocket->SendSystemMessage(2549);
		}
	}
	unguard;
}

void CClassManager::RequestSubClass(User *pUser)
{
	guard;
	if(m_enabled && m_SubClass)
	{
		if(pUser->pSD->nLevel >= 75)
		{
			bool hasPrice = false;
			if(m_SubClassPriceId > 0 && m_SubClassPriceAmount > 0)
			{
				if(pUser->DeleteItemInInventory(m_SubClassPriceId, m_SubClassPriceAmount))
				{
					hasPrice = true;
				}
			}
			if(hasPrice || m_SubClassPriceId == 0)
			{
				typedef void(*f)(LPBYTE, INT32, bool);
				f(0x431360L)(pUser->pSD->OneTimeQuestBitMap, 235, true);
				g_DB.SaveQuestInfo(pUser);
				//2550	1	a,Congratulations! You've finished Sub Class Quest.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2550);
			}else
			{
				//2551	1	a,You dont' have enough item(s) to acquire Sub Class Quest.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2551);
			}
		}else
		{
			//2552	1	a,Your level have to be 75 or above to be able to acquire sub class.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->pSocket->SendSystemMessage(2552);
		}
	}
	unguard;
}