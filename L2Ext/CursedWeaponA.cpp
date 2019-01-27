#include "L2Ext.h"
#include "time.h"

extern Config ConfigFile;
CCursedWeaponData Zariche;
CCursedWeaponData Akamanah;

void CCursedWeaponData::Save()
{
	
	

	if(nWeaponID == 8190)
	{
		ConfigFile.ZaricheData.nDropTime = nDropTime;
		ConfigFile.ZaricheData.nKillCount = nKillCount;
		ConfigFile.ZaricheData.nOwnerDBID = nOwnerDBID;
		ConfigFile.ZaricheData.nX = nX;
		ConfigFile.ZaricheData.nY = nY;
		ConfigFile.ZaricheData.nZ = nZ;
		ConfigFile.SaveConfig();
	}
	else
	{
		ConfigFile.AkamanahData.nDropTime = nDropTime;
		ConfigFile.AkamanahData.nKillCount = nKillCount;
		ConfigFile.AkamanahData.nOwnerDBID = nOwnerDBID;
		ConfigFile.AkamanahData.nX = nX;
		ConfigFile.AkamanahData.nY = nY;
		ConfigFile.AkamanahData.nZ = nZ;
		ConfigFile.SaveConfig();
	}
}

void CCursedWeaponData::Clear()
{
	
	

	nDropTime = 0;
	nKillCount = 0;
	nOwnerDBID = 0;
	nOwnerObjectID = 0;
	nX = 0;
	nY = 0;
	nZ = 0;
	if(nWeaponID == Zariche.nWeaponID)
	{
		ConfigFile.ZaricheData.nDropTime = nDropTime;
		ConfigFile.ZaricheData.nKillCount = nKillCount;
		ConfigFile.ZaricheData.nOwnerDBID = nOwnerDBID;
		ConfigFile.ZaricheData.nX = nX;
		ConfigFile.ZaricheData.nY = nY;
		ConfigFile.ZaricheData.nZ = nZ;
	}else
	{
		ConfigFile.AkamanahData.nDropTime = nDropTime;
		ConfigFile.AkamanahData.nKillCount = nKillCount;
		ConfigFile.AkamanahData.nOwnerDBID = nOwnerDBID;
		ConfigFile.AkamanahData.nX = nX;
		ConfigFile.AkamanahData.nY = nY;
		ConfigFile.AkamanahData.nZ = nZ;
	}
	ConfigFile.SaveConfig();
}

void CursedWeapon::Initialize()
{
	Zariche.Lock();
	Zariche.nWeaponID = 8190;
	Zariche.nSkillID = 3603;
	Zariche.nExpireTime = ConfigFile.ZaricheData.nExpireTime;
	Zariche.nDropRate = ConfigFile.ZaricheData.nDropRate;
	Zariche.nDropTime = ConfigFile.ZaricheData.nDropTime;
	Zariche.nKillCount = ConfigFile.ZaricheData.nKillCount;
	Zariche.nOwnerDBID = ConfigFile.ZaricheData.nOwnerDBID;
	Zariche.nX = ConfigFile.ZaricheData.nX;
	Zariche.nY = ConfigFile.ZaricheData.nY;
	Zariche.nZ = ConfigFile.ZaricheData.nZ;
	Zariche.Unlock();
	Akamanah.Lock();
	Akamanah.nWeaponID = 8689;
	Akamanah.nSkillID = 3629;
	Akamanah.nExpireTime = ConfigFile.AkamanahData.nExpireTime;
	Akamanah.nDropRate = ConfigFile.AkamanahData.nDropRate;
	Akamanah.nDropTime = ConfigFile.AkamanahData.nDropTime;
	Akamanah.nKillCount = ConfigFile.AkamanahData.nKillCount;
	Akamanah.nOwnerDBID = ConfigFile.AkamanahData.nOwnerDBID;
	Akamanah.nX = ConfigFile.AkamanahData.nX;
	Akamanah.nY = ConfigFile.AkamanahData.nY;
	Akamanah.nZ = ConfigFile.AkamanahData.nZ;
	Akamanah.Unlock();
}

