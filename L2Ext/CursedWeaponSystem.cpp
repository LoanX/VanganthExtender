#include "StdAfx.h"
#include "CursedWeaponSystem.h"

CCursedWeaponSystem::CCursedWeaponSystem()
{
}

CCursedWeaponSystem::~CCursedWeaponSystem()
{
	for(map<int, CCursedWeapon*>::iterator Iter = m_Weapons.begin();Iter!=m_Weapons.end();Iter++)
	{
		CCursedWeapon* lpWeapon = Iter->second;
		if(lpWeapon)
			delete lpWeapon;
		Iter->second = NULL;
	}
	m_Weapons.clear();
}

void CCursedWeaponSystem::Initialize()
{
	//Read ini file
	g_Log.Add(CLog::Blue, "[%s] loading data.", __FUNCTION__);
	TCHAR File[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,File);
	lstrcat(File, _T("\\CursedW.ini"));

	g_HookManager.WriteJump(0x7FF554, CursedWeaponKarmaFix1, 1);

	TCHAR temp[8190];
	memset(temp, 0, sizeof(temp));
	if(GetPrivateProfileString(_T("Setting"), _T("WeaponID"), NULL, temp, sizeof(temp), File))
	{
		tstringstream sstr;
		sstr << temp;
		int nWeaponID = 0;
		while(sstr >> nWeaponID)
		{
			CCursedWeapon* lpCursed = new CCursedWeapon();
			if(lpCursed)
			{
				g_Log.Add(CLog::Blue, "[%s] loading data for weapon[%d].", __FUNCTION__, nWeaponID);
				lpCursed->Init(nWeaponID);
				m_Weapons.insert(pair<int, CCursedWeapon*>(nWeaponID, lpCursed));
			}
		}
	}
}
bool CCursedWeaponSystem::IsCursedWeapon(CItem *pItem)
{
	if(pItem->IsValidItem())
	{
		try
		{
			int nItemID = pItem->pSID->nItemID;
			if(m_Weapons.find(nItemID) != m_Weapons.end())
			{
				return true;
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
		}
	}
	return false;
}
bool CCursedWeaponSystem::IsCursedWeapon(int nWeaponID)
{
	if(nWeaponID)
	{
		try
		{
			if(m_Weapons.find(nWeaponID) != m_Weapons.end())
			{
				return true;
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
		}
	}
	return false;
}

void CCursedWeaponSystem::OnPickUp(User *pUser, CItem *pItem)
{
	if(pUser->ValidUser() && pItem->IsValidItem())
	{
		if(pUser->pSD->nCursedOwner)
		{
			if(IsCursedWeapon(pItem))
			{
				int nOwnedID = pUser->pSD->nCursedOwner;
				int nWeaponID = pItem->pSID->nItemID;
				pUser->DeleteItemInInventory(nWeaponID, 1);
				map<int, CCursedWeapon*>::iterator Iter = m_Weapons.find(nOwnedID);
				if(Iter!=m_Weapons.end())
				{
					Iter->second->Lock();
					Iter->second->UpdateKillCount(pUser, 30);
					Iter->second->Unlock();
				}
				Iter = m_Weapons.find(nWeaponID);
				if(Iter!=m_Weapons.end())
				{
					CCursedWeapon *pCW = Iter->second;
					//Delete
					pCW->Lock();
					pCW->OnDelete(0);
					pCW->Unlock();
					CSystemMessage msg(CCursedWeapon::$S1_HAS_BEEN_SEALED);
					msg.AddItem(nWeaponID);
					L2Server::BroadcastToAllUser(msg.GetSize(), msg.GetBuff());
				}
			}
		}else
		{
			if(IsCursedWeapon(pItem))
			{
				int nWeaponID = pItem->pSID->nItemID;
				map<int, CCursedWeapon*>::iterator Iter = m_Weapons.find(nWeaponID);
				if(Iter!=m_Weapons.end())
				{
					Iter->second->Lock();
					Iter->second->OnAcquire(pUser, pItem);
					Iter->second->Unlock();
				}
			}
		}
	}
}

void CCursedWeaponSystem::OnPickUp(CPet *pPet, CItem *pItem)
{
	if(pPet->ValidCreature() && IsCursedWeapon(pItem))
	{
		pPet->DropItem(pItem, 1);
	}
}

void CCursedWeaponSystem::OnDie(User *pUser, CCreature *pKiller)
{
	if(pUser->ValidUser() && pKiller->ValidCreature())
	{
		if(pUser->pSD->nCursedOwner)
		{
			g_Logger.Add(L"Cursed Weapon owner[%s][%d] killed by[%s]", pUser->pSD->wszName, pUser->pSD->nCursedOwner, pKiller->pSD->wszName);
			int nWeaponID = pUser->pSD->nCursedOwner;
			map<int, CCursedWeapon*>::iterator Iter = m_Weapons.find(nWeaponID);
			if(Iter!=m_Weapons.end())
			{
				CCursedWeapon *pCW = Iter->second;
				int nRnd = g_Random.RandInt(100);
				if(nRnd < 30)
				{
					//Drop
					pCW->Lock();
					pCW->OnDrop(pUser);
					pCW->Unlock();
				}else
				{
					//Delete
					pCW->Lock();
					pCW->OnDelete(pUser);
					pCW->Unlock();
					CSystemMessage msg(CCursedWeapon::$S1_HAS_BEEN_SEALED);
					msg.AddItem(nWeaponID);
					L2Server::BroadcastToAllUser(msg.GetSize(), msg.GetBuff());
				}
			}
			pUser->pSD->nCursedVictim = 1;
			pUser->pSD->bGuilty = true;
		}
		if(pKiller->ValidUser() && pKiller->pSD->nCursedOwner)
		{
			int nWeaponID = pKiller->pSD->nCursedOwner;
			map<int, CCursedWeapon*>::iterator Iter = m_Weapons.find(nWeaponID);
			if(Iter!=m_Weapons.end())
			{
				CCursedWeapon *pCW = Iter->second;
				pCW->Lock();
				pCW->UpdateKillCount(pKiller->GetUser(), 1);
				pCW->Unlock();
			}
			pUser->pSD->nCursedVictim = 1;
		}
	}
}

void CCursedWeaponSystem::OnEnterWorld(User *pUser)
{
	if(pUser->ValidUser())
	{
		map<int, CCursedWeapon*>::iterator Iter = m_Weapons.begin();
		for(;Iter!=m_Weapons.end();Iter++)
		{
			Iter->second->Lock();
			Iter->second->OnEnterWorld(pUser);
			Iter->second->Unlock();
		}
	}
}

void CCursedWeaponSystem::OnLeaveWorld(User *pUser)
{
	if(pUser->ValidUser() && pUser->pSD->nCursedOwner)
	{
		map<int, CCursedWeapon*>::iterator Iter = m_Weapons.begin();
		for(;Iter!=m_Weapons.end();Iter++)
		{
			Iter->second->Lock();
			if(Iter->second->IsOwner(pUser))
			{
				Iter->second->OnLeaveWorld(pUser);
				Iter->second->Unlock();
				return;
			}
			Iter->second->Unlock();
		}
	}
}

void CCursedWeaponSystem::OnTeleport(User *pUser)
{
	guard;
	if(pUser->ValidUser() && pUser->pSD->nCursedOwner)
	{
		map<int, CCursedWeapon*>::iterator Iter = m_Weapons.find(pUser->pSD->nCursedOwner);
		if(Iter!= m_Weapons.end())
		{
			Iter->second->Lock();
			Iter->second->OnTeleport(pUser);
			Iter->second->Unlock();
		}
	}
	unguard;
}

void CCursedWeaponSystem::OnChangeSubjob(User *pUser)
{
	if(pUser->ValidUser())
	{
		map<int, CCursedWeapon*>::iterator Iter = m_Weapons.begin();
		for(;Iter!=m_Weapons.end();Iter++)
		{
			Iter->second->Lock();
			if(Iter->second->IsOwner(pUser))
			{
				Iter->second->OnChangeSubjob(pUser);
				Iter->second->Unlock();
				return;
			}
			Iter->second->Unlock();
		}
	}
}

void CCursedWeaponSystem::OnNpcDie(CNPC *pNPC, CCreature *pEnemy)
{
	guard;
	if(pNPC->ValidCreature() && !pNPC->IsSummon() && !pNPC->IsPet() && pNPC->pSD->nLevel > 39 && (pEnemy->ValidUser() || pEnemy->IsSummon()))
	{
		if(pNPC->pSD->nRace!=RaceMercenary && pNPC->pSD->nRace!=RaceCastleGuard)
		{
			map<int, CCursedWeapon*>::iterator Iter = m_Weapons.begin();
			for(;Iter!=m_Weapons.end();Iter++)
			{
				Iter->second->Lock();
				if(Iter->second->NotExistsInTheWorld())
				{
					if(Iter->second->GetDropRate() > g_Random.RandDouble(100))
					{
						Iter->second->NpcDrop(pNPC);
						Iter->second->Unlock();
						unguard;
						return;
					}
				}
				Iter->second->Unlock();
			}
		}
	}
	unguard;
}

void CCursedWeaponSystem::TimerExpired()
{
	guard;
	map<int, CCursedWeapon*>::iterator Iter = m_Weapons.begin();
	for(;Iter!=m_Weapons.end();Iter++)
	{
		Iter->second->Lock();
		Iter->second->TimerExpired();
		Iter->second->Unlock();
	}
	unguard;
}
int CCursedWeaponSystem::GetLevel(User *pUser)
{
	guard;

	if(pUser->ValidUser())
	{
		if(pUser->pSD->nCursedOwner)
		{
			int nWeaponID = pUser->pSD->nCursedOwner;
			map<int, CCursedWeapon*>::iterator Iter = m_Weapons.find(nWeaponID);
			if(Iter!=m_Weapons.end())
			{
				int nLevel = 0;
				Iter->second->Lock();
				nLevel = Iter->second->GetLevel();
				Iter->second->Unlock();
				unguard;
				return nLevel;
			}
		}
	}
	unguard;
	return 0;
}

bool CCursedWeaponSystem::WeaponList(CSocket *pSocket)
{
	if(pSocket->ValidUserSocket())
	{
		CPacket packet;
		packet.WriteC(0xFE);
		packet.WriteH(0x45);
		packet.WriteD((DWORD)m_Weapons.size());
		map<int, CCursedWeapon*>::iterator Iter = m_Weapons.begin();
		for(;Iter!=m_Weapons.end();Iter++)
		{
			packet.WriteD(Iter->first);
		}
		pSocket->Send("b", packet.GetSize(), packet.GetBuff());
		return false;
	}else
		return true;
}

bool CCursedWeaponSystem::WeaponLocation(CSocket *pSocket)
{
	guard;

	if(pSocket->ValidUserSocket())
	{
		int weaponCount = 0;
		CHAR buff[8190];
		int len = 0;

		for(map<int, CCursedWeapon*>::iterator Iter = m_Weapons.begin();Iter!=m_Weapons.end();Iter++)
		{
			CCursedWeapon *pCW = Iter->second;
			pCW->Lock();
			if(pCW->NotExistsInTheWorld() == false)
			{
				weaponCount++;
				int itemId = Iter->first;
				int x = 0, y = 0, z = 0;
				BOOL isOnline = pCW->IsOnline();
				if(isOnline)
				{
					if(pCW->HasOwner())
					{
						UINT ownerId = pCW->GetOwnerSID();
						User *pOwner = User::GetUserBySID(&ownerId);
						if(pOwner->ValidUser())
						{
							x = (int)pOwner->pSD->Pos.x;
							y = (int)pOwner->pSD->Pos.y;
							z = (int)pOwner->pSD->Pos.z;
						}
					}else
					{
						pCW->GetXYZ(&x, &y, &z);
					}
				}else
				{
					pCW->GetXYZ(&x, &y, &z);
				}
				len += Assemble(&buff[len], (8190 - len), "ddddd", itemId, isOnline, x, y, z);

			}
			pCW->Unlock();
		}
		pSocket->Send("chdb", 0xFE, 0x46, weaponCount, len, buff);
	}
	unguard;
	return false;
}

bool CCursedWeaponSystem::RequestReset(User *pUser, int nWeaponID)
{
	map<int, CCursedWeapon*>::iterator Iter = m_Weapons.find(nWeaponID);
	if(Iter!=m_Weapons.end())
	{
		Iter->second->Lock();
		Iter->second->Reset(pUser);
		Iter->second->Unlock();
	}
	return false;
}

bool CCursedWeaponSystem::RequestInfo(User *pUser, int nWeaponID)
{
	map<int, CCursedWeapon*>::iterator Iter = m_Weapons.find(nWeaponID);
	if(Iter!=m_Weapons.end())
	{
		Iter->second->Lock();
		Iter->second->SendInfo(pUser);
		Iter->second->Unlock();
	}
	return false;
}