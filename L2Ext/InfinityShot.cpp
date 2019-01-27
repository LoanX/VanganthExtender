#include "stdafx.h"
#include "InfinityShot.h"
#include "CreatureAction.h"

CInfinityShot g_InfinityShot;

void CInfinityShot::LoadINI()
{
	const TCHAR* section = _T("InfinityShot");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	m_AllowSpiritShotInOlympiad = GetPrivateProfileInt(section, _T("AllowSpiritShotInOlympiad"), 1, g_ConfigFile);
	m_AllowBlessedSpiritShotInOlympiad = GetPrivateProfileInt(section, _T("AllowBlessedSpiritShotInOlympiad"), 0, g_ConfigFile);
	m_AllowSoulShotInOlympiad = GetPrivateProfileInt(section, _T("AllowSoulShotInOlympiad"), 1, g_ConfigFile);
}

void CInfinityShot::Init()
{
	m_Enabled = FALSE;
	LoadINI();
}



bool CInfinityShot::AutoOn(User *pUser, int itemId)
{
	guard;

	if(CItem *pItem = pUser->inventory.GetFirstItemByClassID(itemId))
	{
		UINT etcType = pItem->pII->etcType;
		if(etcType == EtcItemInfinitySoulshot || etcType == EtcItemInfinitySpiritshot || etcType == EtcItemInfinityBlessedSpiritshot)
		{
			if(pItem->pII->etcType == EtcItemInfinitySoulshot)
			{
				//check for olympiad
				if(m_AllowSoulShotInOlympiad || !pUser->olympiadUser.IsInside())
				{
					if(pUser->pED->autoInfinitySoulshot[pItem->pII->nCrystalType])
					{
						AutoOff(pUser, pUser->pED->autoInfinitySoulshot[pItem->pII->nCrystalType]);
					}
					pUser->pED->autoInfinitySoulshot[pItem->pII->nCrystalType] = itemId;
				}else
				{
					//1508	1	a,You cannot use that item in a Grand Olympiad Games match.\0	0	79	9B	B0	FF	a,ItemSound3.sys_shortage\0	a,	0	0	0	0	0	a,	a,none\0
					CSystemMessage msg(1508);
					pUser->SendSystemMessage(&msg);
					unguard;
					return true;
				}
			}else if(pItem->pII->etcType == EtcItemInfinitySpiritshot)
			{
				if(m_AllowSpiritShotInOlympiad || !pUser->olympiadUser.IsInside())
				{
					if(pUser->pED->autoInfinitySpiritshot[pItem->pII->nCrystalType])
					{
						AutoOff(pUser, pUser->pED->autoInfinitySpiritshot[pItem->pII->nCrystalType]);
					}
					pUser->pED->autoInfinitySpiritshot[pItem->pII->nCrystalType] = itemId;
				}else
				{
					//1508	1	a,You cannot use that item in a Grand Olympiad Games match.\0	0	79	9B	B0	FF	a,ItemSound3.sys_shortage\0	a,	0	0	0	0	0	a,	a,none\0
					CSystemMessage msg(1508);
					pUser->SendSystemMessage(&msg);
					unguard;
					return true;
				}
			}else if(pItem->pII->etcType == EtcItemInfinityBlessedSpiritshot)
			{
				if(m_AllowBlessedSpiritShotInOlympiad || !pUser->olympiadUser.IsInside())
				{
					if(pUser->pED->autoInfinitySpiritshot[pItem->pII->nCrystalType])
					{
						AutoOff(pUser, pUser->pED->autoInfinitySpiritshot[pItem->pII->nCrystalType]);
					}
					pUser->pED->autoInfinitySpiritshot[pItem->pII->nCrystalType] = itemId;
				}else
				{
					//1508	1	a,You cannot use that item in a Grand Olympiad Games match.\0	0	79	9B	B0	FF	a,ItemSound3.sys_shortage\0	a,	0	0	0	0	0	a,	a,none\0
					CSystemMessage msg(1508);
					pUser->SendSystemMessage(&msg);
					unguard;
					return true;
				}
			}
			//1433	1	a,The automatic use of $s1 has been activated.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			CSystemMessage msg(1433);
			msg.AddItem(itemId);
			pUser->SendSystemMessage(&msg);
			pUser->pSocket->Send("chdd", 0xFE, 0x12, itemId, 1);
			TurnOn(pUser, pItem);

			unguard;
			return true;
		}
	}

	unguard;
	return false;
}