bool CursedWeapon::CursedWeaponList(CSocket *pSocket)
{
	pSocket->Send("chddd", 0xFE, 0x45, 2, 8190, 8689);
	return false;
}

bool CursedWeapon::CursedWeaponLocation(CSocket *pSocket)
{
	CPacket ZarichePkt(256);
	CPacket AkamanahPkt(256);
	int nAmount = 0;

	Zariche.Lock();
	if(Zariche.nDropTime > 0)
	{
		nAmount++;
		ZarichePkt.WriteD(Zariche.nWeaponID);
		if(Zariche.nOwnerDBID)
		{
			if(Zariche.nOwnerObjectID)
			{
				ZarichePkt.WriteD(1);
				User *pUser = User::GetUserBySID(&Zariche.nOwnerObjectID);
				if(pUser)
				{
					ZarichePkt.WriteD((INT32)pUser->pSD->Pos.x);
					ZarichePkt.WriteD((INT32)pUser->pSD->Pos.y);
					ZarichePkt.WriteD((INT32)pUser->pSD->Pos.z);
				}else
				{
					ZarichePkt.WriteD(Zariche.nX);
					ZarichePkt.WriteD(Zariche.nY);
					ZarichePkt.WriteD(Zariche.nZ);
				}
			}else
			{
				ZarichePkt.WriteD(0);
				ZarichePkt.WriteD(Zariche.nX);
				ZarichePkt.WriteD(Zariche.nY);
				ZarichePkt.WriteD(Zariche.nZ);
			}
		}
		else
		{
			ZarichePkt.WriteD(1);
			ZarichePkt.WriteD(Zariche.nX);
			ZarichePkt.WriteD(Zariche.nY);
			ZarichePkt.WriteD(Zariche.nZ);
		}
	}
	Zariche.Unlock();

	Akamanah.Lock();
	if(Akamanah.nDropTime > 0)
	{
		nAmount++;
		AkamanahPkt.WriteD(Akamanah.nWeaponID);
		if(Akamanah.nOwnerDBID)
		{
			if(Akamanah.nOwnerObjectID)
			{
				AkamanahPkt.WriteD(1);
				User *pUser = User::GetUserBySID(&Akamanah.nOwnerObjectID);
				if(pUser)
				{
					AkamanahPkt.WriteD((INT32)pUser->pSD->Pos.x);
					AkamanahPkt.WriteD((INT32)pUser->pSD->Pos.y);
					AkamanahPkt.WriteD((INT32)pUser->pSD->Pos.z);
				}else
				{
					AkamanahPkt.WriteD(Akamanah.nX);
					AkamanahPkt.WriteD(Akamanah.nY);
					AkamanahPkt.WriteD(Akamanah.nZ);
				}
			}else
			{
				AkamanahPkt.WriteD(0);
				AkamanahPkt.WriteD(Akamanah.nX);
				AkamanahPkt.WriteD(Akamanah.nY);
				AkamanahPkt.WriteD(Akamanah.nZ);
			}
		}
		else
		{
			AkamanahPkt.WriteD(1);
			AkamanahPkt.WriteD(Akamanah.nX);
			AkamanahPkt.WriteD(Akamanah.nY);
			AkamanahPkt.WriteD(Akamanah.nZ);
		}
	}
	Akamanah.Unlock();

	switch(nAmount)
	{
	case 2:
		{
			pSocket->Send("chdbb", 0xFE, 0x46, nAmount, ZarichePkt.GetSize(), ZarichePkt.buff, AkamanahPkt.GetSize(), AkamanahPkt.buff);
			break;
		}
	case 1:
		{
			if(ZarichePkt.GetSize() > 0)
				pSocket->Send("chdb", 0xFE, 0x46, nAmount, ZarichePkt.GetSize(), ZarichePkt.buff);
			else
				pSocket->Send("chdb", 0xFE, 0x46, nAmount, AkamanahPkt.GetSize(), AkamanahPkt.buff);
			break;
		}
	default:
		{
			pSocket->Send("chd", 0xFE, 0x46, 0);
			break;
		}
	}

	return false;
}

