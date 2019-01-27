#include "StdAfx.h"
#include "CursedWeapon.h"

CCursedWeapon::CCursedWeapon()
{
	//Initialize variables
	InitializeCriticalSection(&lock);
	WeaponID = 0;
	DropTime = 0;
	OwnerDBID = 0;
	OwnerSID = 0;
	KillCount = 0;
	m_DropRate = 0;
	Location[0] = 0;
	Location[1] = 0;
	Location[2] = 0;
}

CCursedWeapon::~CCursedWeapon()
{
	DeleteCriticalSection(&lock);
}

void CCursedWeapon::OnAcquire(User *pUser, CItem *pItem)
{
	if(pUser->ValidUser())
	{
		try
		{
			//Set Owner
			if(DropTime == 0)
			{
				DropTime = time(0);
			}
			pUser->pSD->nCursedOwner = WeaponID;
			OwnerSID = pUser->nObjectID;
			OwnerDBID = pUser->nDBID;
			//set loc
			SetLocation((int)pUser->pSD->Pos.x, (int)pUser->pSD->Pos.y, (int)pUser->pSD->Pos.z);
			CParty *pParty = pUser->GetParty();
			if(pParty)
			{
				pParty->LeaveParty(pUser);
			}
			//Acquire skills
			int nLevel = GetLevel();
			pUser->pED->cursedWeaponId = WeaponID;
			pUser->pED->cursedWeaponLevel = nLevel;
			//Passive
			list<int>::iterator Iter = l_PSkills.begin();
			for(;Iter!=l_PSkills.end();Iter++)
			{
				pUser->AcquireSkill(*Iter, nLevel);
			}
			//Active 
			Iter = l_ASkills.begin();
			for(;Iter!=l_ASkills.end();Iter++)
			{
				pUser->AcquireSkill(*Iter, 1);
			}
			pUser->UseItem(pItem, 1);
			//set karma
			pUser->SetKarma(99999999);
			pUser->DispelAllByGM();
			pUser->ValidateParameters();
			pUser->pSD->fHP = pUser->pSD->MaxHP;
			pUser->pSD->fCP = pUser->pSD->MaxCP;
			pUser->pSD->fMP = pUser->pSD->MaxMP;
			pUser->SendHpInfo();
			SendTimeLeft(pUser);
			Save();
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
		}
		CSystemMessage msg(AN_EVIL_IS_PULSATING_FROM_$S2_IN_$S1);
		msg.AddZone(Location[X], Location[Y], Location[Z]);
		msg.AddText(pUser->pSD->wszName);
		L2Server::BroadcastToAllUser(msg.GetSize(), msg.GetBuff());
	}
}

void CCursedWeapon::OnDelete(User *pUser)
{
	if(pUser->ValidCreature())
	{
		try
		{
			pUser->DeleteItemInInventory(WeaponID, 1);
			pUser->SetKarma(0);
			pUser->pED->cursedWeaponId = 0;
			pUser->pED->cursedWeaponLevel = 0;
			//Delete skills
			//Passive
			list<int>::iterator Iter = l_PSkills.begin();
			for(;Iter!=l_PSkills.end();Iter++)
			{
				pUser->DeleteSkill(*Iter);
			}
			//Active 
			Iter = l_ASkills.begin();
			for(;Iter!=l_ASkills.end();Iter++)
			{
				pUser->DeleteSkill(*Iter);
			}
			pUser->ValidateParameters();
			pUser->pSD->nCursedOwner = 0;
			Delete();
			Save();
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
		}
	}else
	{
		Delete();
		Save();
	}
}

