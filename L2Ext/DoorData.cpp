#include "stdafx.h"
#include "DoorData.h"

CDoorData g_DoorData;

extern BOOL g_DoorInActionTarget[16];

void CDoorData::Init()
{
	m_Enabled = FALSE;
	m_SearchRange = 1400.0;
	Sleep(500);
	const TCHAR* section = _T("DoorData");
	BOOL enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	m_SearchRange = GetPrivateProfileDouble(section, _T("SearchRange"), 1400.0, g_ConfigFile);
	m_BlockRideRange = GetPrivateProfileDouble(section, _T("BlockRideRange"), 0, g_ConfigFile);
	double value = GetPrivateProfileDouble(section, _T("SeeThroughDoorByHp"), 0.0, g_ConfigFile);
	m_AffectCloseCombat = GetPrivateProfileInt(section, _T("AffectCloseCombat"), 0, g_ConfigFile);
	m_Disable2ndSkillGeoCheck = GetPrivateProfileInt(section, _T("Disable2ndSkillGeoCheck"), 0, g_ConfigFile);
	if(value == 0)
	{
		m_MissingHpPercent = 0;
	}else
	{
		m_MissingHpPercent = 1.0 / value;
	}
	if(enabled)
	{
		m_Doors.clear();	// special memory leak to avoid crash in case if some door info would be in use
		LoadData();
		m_Enabled = enabled;
		g_Log.Add(CLog::Blue, "[%s] Loaded [%d] doors.", __FUNCTION__, m_Doors.size());
		//CAttackAction::OnStart
		g_HookManager.WriteCall(0x6A1E0D, CDoorData::OnGeoCanSee);
		g_HookManager.WriteCall(0x6B41D0, CDoorData::OnGeoCanSee);
		if(!m_Disable2ndSkillGeoCheck)
		{
			g_HookManager.WriteCall(0x6B7695, CDoorData::OnGeoCanSee);
		}else
		{
			g_HookManager.WriteCall(0x6B7695, CDoorData::ReturnTrue);
		}
		g_HookManager.WriteCall(0x8C1495, CDoorData::OnGeoCanSee);
	}
}

bool CDoorData::ReturnTrue()
{
	return true;
}

bool CDoorData::OnGeoCanSee(PVOID pInstance, FVector pos1, FVector pos2)
{
	CTL;

	typedef bool (*f)(PVOID, FVector, FVector);
	bool ret = f(0x5CFAC0L)(pInstance, pos1, pos2);

	if(ret)
	{
		ret = g_DoorData.CanSee(pos1, pos2);
	}

	return ret;
}

void CDoorData::LoadData()
{
	CTL;
	
	LPBYTE lpFile = 0;
	UINT size = ReadFileBinary(L"..\\Script\\DoorData.txt", lpFile);

	if(lpFile)
	{
		UINT lineCounter = 0;
		wstringstream file((PWCHAR)&lpFile[2]);
		wstring line;
		while(getline(file, line))
		{
			lineCounter++;
			if(line.find(L"door_begin") == 0)
			{
				//door_begin	[gludio_castle_outter_001]	type=normal_type	editor_id=19210001	open_method=by_npc	height=320	hp=253200	physical_defence=644	magical_defence=518	pos={-18481;113065;-2476}	range={{-18481;113059;-2799};{-18351;113059;-2799};{-18351;113071;-2799};{-18481;113071;-2799}}	door_end
				DoorInfo *pInfo = new DoorInfo();

				wstring name = line;
				
				name = Parser::Replace(name, L'[', L' ');
				name = Parser::Replace(name, L']', L' ');

				wstringstream paramStream(name);
				paramStream >> name >> name;

				pInfo->objectId = Parser::ParseOptionInt(line, L"editor_id");
				wstring pos = Parser::ParseOptionString(line, L"pos");
				FVector position;
				if(pos.size() > 2)
				{
					pos = Parser::Replace(pos, L'{', L' ');
					pos = Parser::Replace(pos, L'}', L' ');
					pos = Parser::Replace(pos, L';', L' ');

					wstringstream posStream(pos);
					posStream >> position.x >> position.y >> position.z;
				}

				wstring range = Parser::ParseOptionString(line, L"range");
				if(range.size() > 2)
				{
					range = Parser::Replace(range, L'{', L' ');
					range = Parser::Replace(range, L'}', L' ');
					range = Parser::Replace(range, L';', L' ');

					wstringstream posStream(range);
					FVector range[4];
					for(UINT n=0;n<4;n++)
					{
						posStream >> range[n].x >> range[n].y >> range[n].z;
					}
					if(pos.size() < 4)
					{
						position = range[0];
						position.z += 200;
					}
					pInfo->doorGeo.Init(range, position, pInfo->objectId);
				}

				if(pInfo->objectId > 0)
				{
					pInfo->pObject = g_DoorDB.GetDoor(name.c_str());
				}

				if(pInfo->objectId && pInfo->pObject)
				{
					bool exist = false;
					//check if duplicated
					for(UINT n=0;n<m_Doors.size();n++)
					{
						if(m_Doors[n]->objectId == pInfo->objectId)
						{
							exist = true;
							g_Log.Add(CLog::Error, "[%s] Duplicated door[%d] - line[%d]!", __FUNCTION__, pInfo->objectId, lineCounter);
							break;
						}
					}
					if(!exist)
					{
						m_Doors.push_back(pInfo);
					}else
					{
						delete pInfo;
					}
				}
			}
		}

		delete [] lpFile;
	}

}