bool CursedWeapon::NpcDropZariche(CNPC *pNPC, CCreature *pKiller)
{
	
	

	if(pNPC->isNPC() && (pKiller->ValidUser() || pKiller->isSummon()))
	{
		Zariche.Lock();
		if(Zariche.nDropTime == 0)
		{
			int nChance = L2Server::Rnd(100000);
			if(nChance < Zariche.nDropRate)
			{
				pNPC->AddItemToInventory(Zariche.nWeaponID, 1);
				Zariche.nDropTime = time(NULL);
				Zariche.nX = (int)pNPC->pSD->Pos.x;
				Zariche.nY = (int)pNPC->pSD->Pos.y;
				Zariche.nZ = (int)pNPC->pSD->Pos.z;
				
				CLog::Add(CLog::System_blue, "[Ext] NPC[%ls] has dropped zariche in %d/%d/%d .", pNPC->pSD->wszName, Zariche.nX, Zariche.nY, Zariche.nZ);
				
				L2Server::BroadcastToAllUser("cdddddddd", 0x64, 1815, 2, 7,	Zariche.nX, Zariche.nY, Zariche.nZ, 3, Zariche.nWeaponID); //SendInfo that zariche has been dropped
				L2Server::BroadcastToAllUser("chd", 0xFE, 0x40, 5); //RedSky
				L2Server::BroadcastToAllUser("cddddd", 0xc4, Zariche.nX, Zariche.nY, Zariche.nZ, 30, 5); //EarthQuake
				Zariche.Unlock();
				return true;
			}
		}
		Zariche.Unlock();
	}
	return false;
}

bool CursedWeapon::NpcDropAkamanah(CNPC *pNPC, CCreature *pKiller)
{
	
	

	if(pNPC->isNPC() && (pKiller->ValidUser() || pKiller->isSummon()))
	{
		Akamanah.Lock();
		if(Akamanah.nDropTime == 0)
		{
			int nChance = L2Server::Rnd(100000);
			if(nChance < Akamanah.nDropRate)
			{
				pNPC->AddItemToInventory(Akamanah.nWeaponID, 1);
				Akamanah.nDropTime = time(NULL);
				Akamanah.nX = (int)pNPC->pSD->Pos.x;
				Akamanah.nY = (int)pNPC->pSD->Pos.y;
				Akamanah.nZ = (int)pNPC->pSD->Pos.z;
				
				CLog::Add(CLog::System_blue, "[Ext] NPC[%ls] has dropped akamanah in %d/%d/%d .", pNPC->pSD->wszName, Akamanah.nX, Akamanah.nY, Akamanah.nZ);
				
				L2Server::BroadcastToAllUser("cdddddddd", 0x64, 1815, 2, 7,	Akamanah.nX, Akamanah.nY, Akamanah.nZ, 3, Akamanah.nWeaponID); //SendInfo that zariche has been dropped
				L2Server::BroadcastToAllUser("chd", 0xFE, 0x40, 5); //RedSky
				L2Server::BroadcastToAllUser("cddddd", 0xc4, Akamanah.nX, Akamanah.nY, Akamanah.nZ, 30, 5); //EarthQuake
				Akamanah.Unlock();
				return true;
			}
		}
		Akamanah.Unlock();
	}
	return false;
}