void CInfinityShot::AutoOff(User *pUser)
{
	guard;
	for(UINT n=0;n<6;n++)
	{
		if(pUser->pED->autoInfinitySoulshot[n] != 0)
		{
			//1434	1	a,The automatic use of $s1 has been deactivated.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			CSystemMessage msg(1434);
			msg.AddItem(pUser->pED->autoInfinitySoulshot[n]);
			pUser->SendSystemMessage(&msg);
			pUser->pSocket->Send("chdd", 0xFE, 0x12, pUser->pED->autoInfinitySoulshot[n], 0);
			pUser->pED->autoInfinitySoulshot[n] = 0;
		}
		if(pUser->pED->autoInfinitySpiritshot[n] != 0)
		{
			//1434	1	a,The automatic use of $s1 has been deactivated.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			CSystemMessage msg(1434);
			msg.AddItem(pUser->pED->autoInfinitySpiritshot[n]);
			pUser->SendSystemMessage(&msg);
			pUser->pSocket->Send("chdd", 0xFE, 0x12, pUser->pED->autoInfinitySpiritshot[n], 0);
			pUser->pED->autoInfinitySpiritshot[n] = 0;
		}
	}
	unguard;
}

bool CInfinityShot::AutoOff(User *pUser, int itemId)
{
	guard;
	for(UINT n=0;n<6;n++)
	{
		if(pUser->pED->autoInfinitySoulshot[n] == itemId)
		{
			pUser->pED->autoInfinitySoulshot[n] = 0;
			//1434	1	a,The automatic use of $s1 has been deactivated.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			CSystemMessage msg(1434);
			msg.AddItem(itemId);
			pUser->SendSystemMessage(&msg);
			pUser->pSocket->Send("chdd", 0xFE, 0x12, itemId, 0);

			unguard;
			return true;
		}else if(pUser->pED->autoInfinitySpiritshot[n] == itemId)
		{
			pUser->pED->autoInfinitySpiritshot[n] = 0;
			//1434	1	a,The automatic use of $s1 has been deactivated.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			CSystemMessage msg(1434);
			msg.AddItem(itemId);
			pUser->SendSystemMessage(&msg);
			pUser->pSocket->Send("chdd", 0xFE, 0x12, itemId, 0);

			unguard;
			return true;
		}
	}
	unguard;
	return false;
}

