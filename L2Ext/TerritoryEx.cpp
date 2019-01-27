#include "StdAfx.h"
#include "TerritoryEx.h"
#include "KillDb.h"

CTerritoryEx::CTerritoryEx(): m_Type(TERRITORY_NONE), m_Extend(false)
{
	m_MinPos.x = static_cast<double>(INT_MAX);
	m_MinPos.y = static_cast<double>(INT_MAX);
	m_MinPos.z = static_cast<double>(INT_MAX);
	m_MaxPos.x = static_cast<double>(INT_MIN);
	m_MaxPos.y = static_cast<double>(INT_MIN);
	m_MaxPos.z = static_cast<double>(INT_MIN);
}

CNickNameChangeTerritory* CTerritoryEx::SafeCastNickNameChange()
{ 
	if(this)
	{
		if(m_Type == NICKNAME_CHANGE_ZONE)
			return (CNickNameChangeTerritory*)this;
	}
	return NULL;
}

CBlockItemsTerritory* CTerritoryEx::SafeCastBlockItems()
{
	if(this)
	{
		if(m_Type == BLOCK_ITEMS_ZONE)
			return (CBlockItemsTerritory*)this;
	}
	return NULL;
}

CBlockSkillsTerritory* CTerritoryEx::SafeCastBlockSkills()
{ 
	if(this)
	{
		if(m_Type == BLOCK_SKILLS_ZONE)
			return (CBlockSkillsTerritory*)this;
	}
	return NULL;
}

CRaceChangeTerritory* CTerritoryEx::SafeCastRaceChange()
{
	if(this)
	{
		if(m_Type == RACE_CHANGE_ZONE)
		{
			return (CRaceChangeTerritory*)this;
		}
	}
	return NULL;
}

CRequiredItemTerritory* CTerritoryEx::SafeCastRequiredItem()
{
	if(this)
	{
		if(m_Type == REQUIRED_ITEM_ZONE)
		{
			return (CRequiredItemTerritory*)this;
		}
	}
	return 0;
}

CInstanceTerritory* CTerritoryEx::SafeCastInstance()
{
	if(this)
	{
		if(m_Type == INSTANCE_ZONE)
		{
			return (CInstanceTerritory*)this;
		}
	}
	return 0;
}

CBlockChatTerritory* CTerritoryEx::SafeCastBlockChat()
{
	if(this)
	{
		if(m_Type == BLOCK_CHAT_ZONE)
		{
			return (CBlockChatTerritory*)this;
		}
	}
	return 0;
}

CPvPRewardTerritory* CTerritoryEx::SafeCastPvPReward()
{
	if(this)
	{
		if(m_Type == PVP_REWARD_ZONE)
		{
			return (CPvPRewardTerritory*)this;
		}
	}
	return 0;
}

CDespawnSummonTerritory* CTerritoryEx::SafeCastDespawnSummon()
{
	if(this)
	{
		if(m_Type == DESPAWN_SUMMON_ZONE)
		{
			return (CDespawnSummonTerritory*)this;
		}
	}
	return 0;
}

CDispelSkillTerritory* CTerritoryEx::SafeCastDispelSkill()
{
	if(this)
	{
		if(m_Type == DISPEL_SKILL_ZONE)
		{
			return (CDispelSkillTerritory*)this;
		}
	}
	return 0;
}

CMiningTerritory* CTerritoryEx::SafeCastMining()
{
	if(this)
	{
		if(m_Type == MINING_ZONE)
		{
			return (CMiningTerritory*)this;
		}
	}
	return 0;
}

CLevelRestrictionTerritory* CTerritoryEx::SafeCastLevelRestriction()
{
	if(this)
	{
		if(m_Type == LEVEL_RESTRICTION_ZONE)
		{
			return (CLevelRestrictionTerritory*)this;
		}
	}
	return 0;
}

CPartyTerritory* CTerritoryEx::SafeCastParty()
{
	if(this)
	{
		if(m_Type == PARTY_ZONE)
		{
			return (CPartyTerritory*)this;
		}
	}
	return 0;
}

CRespawnTerritory* CTerritoryEx::SafeCastRespawn()
{
	if(this)
	{
		if(m_Type == RESPAWN_ZONE)
		{
			return (CRespawnTerritory*)this;
		}
	}
	return 0;
}