void CursedWeapon::PickUp(User *pUser, CItem *pItem)
{
	
	

	if(pUser->ValidUser() && pItem)
	{
		switch(pItem->pSID->nItemID)
		{
		case 8190:
			{
				if(pUser->pSD->nCursedOwner)
				{
					pUser->DropItem(pItem, 1);
					return;
				}
				pUser->UseItem(pItem, 1);
				pUser->SetKarma(999999999); //999kk
				pUser->AcquireSkill(Zariche.nSkillID, 1);
				pUser->DispelAllByGM();
				pUser->ValidateParameters();
				pUser->pSD->fHP = pUser->pSD->MaxHP;
				pUser->pSD->fCP = pUser->pSD->MaxCP;
				pUser->pSD->fMP = pUser->pSD->MaxMP;
				pUser->pSD->nCursedOwner = 8190;
				pUser->SendHpInfo();

				CParty *pParty = pUser->GetParty();
				if(pParty)
					pParty->LeaveParty(pUser);

				Zariche.Lock();
				Zariche.nOwnerDBID = pUser->nDBID;
				Zariche.nOwnerObjectID = pUser->nObjectID;
				Zariche.nX = (int)pUser->pSD->Pos.x;
				Zariche.nY = (int)pUser->pSD->Pos.y;
				Zariche.nZ = (int)pUser->pSD->Pos.z;
				Zariche.nDropTime = time(NULL);
				Zariche.Save();
				Zariche.Unlock();

				CLog::Add(CLog::System_blue, "[Ext]Char[%ls] has picked up Zariche in %d/%d/%d .", pUser->pSD->wszName, Zariche.nX, Zariche.nY, Zariche.nZ);
				L2Server::BroadcastToAllUser("cdddddddd", 0x64, 1816, 2, 7, Zariche.nX, Zariche.nY, Zariche.nZ, 3, Zariche.nWeaponID); //The owner of $s2 has appeared in the $s1 region.

				break;
			}
		case 8689:
			{
				if(pUser->pSD->nCursedOwner)
				{
					pUser->DropItem(pItem, 1);
					return;
				}
				pUser->UseItem(pItem, 1);
				pUser->SetKarma(999999999); //999kk
				pUser->AcquireSkill(Akamanah.nSkillID, 1);
				pUser->DispelAllByGM();
				pUser->ValidateParameters();
				pUser->pSD->fHP = pUser->pSD->MaxHP;
				pUser->pSD->fCP = pUser->pSD->MaxCP;
				pUser->pSD->fMP = pUser->pSD->MaxMP;
				pUser->pSD->nCursedOwner = 8689;
				pUser->SendHpInfo();

				CParty *pParty = pUser->GetParty();
				if(pParty)
					pParty->LeaveParty(pUser);

				Akamanah.Lock();
				Akamanah.nOwnerDBID = pUser->nDBID;
				Akamanah.nOwnerObjectID = pUser->nObjectID;
				Akamanah.nX = (int)pUser->pSD->Pos.x;
				Akamanah.nY = (int)pUser->pSD->Pos.y;
				Akamanah.nZ = (int)pUser->pSD->Pos.z;
				Akamanah.nDropTime = time(NULL);
				Akamanah.Save();
				Akamanah.Unlock();

				CLog::Add(CLog::System_blue, "[Ext]Char[%ls] has picked up Akamanah in %d/%d/%d .", pUser->pSD->wszName, Akamanah.nX, Akamanah.nY, Akamanah.nZ);
				L2Server::BroadcastToAllUser("cdddddddd", 0x64, 1816, 2, 7, Akamanah.nX, Akamanah.nY, Akamanah.nZ, 3, Akamanah.nWeaponID); //The owner of $s2 has appeared in the $s1 region.

				break;
			}
		default:
			{
				return;
				break;
			}
		}
	}
}

void CursedWeapon::PickUp(CPet *pPet, CItem *pItem)
{
	if(pPet && pItem)
	{
		if(pItem->pSID->nItemID == Zariche.nWeaponID || pItem->pSID->nItemID == Akamanah.nWeaponID)
			pPet->DropItem(pItem, 1);
	}
}