bool CInfinityShot::Activate(User *pUser, CItem *pItem)
{
	guard;
	if(pItem->pII->etcType == EtcItemInfinitySoulshot)
	{
		if(m_AllowSoulShotInOlympiad || !pUser->olympiadUser.IsInside())
		{
			if(pUser->pSD->dSoulshotPower == 0.0)
			{
				if(CSharedItemData *pWD = pUser->GetEquipedWeapon())
				{
					//grade check
					if(CItemInfo *pWI = pWD->GetItemInfo())
					{
						if(pItem->pII->nCrystalType == pWI->nCrystalType)
						{
							if(CSkillInfo *pInfo = g_SkillDB.GetSkillInfo(pItem->pII->itemSkillId, pItem->pII->itemSkillLevel))
							{
								pUser->soulShotInUse = true;
								pUser->soulshotUseTick = GetTickCount();
								pInfo->ActivateSkill(pUser, pUser);
								CHAR buff[512];
								FVector pos = pUser->pSD->Pos;
								int len = Assemble(buff, 512, "cddddddddddd", 0x48, pUser->nObjectID, pUser->nObjectID, pItem->pII->itemSkillId, pItem->pII->itemSkillLevel, 0, 0, (int)pos.x, (int)pos.y, (int)pos.z, 0, 0);
								L2Server::BroadcastToNeighborDist(pUser->nObjectID, &pos, 8000, len, buff);
								//46	1	a,You use $s1.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
								CSystemMessage msg(46);
								msg.AddItem(pItem->pSID->nItemID);
								pUser->SendSystemMessage(&msg);
								unguard;
								return true;
							}
						}else
						{
							//337	1	a,The soulshot you are attempting to use does not match the grade of your equipped weapon.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							CSystemMessage msg(337);
							pUser->SendSystemMessage(&msg);
						}
					}else
					{
						g_Log.Add(CLog::Error, "[%s] cannot find item info for index[%d]!", __FUNCTION__, pWD->nItemIndex);
					}
				}else
				{
					//337	1	a,The soulshot you are attempting to use does not match the grade of your equipped weapon.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					CSystemMessage msg(337);
					pUser->SendSystemMessage(&msg);
				}
			}
		}else
		{
			//1508	1	a,You cannot use that item in a Grand Olympiad Games match.\0	0	79	9B	B0	FF	a,ItemSound3.sys_shortage\0	a,	0	0	0	0	0	a,	a,none\0
			CSystemMessage msg(1508);
			pUser->SendSystemMessage(&msg);
			unguard;
			return false;
		}
	}else if(pItem->pII->etcType == EtcItemInfinitySpiritshot)
	{
		if(m_AllowSpiritShotInOlympiad || !pUser->olympiadUser.IsInside())
		{
			if(pUser->pSD->dSpiritShotPower == 0.0)
			{
				if(CSharedItemData *pWD = pUser->GetEquipedWeapon())
				{
					//grade check
					if(CItemInfo *pWI = pWD->GetItemInfo())
					{
						if(pItem->pII->nCrystalType == pWI->nCrystalType)
						{
							if(CSkillInfo *pInfo = g_SkillDB.GetSkillInfo(pItem->pII->itemSkillId, pItem->pII->itemSkillLevel))
							{
								pUser->spiritShotInUse = true;
								pInfo->ActivateSkill(pUser, pUser);
								CHAR buff[512];
								FVector pos = pUser->pSD->Pos;
								int len = Assemble(buff, 512, "cddddddddddd", 0x48, pUser->nObjectID, pUser->nObjectID, pItem->pII->itemSkillId, pItem->pII->itemSkillLevel, 0, 0, (int)pos.x, (int)pos.y, (int)pos.z, 0, 0);
								L2Server::BroadcastToNeighborDist(pUser->nObjectID, &pos, 8000, len, buff);
								//46	1	a,You use $s1.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
								CSystemMessage msg(46);
								msg.AddItem(pItem->pSID->nItemID);
								pUser->SendSystemMessage(&msg);
								unguard;
								return true;
							}
						}else
						{
							//337	1	a,The soulshot you are attempting to use does not match the grade of your equipped weapon.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							CSystemMessage msg(337);
							pUser->SendSystemMessage(&msg);
						}
					}else
					{
						g_Log.Add(CLog::Error, "[%s] cannot find item info for index[%d]!", __FUNCTION__, pWD->nItemIndex);
					}
				}else
				{
					//337	1	a,The soulshot you are attempting to use does not match the grade of your equipped weapon.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					CSystemMessage msg(337);
					pUser->SendSystemMessage(&msg);
				}
			}
		}else
		{
			//1508	1	a,You cannot use that item in a Grand Olympiad Games match.\0	0	79	9B	B0	FF	a,ItemSound3.sys_shortage\0	a,	0	0	0	0	0	a,	a,none\0
			CSystemMessage msg(1508);
			pUser->SendSystemMessage(&msg);
			unguard;
			return false;
		}
	}else if(pItem->pII->etcType == EtcItemInfinityBlessedSpiritshot)
	{
		if(m_AllowBlessedSpiritShotInOlympiad || !pUser->olympiadUser.IsInside())
		{
			if(pUser->pSD->dSpiritShotPower == 0.0)
			{
				if(CSharedItemData *pWD = pUser->GetEquipedWeapon())
				{
					//grade check
					if(CItemInfo *pWI = pWD->GetItemInfo())
					{
						if(pItem->pII->nCrystalType == pWI->nCrystalType)
						{
							if(CSkillInfo *pInfo = g_SkillDB.GetSkillInfo(pItem->pII->itemSkillId, pItem->pII->itemSkillLevel))
							{
								pUser->spiritShotInUse = true;
								pInfo->ActivateSkill(pUser, pUser);
								CHAR buff[512];
								FVector pos = pUser->pSD->Pos;
								int len = Assemble(buff, 512, "cddddddddddd", 0x48, pUser->nObjectID, pUser->nObjectID, pItem->pII->itemSkillId, pItem->pII->itemSkillLevel, 0, 0, (int)pos.x, (int)pos.y, (int)pos.z, 0, 0);
								L2Server::BroadcastToNeighborDist(pUser->nObjectID, &pos, 8000, len, buff);
								//46	1	a,You use $s1.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
								CSystemMessage msg(46);
								msg.AddItem(pItem->pSID->nItemID);
								pUser->SendSystemMessage(&msg);
								unguard;
								return true;
							}
						}else
						{
							//337	1	a,The soulshot you are attempting to use does not match the grade of your equipped weapon.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							CSystemMessage msg(337);
							pUser->SendSystemMessage(&msg);
						}
					}else
					{
						g_Log.Add(CLog::Error, "[%s] cannot find item info for index[%d]!", __FUNCTION__, pWD->nItemIndex);
					}
				}else
				{
					//337	1	a,The soulshot you are attempting to use does not match the grade of your equipped weapon.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					CSystemMessage msg(337);
					pUser->SendSystemMessage(&msg);
				}
			}
		}else
		{
			//1508	1	a,You cannot use that item in a Grand Olympiad Games match.\0	0	79	9B	B0	FF	a,ItemSound3.sys_shortage\0	a,	0	0	0	0	0	a,	a,none\0
			CSystemMessage msg(1508);
			pUser->SendSystemMessage(&msg);
			unguard;
			return false;
		}
	}
	unguard;
	return false;
}

bool CInfinityShot::TurnOn(User *pUser, CItem *pItem)
{
	return CreatureAction::UseItem(pUser, pItem, FALSE);
}