CNoblessTerritory* CTerritoryEx::SafeCastNobless()
{
	if(this)
	{
		if(m_Type == NOBLESS_ZONE)
		{
			return (CNoblessTerritory*)this;
		}
	}
	return 0;
}



CDispelAllTerritory* CTerritoryEx::SafeCastDispelAll()
{
	if(this)
	{
		if(m_Type == DISPEL_ALL_ZONE)
		{
			return (CDispelAllTerritory*)this;
		}
	}
	return 0;
}

CNoRestartTerritory* CTerritoryEx::SafeCastNoRestart()
{
	if(this)
	{
		if(m_Type == NO_RESTART_ZONE)
		{
			return (CNoRestartTerritory*)this;
		}
	}
	return 0;
}

void CTerritoryEx::Set(double x, double X, double y, double Y, double z, double Z, TerritoryType type)
{
	m_Extend = false;
	m_MinPos.x = x;
	m_MinPos.y = y;
	m_MinPos.z = z;
	m_MaxPos.x = X;
	m_MaxPos.y = Y;
	m_MaxPos.z = Z;
	m_Type = type;
}

bool CTerritoryEx::IsInside(double x, double y, double z)
{
	FVector point(x, y, z);
	return IsInside(point);
}

bool CTerritoryEx::IsInside(FVector point)
{
	if(m_Extend)
	{
		if(point.x < m_MinPos.x || point.y < m_MinPos.y || point.z < m_MinPos.z)
			return false;
		if(point.x > m_MaxPos.x || point.y > m_MaxPos.y || point.z > m_MaxPos.z)
			return false;

		UINT cutCount = 0;
		
		for(INT32 n=0;n<m_Points.size();n++)
		{
			if( CrossHalfLine(point, m_Points[n], m_Points[ (n+1) % m_Points.size() ]) )
			{
				cutCount++;
			}
		}

		return (cutCount % 2) == 1;
	}else
	{
		if( point.x >= m_MinPos.x && point.x <= m_MaxPos.x)
		{
			if( point.y >= m_MinPos.y && point.y <= m_MaxPos.y)
			{
				if( point.z >= m_MinPos.z && point.z <= m_MaxPos.z)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void CTerritoryEx::AddPoint(FVector point)
{
	m_Extend = true;
	m_Points.push_back(point);

	if(m_MaxPos.x < point.x)
		m_MaxPos.x = point.x;
	if(m_MaxPos.y < point.y)
		m_MaxPos.y = point.y;
	if(m_MaxPos.z < point.z)
		m_MaxPos.z = point.z;
	if(m_MinPos.x > point.x)
		m_MinPos.x = point.x;
	if(m_MinPos.y > point.y)
		m_MinPos.y = point.y;
	if(m_MinPos.z > point.z)
		m_MinPos.z = point.z;
}

bool CTerritoryEx::CrossHalfLine(FVector point, FVector p1, FVector p2)
{
	if(p1.y == p2.y)
	{
		return false;
	}

	//sort
	if(p1.y < p2.y)
	{
		FVector tmp = p1;
		p1 = p2;
		p2 = tmp;
	}

	if( (p1.y > point.y && p2.y > point.y) /* under halfline */ || (p1.y < point.y && p2.y < point.y) /* over halfline */ )
	{
		return false;
	}

	if( p1.x < point.x && p2.x < point.x)
	{
		return false;
	}

	if( p1.x >= point.x && p2.x >= point.x)
	{
		return true;
	}

	double t = (point.y - p1.y) / (p2.y - p1.y);
	double xp = p1.x + t*(p2.x - p1.x);
	if( xp >= point.x)
	{
		return true;
	}

	return false;
}

void CTerritoryEx::AddZRange(double z1, double z2)
{
	if(m_MaxPos.z < z1)
		m_MaxPos.z = z1;
	
	if(m_MinPos.z > z1)
		m_MinPos.z = z1;

	if(m_MaxPos.z < z2)
		m_MaxPos.z = z2;
	
	if(m_MinPos.z > z2)
		m_MinPos.z = z2;
}

TerritoryType CTerritoryEx::CastType(int nType)
{
	switch(nType)
	{
	case FORBIDDEN_FLY_ZONE:
		{
			return FORBIDDEN_FLY_ZONE;
			break;
		};
	case FORBIDDEN_RECALL_ZONE:
		{
			return FORBIDDEN_RECALL_ZONE;
			break;
		}
	case FORBIDDEN_RIDE_ZONE:
		{
			return FORBIDDEN_RIDE_ZONE;
			break;
		}
	case GLUDIO_CASTLE:
		{
			return GLUDIO_CASTLE;
			break;
		}
	case DION_CASTLE:
		{
			return DION_CASTLE;
			break;
		}
	case GIRAN_CASTLE:
		{
			return GIRAN_CASTLE;
			break;
		}
	case OREN_CASTLE:
		{
			return OREN_CASTLE;
			break;
		}
	case ADEN_CASTLE:
		{
			return ADEN_CASTLE;
			break;
		}
	case INNADRIL_CASTLE:
		{
			return INNADRIL_CASTLE;
			break;
		}
	case GODDARD_CASTLE:
		{
			return GODDARD_CASTLE;
			break;
		}
	case RUNE_CASTLE:
		{
			return RUNE_CASTLE;
			break;
		}
	case SCHUTTGARD_CASTLE:
		{
			return SCHUTTGARD_CASTLE;
			break;
		}
	case FORBIDDEN_OFFLINE_SHOP_ZONE:
		{
			return FORBIDDEN_OFFLINE_SHOP_ZONE;
			break;
		}
	case TRADE_GEODATA_CHECK_ZONE:
		{
			return TRADE_GEODATA_CHECK_ZONE;
			break;
		}
	case FORBIDDEN_TRADE_ZONE:
		{
			return FORBIDDEN_TRADE_ZONE;
			break;
		}
	case SSQ_ZONE:
		{
			return SSQ_ZONE;
			break;
		}
	case PVP_ZONE:
		{
			return PVP_ZONE;
			break;
		}
	case NICKNAME_CHANGE_ZONE:
		{
			return NICKNAME_CHANGE_ZONE;
			break;
		}
	case PVP_REWARD_ZONE:
		{
			return PVP_REWARD_ZONE;
			break;
		}
	case DESPAWN_SUMMON_ZONE:
		{
			return DESPAWN_SUMMON_ZONE;
			break;
		}
		
	case DISPEL_SKILL_ZONE:
		{
			return DISPEL_SKILL_ZONE;
			break;
		}
		
	case MINING_ZONE:
		{
			return MINING_ZONE;
			break;
		}
	default:
		{
			return TERRITORY_NONE;
			break;
		}
	}
}

void CRespawnTerritory::AddRespawn(int x, int y, int z)
{
	m_Respawns.push_back(IVector(x, y, z));
}

IVector CRespawnTerritory::GetRandomRespawn()
{
	if(m_Respawns.size() > 0)
	{
		UINT rnd = g_Random.RandInt(static_cast<UINT>(m_Respawns.size()));
		return m_Respawns[rnd];
	}

	return IVector();
}

bool CRequiredItemTerritory::HasItem(User *pUser)
{
	for(UINT n=0;n<m_Items.size();n++)
	{
		if(pUser->inventory.HaveItemByClassID(m_Items[n], 1))
		{
			return true;
		}
	}
	return false;
}

CBlockChatTerritory::CBlockChatTerritory()
{
	memset(this, 0, sizeof(CBlockChatTerritory));
}

void CBlockChatTerritory::SetData(bool value, TYPE type)
{
	data[type] = value;
};

bool CBlockChatTerritory::IsSet(TYPE type)
{
	return data[type];
}

bool CDespawnSummonTerritory::IsBlocked(INT32 classId)
{
	for(INT32 n=0;n<npcs.size();n++)
	{
		if(npcs[n] == classId)
		{
			return true;
		}
	}
	return false;
}

void CDespawnSummonTerritory::Block(INT32 classId)
{
	npcs.push_back(classId);
}

bool CDispelSkillTerritory::IsBlocked(INT32 skillId, INT32 level)
{
	for(INT32 n=0;n<skills.size();n++)
	{
		if(skills[n].skill_id == skillId && skills[n].level == level)
		{
			return true;
		}
	}
	return false;
}

void CDispelSkillTerritory::Block(INT32 skillId, INT32 level)
{
	CSkillKey key(skillId, level);
	skills.push_back(key);
}

CPvPRewardTerritory::CPvPRewardTerritory() : requiredLevel(0), rewardId(0), rewardCount(0), stealFromVictim(0), checkHWID(0), killReuse(0)
{

}

void CPvPRewardTerritory::Handle(User *pUser, User* pAttacker)
{
	if(pUser->ValidUser() && pAttacker->ValidUser())
	{
		if(pUser->pSD->nLevel >= requiredLevel && pAttacker->pSD->nLevel >= requiredLevel)
		{
			if(checkHWID)
			{
				if(pUser->pSocket->addr.S_un.S_addr == pAttacker->pSocket->addr.S_un.S_addr &&
					pUser->pSocket->pED->longHWID.IsEqual(pAttacker->pSocket->pED->longHWID))
				{
					return;
				}
			}
			if(killReuse > 0)
			{
				if(g_KillDb.IsInReuse(pAttacker->nDBID, pUser->nDBID, killReuse))
				{
					return;
				}
			}

			if(pAttacker->IsNowTrade())
			{
				pAttacker->TradeCancel();
			}

			if(stealFromVictim)
			{
				if(pUser->IsNowTrade())
				{
					pUser->TradeCancel();
				}
				if(pUser->inventory.HaveItemByClassID(rewardId, rewardCount))
				{
					if(pAttacker->inventory.CheckAddable(rewardId, rewardCount))
					{
						pAttacker->AddItemToInventory(rewardId, rewardCount);
						pUser->DeleteItemInInventory(rewardId, rewardCount);
					}
				}
			}else
			{
				if(pAttacker->inventory.CheckAddable(rewardId, rewardCount))
				{
					pAttacker->AddItemToInventory(rewardId, rewardCount);
				}
			}
		}
	}
}

void CDispelSkillTerritory::Validate(CCreature *pCreature)
{
	guard;
	if(pCreature->ValidCreature())
	{
		vector<INT32> toDispel;
		pCreature->abnormalStatusLock.Lock(__FILEW__, __LINE__);
		for(INT32 n=0;n<pCreature->vAbnormalStatus.size();n++)
		{
			AbnormalData& ad = pCreature->vAbnormalStatus[n];
			if(ad.pSI)
			{
				if(IsBlocked(ad.pSI->nSkillID, ad.pSI->nLevel))
				{
					toDispel.push_back(ad.pSI->nAbnormalType);
				}
			}
		}

		for(INT32 n=0;n<toDispel.size();n++)
		{
			for(AbnormalDataVector::iterator Iter = pCreature->vAbnormalStatus.begin();Iter!=pCreature->vAbnormalStatus.end();Iter++)
			{
				if(Iter->pSI)
				{
					if(Iter->pSI->nAbnormalType == toDispel[n])
					{
						if(Iter->pSI->nAbnormalType > 63 &&  Iter->pSI->nAbnormalType < 78)
						{
							//song
							if(pCreature->songSlotsUsed > 0)
								pCreature->songSlotsUsed--;

						}else if(Iter->pSI->nAbnormalType > 77 && Iter->pSI->nAbnormalType < 91)
						{
							//dance
							if(pCreature->danceSlotsUsed > 0)
								pCreature->danceSlotsUsed--;
						}
						pCreature->pSD->DeleteAbnormalStatus(Iter->pSI->nAbnormalType);
						pCreature->vAbnormalStatus.erase(Iter);
						break;
					}
				}
			}
		}
		pCreature->abnormalStatusLock.Unlock();

		if(toDispel.size() > 0)
		{
			pCreature->lpSharedDataLock->Lock(__FILEW__, __LINE__);

			pCreature->ValidateAllOnSIM();

			pCreature->lpSharedDataLock->Unlock();

			pCreature->broadcastInfoPacket = true;
			
			pCreature->CheckAbnormalVisualEffect();

			if(User *pUser = pCreature->GetUser())
			{
				pUser->RequestUpdateSpelledInfo();
				pUser->SendAbnormalStatusInfo();
				pUser->SetUserInfoChanged();
				pUser->SetCharInfoChanged();
			}
		}
	}
	unguard;
}