void CursedWeapon::OwnerDie(User *pUser, CCreature *pKiller)
{
	
	

	if(pUser->ValidUser())
	{
		if(pUser->pSD->nCursedOwner)
		{
			int nWeaponID = 0;
			int nSkillID = 0;
			if(pUser->pSD->nCursedOwner == 8190)
			{
				Zariche.Lock();
				nSkillID = Zariche.nSkillID;
				nWeaponID = Zariche.nWeaponID;
				Zariche.Clear();
				Zariche.Save();
				Zariche.Unlock();
			}else
			if(pUser->pSD->nCursedOwner == 8689)
			{
				Akamanah.Lock();
				nSkillID = Akamanah.nSkillID;
				nWeaponID = Akamanah.nWeaponID;
				Akamanah.Clear();
				Akamanah.Save();
				Akamanah.Unlock();
			}
			CItem *pItem = pUser->GetInventory()->GetFirstItemByClassID(nWeaponID);
			if(pItem)
			{
				pUser->pCC->DoNothing();
				pUser->DeleteSkill(nSkillID);
				pUser->SetKarma(0);
				pUser->ValidateParameters();

				if(50 < L2Server::Rnd(100))
				{
					pUser->DeleteItemInInventory(nWeaponID, 1);
					L2Server::BroadcastToAllUser("cdddd", 0x64, 1822, 1, 3, nWeaponID); //zariche has been sealed
				}else
				{
					pUser->DropItem(pItem, 1);
					if(nWeaponID == 8190)
					{
						Zariche.Lock();
						Zariche.nX = (int)pUser->pSD->Pos.x;
						Zariche.nY = (int)pUser->pSD->Pos.y;
						Zariche.nZ = (int)pUser->pSD->Pos.z;
						Zariche.nDropTime = time(NULL);
						Zariche.Unlock();
					}else
					{
						Akamanah.Lock();
						Akamanah.nX = (int)pUser->pSD->Pos.x;
						Akamanah.nY = (int)pUser->pSD->Pos.y;
						Akamanah.nZ = (int)pUser->pSD->Pos.z;
						Akamanah.nDropTime = time(NULL);
						Akamanah.Unlock();
					}
					L2Server::BroadcastToAllUser("cdddddddd", 0x64, 1815, 2, 7, (int)pUser->pSD->Pos.x, (int)pUser->pSD->Pos.y, (int)pUser->pSD->Pos.z, 3, nWeaponID); //SendInfo that zariche has been dropped
					L2Server::BroadcastToAllUser("chd", 0xFE, 0x40, 5); //RedSky
					L2Server::BroadcastToAllUser("cddddd", 0xc4, (int)pUser->pSD->Pos.x, (int)pUser->pSD->Pos.y, (int)pUser->pSD->Pos.z, 30, 5); //EarthQuake
				}
				pUser->pSD->nCursedOwner = 0;
			}else
				CLog::Add(CLog::ErrorLog, "[%s]Coulnt find item in user[%ls] inventory", __FUNCTION__, pUser->pSD->wszName);
		}
	}
}

