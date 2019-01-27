#include "StdAfx.h"
#include "TeleportBypass.h"
#include "PlayerAction.h"
#include "Geo.h"

CTeleportBypass g_TeleportBypass;

CTeleportBypass::CTeleportBypass()
{
	//aden_town:1500:147450:26741:-2204
};

void CTeleportBypass::Init()
{
	mData.clear();
	ReadData();
	g_Log.Add(CLog::Blue, "[%s] Loaded [%d] location(s).", __FUNCTION__, mData.size());
}

void CTeleportBypass::ReadData()
{
	guard;
	//begin name=giran_town price=1500 x=83400 y=147943 z=-3404 end
	ifstream file("..\\Script\\TeleportData.txt");
	if(file)
	{
		int nTeleports = 0;
		string sLine;
		mData.clear();
		while(getline(file, sLine))
		{
			if(sLine.find("//") == 0)
				continue;
			if(sLine.find("begin") == 0)
			{
				string Name = Parser::ParseOptionString(sLine, "name");
				int price = Parser::ParseOptionInt(sLine, "price");
				int x = Parser::ParseOptionInt(sLine, "x_loc");
				int y = Parser::ParseOptionInt(sLine, "y_loc");
				int z = Parser::ParseOptionInt(sLine, "z_loc");
				int min_level = Parser::ParseOptionInt(sLine, "min_level");
				int max_level = Parser::ParseOptionInt(sLine, "max_level");
				int required_item_id = Parser::ParseOptionInt(sLine, "required_item_id");
				int required_item_count = Parser::ParseOptionInt(sLine, "required_item_count");
				int castle_id = Parser::ParseOptionInt(sLine, "castle_id");
				if(Name.size() > 0)
				{
					try
					{
						TeleportData td;
						td.x = x;
						td.y = y;
						td.z = z;
						td.max_level = max_level;
						td.min_level = min_level;
						td.required_item_count = required_item_count;
						td.required_item_id = required_item_id;
						td.price = price;
						td.castle_id = castle_id;

						WCHAR wTmpName[8192]; memset(wTmpName, 0, sizeof(wTmpName));
						AnsiToUnicode((PCHAR)Name.c_str(), 8190, wTmpName, 8190);
						wstring wName = wTmpName;
						if(mData.find(wName) == mData.end())
						{
							mData.insert(pair<wstring, TeleportData>(wName, td));
						}else
							g_Log.Add(CLog::Error, "[%s] Teleport[%s] already exists. Duplicated entry.", __FUNCTION__, Name.c_str());
					}catch(...)
					{
						g_Log.Add(CLog::Error, "[%s] Exception while loading [%s][%d] teleport!", __FUNCTION__, Name.c_str(), nTeleports);
					}
					nTeleports++;
				}
			}
		}
		g_Log.Add(CLog::Blue, "[%s] Loaded [%d] teleports to database.", __FUNCTION__, mData.size());
	}else
		g_Log.Add(CLog::Error, "[%s] Cannot find file[..\\Script\\TeleportData.txt]", __FUNCTION__);
	unguard;
}

void CTeleportBypass::RequestTeleport(User *pUser, wstring wName)
{
	map<wstring, TeleportData>::iterator Iter = mData.find(wName);
	if(Iter!=mData.end())
	{
		if(!pUser->IsInCombatMode())
		{
			if( pUser->pSD->nLevel < Iter->second.min_level )
			{
				//2558	1	a,Your character level is to low.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2558);
				return;
			}

			if( pUser->pSD->nLevel > Iter->second.max_level )
			{
				//2559	1	a,Your character level is to high.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2559);
				return;
			}
			if(Iter->second.castle_id > 0)
			{
				CResidence *pCastle = g_ResidenceDB.GetResidence(Iter->second.castle_id);
				if(pCastle->IsCastle())
				{
					if(pCastle->IsSiegeTime())
					{
						//2560	1	a,You cannot use this teleport during the castle siege.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->pSocket->SendSystemMessage(2560);
						return;
					}
				}
			}
			if(pUser->olympiadUser.GetState() != 0)
			{
				//2561	1	a,You cannot use this teleport while participing in olympiad games!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2561);
				return;
			}
			pUser->pSD->pExData->TeleportBypassData.Set(wName);
			//ask user
			CPacket pck;
			pck.WriteC(0xED);
			pck.WriteD(MsgID); //msg id 2413	1	You will be moved to ($s1). Do you wish to continue?	4	79	9B	B0	FF			0	0	0	0	0		popup

			pck.WriteD(1); //number of parameters
			pck.WriteD(7); //location
			pck.WriteD(Iter->second.x); // x
			pck.WriteD(Iter->second.y); // x
			pck.WriteD(Iter->second.z); // x
			pck.WriteD(9000); //timeout
			pck.WriteD(0); //dunno
			pUser->GetSocket()->Send("b", pck.GetSize(), pck.GetBuff());
		}else
		{
			//2562	1	a,You cannot use teleport in combat mode!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->pSocket->SendSystemMessage(2562);
		}

	}
}

void CTeleportBypass::Teleport(User *pUser, wstring wName)
{
	map<wstring, TeleportData>::iterator Iter = mData.find(wName);
	if(Iter!=mData.end())
	{
		if(!pUser->IsInCombatMode())
		{
			if( pUser->pSD->nLevel < Iter->second.min_level )
			{
				//2558	1	a,Your character level is to low.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2558);
				return;
			}

			if( pUser->pSD->nLevel > Iter->second.max_level )
			{
				//2559	1	a,Your character level is to high.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2559);
				return;
			}

			if( pUser->inventory.GetAdenaAmount() < Iter->second.price )
			{
				//2563	1	a,You don't have enough adena to pay for our service!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->pSocket->SendSystemMessage(2563);
				return;
			}

			if(Iter->second.required_item_id > 0 && Iter->second.required_item_count > 0)
			{
				if(!pUser->inventory.HaveItemByClassID(Iter->second.required_item_id, Iter->second.required_item_count))
				{
					//2564	1	a,You don't have enough item(s) to pay for our service!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->pSocket->SendSystemMessage(2564);
					return;
				}
				pUser->DeleteItemInInventory(Iter->second.required_item_id, Iter->second.required_item_count);
			}

			if(Iter->second.price > 0)
				pUser->DeleteItemInInventory(57, Iter->second.price);

			pUser->StopMove();
			PlayerAction::OnTeleportToLocation(pUser, Iter->second.x, Iter->second.y, CGeo::GetInstance()->GetZLoc(Iter->second.x, Iter->second.y, Iter->second.z), false);
		}else
		{
			//2562	1	a,You cannot use teleport in combat mode!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->pSocket->SendSystemMessage(2562);
		}
	}else
		g_Log.Add(CLog::Error, "[%s] User[%S] requested invalid teleport name[%S]", __FUNCTION__, pUser->pSD->wszName, wName.c_str());
}