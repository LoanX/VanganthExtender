#include "stdafx.h"
#include "ItemAutoConsume.h"

CItemAutoConsume g_ItemAutoConsume;

CItemAutoConsume::CItemAutoConsume() : m_enabled(FALSE)
{
}

CItemAutoConsume::~CItemAutoConsume()
{
}

void CItemAutoConsume::Init()
{
	m_enabled = FALSE;
	m_items.clear();
	LoadData();
	if(m_enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] Feature is enabled. Loaded [%d] items.", __FUNCTION__, m_items.size());
	}
}

void CItemAutoConsume::LoadData()
{	
	guard;
	wstring filew = ReadFileW(L"..//Script//ItemAutoConsume.txt");
	if(filew.size() > 0)
	{
		wstringstream file(filew);
		wstring line;
		while(getline(file, line))
		{
			if(line.size() > 4)
			{
				if( line[0] == L'/' || line[0] == L';' )
					continue;

				if( line.find(L"setting_begin") == 0 )
				{
					m_enabled = Parser::ParseOptionInt(line, L"enabled", 0);
					
					continue;
				}
				if( line.find(L"item_begin") == 0 )
				{
					AutoConsumeInfo* pAci = new AutoConsumeInfo();
//item_begin	item_id=20424	consume_type=hp	activate_on_missing_percent=30	activate_on_missing_fixed=500	reuse_delay=800	required_level=60	can_use_in_olympiad=0	item_end					
					pAci->itemId = Parser::ParseOptionInt(line, L"item_id");
					wstring sType = Parser::ParseOptionString(line, L"consume_type", L"None");
					if(sType == L"hp")
						pAci->type = AutoConsumeHp;
					else if(sType == L"mp")
						pAci->type = AutoConsumeMp;
					else if(sType == L"cp")
						pAci->type = AutoConsumeCp;
					else if(sType == L"cp_adv")
						pAci->type = AutoConsumeCpAdv;
					else
						pAci->type = AutoConsumeNone;
					pAci->activateOnMissingPercent = static_cast<double>(Parser::ParseOptionInt(line, L"activate_on_missing_percent", INT_MAX));
					pAci->activateOnMissingFixed = static_cast<double>(Parser::ParseOptionInt(line, L"activate_on_missing_fixed", INT_MAX));
					pAci->reuseDelay = Parser::ParseOptionInt(line, L"reuse_delay", 0);
					pAci->requiredLevel = Parser::ParseOptionInt(line, L"required_level", 0);
					if(pAci->itemId > 0 && pAci->type != AutoConsumeNone)
					{
						m_items.insert(pair<INT32, AutoConsumeInfo*>(pAci->itemId, pAci));
					}
				}
			}
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Cannot find ..//Script//ItemAutoConsume.txt !", __FUNCTION__);
	}
	unguard;
}

BOOL CItemAutoConsume::IsAutoConsume(INT32 itemId)
{
	guard;
	if(m_enabled)
	{
		map<INT32, AutoConsumeInfo*>::iterator Iter = m_items.find(itemId);
		if(Iter!=m_items.end())
		{
			unguard;
			return true;
		}
	}
	unguard;
	return false;
}

void CItemAutoConsume::TurnOn(User *pUser, INT32 itemId)
{
	guard;
	if(m_enabled)
	{
		if(pUser->ValidUser() && pUser->pSD->nAlive && pUser->IsItemUsable())
		{
			if(pUser->IsNowTrade())
				pUser->TradeCancel();

			map<INT32, AutoConsumeInfo*>::iterator Iter = m_items.find(itemId);
			if(Iter!=m_items.end())
			{
				if(CItem *pItem = pUser->inventory.GetFirstItemByClassID(itemId))
				{
					AutoConsumeInfo *pInfo = Iter->second;
					if(pInfo->canUseInOlympiad)
					{
						if(pUser->olympiadUser.IsInside())
						{
							pUser->SendSystemMessage(L"You cannot perform this action while fighting in olympiad.");
							unguard;
							return;
						}
					}
					
					if(pInfo->requiredLevel <= pUser->pSD->nLevel)
					{
						pUser->pED->lock.Enter();
						if(pInfo->type == AutoConsumeHp)
						{
							if(pUser->pED->autoConsumeHp.itemId)
							{
								TurnOff(pUser, pUser->pED->autoConsumeHp.itemId, FALSE);
							}
							pUser->pED->autoConsumeHp.itemId = itemId;
							pUser->pED->autoConsumeHp.lastUseTick = 0;
							pUser->pED->autoConsumeHp.pInfo = pInfo;
							
						}else if(pInfo->type == AutoConsumeMp)
						{
							if(pUser->pED->autoConsumeMp.itemId)
							{
								TurnOff(pUser, pUser->pED->autoConsumeMp.itemId, FALSE);
							}
							pUser->pED->autoConsumeMp.itemId = itemId;
							pUser->pED->autoConsumeMp.lastUseTick = 0;
							pUser->pED->autoConsumeMp.pInfo = pInfo;
							
						}else if(pInfo->type == AutoConsumeCp)
						{
							if(pUser->pED->autoConsumeCp.itemId)
							{
								TurnOff(pUser, pUser->pED->autoConsumeCp.itemId, FALSE);
							}
							pUser->pED->autoConsumeCp.itemId = itemId;
							pUser->pED->autoConsumeCp.lastUseTick = 0;
							pUser->pED->autoConsumeCp.pInfo = pInfo;
							
						}else if(pInfo->type == AutoConsumeCpAdv)
						{
							if(pUser->pED->autoConsumeCpAdv.itemId)
							{
								TurnOff(pUser, pUser->pED->autoConsumeCpAdv.itemId, FALSE);
							}
							pUser->pED->autoConsumeCpAdv.itemId = itemId;
							pUser->pED->autoConsumeCpAdv.lastUseTick = 0;
							pUser->pED->autoConsumeCpAdv.pInfo = pInfo;
							
						}

						pUser->pED->lock.Leave();
						pUser->pSocket->Send("chdd", 0xFE, 0x12, itemId, 1);
						CSystemMessage msg(1433);
						msg.AddItem(itemId);
						pUser->SendSystemMessage(&msg);
					}else
					{
						pUser->pSocket->SendSystemMessage(L"Your level is too low to perform this action.");
					}
				}
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Not registered item[%d] - user[%S] !", __FUNCTION__, itemId, pUser->pSD->wszName);
			}
		}
	}
	unguard;
}

void CItemAutoConsume::TurnOff(User *pUser, INT32 itemId, BOOL useLock, BOOL outOfPots)
{
	guard;

	if(m_enabled)
	{
		if(pUser->ValidUser())
		{
			if(itemId == 0)
			{
				if(useLock)
					pUser->pED->lock.Enter();

				if(pUser->pED->autoConsumeHp.itemId > 0)
				{
					pUser->pSocket->Send("chdd", 0xFE, 0x12, pUser->pED->autoConsumeHp.itemId, 0);
					pUser->pED->autoConsumeHp.itemId = 0;
					pUser->pED->autoConsumeHp.lastUseTick = 0;
					pUser->pED->autoConsumeHp.pInfo = 0;
				}

				if(pUser->pED->autoConsumeMp.itemId > 0)
				{
					pUser->pSocket->Send("chdd", 0xFE, 0x12, pUser->pED->autoConsumeMp.itemId, 0);
					pUser->pED->autoConsumeMp.itemId = 0;
					pUser->pED->autoConsumeMp.lastUseTick = 0;
					pUser->pED->autoConsumeMp.pInfo = 0;
				}

				if(pUser->pED->autoConsumeCp.itemId > 0)
				{
					pUser->pSocket->Send("chdd", 0xFE, 0x12, pUser->pED->autoConsumeCp.itemId, 0);
					pUser->pED->autoConsumeCp.itemId = 0;
					pUser->pED->autoConsumeCp.lastUseTick = 0;
					pUser->pED->autoConsumeCp.pInfo = 0;
				}

				if(pUser->pED->autoConsumeCpAdv.itemId > 0)
				{
					pUser->pSocket->Send("chdd", 0xFE, 0x12, pUser->pED->autoConsumeCpAdv.itemId, 0);
					pUser->pED->autoConsumeCpAdv.itemId = 0;
					pUser->pED->autoConsumeCpAdv.lastUseTick = 0;
					pUser->pED->autoConsumeCpAdv.pInfo = 0;
				}

				if(useLock)
					pUser->pED->lock.Leave();
				unguard;
				return;
			}

			map<INT32, AutoConsumeInfo*>::iterator Iter = m_items.find(itemId);
			if(Iter!=m_items.end())
			{
				AutoConsumeInfo *pInfo = Iter->second;
				
				if(useLock)
					pUser->pED->lock.Enter();

				if(pInfo->type == AutoConsumeHp)
				{
					if(pUser->pED->autoConsumeHp.itemId == itemId)
					{
						pUser->pED->autoConsumeHp.itemId = 0;
						pUser->pED->autoConsumeHp.lastUseTick = 0;
						pUser->pED->autoConsumeHp.pInfo = 0;
					}					
				}else if(pInfo->type == AutoConsumeMp)
				{
					if(pUser->pED->autoConsumeMp.itemId == itemId)
					{
						pUser->pED->autoConsumeMp.itemId = 0;
						pUser->pED->autoConsumeMp.lastUseTick = 0;
						pUser->pED->autoConsumeMp.pInfo = 0;
					}					
				}else if(pInfo->type == AutoConsumeCp)
				{
					if(pUser->pED->autoConsumeCp.itemId == itemId)
					{
						pUser->pED->autoConsumeCp.itemId = 0;
						pUser->pED->autoConsumeCp.lastUseTick = 0;
						pUser->pED->autoConsumeCp.pInfo = 0;
					}					
				}else if(pInfo->type == AutoConsumeCpAdv)
				{
					if(pUser->pED->autoConsumeCpAdv.itemId == itemId)
					{
						pUser->pED->autoConsumeCpAdv.itemId = 0;
						pUser->pED->autoConsumeCpAdv.lastUseTick = 0;
						pUser->pED->autoConsumeCpAdv.pInfo = 0;
					}					
				}
				
				if(useLock)
					pUser->pED->lock.Leave();

				pUser->pSocket->Send("chdd", 0xFE, 0x12, itemId, 0);

				if(outOfPots)
				{
					CSystemMessage msg(1435);
					msg.AddItem(itemId);
					pUser->SendSystemMessage(&msg);
				}else
				{
					CSystemMessage msg(1434);
					msg.AddItem(itemId);
					pUser->SendSystemMessage(&msg);
				}

				
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Not registered item[%d] - user[%S] !", __FUNCTION__, itemId, pUser->pSD->wszName);
			}
		}
	}
	unguard;
}

void CItemAutoConsume::OnTiggerHp(User *pUser)
{
	guard;
	if(m_enabled)
	{
		if(pUser->ValidUser())
		{		
			if(!pUser->canUseItem)
			{
				unguard;
				return;
			}

			if(!pUser->canDamageHP)
			{
				unguard;
				return;
			}

			if(!pUser->pSD->nAlive)
			{
				unguard;
				return;
			}

			INT32 itemId = 0;
			UINT lastUseTick = 0;
			AutoConsumeInfo *pInfo = 0;
			pUser->pED->lock.Enter();
			itemId = pUser->pED->autoConsumeHp.itemId;
			if(itemId > 0)
			{
				lastUseTick = pUser->pED->autoConsumeHp.lastUseTick;
				pInfo = pUser->pED->autoConsumeHp.pInfo;
			}
			pUser->pED->lock.Leave();
			if(itemId > 0 && pInfo)
			{
				if(!pInfo->canUseInOlympiad)
				{
					if(pUser->olympiadUser.IsInside())
					{
						pUser->SendSystemMessage(L"You cannot perform this action while fighting in olympiad.");
						unguard;
						return;
					}
				}

				if(CItem *pItem = pUser->inventory.GetFirstItemByClassID(itemId))
				{
					double fixedMissing = pUser->pSD->MaxHP - pUser->pSD->fHP;
					double percentMissing = fixedMissing / pUser->pSD->MaxHP * 100;
					if(fixedMissing >= pInfo->activateOnMissingFixed)
					{
						if( GetTickCount() > (lastUseTick + pInfo->reuseDelay) )
						{
							if(pUser->UseItem(pItem, 0))
							{
								pUser->pED->autoConsumeHp.lastUseTick = GetTickCount();
								if(pInfo->reuseDelay)
								{
									pUser->AddTimer(pInfo->reuseDelay, VALIDATE_AUTO_HP_TIMER);
								}else
								{
									pUser->AddTimer(250, VALIDATE_AUTO_HP_TIMER);
								}
							}
						}else if( GetTickCount () < lastUseTick)
						{
							if(pUser->UseItem(pItem, 0))
							{
								pUser->pED->autoConsumeHp.lastUseTick = GetTickCount();
								if(pInfo->reuseDelay)
								{
									pUser->AddTimer(pInfo->reuseDelay, VALIDATE_AUTO_HP_TIMER);
								}else
								{
									pUser->AddTimer(250, VALIDATE_AUTO_HP_TIMER);
								}
							}
						}
					}else if(percentMissing >= pInfo->activateOnMissingPercent)
					{
						if( GetTickCount() > (lastUseTick + pInfo->reuseDelay) )
						{
							if(pUser->UseItem(pItem, 0))
							{
								pUser->pED->autoConsumeHp.lastUseTick = GetTickCount();
								if(pInfo->reuseDelay)
								{
									pUser->AddTimer(pInfo->reuseDelay, VALIDATE_AUTO_HP_TIMER);
								}else
								{
									pUser->AddTimer(250, VALIDATE_AUTO_HP_TIMER);
								}
							}
						}else if( GetTickCount () < lastUseTick)
						{
							if(pUser->UseItem(pItem, 0))
							{
								pUser->pED->autoConsumeHp.lastUseTick = GetTickCount();
								if(pInfo->reuseDelay)
								{
									pUser->AddTimer(pInfo->reuseDelay, VALIDATE_AUTO_HP_TIMER);
								}else
								{
									pUser->AddTimer(250, VALIDATE_AUTO_HP_TIMER);
								}
							}
						}
					}
				}else
				{
					TurnOff(pUser, itemId, TRUE, TRUE);
				}
			}

		}
	}
	unguard;
}

void CItemAutoConsume::OnTiggerCp(User *pUser)
{
	guard;
	if(m_enabled)
	{
		if(pUser->ValidUser())
		{
			if(!pUser->canUseItem)
			{
				unguard;
				return;
			}

			if(!pUser->canDamageHP)
			{
				unguard;
				return;
			}

			if(!pUser->pSD->nAlive)
			{
				unguard;
				return;
			}

			INT32 itemId = 0;
			UINT lastUseTick = 0;
			AutoConsumeInfo *pInfo = 0;
			pUser->pED->lock.Enter();
			itemId = pUser->pED->autoConsumeCp.itemId;
			if(itemId > 0)
			{
				lastUseTick = pUser->pED->autoConsumeCp.lastUseTick;
				pInfo = pUser->pED->autoConsumeCp.pInfo;
			}
			pUser->pED->lock.Leave();
			if(itemId > 0 && pInfo)
			{
				if(!pInfo->canUseInOlympiad)
				{
					if(pUser->olympiadUser.IsInside())
					{
						pUser->SendSystemMessage(L"You cannot perform this action while fighting in olympiad.");
						unguard;
						return;
					}
				}
				if(CItem *pItem = pUser->inventory.GetFirstItemByClassID(itemId))
				{
					double fixedMissing = pUser->pSD->MaxCP - pUser->pSD->fCP;
					double percentMissing = fixedMissing / pUser->pSD->MaxCP * 100;
					if(fixedMissing >= pInfo->activateOnMissingFixed)
					{
						if( GetTickCount() > (lastUseTick + pInfo->reuseDelay) )
						{
							if(pUser->UseItem(pItem, 0))
							{
								pUser->pED->autoConsumeCp.lastUseTick = GetTickCount();
								if(pInfo->reuseDelay)
								{
									pUser->AddTimer(pInfo->reuseDelay, VALIDATE_AUTO_CP_TIMER);
								}else
								{
									pUser->AddTimer(250, VALIDATE_AUTO_CP_TIMER);
								}
							}
						}else if( GetTickCount () < lastUseTick)
						{
							if(pUser->UseItem(pItem, 0))
							{
								pUser->pED->autoConsumeCp.lastUseTick = GetTickCount();
								if(pInfo->reuseDelay)
								{
									pUser->AddTimer(pInfo->reuseDelay, VALIDATE_AUTO_CP_TIMER);
								}else
								{
									pUser->AddTimer(250, VALIDATE_AUTO_CP_TIMER);
								}
							}
						}
					}else if(percentMissing >= pInfo->activateOnMissingPercent)
					{
						if( GetTickCount() > (lastUseTick + pInfo->reuseDelay) )
						{
							if(pUser->UseItem(pItem, 0))
							{
								pUser->pED->autoConsumeCp.lastUseTick = GetTickCount();
								if(pInfo->reuseDelay)
								{
									pUser->AddTimer(pInfo->reuseDelay, VALIDATE_AUTO_CP_TIMER);
								}else
								{
									pUser->AddTimer(250, VALIDATE_AUTO_CP_TIMER);
								}
							}
						}else if( GetTickCount () < lastUseTick)
						{
							if(pUser->UseItem(pItem, 0))
							{
								pUser->pED->autoConsumeCp.lastUseTick = GetTickCount();
								if(pInfo->reuseDelay)
								{
									pUser->AddTimer(pInfo->reuseDelay, VALIDATE_AUTO_CP_TIMER);
								}else
								{
									pUser->AddTimer(250, VALIDATE_AUTO_CP_TIMER);
								}
							}
						}
					}
				}else
				{
					TurnOff(pUser, itemId, TRUE, TRUE);
				}
			}

		}
	}
	unguard;
}

void CItemAutoConsume::OnTiggerCpAdv(User *pUser)
{
	guard;
	if(m_enabled)
	{
		if(pUser->ValidUser())
		{
			if(!pUser->canUseItem)
			{
				unguard;
				return;
			}

			if(!pUser->canDamageHP)
			{
				unguard;
				return;
			}

			if(!pUser->pSD->nAlive)
			{
				unguard;
				return;
			}

			INT32 itemId = 0;
			UINT lastUseTick = 0;
			AutoConsumeInfo *pInfo = 0;
			pUser->pED->lock.Enter();
			itemId = pUser->pED->autoConsumeCpAdv.itemId;
			if(itemId > 0)
			{
				lastUseTick = pUser->pED->autoConsumeCpAdv.lastUseTick;
				pInfo = pUser->pED->autoConsumeCpAdv.pInfo;
			}
			pUser->pED->lock.Leave();
			if(itemId > 0 && pInfo)
			{
				if(!pInfo->canUseInOlympiad)
				{
					if(pUser->olympiadUser.IsInside())
					{
						pUser->SendSystemMessage(L"You cannot perform this action while fighting in olympiad.");
						unguard;
						return;
					}
				}
				if(CItem *pItem = pUser->inventory.GetFirstItemByClassID(itemId))
				{
					double fixedMissing = pUser->pSD->MaxCP - pUser->pSD->fCP;
					double percentMissing = fixedMissing / pUser->pSD->MaxCP * 100;
					if(fixedMissing >= pInfo->activateOnMissingFixed)
					{
						if( GetTickCount() > (lastUseTick + pInfo->reuseDelay) )
						{
							if(pUser->UseItem(pItem, 0))
							{
								pUser->pED->autoConsumeCpAdv.lastUseTick = GetTickCount();
								if(pInfo->reuseDelay)
								{
									pUser->AddTimer(pInfo->reuseDelay, VALIDATE_AUTO_CP_ADV_TIMER);
								}else
								{
									pUser->AddTimer(250, VALIDATE_AUTO_CP_ADV_TIMER);
								}
							}
						}else if( GetTickCount () < lastUseTick)
						{
							if(pUser->UseItem(pItem, 0))
							{
								pUser->pED->autoConsumeCpAdv.lastUseTick = GetTickCount();
								if(pInfo->reuseDelay)
								{
									pUser->AddTimer(pInfo->reuseDelay, VALIDATE_AUTO_CP_ADV_TIMER);
								}else
								{
									pUser->AddTimer(250, VALIDATE_AUTO_CP_ADV_TIMER);
								}
							}
						}
					}else if(percentMissing >= pInfo->activateOnMissingPercent)
					{
						if( GetTickCount() > (lastUseTick + pInfo->reuseDelay) )
						{
							if(pUser->UseItem(pItem, 0))
							{
								pUser->pED->autoConsumeCpAdv.lastUseTick = GetTickCount();
								if(pInfo->reuseDelay)
								{
									pUser->AddTimer(pInfo->reuseDelay, VALIDATE_AUTO_CP_ADV_TIMER);
								}else
								{
									pUser->AddTimer(250, VALIDATE_AUTO_CP_ADV_TIMER);
								}
							}
						}else if( GetTickCount () < lastUseTick)
						{
							if(pUser->UseItem(pItem, 0))
							{
								pUser->pED->autoConsumeCpAdv.lastUseTick = GetTickCount();
								if(pInfo->reuseDelay)
								{
									pUser->AddTimer(pInfo->reuseDelay, VALIDATE_AUTO_CP_ADV_TIMER);
								}else
								{
									pUser->AddTimer(250, VALIDATE_AUTO_CP_ADV_TIMER);
								}
							}
						}
					}
				}else
				{
					TurnOff(pUser, itemId, TRUE, TRUE);
				}
			}

		}
	}
	unguard;
}

void CItemAutoConsume::OnTiggerMp(User *pUser)
{
	guard;
	if(m_enabled)
	{
		if(pUser->ValidUser())
		{
			if(!pUser->canUseItem)
			{
				unguard;
				return;
			}

			if(!pUser->canDamageMP)
			{
				unguard;
				return;
			}

			if(!pUser->pSD->nAlive)
			{
				unguard;
				return;
			}

			INT32 itemId = 0;
			UINT lastUseTick = 0;
			AutoConsumeInfo *pInfo = 0;
			pUser->pED->lock.Enter();
			itemId = pUser->pED->autoConsumeMp.itemId;
			if(itemId > 0)
			{
				lastUseTick = pUser->pED->autoConsumeMp.lastUseTick;
				pInfo = pUser->pED->autoConsumeMp.pInfo;
			}
			pUser->pED->lock.Leave();
			if(itemId > 0 && pInfo)
			{
				if(!pInfo->canUseInOlympiad)
				{
					if(pUser->olympiadUser.IsInside())
					{
						pUser->SendSystemMessage(L"You cannot perform this action while fighting in olympiad.");
						unguard;
						return;
					}
				}
				if(CItem *pItem = pUser->inventory.GetFirstItemByClassID(itemId))
				{
					double fixedMissing = pUser->pSD->MaxMP - pUser->pSD->fMP;
					double percentMissing = fixedMissing / pUser->pSD->MaxMP * 100;
					if(fixedMissing >= pInfo->activateOnMissingFixed)
					{
						if( GetTickCount() > (lastUseTick + pInfo->reuseDelay) )
						{
							if(pUser->UseItem(pItem, 0))
							{
								pUser->pED->autoConsumeMp.lastUseTick = GetTickCount();
								if(pInfo->reuseDelay)
								{
									pUser->AddTimer(pInfo->reuseDelay, VALIDATE_AUTO_MP_TIMER);
								}else
								{
									pUser->AddTimer(250, VALIDATE_AUTO_MP_TIMER);
								}
							}
						}else if( GetTickCount () < lastUseTick)
						{
							if(pUser->UseItem(pItem, 0))
							{
								pUser->pED->autoConsumeMp.lastUseTick = GetTickCount();
								if(pInfo->reuseDelay)
								{
									pUser->AddTimer(pInfo->reuseDelay, VALIDATE_AUTO_MP_TIMER);
								}else
								{
									pUser->AddTimer(250, VALIDATE_AUTO_MP_TIMER);
								}
							}
						}
					}else if(percentMissing >= pInfo->activateOnMissingPercent)
					{
						if( GetTickCount() > (lastUseTick + pInfo->reuseDelay) )
						{
							if(pUser->UseItem(pItem, 0))
							{
								pUser->pED->autoConsumeMp.lastUseTick = GetTickCount();
								if(pInfo->reuseDelay)
								{
									pUser->AddTimer(pInfo->reuseDelay, VALIDATE_AUTO_MP_TIMER);
								}else
								{
									pUser->AddTimer(250, VALIDATE_AUTO_MP_TIMER);
								}
							}
						}else if( GetTickCount () < lastUseTick)
						{
							if(pUser->UseItem(pItem, 0))
							{
								pUser->pED->autoConsumeHp.lastUseTick = GetTickCount();
								if(pInfo->reuseDelay)
								{
									pUser->AddTimer(pInfo->reuseDelay, VALIDATE_AUTO_MP_TIMER);
								}else
								{
									pUser->AddTimer(250, VALIDATE_AUTO_MP_TIMER);
								}
							}
						}
					}
				}else
				{
					TurnOff(pUser, itemId, TRUE, TRUE);
				}
			}

		}
	}
	unguard;
}