void CursedWeapon::TimerExpired()
{
	
	

	DWORD nTime = time(NULL);
	Zariche.Lock();
	if((nTime > (Zariche.nDropTime + Zariche.nExpireTime)) && (Zariche.nDropTime != 0))
	{
		if(Zariche.nOwnerObjectID > 0)
		{
			User *pUser = User::GetUserBySID(&Zariche.nOwnerObjectID);
			if(pUser->ValidUser())
			{
				pUser->pCC->DoNothing();
				pUser->DeleteSkill(Zariche.nSkillID);
				pUser->SetKarma(0);
				pUser->ValidateParameters();
				pUser->DeleteItemInInventory(Zariche.nWeaponID, 1);
				pUser->pSD->nCursedOwner = 0;
				CLog::Add(CLog::System_blue, "[Ext]Demonic Sword of Zariche has disappeared.");
				L2Server::BroadcastToAllUser("cdddd", 0x64, 1818, 1, 3, Zariche.nWeaponID);
			}else
				CLog::Add(CLog::ErrorLog, "[%s][%d]Couldnt find user.", __FUNCTION__, __LINE__);
		}else
		{
			CLog::Add(CLog::System_blue, "[Ext]Demonic Sword of Zariche has disappeared.");
			L2Server::BroadcastToAllUser("cdddd", 0x64, 1818, 1, 3, Zariche.nWeaponID);
		}
		Zariche.Clear();
		Zariche.Save();
	}
	Zariche.Unlock();
	
	Akamanah.Lock();
	if((nTime > (Akamanah.nDropTime + Akamanah.nExpireTime)) && (Akamanah.nDropTime != 0))
	{
		if(Akamanah.nOwnerObjectID > 0)
		{
			User *pUser = User::GetUserBySID(&Akamanah.nOwnerObjectID);
			if(pUser->ValidUser())
			{
				pUser->pCC->DoNothing();
				pUser->DeleteSkill(Akamanah.nSkillID);
				pUser->SetKarma(0);
				pUser->ValidateParameters();
				pUser->DeleteItemInInventory(Akamanah.nWeaponID, 1);
				pUser->pSD->nCursedOwner = 0;
				CLog::Add(CLog::System_blue, "[Ext]Demonic Duals Akamanah have disappeared.");
				L2Server::BroadcastToAllUser("cdddd", 0x64, 1818, 1, 3, Akamanah.nWeaponID);
			}else
				CLog::Add(CLog::ErrorLog, "[%s][%d]Couldnt find user.", __FUNCTION__, __LINE__);
		}else
		{
			CLog::Add(CLog::System_blue, "[Ext]Demonic Duals Akamanah have disappeared.");
			L2Server::BroadcastToAllUser("cdddd", 0x64, 1818, 1, 3, Akamanah.nWeaponID);
		}
		Akamanah.Clear();
		Akamanah.Save();
		CLog::Add(CLog::System_blue, "[%s] saving data", __FUNCTION__);
	}
	Akamanah.Unlock();
}

void CursedWeapon::Victim(User *pUser, CCreature *pKiller)
{
	if(pKiller->ValidUser() && pUser->ValidUser())
	{
		if(pKiller->pSD->nCursedOwner)
		{
			if(pKiller->pSD->nCursedOwner == Zariche.nWeaponID)
			{
				Zariche.Lock();
				Zariche.nKillCount++;
				Zariche.nDropTime-=3600;
				Zariche.Save();
				Zariche.Unlock();
				SendTimeLeft(pKiller->CastUser()->GetSocket(), 8190);
			}else
			{
				Akamanah.Lock();
				Akamanah.nKillCount++;
				Akamanah.nDropTime-=3600;
				Akamanah.Save();
				Akamanah.Unlock();
				SendTimeLeft(pKiller->CastUser()->GetSocket(), 8689);
			}
			ValidateLevel(pKiller->CastUser(), pKiller->pSD->nCursedOwner);
			pUser->pSD->nCursedVictim = 1;
		}
		if(pUser->pSD->nCursedOwner)
			pKiller->pSD->nCursedKiller = 1;
	}else
		if(pKiller->isSummon())
		{
			if(pUser->pSD->nCursedOwner)
			{
				pKiller->pSD->nCursedKiller = 1;
				User *pMaster = pKiller->CastSummon()->GetMaster();
				if(pMaster)
					pMaster->pSD->nCursedKiller = 1;
			}
		}
}

void CursedWeapon::ValidateLevel(User *pUser, int nWeaponID)
{
	if(nWeaponID == Zariche.nWeaponID)
	{
		int nLevel = 1;
		for(int n=1;n<10;n++)
		{
			if(Zariche.nKillCount >= ConfigFile.ZaricheData.nLevelUp[n])
				nLevel++;
		}
		pUser->AcquireSkill(Zariche.nSkillID, nLevel);
		pUser->ValidateParameters();
	}else
	if(nWeaponID == Akamanah.nWeaponID)
	{
		int nLevel = 1;
		for(int n=1;n<10;n++)
		{
			if(Akamanah.nKillCount >= ConfigFile.AkamanahData.nLevelUp[n])
				nLevel++;
		}
		pUser->AcquireSkill(Akamanah.nSkillID, nLevel);
		pUser->ValidateParameters();
	}
}