void CCursedWeapon::OnDrop(User *pUser)
{
	if(pUser->ValidCreature())
	{
		try
		{
			CItem *pItem = pUser->GetInventory()->GetFirstItemByClassID(WeaponID);
			if(pItem->IsValidItem())
			{
				g_Log.Add(CLog::Blue, "User[%S] drops cursed weapon[%d]", pUser->pSD->wszName, pUser->pSD->nCursedOwner);
				pUser->DropItem(pItem, 1);
				pUser->SetKarma(0);
				pUser->pED->cursedWeaponId = 0;
				pUser->pED->cursedWeaponLevel = 0;
				//Delete skills
				//Passive
				list<int>::iterator Iter = l_PSkills.begin();
				for(;Iter!=l_PSkills.end();Iter++)
				{
					pUser->DeleteSkill(*Iter);
				}
				//Active 
				Iter = l_ASkills.begin();
				for(;Iter!=l_ASkills.end();Iter++)
				{
					pUser->DeleteSkill(*Iter);
				}
				pUser->ValidateParameters();
				pUser->pSD->nCursedOwner = 0;
				SetLocation((int)pUser->pSD->Pos.x, (int)pUser->pSD->Pos.y, (int)pUser->pSD->Pos.z);
				OwnerSID = 1;
				OwnerDBID = 0;
				KillCount = 0;
				DropTime = time(NULL);
				Save();
				CSystemMessage msg($S2_WAS_DROPPED_IN_THE_$S1_REGION);
				msg.AddZone(Location[X], Location[Y], Location[Z]);
				msg.AddItem(WeaponID);
				BroadcastSpecialEffect();
				L2Server::BroadcastToAllUser(msg.GetSize(), msg.GetBuff());
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
		}
	}
}

void CCursedWeapon::OnEnterWorld(User *pUser)
{
	if(pUser->ValidUser())
	{
		try
		{
			if(OwnerDBID != 0 && pUser->nDBID == OwnerDBID)
			{
				CItem *pItem = pUser->GetInventory()->GetFirstItemByClassID(WeaponID);
				if(pItem->IsValidItem())
				{
					//Set Owner
					pUser->pSD->nCursedOwner = WeaponID;
					OwnerSID = pUser->nObjectID;
					//set loc
					SetLocation((int)pUser->pSD->Pos.x, (int)pUser->pSD->Pos.y, (int)pUser->pSD->Pos.z);
					//Acquire skills
					int nLevel = GetLevel();
					pUser->pED->cursedWeaponId = WeaponID;
					pUser->pED->cursedWeaponLevel = nLevel;
					//Passive
					list<int>::iterator Iter = l_PSkills.begin();
					for(;Iter!=l_PSkills.end();Iter++)
					{
						pUser->AcquireSkill(*Iter, nLevel);
					}
					//Active 
					Iter = l_ASkills.begin();
					for(;Iter!=l_ASkills.end();Iter++)
					{
						pUser->AcquireSkill(*Iter, 1);
					}
					pUser->SetKarma(99999999);
					pUser->ValidateParameters();
					SendTimeLeft(pUser);
					CSystemMessage msg($S2_S_OWNER_HAS_LOGGED_INTO_THE_$S1_REGION);
					msg.AddZone(Location[X], Location[Y], Location[Z]);
					msg.AddItem(WeaponID);
					L2Server::BroadcastToAllUser(msg.GetSize(), msg.GetBuff());
				}else
				{
					pUser->SetKarma(0);
					//Passive
					list<int>::iterator Iter = l_PSkills.begin();
					for(;Iter!=l_PSkills.end();Iter++)
					{
						pUser->DeleteSkill(*Iter);
					}
					//Active 
					Iter = l_ASkills.begin();
					for(;Iter!=l_ASkills.end();Iter++)
					{
						pUser->DeleteSkill(*Iter);
					}
					pUser->ValidateParameters();
					pUser->pSD->nCursedOwner = 0;
					Delete();
					Save();
				}
				
			}
			else
			{
				CItem *pItem = pUser->GetInventory()->GetFirstItemByClassID(WeaponID);
				if(pItem->IsValidItem())
				{
					pUser->DeleteItemInInventory(WeaponID, 1);
					pUser->SetKarma(0);
				}
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
		}
	}
}

void CCursedWeapon::OnLeaveWorld(User *pUser)
{
	if(pUser->ValidUser())
	{
		try
		{
			if(pUser->nDBID == OwnerDBID && pUser->nDBID != 0)
			{
				SetLocation((int)pUser->pSD->Pos.x, (int)pUser->pSD->Pos.y, (int)pUser->pSD->Pos.z);
				OwnerSID = 0;
				CSystemMessage msg($S1_IS_CURRENTLY_ASLEEP);
				msg.AddItem(WeaponID);
				L2Server::BroadcastToAllUser(msg.GetSize(), msg.GetBuff());
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
		}
	}
}

void CCursedWeapon::OnTeleport(User *pUser)
{
	if(pUser->ValidUser())
	{
		try
		{
			SetLocation((int)pUser->pSD->Pos.x, (int)pUser->pSD->Pos.y, (int)pUser->pSD->Pos.z);
			//Send System Message
			CSystemMessage msg(THE_OWNER_OF_$S2_HAS_APPEARED_IN_THE_$S1_REGION);
			msg.AddZone(Location[X], Location[Y], Location[Z]);
			msg.AddItem(WeaponID);
			L2Server::BroadcastToAllUser(msg.GetSize(), msg.GetBuff());
			Save();
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
		}
	}
}

void CCursedWeapon::OnChangeSubjob(User *pUser)
{
	if(pUser->ValidUser())
	{
		try
		{
			//Set Owner
			pUser->pSD->nCursedOwner = WeaponID;
			OwnerSID = pUser->nObjectID;
			//set loc
			SetLocation((int)pUser->pSD->Pos.x, (int)pUser->pSD->Pos.y, (int)pUser->pSD->Pos.z);
			//Acquire skills
			int nLevel = GetLevel();
			//Passive
			list<int>::iterator Iter = l_PSkills.begin();
			for(;Iter!=l_PSkills.end();Iter++)
			{
				pUser->AcquireSkill(*Iter, nLevel);
			}
			//Active 
			Iter = l_ASkills.begin();
			for(;Iter!=l_ASkills.end();Iter++)
			{
				pUser->AcquireSkill(*Iter, 1);
			}
			pUser->SetKarma(99999999);
			pUser->ValidateParameters();
			SendTimeLeft(pUser);
			CSystemMessage msg(AN_EVIL_IS_PULSATING_FROM_$S2_IN_$S1);
			msg.AddZone(Location[X], Location[Y], Location[Z]);
			msg.AddText(pUser->pSD->wszName);
			L2Server::BroadcastToAllUser(msg.GetSize(), msg.GetBuff());
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
		}
	}
}

void CCursedWeapon::UpdateKillCount(User *pUser, int nKillCount)
{
	try
	{
		KillCount += nKillCount;
		DropTime -= 3600;
		if(pUser->ValidUser())
		{
			//Acquire skills
			int nLevel = GetLevel();
			if(pUser->pED->cursedWeaponLevel < nLevel)
			{
				pUser->pED->cursedWeaponLevel = nLevel;
				//send packet
				pUser->SocialAction(SocialActionCursedWeaponLevelUp);
				CSystemMessage msg($S2_S_EVIL_PRESENCE_IS_FELT_IN_$S1);
				msg.AddZone(Location[X], Location[Y], Location[Z]);
				msg.AddItem(WeaponID);
				L2Server::BroadcastToAllUser(msg.GetSize(), msg.GetBuff());
			}
			//Passive
			list<int>::iterator Iter = l_PSkills.begin();
			for(;Iter!=l_PSkills.end();Iter++)
			{
				int nAqLvl = pUser->GetAcquiredSkillLevel(*Iter);
				if(nAqLvl < nLevel)
				{
					pUser->AcquireSkill(*Iter, nLevel);
				}
			}
			//Active 
			Iter = l_ASkills.begin();
			for(;Iter!=l_ASkills.end();Iter++)
			{
				int nAqLvl = pUser->GetAcquiredSkillLevel(*Iter);
				if(nAqLvl < 1)
					pUser->AcquireSkill(*Iter, 1);
			}
			pUser->ValidateParameters();
			SendTimeLeft(pUser);
		}
		Save();
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
	}
}

void CCursedWeapon::Delete()
{
	try
	{
		SetLocation(0, 0, 0);
		OwnerSID = 0;
		OwnerDBID = 0;
		KillCount = 0;
		DropTime = 0;
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
	}
}
int CCursedWeapon::GetLevel()
{
	if(KillCount < 3)
		return 1;
	else if(KillCount < 9)
		return 2;
	else if(KillCount < 18)
		return 3;
	else if(KillCount < 30)
		return 4;
	else if(KillCount < 45)
		return 5;
	else if(KillCount < 63)
		return 6;
	else if(KillCount < 84)
		return 7;
	else if(KillCount < 108)
		return 8;
	else if(KillCount < 135)
		return 9;
	else
		return 10;
}

void CCursedWeapon::SetLocation(int x, int y, int z)
{
	Location[X] = x;
	Location[Y] = y;
	Location[Z] = z;
}

void CCursedWeapon::GetXYZ(LPINT pX, LPINT pY, LPINT pZ)
{
	try
	{
		*pX = Location[X];
		*pY = Location[Y];
		*pZ = Location[Z];
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
	}
}

void CCursedWeapon::Init(int nWeaponID)
{
	TCHAR File[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,File);
	lstrcat(File, _T("\\CursedW.ini"));
	TCHAR AppName[8];
	_itot_s(nWeaponID, AppName, 8, 10);
	//Static data
	WeaponID = nWeaponID;
	ExpireTime = GetPrivateProfileInt(AppName, _T("ExpireTime"), 0, File);
	m_DropRate = GetPrivateProfileDouble(AppName, _T("DropRate"), 0.001, File);
	//Dynamic Data
	OwnerDBID = GetPrivateProfileInt(AppName, _T("OwnerDBID"), 0, File);
	KillCount = GetPrivateProfileInt(AppName, _T("KillCount"), 0, File);
	DropTime = GetPrivateProfileInt(AppName, _T("DropTime"), 0, File);
	Location[X] = GetPrivateProfileInt(AppName, _T("X"), 0, File);
	Location[Y] = GetPrivateProfileInt(AppName, _T("Y"), 0, File);
	Location[Z] = GetPrivateProfileInt(AppName, _T("Z"), 0, File);
	//read skills
	{
		tstringstream sstr;
		TCHAR sSkill[2048];
		memset(sSkill, 0, sizeof(sSkill));
		GetPrivateProfileString(AppName, _T("PassiveSkill"), NULL, sSkill, 2048, File);
		sstr << sSkill;
		int nSkillID = 0;
		while(sstr >> nSkillID)
		{
			l_PSkills.push_back(nSkillID);
		}
	}
	
	{
		tstringstream sstr;
		TCHAR sSkill[2048];
		memset(sSkill, 0, sizeof(sSkill));
		GetPrivateProfileString(AppName, _T("ActiveSkill"), NULL, sSkill, 2048, File);
		
		sstr << sSkill;
		int nSkillID = 0;
		while(sstr >> nSkillID)
		{
			l_ASkills.push_back(nSkillID);
		}
	}
}

void CCursedWeapon::Save()
{
	TCHAR File[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,File);
	lstrcat(File, _T("\\CursedW.ini"));
	TCHAR AppName[8];
	_itot_s(WeaponID, AppName, 8, 10);
	WritePrivateProfileInt(AppName, _T("OwnerDBID"), OwnerDBID, File);
	WritePrivateProfileInt(AppName, _T("X"), Location[X], File);
	WritePrivateProfileInt(AppName, _T("Y"), Location[Y], File);
	WritePrivateProfileInt(AppName, _T("Z"), Location[Z], File);
	WritePrivateProfileInt(AppName, _T("KillCount"), KillCount, File);
	WritePrivateProfileInt(AppName, _T("DropTime"), DropTime, File);
}

void CCursedWeapon::TimerExpired()
{
	try
	{
		if(DropTime!=0)
		{
			time_t Time = time(NULL);
			if( (DropTime+ExpireTime) < Time && OwnerDBID)
			{
				if(OwnerSID > 1)
				{
					//Online
					User *pUser = User::GetUserBySID(&OwnerSID);
					if(pUser->ValidUser())
					{
						OnDelete(pUser);
					}else
					{
						g_Log.Add(CLog::Error, "[%s] Cannot find owner - dbid[%d]", __FUNCTION__, OwnerDBID);
						Delete();
						Save();
					}
				}else
				{
					//Offline
					Delete();
					Save();
				}
				CSystemMessage msg($S1_HAS_DISAPPEARED);
				msg.AddItem(WeaponID);
				L2Server::BroadcastToAllUser(msg.GetSize(), msg.GetBuff());
			}
		}
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
	}
}
bool CCursedWeapon::HasOwner()
{
	if(OwnerDBID)
		return true;
	return false;
}
UINT CCursedWeapon::GetOwnerSID()
{
	if(OwnerSID)
		return OwnerSID;
	else
		return 0;
}
bool CCursedWeapon::NotExistsInTheWorld()
{
	if(!OwnerDBID && !OwnerSID)
		return true;
	return false;
}
double CCursedWeapon::GetDropRate()
{
	return m_DropRate;
}
bool CCursedWeapon::IsOwner(User *pUser)
{
	if(pUser->ValidUser())
	{
		if(pUser->nDBID == OwnerDBID && OwnerDBID != 0)
			return true;
	}
	return false;
}
bool CCursedWeapon::IsOnline()
{
	if(OwnerSID)
		return true;
	else
		return false;
}

void CCursedWeapon::SendTimeLeft(User *pUser)
{
	if(pUser->ValidUser())
	{
		try
		{
			time_t CurrentTime = time(NULL);
			time_t LeftTime = CurrentTime - DropTime;
			LeftTime = ExpireTime - LeftTime;
			if(LeftTime > 3600)
			{
				CSystemMessage msg($S1_HAS_$S2_HOURS_OF_USAGE_TIME_REMAINING);
				msg.AddItem(WeaponID);
				LeftTime /= 3600;
				msg.AddNumber((DWORD)LeftTime);
				pUser->GetSocket()->Send("b", msg.GetSize(), msg.GetBuff());
			}else
			{
				CSystemMessage msg($S1_HAS_$S2_MINUTES_OF_USAGE_TIME_REMAINING);
				msg.AddItem(WeaponID);
				LeftTime /= 60;
				msg.AddNumber((DWORD)LeftTime);
				pUser->GetSocket()->Send("b", msg.GetSize(), msg.GetBuff());
			}
		}catch(...)
		{
			g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
		}
	}
}

void CCursedWeapon::BroadcastSpecialEffect()
{
	char buff[8190];
	int len = Assemble(buff, 8190, "chd", 0xFE, 0x40, 5);
	L2Server::BroadcastToAllUser(len, buff); //RedSky
	char buff2[8190];
	int len2 = Assemble(buff2, 8190, "cddddd", 0xC4, Location[X], Location[Y], Location[Z], 30, 5);
	L2Server::BroadcastToAllUser(len2, buff2); //EarthQuake
}

void CCursedWeapon::NpcDrop(CNPC *pNPC)
{
	try
	{
		pNPC->AddItemToInventory(WeaponID, 1);
		OwnerSID = 1;
		SetLocation((int)pNPC->pSD->Pos.x, (int)pNPC->pSD->Pos.y, (int)pNPC->pSD->Pos.z);
		CSystemMessage msg($S2_WAS_DROPPED_IN_THE_$S1_REGION);
		msg.AddZone(Location[X], Location[Y], Location[Z]);
		msg.AddItem(WeaponID);
		DropTime = time(NULL);
		BroadcastSpecialEffect();
		L2Server::BroadcastToAllUser(msg.GetSize(), msg.GetBuff());
	}catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
	}
}

void CCursedWeapon::Reset(User *pUser)
{
	if(pUser->ValidUser())
	{
		g_Log.Add(CLog::Blue, "[%s] WeaponID[%d] GM[%S]", __FUNCTION__, WeaponID, pUser->pSD->wszName);
		DropTime = 1;
	}
}

void CCursedWeapon::SendInfo(User *pUser)
{
	if(pUser->ValidUser())
	{
		CUserSocket *pSocket = pUser->GetSocket();
		if(pSocket)
		{
			pSocket->SendSystemMessage(L"Cursed Weapon Info:");
			SendTimeLeft(pUser);
			WCHAR wsInfo[4096];
			swprintf_s(wsInfo, 4096, L"Weapon[%d] OwnerDBID[%d] X[%d] Y[%d] Z[%d] KillCount[%d] Level[%d]", WeaponID, OwnerDBID, Location[X], Location[Y], Location[Z], KillCount, GetLevel());
			pSocket->SendSystemMessage(wsInfo);
		}
	}
}