void CDoorData::GatherNearestDoors(FVector l1, std::vector<DoorInfo*> &doors, double range)
{
	CTL;
	
	if(m_Enabled)
	{
		for(UINT n=0;n<m_Doors.size();n++)
		{
			const FVector& pos2 = m_Doors[n]->doorGeo.GetPos();
			if((pos2.x + range) > l1.x && (pos2.x - range) < l1.x)
			{
				if((pos2.y + range) > l1.y && (pos2.y - range) < l1.y)
				{
					if(sqrt(pow((pos2.x-l1.x), 2) + pow((pos2.y-l1.y), 2) + pow((pos2.z-l1.z), 2)) < range)
					{
						doors.push_back(m_Doors[n]);
					}
				}
			}
		}
	}

}
/* Not working properly - it's axis aligned box 
int inline GetIntersection( double fDst1, double fDst2, FVector P1, FVector P2, FVector &Hit)
{
	if ( (fDst1 * fDst2) >= 0.0f) return 0;
	if ( fDst1 == fDst2) return 0; 
	Hit = P1 + (P2-P1) * ( -fDst1/(fDst2-fDst1) );
	return 1;
}

int inline InBox( FVector Hit, FVector B1, FVector B2, const int Axis)
{
	if ( Axis==1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return 1;
	if ( Axis==2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return 1;
	if ( Axis==3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return 1;
	return 0;
}

// returns true if line (L1, L2) intersects with the box (B1, B2)
// returns intersection point in Hit
bool CheckLineBox( FVector B1, FVector B2, FVector L1, FVector L2, FVector &Hit)
{
	if (L2.x < B1.x && L1.x < B1.x) return false;
	if (L2.x > B2.x && L1.x > B2.x) return false;
	if (L2.y < B1.y && L1.y < B1.y) return false;
	if (L2.y > B2.y && L1.y > B2.y) return false;
	if (L2.z < B1.z && L1.z < B1.z) return false;
	if (L2.z > B2.z && L1.z > B2.z) return false;
	if (L1.x > B1.x && L1.x < B2.x &&
		L1.y > B1.y && L1.y < B2.y &&
		L1.z > B1.z && L1.z < B2.z) 
	{
		Hit = L1; 
		return true;
	}
	if ( (GetIntersection( L1.x-B1.x, L2.x-B1.x, L1, L2, Hit) && InBox( Hit, B1, B2, 1 ))
		|| (GetIntersection( L1.y-B1.y, L2.y-B1.y, L1, L2, Hit) && InBox( Hit, B1, B2, 2 )) 
		|| (GetIntersection( L1.z-B1.z, L2.z-B1.z, L1, L2, Hit) && InBox( Hit, B1, B2, 3 )) 
		|| (GetIntersection( L1.x-B2.x, L2.x-B2.x, L1, L2, Hit) && InBox( Hit, B1, B2, 1 )) 
		|| (GetIntersection( L1.y-B2.y, L2.y-B2.y, L1, L2, Hit) && InBox( Hit, B1, B2, 2 )) 
		|| (GetIntersection( L1.z-B2.z, L2.z-B2.z, L1, L2, Hit) && InBox( Hit, B1, B2, 3 )))
	{
		return true;
	}

	return false;
}
*/

bool CDoorData::CanSee(FVector l1, FVector l2)
{
	CTL;

	bool ret = true;
	if(m_Enabled)
	{
		UINT threadIndex = GetThreadIndex();
		if(threadIndex < 16 && !g_DoorInActionTarget[threadIndex])
		{
			vector<DoorInfo*> doors;
			GatherNearestDoors(l1, doors, m_SearchRange);
		//	g_Log.Add(CLog::Blue, "[%s] found [%d] doors", __FUNCTION__, doors.size());
			if(doors.size() > 0)
			{
				FVector hit;
				//check collision for each doors
				for(UINT n=0;n<doors.size();n++)
				{
					if(doors[n]->doorGeo.InLineCollision(l1, l2, hit))
					{
						bool isDamaged = false;
						bool isOpen = false;
						try
						{
							CDoor *pDoor = doors[n]->pObject;
							if(pDoor)
							{
								if(pDoor->pSD->isOpen)
								{
									isOpen = true;
								}else if(pDoor->pSD->breakable)
								{
									if(m_MissingHpPercent > 0)
									{
										if((m_MissingHpPercent * pDoor->pSD->hp) < pDoor->pSD->maxHp)
										{
											isDamaged = true;
										}
									}else if(pDoor->pSD->hp == 0.0)
									{
										isDamaged = true;
									}
								}
							}
						}catch(...)
						{
							g_Log.Add(CLog::Error, "[%s] exception caught door id [%d]!", __FUNCTION__, doors[n]->objectId);
						}
			//			g_Log.Add(CLog::Blue, "[%s] doors detected in (%f, %f, %f) isOpen[%d] isDamaged[%d] id[%d]!", __FUNCTION__, hit.x, hit.y, hit.z, isOpen, isDamaged, doors[n]->pObject->pSD->id);
						if(!isOpen && !isDamaged)
						{
							ret = false;
						}
						break;
					}
				}
			}
		}
	}

	return ret;
}