void CursedWeapon::LogIn(User *pUser)
{
	
	

	if(pUser->ValidUser())
	{
		CItem *pItem = pUser->GetInventory()->GetFirstItemByClassID(8190);
		if(pItem)
		{
			Zariche.Lock();
			if(pUser->nDBID == Zariche.nOwnerDBID)
			{
				ValidateLevel(pUser, 8190);
				Zariche.nOwnerObjectID = pUser->nObjectID;
				pUser->pSD->nCursedOwner = 8190;
				L2Server::BroadcastToAllUser("cdddddddd", 0x64, 1817, 2, 7, (int)pUser->pSD->Pos.x, (int)pUser->pSD->Pos.y, (int)pUser->pSD->Pos.z, 3, 8190); //zariche owner has logged in.
			}else
				pUser->DeleteItemInInventory(8190, 1);
			Zariche.Unlock();
		}
		pItem = pUser->GetInventory()->GetFirstItemByClassID(8689);
		if(pItem)
		{
			Akamanah.Lock();
			if(pUser->nDBID == Akamanah.nOwnerDBID)
			{
				ValidateLevel(pUser, 8689);
				Akamanah.nOwnerObjectID = pUser->nObjectID;
				pUser->pSD->nCursedOwner = 8689;
				L2Server::BroadcastToAllUser("cdddddddd", 0x64, 1817, 2, 7, (int)pUser->pSD->Pos.x, (int)pUser->pSD->Pos.y, (int)pUser->pSD->Pos.z, 3, 8689); //akamanah owner has logged in.
			}else
				pUser->DeleteItemInInventory(8689, 1);
			Akamanah.Unlock();
		}
	}
}

void CursedWeapon::LogOut(User *pUser)
{
	
	

	if(pUser->ValidUser())
	{
		Zariche.Lock();
		if(pUser->nDBID == Zariche.nOwnerDBID)
		{
			Zariche.nOwnerObjectID = 0;
			Zariche.nX = (int)pUser->pSD->Pos.x;
			Zariche.nY = (int)pUser->pSD->Pos.y;
			Zariche.nZ = (int)pUser->pSD->Pos.z;
			L2Server::BroadcastToAllUser("cdddd", 0x64, 1820, 1, 3, 8190); //$s1 is currently asleep
			Zariche.Save();
		}
		Zariche.Unlock();
		Akamanah.Lock();
		if(pUser->nDBID == Akamanah.nOwnerDBID)
		{
			Akamanah.nOwnerObjectID = 0;
			Akamanah.nX = (int)pUser->pSD->Pos.x;
			Akamanah.nY = (int)pUser->pSD->Pos.y;
			Akamanah.nZ = (int)pUser->pSD->Pos.z;
			L2Server::BroadcastToAllUser("cdddd", 0x64, 1820, 1, 3, 8689); //$s1 is currently asleep
			Akamanah.Save();
		}
		Akamanah.Unlock();
	}
}

void CursedWeapon::SendTimeLeft(CSocket *pSocket, int nWeaponID)
{
	DWORD nCurrent = time(NULL);
	Zariche.Lock();
	if(nWeaponID == Zariche.nWeaponID)
	{
		DWORD nLeft = nCurrent - Zariche.nDropTime;
		nLeft = Zariche.nExpireTime - nLeft;
		if(nLeft > 3600)
		{
			nLeft /= 3600;
			pSocket->Send("cdddddd", 0x64, 1813, 2, 3, 8190, 1, nLeft);
		}else
		{
			nLeft /= 60;
			pSocket->Send("cdddddd", 0x64, 1814, 2, 3, 8190, 1, nLeft);
		}
	}
	Zariche.Unlock();
	Akamanah.Lock();
	if(nWeaponID == Akamanah.nWeaponID)
	{
		DWORD nLeft = nCurrent - Akamanah.nDropTime;
		nLeft = Akamanah.nExpireTime - nLeft;
		if(nLeft > 3600)
		{
			nLeft /= 3600;
			pSocket->Send("cdddddd", 0x64, 1813, 2, 3, 8689, 1, nLeft);
		}else
		{
			nLeft /= 60;
			pSocket->Send("cdddddd", 0x64, 1814, 2, 3, 8689, 1, nLeft);
		}
	}
	Akamanah.Unlock();
}

int CursedWeapon::GetCursedWeaponLevel(User *pUser)
{
	
	

	if(pUser->ValidUser())
	{
		if(pUser->pSD->nCursedOwner)
		{
			if(pUser->pSD->nCursedOwner == 8190)
			{
				int nLevel = 1;
				Zariche.Lock();
				for(int n=1;n<10;n++)
				{
					if(Zariche.nKillCount >= ConfigFile.ZaricheData.nLevelUp[n])
						nLevel++;
				}
				Zariche.Unlock();
				return nLevel;
			}else if(pUser->pSD->nCursedOwner == 8689)
			{
				int nLevel = 1;
				Akamanah.Lock();
				for(int n=1;n<10;n++)
				{
					if(Akamanah.nKillCount >= ConfigFile.AkamanahData.nLevelUp[n])
						nLevel++;
				}
				Akamanah.Unlock();
				return nLevel;
			}
			return 0;
		}else
			return 0;
	}else
		return 0;
}

void CursedWeapon::GetZaricheInfo(CSocket *pSocket)
{
	
	

	wstring wsStr;
	Zariche.Lock();
	if(Zariche.nOwnerDBID)
	{
		wsStr = L"OwnerDBID: ";
		WCHAR wszVal[256];
		_itow(Zariche.nOwnerDBID, wszVal, 10);
		wsStr += wszVal;
		wsStr += L" ";
		if(Zariche.nOwnerObjectID)
		{
			User *pUser = User::GetUserBySID(&Zariche.nOwnerObjectID);
			if(pUser->ValidUser())
			{
				wsStr += L"Name: ";
				wsStr += pUser->pSD->wszName;
			}
		}
		pSocket->SendSystemMessage((PWCHAR)wsStr.c_str());
	}
	Zariche.Unlock();
}

void CursedWeapon::GetAkamanahInfo(CSocket *pSocket)
{
	
	

	wstring wsStr;
	Akamanah.Lock();
	if(Akamanah.nOwnerDBID)
	{
		wsStr = L"OwnerDBID: ";
		WCHAR wszVal[256];
		_itow(Akamanah.nOwnerDBID, wszVal, 10);
		wsStr += wszVal;
		wsStr += L" ";
		if(Akamanah.nOwnerObjectID)
		{
			User *pUser = User::GetUserBySID(&Akamanah.nOwnerObjectID);
			if(pUser->ValidUser())
			{
				wsStr += L"Name: ";
				wsStr += pUser->pSD->wszName;
			}
		}
		pSocket->SendSystemMessage((PWCHAR)wsStr.c_str());
	}
	Akamanah.Unlock();
}

void CursedWeapon::ResetAkamanah(CSocket *pSocket)
{
	
	

	Akamanah.Lock();
	if(Akamanah.nDropTime)
		Akamanah.nDropTime -= Akamanah.nExpireTime;
	Akamanah.Unlock();

	pSocket->SendSystemMessage(L"Akamanah has been reseted.");
}

void CursedWeapon::ResetZariche(CSocket *pSocket)
{
	
	

	Zariche.Lock();
	if(Zariche.nDropTime)
		Zariche.nDropTime -= Zariche.nExpireTime;
	Zariche.Unlock();
	pSocket->SendSystemMessage(L"Zariche has been reseted.");
}