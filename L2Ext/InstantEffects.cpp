#include "StdAfx.h"
#include "Geo.h"
#include "InstantEffects.h"
#include "Random.h"
#include "math.h"
#include "TerritoryData.h"
#include "MiningSystem.h"
#include "CreatureAction.h"
#include "PlayerAction.h"
#include "AccountDB.h"
#include "DeathPenalty.h"
#include "ObjectDB.h"
#include "ClanSys.h"
#include "DB.h"
#include "AIO.h"
#include "HtmlCache.h"

void CSkillEffect_i_add_enchanted_item::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(User *pUser = pCreature->SafeCastUser())
	{
		if(pUser->IsNowTrade())
		{
			pUser->TradeCancel();
		}
		pUser->AddItemToInventory((int)m_ItemId, 1, false, (int)m_EnchantLevel);
		g_Logger.Add(L"User[%s][%d] got enchanted[%d] item[%d] - skill effect.", pUser->pSD->wszName, pUser->nDBID, (int)m_EnchantLevel, (int)m_ItemId);
	}
}

void CSkillEffect_i_charge::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(pCreature->ValidCreature() && pTarget->ValidCreature())
	{
		double angle = HeadingToDegree(pCreature->pSD->sHeading);

		double radian = DegreeToRadian(angle);
		double course = DegreeToRadian(180);

		int x1 = (int) (cos((PI + radian + course)) * dRange);
		int y1 = (int) (sin((PI + radian + course)) * dRange);
		IVector pos(pCreature->pSD->Pos);
		pos.x += x1;
		pos.y += y1;
		
		FVector destPos;
		destPos.x = (double)pos.x;
		destPos.y = (double)pos.y;
		destPos.z = (double)pos.z;

		if(CGeo::GetInstance()->CanSee(&pCreature->pSD->Pos, &destPos))
		{
			CFlyToLocation pck(pCreature->nObjectID, pos.x, pos.y, pos.z, pCreature->pSD->Pos.x, pCreature->pSD->Pos.y, pCreature->pSD->Pos.z, (int)dType); 
			FVector vPos = pCreature->pSD->Pos;
			L2Server::BroadcastToNeighborDist(pCreature->nObjectID, &vPos, 8000, (int)pck.GetSize(), pck.GetBuff());
			pCreature->pSD->Pos.x = static_cast<double>(pos.x);
			pCreature->pSD->Pos.y = static_cast<double>(pos.y);
			pCreature->pSD->Pos.z = static_cast<double>(pos.z);
			pCreature->ValidateLocation();
		}
	}
}

void CSkillEffect_i_update_eval_score::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(User *pUser = pCreature->SafeCastUser())
	{
		int amount = (int)m_Amount;

		amount += pUser->pSD->nEvalScore;
		if(amount > 255)
		{
			amount = 255;
		}
		pUser->pSD->nEvalScore = amount;

		SYSTEMTIME st;
		GetLocalTime(&st);

		typedef void(*f)(UINT64, UINT, INT32, INT32, LPSYSTEMTIME);
		f(0x530F70L)(0x10488E0L, pUser->nDBID, amount, pUser->pSD->nUsedSulffrage, &st);
		g_Logger.Add(L"User[%s] got eval score updated[%d] - skill effect.", pUser->pSD->wszName, amount);

		pUser->SetUserInfoChanged();
		pUser->SetCharInfoChanged();
	}
}


void CSkillEffect_i_consume_energy::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(pCreature->ValidCreature() && pTarget->ValidCreature())
	{
		int nConsume = (int)dCount;
		int nEnergyCount = pTarget->energyCount;
		nEnergyCount -= nConsume;
		if(nEnergyCount < 0)
			nEnergyCount = 0;
		pTarget->energyCount = nEnergyCount;
		if(pTarget->ValidUser())
			pTarget->GetUser()->SendETCBuffStatus();
	}
}

void CSkillEffect_i_update_pledge_skill::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	guard;
	if(User *pUser = pCreature->SafeCastUser())
	{
		if(pUser->pSD->nIsPledgeLeader)
		{
			if(CPledge *pPledge = pUser->GetPledge())
			{
				int acquiredLevel = 0;
				CPledgeEx *pPledgeEx = pPledge->pSD->pPledgeEx;
				if(pPledgeEx)
				{
					pPledgeEx->m_Lock.Enter();
					PledgeSkillMap::iterator it = pPledgeEx->m_Skills.find(m_SkillId);
					if(it!=pPledgeEx->m_Skills.end())
					{
						acquiredLevel = it->second.skillLevel;
					}
					pPledgeEx->m_Lock.Leave();

					acquiredLevel++;
					const PledgeSkillInfo& psi = g_PledgeSkillDb.GetSkillInfo(m_SkillId, acquiredLevel, false);
					if(psi.skillId > 0)
					{
						if(psi.requiredPledgeLevel <= pPledge->pSD->nLevel)
						{
							pPledgeEx->m_Lock.Enter();
							pPledgeEx->m_Skills[psi.skillId] = psi;
							pPledgeEx->m_Lock.Leave();
							g_DB.RequestAddPledgeSkill(pPledge->pSD->nDBID, psi.skillId, acquiredLevel);
							pPledge->NotifyPledgeSkillAcquired(psi.skillId, acquiredLevel);
							pPledge->UpdateInfo();
							g_Logger.Add(L"User[%s] updated pledge skill[%d][%d] for pledge[%d]", pUser->pSD->wszName, psi.skillId, acquiredLevel, pPledge->pSD->nDBID);
						}else
						{
							pUser->SendSystemMessage(L"Your clan level is to low to update this skill!");
						}
					}else
					{
						pUser->SendSystemMessage(L"You cannot update this skill level!");
					}
				}
			}
		}else
		{
			pUser->SendSystemMessage(L"In order to proceed you have to be a clan leader.");
		}
	}
	unguard;
}

void CSkillEffect_i_pledge_name_value::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	guard;
	if(User *pUser = pCreature->SafeCastUser())
	{
		if(pUser->pSD->nPledgeID)
		{
			if(m_Amount > 0 || pUser->pSD->nIsPledgeLeader)
			{
				if(CPledge *pPledge = pUser->GetPledge())
				{
					pPledge->UpdatePledgeNameValue(m_Amount);
					g_Logger.Add(L"User[%s] updated pledge name value[%d] for pledge[%d]", pUser->pSD->wszName, (INT32)m_Amount, pPledge->pSD->nDBID);
				}
			}else
			{
				pUser->SendSystemMessage(L"Only clan leader can use reputation points.");
			}
		}else
		{
			pUser->SendSystemMessage(L"In order to proceed you have to join a clan.");
		}
	}
	unguard;
}

void CSkillEffect_i_set_aio::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	guard;
	if(User *pUser = pCreature->SafeCastUser())
	{
		g_AIOSystem.BecomeAIO(pUser, (int)m_Id, true);
		g_Logger.Add(L"User[%S][%d] - i_set_aio [%d]", pUser->pSD->wszName, pUser->nDBID, (int)m_Id);
	}
	unguard;
}

void CSkillEffect_i_show_menu::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(User *pUser = pCreature->SafeCastUser())
	{
		const WCHAR* pFile = g_HtmlCache.Get(L"menu.htm");
		if(pFile)
		{
			PlayerAction::ShowHTML(pUser, L"menu.htm", pFile, 0);
		}
	}
}

void CSkillEffect_i_set_nobless::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(User *pUser = pCreature->SafeCastUser())
	{
		g_DB.RequestSaveNoblessType(pUser, 1);
		pUser->SendSystemMessage(L"Congratulations! You're now a nobless!");
		g_Logger.Add(L"User[%S][%d] - i_set_nobless.", pUser->pSD->wszName, pUser->nDBID);
	}
}

void CSkillEffect_i_ride_wyvern::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(User *pUser = pCreature->GetUser())
	{
		if(pUser->pSD)
		{
			if(pUser->pSD->nYongmaType == 0)
			{
				pUser->StopMove();
				pUser->RideWyvern(1012621, 56);
			}
		}
	}
}

void CSkillEffect_i_death2::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(pCreature->ValidCreature() && pTarget->ValidCreature())
	{
		if(!pTarget->IsZZoldagu() && !pTarget->IsBoss() && pTarget->canDamageHP)
		{
			if( pSkillInfo && (pSkillInfo->nMagicLevel + 9) > pTarget->pSD->nLevel )
			{
				double attributeValue = pTarget->GetAttribute(pSkillInfo->attributeType);
				double attributeBonus = pTarget->GetAttributeBonus(pCreature);

				if(effect2 > 0)
				{
					double effectChance = attributeBonus * this->effect2 * attributeValue;
					double dice = g_Random.RandDouble(100);
				//	g_Log.Add(CLog::Blue, "[%s] attribute[%f][%f] chance[%f][%f]", __FUNCTION__, attributeValue, attributeBonus, effect1, effect2);
					if(dice < effectChance)
					{
						//perform effect 2
						double value = pTarget->pSD->fHP - 1.0;
						if(value < 1.0)
							value = 1.0;
						pTarget->pSD->fHP = 1;
						if(pTarget->pSD->nIsPC)
						{
							pTarget->pSD->fCP = 1;
						}

						if(pTarget->IsNPC())
						{
							pTarget->AddContribution(pCreature, value);
						}

						pTarget->UpdateStatus(CCreature::UPDATE_HP);

						if(User *pUser = pCreature->GetUser())
						{
							////1667	1	Lethal Strike!	0	A0	D7	FE	FF			0	0	0	0	0		none
							pUser->pSocket->SendSystemMessage(1667);
							//1668	1	Your lethal strike was successful!	0	A0	D7	FE	FF			0	0	0	0	0		none
							pUser->pSocket->SendSystemMessage(1668);
						}
					}else
					{
						effectChance = attributeBonus * this->effect1 * attributeValue;
						dice = g_Random.RandDouble(100);
						if(dice < effectChance)
						{
							//perform effect 1
							double value = pTarget->pSD->fHP / 2.0;
							if(value < 1.0)
								value = 1.0;

							if(pTarget->pSD->nIsPC)
							{
								pTarget->pSD->fCP = 1;
							}else
							{
								pTarget->pSD->fHP = value;
							}

							if(pTarget->IsNPC())
							{
								pTarget->AddContribution(pCreature, value);
							}

							
							pTarget->UpdateStatus(CCreature::UPDATE_HP);

							if(User *pUser = pCreature->GetUser())
							{
								//1667	1	Lethal Strike!	0	A0	D7	FE	FF			0	0	0	0	0		none
								pUser->pSocket->SendSystemMessage(1667);
							}
						}
					}
				}else
				{
					double effectChance = attributeBonus * this->effect1 * attributeValue;
					double dice = g_Random.RandDouble(100);
					if(dice < effectChance)
					{
						//perform effect 1
						double value = pTarget->pSD->fHP / 2.0;
						if(value < 1.0)
							value = 1.0;

						if(pTarget->pSD->nIsPC)
						{
							pTarget->pSD->fCP = 1;
						}else
						{
							pTarget->pSD->fHP = value;
						}

						if(pTarget->IsNPC())
						{
							pTarget->AddContribution(pCreature, value);
						}
						
						pTarget->UpdateStatus(CCreature::UPDATE_HP);

						if(User *pUser = pCreature->GetUser())
						{
							//1667	1	Lethal Strike!	0	A0	D7	FE	FF			0	0	0	0	0		none
							pUser->pSocket->SendSystemMessage(1667);
						}
					}
				}
			}
		}
	}
}

void CSkillEffect_i_dig::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(User *pUser = pCreature->GetUser())
	{
		double chance = baseChance;
		chance /= 100;
		if(g_MiningSystem.CanDig(pUser))
		{
			g_MiningSystem.Dig(pUser, chance);
		}
	}
}

extern vector<int> g_DispelDebuffException;

void CSkillEffect_i_dispel_debuff_with_exception::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	guard;

	if( pTarget->ValidCreature() )
	{
		pTarget->abnormalStatusLock.Lock(__FILEW__, __LINE__);

		SIZE_T nCount = static_cast<SIZE_T>(m_Amount);

		if(nCount > pTarget->vAbnormalStatus.size() )
			nCount = pTarget->vAbnormalStatus.size();

		for(int n = 0; n < nCount; n++)
		{
			AbnormalDataVector::iterator Iter = pTarget->vAbnormalStatus.begin();
			for(;Iter!=pTarget->vAbnormalStatus.end();Iter++)
			{
				if( Iter->pSI )
				{
					//check if isn't blocked
					bool exception = false;
					for(UINT i = 0; i < g_DispelDebuffException.size(); i++)
					{
						if(Iter->pSI->nSkillID == g_DispelDebuffException[i])
						{
							exception = true;
							break;
						}
					}
					if(Iter->pSI->nDebuff == 1 && !exception)
					{
						//Deleting from abnormal table
						CreatureAction::OnDeleteAbnormalStatus(pTarget->pSD, Iter->pSI->nAbnormalType);
						//Send msg to user (if target is user)
						if(User *pUser = pTarget->SafeCastUser())
							pUser->pSocket->Send("cddddd", 0x64, 749, 1, 4, Iter->pSI->nSkillID, Iter->pSI->nLevel);

						pTarget->vAbnormalStatus.erase(Iter);
						break;
					}
				}
			}
		}
		pTarget->abnormalStatusLock.Unlock();

		pTarget->lpSharedDataLock->Lock(__FILEW__, __LINE__);
		pTarget->ValidateAllOnSIM();
		pTarget->lpSharedDataLock->Unlock();
		
		pTarget->broadcastInfoPacket = true;

		pTarget->CheckAbnormalVisualEffect();

		if(User *pUser = pTarget->GetUser())
		{
			pUser->RequestUpdateSpelledInfo();
			pUser->SendAbnormalStatusInfo();
			pUser->SetUserInfoChanged();
			pUser->SetCharInfoChanged();
		}
	}
	unguard;
}

void CSkillEffect_i_dispel_npc_buff::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if( pTarget->ValidCreature() )
	{
		pTarget->abnormalStatusLock.Lock(__FILEW__, __LINE__);

		int nCount = (int)dCount;

		if(nCount > pTarget->vAbnormalStatus.size() )
			nCount = (int)pTarget->vAbnormalStatus.size();

		for(int n = 0; n < nCount; n++)
		{
			AbnormalDataVector::iterator Iter = pTarget->vAbnormalStatus.begin();
			for(;Iter!=pTarget->vAbnormalStatus.end();Iter++)
			{
				if( !(Iter->selfBuff & 1) )
				{
					if(Iter->pSI->nAbnormalType > 63 &&  Iter->pSI->nAbnormalType < 78)
					{
						//song
						if(pTarget->songSlotsUsed > 0)
							pTarget->songSlotsUsed--;

					}else if(Iter->pSI->nAbnormalType > 77 && Iter->pSI->nAbnormalType < 91)
					{
						//dance
						if(pTarget->danceSlotsUsed > 0)
							pTarget->danceSlotsUsed--;
					}
					//Deleting from abnormal table
					CreatureAction::OnDeleteAbnormalStatus(pTarget->pSD, Iter->pSI->nAbnormalType);
					//Send msg to user (if target is user)
					if(User *pUser = pTarget->GetUser())
						pUser->pSocket->Send("cddddd", 0x64, 749, 1, 4, Iter->pSI->nSkillID, Iter->pSI->nLevel);

					pTarget->vAbnormalStatus.erase(Iter);
					break;
				}
			}
		}
		pTarget->abnormalStatusLock.Unlock();

		pTarget->lpSharedDataLock->Lock(__FILEW__, __LINE__);
		pTarget->ValidateAllOnSIM();
		pTarget->lpSharedDataLock->Unlock();
		
		pTarget->broadcastInfoPacket = true;

		pTarget->CheckAbnormalVisualEffect();

		if(User *pUser = pTarget->GetUser())
		{
			pUser->RequestUpdateSpelledInfo();
			pUser->SendAbnormalStatusInfo();
			pUser->SetUserInfoChanged();
			pUser->SetCharInfoChanged();
		}
	}
}

void CSkillEffect_i_dispel_by_category_count::Instant(CCreature *pSpeller, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(pSpeller->ValidCreature() && pTarget->ValidCreature())
	{
		int nCount = (int)dCount; //Max buffs/debuffs that can be removed
		int nType = (int)dType;
		double dChance = 250;			//Base chance
		if(pSpeller->ValidCreature() && pTarget->ValidCreature())
		{
			bool bBlocked = false;
			if(nType == CSkillInfo::DEBUFF) //1- Debuff 0- Buff
			{
				dChance *= pTarget->resistDispelByCategorySlotDebuff;
				if(pTarget->isBlockDebuff)
					bBlocked = true;
			}
			else if(nType == CSkillInfo::BUFF)
			{
				dChance *= pTarget->resistDispelByCategorySlotBuff;
				if(pTarget->isBlockBuff)
					bBlocked = true;
			}
			if(!bBlocked && pSkillInfo)
			{
				dChance *= pSkillInfo->nMagicLevel;
				dChance /= pTarget->pSD->nLevel;
				CSocket *pSocket = NULL;
				if(pTarget->ValidUser())
				{
					pSocket = pTarget->GetUser()->GetSocket();
				}			
				
				pTarget->abnormalStatusLock.Lock(__FILEW__, __LINE__);
				for(int n = 0; n < nCount; n++)
				{
					bool bDispell = false;
					AbnormalDataVector::iterator Iter=pTarget->vAbnormalStatus.begin();
					for(; Iter != pTarget->vAbnormalStatus.end(); Iter++)
					{
						AbnormalData BI = *Iter;
						if(BI.pSI)
						{
							if(BI.pSI->nResistCancel)
							{
								continue;
							}
							if(BI.pSI->nSkillID == 1323) //blessing of nobless
							{
								continue;
							}

							if(BI.pSI->nDebuff == nType)
							{
								double dRand = g_Random.RandDouble(1000);
								if(dRand < dChance)
								{
									if(BI.pSI->nAbnormalType > 63 &&  BI.pSI->nAbnormalType < 78)
									{
										//song
										if(pTarget->songSlotsUsed > 0)
											pTarget->songSlotsUsed--;

									}else if(BI.pSI->nAbnormalType > 77 && BI.pSI->nAbnormalType < 91)
									{
										//dance
										if(pTarget->danceSlotsUsed > 0)
											pTarget->danceSlotsUsed--;
									}
									//Deleting from abnormal table
									CreatureAction::OnDeleteAbnormalStatus(pTarget->pSD, BI.pSI->nAbnormalType);
									//Send msg to user (if target is user)
									if(pSocket->ValidUserSocket())
										pSocket->Send("cddddd", 0x64, 749, 1, 4, BI.pSI->nSkillID, BI.pSI->nLevel);
									//Deleting buff info from container
									bDispell = true;
									break;
								}
							}
						}
					}
					if(bDispell)
						pTarget->vAbnormalStatus.erase(Iter);
				}
				pTarget->abnormalStatusLock.Unlock();

				pTarget->lpSharedDataLock->Lock(__FILEW__, __LINE__);
				pTarget->ValidateAllOnSIM();
				pTarget->lpSharedDataLock->Unlock();

				pTarget->broadcastInfoPacket = true;

				pTarget->CheckAbnormalVisualEffect();

				if(User *pTargetU = pTarget->GetUser())
				{
					pTargetU->RequestUpdateSpelledInfo();
					pTargetU->SendAbnormalStatusInfo();
					pTargetU->SetUserInfoChanged();
					pTargetU->SetCharInfoChanged();
				}
			}
		}
	}
}

void CSkillEffect_i_dispel_by_chance::Instant(CCreature *pSpeller, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(pSpeller->ValidCreature() && pTarget->ValidCreature())
	{
		int nCount = (int)dCount; //Max buffs/debuffs that can be removed
		double chance = dChance;			//Base chance
		if(pSpeller->ValidCreature() && pTarget->ValidCreature())
		{
			bool bBlocked = false;
			chance *= pTarget->resistDispelByCategorySlotBuff;
			if(pTarget->isBlockBuff)
				bBlocked = true;
			
			if(!bBlocked && pSkillInfo)
			{
				chance *= pSkillInfo->nMagicLevel;
				chance /= pTarget->pSD->nLevel;
				CSocket *pSocket = NULL;
				if(pTarget->ValidUser())
				{
					pSocket = pTarget->GetUser()->GetSocket();
				}			
				
				pTarget->abnormalStatusLock.Lock(__FILEW__, __LINE__);
				for(int n = 0; n < nCount; n++)
				{
					bool bDispell = false;
					AbnormalDataVector::iterator Iter=pTarget->vAbnormalStatus.begin();
					for(; Iter != pTarget->vAbnormalStatus.end(); Iter++)
					{
						AbnormalData BI = *Iter;
						if(BI.pSI)
						{
							if(BI.pSI->nResistCancel)
								continue;

							if(BI.pSI->nSkillID == 1323) //blessing of nobless
								continue;

							if(BI.pSI->nDebuff != 1)
							{
								double dRand = g_Random.RandDouble(100);
								if(dRand < chance)
								{
									if(BI.pSI->nAbnormalType > 63 &&  BI.pSI->nAbnormalType < 78)
									{
										//song
										if(pTarget->songSlotsUsed > 0)
											pTarget->songSlotsUsed--;

									}else if(BI.pSI->nAbnormalType > 77 && BI.pSI->nAbnormalType < 91)
									{
										//dance
										if(pTarget->danceSlotsUsed > 0)
											pTarget->danceSlotsUsed--;
									}
									//Deleting from abnormal table
									CreatureAction::OnDeleteAbnormalStatus(pTarget->pSD, BI.pSI->nAbnormalType);
									//Send msg to user (if target is user)
									if(pSocket->ValidUserSocket())
										pSocket->Send("cddddd", 0x64, 749, 1, 4, BI.pSI->nSkillID, BI.pSI->nLevel);
									//Deleting buff info from container
									bDispell = true;
									break;
								}
							}
						}
					}
					if(bDispell)
						pTarget->vAbnormalStatus.erase(Iter);
				}
				pTarget->abnormalStatusLock.Unlock();

				pTarget->lpSharedDataLock->Lock(__FILEW__, __LINE__);
				pTarget->ValidateAllOnSIM();
				pTarget->lpSharedDataLock->Unlock();

				pTarget->broadcastInfoPacket = true;
				pTarget->CheckAbnormalVisualEffect();

				if(User *pTargetU = pTarget->GetUser())
				{
					pTargetU->RequestUpdateSpelledInfo();
					pTargetU->SendAbnormalStatusInfo();
					pTargetU->SetUserInfoChanged();
					pTargetU->SetCharInfoChanged();
				}
			}
		}
	}
}

void CSkillEffect_i_hide_abnormal::Instant(CCreature *pSpeller, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(pSpeller->ValidCreature() && pTarget->ValidCreature())
	{
		int nCount = (int)this->dCount; //Max buffs/debuffs that can be removed
		int nType = (int)this->dType;	//Type: 0 - Buff, 1 - Debuff, 2 - not set
		if(nType > 1 || nType < 0)
		{
			g_Log.Add(CLog::Error, "[%s] Invalid Abnormal Type[%d] - SkillID[%d] Level[%d]", __FUNCTION__, nType, pSkillInfo->nSkillID, pSkillInfo->nLevel);
			return;
		}
		double dChance = 250;			//Base chance
		if(pSpeller->ValidCreature() && pTarget->ValidCreature())
		{
			if(nType == CSkillInfo::DEBUFF) //1- Debuff 0- Buff
			{
				dChance *= pTarget->resistDispelByCategorySlotDebuff;
				if(pTarget->isBlockDebuff)
					return;
			}
			else if(nType == CSkillInfo::BUFF)
			{
				dChance *= pTarget->resistDispelByCategorySlotBuff;
				if(pTarget->isBlockBuff)
					return;
			}

			if(pSkillInfo)
			{
				dChance *= pSkillInfo->nMagicLevel;
				dChance /= pTarget->pSD->nLevel;
				CSocket *pSocket = 0;
				if(pTarget->ValidUser())
				{
					pSocket = pTarget->GetUser()->GetSocket();
				}			
				
				pTarget->abnormalStatusLock.Lock(__FILEW__, __LINE__);
				for(int n = 0; n < nCount; n++)
				{
					bool bDispell = false;
					AbnormalDataVector::iterator Iter=pTarget->vAbnormalStatus.begin();
					for(; Iter != pTarget->vAbnormalStatus.end(); Iter++)
					{
						AbnormalData BI = *Iter;
						if(BI.pSI && BI.pSI->nResistCancel == FALSE)
						{
							if(BI.pSI->nDebuff == nType)
							{
								double dRand = g_Random.RandDouble(1000);
								if(dRand < dChance)
								{
									//Deleting from abnormal table
									if(BI.pSI->nAbnormalType > 63 &&  BI.pSI->nAbnormalType < 78)
									{
										//song
										if(pTarget->songSlotsUsed > 0)
											pTarget->songSlotsUsed--;

									}else if(BI.pSI->nAbnormalType > 77 && BI.pSI->nAbnormalType < 91)
									{
										//dance
										if(pTarget->danceSlotsUsed > 0)
											pTarget->danceSlotsUsed--;
									}
									pTarget->pSD->DeleteAbnormalStatus(BI.pSI->nAbnormalType);
									
									//Send msg to user (if target is user)
									if(pSocket->ValidUserSocket())
										pSocket->Send("cddddd", 0x64, 749, 1, 4, BI.pSI->nSkillID, BI.pSI->nLevel);
									//Deleting buff info from container
									bDispell = true;
									break;
								}
							}
						}
					}
					if(bDispell)
					{
						if(User *pTU = pTarget->SafeCastUser())
						{
							pTU->HideAbnormal((*Iter), pSkillInfo->nAbnormalTime);
						//	pTarget->pSD->pExData->HiddenAbnormalMgr.HideAbnoraml((*Iter), pSkillInfo->nAbnormalTime);
						}
						pTarget->vAbnormalStatus.erase(Iter);
					}
				}
				pTarget->abnormalStatusLock.Unlock();

				pTarget->lpSharedDataLock->Lock(__FILEW__, __LINE__);
				pTarget->ValidateAllOnSIM();
				pTarget->lpSharedDataLock->Unlock();

				pTarget->broadcastInfoPacket = true;

				pTarget->CheckAbnormalVisualEffect();
				if(User *pUser = pTarget->GetUser())
				{
					pUser->RequestUpdateSpelledInfo();
					pUser->SendAbnormalStatusInfo();
					pUser->SetUserInfoChanged();
					pUser->SetCharInfoChanged();
				}
			}
		}
	}
}

void CSkillEffect_i_kill_myself::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(User *pUser = pCreature->GetUser())
	{
		pUser->KillMyself();
		g_Log.Add(CLog::In, "[%s] casted by [%S].", __FUNCTION__, pUser->pSD->wszName);
	}
}

void CSkillEffect_i_lift_death_penalty::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(pCreature->ValidCreature() && pTarget->ValidUser())
	{
		g_DeathPenalty.LiftPenalty(pTarget->SafeCastUser(), dChance);
	}
}

void CSkillEffect_i_mana_burn::Instant(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(pSkillUser->ValidCreature() && pTarget->ValidCreature() && pTarget->canDamageMP)
	{
		double dRandom = g_Random.RandDouble(100);
		double LandRate = chance;
		LandRate /= pTarget->pSD->nLevel;
		LandRate *= pSkillInfo->nMagicLevel;
		
		if(dRandom <= LandRate)
		{
		//	double manaAmount = (sqrt(pSkillUser->pSD->fMAtk)*power*(pTarget->pSD->MaxMP/97)) / pTarget->pSD->fMDef;
			double mAtk = pSkillUser->pSD->fMAtk;
			mAtk = mAtk + (mAtk * pSkillUser->pSD->dSpiritShotPower);
			double manaAmount = sqrt(mAtk) * power * 15 * (pTarget->pSD->MaxMP / 1600) / pTarget->pSD->fMDef;

			double critDice = g_Random.RandDouble(100);
			double critChance = 0.5 * pSkillUser->magicCriticalRatePER;
			critChance += pSkillUser->magicCriticalRateDIFF;

			if(critChance > critDice)
			{
				//magic critical hit
				if(User *pUser = pSkillUser->GetUser())
				{
					pUser->pSocket->SendSystemMessage(1280);
				}
				manaAmount *= 4.0;
			}
			pTarget->lpSharedDataLock->Lock(__FILEW__, __LINE__);

			if(pTarget->pSD->fMP > manaAmount)
				pTarget->pSD->fMP -= manaAmount;
			else
				pTarget->pSD->fMP = 1;

			pTarget->lpSharedDataLock->Unlock();

			pTarget->UpdateStatus(CCreature::UPDATE_MP);

			if(User *pUser = pSkillUser->GetUser())
			{
				CSystemMessage msg(970);
				msg.AddNumber((int)manaAmount);

				if(pTarget->pSD->nIsPC)
					msg.AddText(pTarget->pSD->wszName);
				else
					msg.AddNpc(pTarget->pSD->nNpcClassID);

				

				pUser->pSocket->SendSystemMessage(&msg);
			}
		}
	}
}

void CSkillEffect_i_release_soul::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(pCreature->ValidUser())
	{
		g_SpiritSystem.OnRelease(pCreature->GetUser());
	}
}

void CSkillEffect_i_spawn_casting_npc::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{	
	if(pCreature->ValidUser())
	{
		int nClassID = dNpcID;
		nClassID += 1000000;
		const WCHAR* wszName = g_ObjectDB.GetName(nClassID);
		CNPC *pNPC = g_ObjectDB.CreateNPC(nClassID);
		if(pNPC)
		{
			const WCHAR* wszAI = g_ObjectDB.GetAI(nClassID);
			if(wszAI)
			{
				//TODO: Generate some kind of buff - ai params
				CPacket packet;
				int nCnt = 0;
				//Fill data with correct info
				wcscpy_s((PWCHAR)((UINT64)pNPC + 0x19D8), 25, pCreature->pSD->wszName);
				pNPC->pSD->nMasterID = pCreature->pSD->nObjectID;
				CHAR buff[2048];
				memset(buff, 0, 2048);
				if(pCreature->pSD->pExData->TargetGroundLoc.IsSet())
					pCreature->GetUser()->ChangeTarget(pNPC, false);
				else
					pCreature->pSD->pExData->TargetGroundLoc.Set(pCreature->pSD->Pos.x, pCreature->pSD->Pos.y, pCreature->pSD->Pos.z);
				int nBuffSize = Assemble(buff, 2048,"cddpddddddddSdb", 0x01, pNPC->pSD->nServerIndex, pNPC->nObjectID, 
					0, 0, pCreature->pSD->pExData->TargetGroundLoc.nX, pCreature->pSD->pExData->TargetGroundLoc.nY, pCreature->pSD->pExData->TargetGroundLoc.nZ,
					pCreature->pSD->sHeading, 0, 0, 0, wszAI, nCnt, (int)packet.GetSize(), packet.GetBuff());
				NpcServer::Send(nBuffSize, buff);
				pCreature->pSD->pExData->TargetGroundLoc.Clear();
			}else
				g_Log.Add(CLog::Error, "[%s] There is no AI specified for npc with class id[%d]", __FUNCTION__, nClassID);
		}else
			g_Log.Add(CLog::Error, "[%s] There is no npc with class id[%d]", __FUNCTION__, nClassID);
	}
}

void CSkillEffect_i_summon_erase::Instant(CCreature *pSkillUser, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(pTarget->IsSummon())
	{
		int nClassID = pTarget->pSD->nNpcClassID;
		if(nClassID >= 14737 && nClassID <= 14767)
			return;
		if(nClassID >= 14768 && nClassID <= 14798)
			return;
		if(nClassID >= 14839 && nClassID <= 14869)
			return;

		double chance = dChance;
		chance *= pSkillInfo->nMagicLevel;
		chance /= pTarget->pSD->nLevel;
		if(chance >= g_Random.RandDouble(100))
		{
			User *pMaster = pTarget->CastSummon()->GetMaster();
			if(pMaster)
				pMaster->DespawnSummon();
		}
	}else if(User* pUser = pSkillUser->GetUser())
	{
		pUser->pSocket->SendSystemMessage(109); //Invalid target.
	}
}


void CSkillEffect_i_summon_friend::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(User *pUser = pCreature->GetUser())
	{
		if(!pCreature->pSD->nTargetObjId)
			return;

		User *pTU = User::GetUserBySID(&pCreature->targetId);
		if(pTU->ValidUser() && pTU != pUser)
		{
			CParty *pParty1 = pUser->GetParty();
			CParty *pParty2 = pTU->GetParty();
			CUserSocket *pSocket1 = pUser->GetSocket();
			CUserSocket *pSocket2 = pTU->GetSocket();
			if( pParty1 == pParty2 && pParty1 )
			{
				if(pTU->pSD->nAlive)
				{
					if(pUser->InCombatZone())
					{
						if(pSocket1)
							pSocket1->Send("cdddS", 0x64, 1380, 1, 0, pTU->pSD->wszName);
						return;
					}
					if(pUser->olympiadUser.GetState() != 0)
					{
						pUser->SendSystemMessage(L"You cannot summon anyone when participing in Olympiad Games!");
						return;
					}
					if(pTU->olympiadUser.GetState() != 0)
					{
						pUser->SendSystemMessage(L"You cannot summon a player who is participing in Olympiad Games!");
						return;
					}
					if(pTU->pSD->nActiveUser == 0)
					{
						pUser->SendSystemMessage(L"Your target is currently teleporting and cannot be summoned!");
						return;
					}
					if(pTU->InCombat())
					{
						DWORD dCombatTime = pTU->CombatTime();
						DWORD dCurrentTime = (int) time(NULL);
						if( (dCombatTime+30) >  dCurrentTime)
						{
							if(pSocket1)
								pSocket1->Send("cdddS", 0x64, 1843, 1, 0, pTU->pSD->wszName);
							return;
						}
					}
					// Check zone for anti recall zone
					if(pTU->pSD->pExData->territoryData.territoryId[ANTI_RECALL_ZONE] != 0)
					{
						//1899	1	Your target is in an area which blocks summoning.
						if(pSocket1)
							pSocket1->SendSystemMessage(1899);
						return;
					}
					if(pTU->IsNowTrade() || pTU->pSD->activateOfflineShopTime)
					{
						if(pSocket1)
							pSocket1->Send("cdddS", 0x64, 1898, 1, 0, pTU->pSD->wszName);
						return;
					}
					CItem *pItem = pTU->GetInventory()->GetFirstItemByClassID((int)this->dItemID);
					if(pItem)
					{
						if(pSocket2)
						{
							pTU->pSD->pExData->SummonFriend.Set(pUser->nObjectID, (int)pUser->pSD->Pos.x, (int)pUser->pSD->Pos.y, (int)pUser->pSD->Pos.z);
							pSocket2->Send("cdddSdddddd", 0xED, 1842, 2, 0, pUser->pSD->wszName, 7, pTU->pSD->pExData->SummonFriend.nX, pTU->pSD->pExData->SummonFriend.nY, pTU->pSD->pExData->SummonFriend.nZ, 9000, 0);
						}
					}else
						if(pSocket1)
							pSocket1->Send("cdddd", 0x64, 1897, 1, 3, 8615);
				}else 
					if(pSocket1)
						pSocket1->Send("cdddd", 0x64, 1844, 1, 0, pTU->pSD->wszName);
			}else 
				if(pSocket1)
					pSocket1->SendSystemMessage(109);
		}
	}
}

void CSkillEffect_i_summon_party::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(User *pUser = pCreature->SafeCastUser())
	{
		if(CParty *pParty = pUser->GetParty())
		{
			if(pUser->olympiadUser.GetState() != 0)
			{
				pUser->SendSystemMessage(L"You cannot summon anyone when participing in Olympiad Games!");
				return;
			}
			int x = (int)pCreature->pSD->Pos.x;
			int y = (int)pCreature->pSD->Pos.y;
			int z = (int)pCreature->pSD->Pos.z;
			
			CUserSocket *pSocket = pUser->pSocket;
			if(pSocket->ValidUserSocket())
			{
				if(!pUser->pSD->nInBattleField && !pUser->InCombat() && !pUser->InCombatZone())
				{
					UserVector vUsers;
					pParty->GetAllMembersEx(vUsers);
					UserVector::iterator Iter = vUsers.begin();
					for(UINT n=0;n<vUsers.size();n++)
					{
						if(User *pMember = vUsers[n]->SafeCastUser())
						{
							if(pMember->nObjectID != pUser->nObjectID)
							{
								wstring wMsg = pMember->pSD->wszName;
								// Change zone to new anti summoning zone
								if(pMember->pSD->pExData->territoryData.territoryId[ANTI_RECALL_ZONE] != 0)
								{
									wMsg += L" is in an area which blocks summoning.";
									pSocket->SendSystemMessage(wMsg.c_str());
								}else if(pMember->olympiadUser.GetState() != 0)
								{
									wMsg += L" is currently participating in the Grand Olympiad and cannot be summoned.";
									pSocket->SendSystemMessage(wMsg.c_str());
								}else if (!pMember->pSD->nAlive)
								{
									wMsg += L" is dead at the moment and cannot be summoned.";
									pSocket->SendSystemMessage(wMsg.c_str());
								}else if (pMember->InCombat() && (pMember->CombatTime() + 30) > (DWORD)time(NULL))
								{
									wMsg += L"  is engaged in combat and cannot be summoned.";
									pSocket->SendSystemMessage(wMsg.c_str());
								}else if (pMember->pSD->nStoreMode > 0 || pMember->IsNowTrade())
								{
									wMsg += L" is currently trading or operating a private store and cannot be summoned.";
									pSocket->SendSystemMessage(wMsg.c_str());
								}else if (pMember->pSD->nActiveUser == 0)
								{
									wMsg += L" is currently teleporting and cannot be summoned.";
									pSocket->SendSystemMessage(wMsg.c_str());
								}else 
								{
									pMember->StopMove();
									PlayerAction::OnTeleportToLocation(pMember, x, y, CGeo::GetInstance()->GetZLoc(x, y, z), false);
								}
							}
						}
					}
				}
			}
		}
	}
}

void CSkillEffect_i_target_cancel_chance::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(pCreature->ValidCreature() && pTarget->ValidCreature())
	{
		if(!pTarget->IsBoss() && !pTarget->IsZZoldagu())
		{
			double LandRate = g_Random.RandDouble(100);
			LandRate *= pTarget->pSD->nLevel;
			LandRate /= pSkillInfo->nMagicLevel;
			if(pSkillInfo && pSkillInfo->attributeType == CSkillInfo::DERANGEMENT)
			{
				LandRate /= pTarget->GetDefAttribute(CSkillInfo::DERANGEMENT);
				
			}

			if(LandRate < dChance)
			{
				typedef void (*f) (CSkillEffect *, CCreature *, CCreature *, CSkillInfo *, CSkillAction *, double);
				f(0x007CD1B0)(this, pCreature, pTarget, pSkillInfo, pAction, distance);
			}
		}
	}
}

void CSkillEffect_i_target_me_chance::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(pCreature->ValidCreature() && pTarget->ValidCreature())
	{
		if(pTarget->IsBoss() || pTarget->IsZZoldagu())
			return;

		if(User *pUser = pTarget->GetUser())
		{
			if(pUser->pED && pUser->pED->spawnProtection)
			{
				return;
			}
		}

		if(pTarget->pSD->nNpcClassID > 1027164 && pTarget->pSD->nNpcClassID < 1027168)
			return;

		double LandRate = g_Random.RandDouble(100);
		LandRate *= pTarget->pSD->nLevel;
		LandRate /= pSkillInfo->nMagicLevel;
		if(pSkillInfo && pSkillInfo->attributeType == CSkillInfo::DERANGEMENT)
		{
			LandRate /= pTarget->GetDefAttribute(CSkillInfo::DERANGEMENT);
		}
		
		if(LandRate < dChance)
		{
			typedef void (*f) (CSkillEffect *, CCreature *, CCreature *, CSkillInfo *, CSkillAction *, double);
			f(0x007B7640)(this, pCreature, pTarget, pSkillInfo, pAction, distance);
		}
	}
}

void CSkillEffect_i_teleport_distance::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(pCreature->ValidUser())
	{
		//g_Log.Add(CLog::Blue, "[%s] Heading: %d", __FUNCTION__, pCreature->pSD->sHeading);
		int nHeading = pCreature->pSD->sHeading;
		
		static const int HalfHeading = 32768;
		nHeading = 65536 - nHeading;
		double Radian = PI * nHeading/HalfHeading;
		double x = pCreature->pSD->Pos.x;
		double y = pCreature->pSD->Pos.y;
		double z = pCreature->pSD->Pos.z;
		x += (this->dDistance * cos(Radian));
		y -= (this->dDistance * sin(Radian));
		pCreature->StopMove();
		PlayerAction::OnTeleportToLocation(pCreature->GetUser(), (int)x, (int)y, CGeo::GetInstance()->GetZLoc((int)x, (int)y, (int)z), true);
	}
}

void CSkillEffect_i_unhide_abnormal::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(pCreature->ValidCreature())
	{
		if(User *pTU = pTarget->SafeCastUser())
		{
			if(pTU->pSD->nAlive && pTU->olympiadUser.IsInside() == FALSE && pTU->pSD->DuelInfo.DuelID == 0)
			{
				pTU->TimerExpiredForHiddenAbnormals(true);
			}else
			{
				pTU->ClearHiddenAbnormals();
			}
		}
	}
}

void CSkillEffect_i_use_skill::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	if(pCreature->ValidCreature() && pTarget)
	{
		DWORD nClassID = (DWORD)skillClassId;

		DWORD nSkillID = nClassID >> 8;
		DWORD nLevel = nClassID - (nSkillID << 8);

		if(CSkillInfo *pSI = g_SkillDB.GetSkillInfo(nSkillID, nLevel))
		{
			pSI->ActivateSkill(pCreature, pTarget, 0, 0);

		}
	}
}

void CSkillEffect_i_dispel_buff::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	guard;

	if(pCreature->ValidCreature() && pTarget->ValidCreature())
	{
		if(pTarget->isBlockBuff)
		{
			unguard;
			return;
		}

		bool validate = false;

		pTarget->abnormalStatusLock.Lock(__FILEW__, __LINE__);

		for(int counter = 0;counter < static_cast<INT32>(maxCount); counter++)
		{
			INT32 buffCount = pTarget->GetBuffAmount();
			if(buffCount > 0)
			{
				INT32 buffIndex = g_Random.RandInt(buffCount);
				INT32 index = 0;
				for( AbnormalDataVector::iterator Iter=pTarget->vAbnormalStatus.begin(); Iter != pTarget->vAbnormalStatus.end(); Iter++)
				{
					AbnormalData& ad = (*Iter);
					if(ad.pSI)
					{
						if(ad.pSI->nResistCancel)
							continue;

						if(ad.pSI->nDebuff == 0 || ad.pSI->nDebuff == 2)
						{
							if(index == buffIndex)
							{
								//chance + chance*dml/50 = chance * (1 + dml/50)
								double dmlParam = static_cast<double>(pSkillInfo->nMagicLevel - ad.pSI->nMagicLevel);
								double chance = baseChance * ( 1.0 + (dmlParam / 50.0) );
								double dice = g_Random.RandDouble(100);
							//	g_Log.Add(CLog::Blue, "[%s] Id[%d] dice[%f] vs chance[%f] baseChance[%f] dmlParam[%f] magicLevel[%d][%d]", __FUNCTION__, ad.pSI->nSkillID, dice, chance, baseChance, dmlParam, pSkillInfo->nMagicLevel, ad.pSI->nMagicLevel);
								chance *= pTarget->resistDispelByCategorySlotBuff;
							//	g_Log.Add(CLog::Blue, "[%s] Id[%d] dice[%f] vs chance[%f] with resist", __FUNCTION__, ad.pSI->nSkillID, dice, chance);

								if(dice < chance && ad.pSI->nSkillID != 1323)
								{
									if(ad.pSI->nAbnormalType > 63 &&  ad.pSI->nAbnormalType < 78)
									{
										//song
										if(pTarget->songSlotsUsed > 0)
											pTarget->songSlotsUsed--;

									}else if(ad.pSI->nAbnormalType > 77 && ad.pSI->nAbnormalType < 91)
									{
										//dance
										if(pTarget->danceSlotsUsed > 0)
											pTarget->danceSlotsUsed--;
									}
									//Deleting from abnormal table
									CreatureAction::OnDeleteAbnormalStatus(pTarget->pSD, ad.pSI->nAbnormalType);
									
									if(User *pUser = pTarget->GetUser())
									{
										pUser->pSocket->Send("cddddd", 0x64, 749, 1, 4, ad.pSI->nSkillID, ad.pSI->nLevel);
									}
									
									pTarget->vAbnormalStatus.erase(Iter);
									validate = true;
								}
								break;
							}
							index++;
						}
					}
				}
			}
		}

		pTarget->abnormalStatusLock.Unlock();

		if(validate)
		{
			pTarget->lpSharedDataLock->Lock(__FILEW__, __LINE__);
			pTarget->ValidateAllOnSIM();
			pTarget->lpSharedDataLock->Unlock();

			pTarget->broadcastInfoPacket = true;

			pTarget->CheckAbnormalVisualEffect();
			if(User *pUser = pTarget->GetUser())
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

void CSkillEffect_i_give_item::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	guard;
	if(User *pUser = pCreature->GetUser())
	{
		if(pUser->IsNowTrade())
		{
			pUser->TradeCancel();
		}

		if(pUser->inventory.CheckAddable(itemId, count))
		{
			pUser->AddItemToInventory(itemId, count);
			g_Logger.Add(L"User[%s] get item[%d][%d] - skill effect i_give_item", pUser->pSD->wszName, itemId, count);
		}

	}
	unguard;
}

void CSkillEffect_i_acquire_skill::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	guard;
	if(User *pUser = pCreature->GetUser())
	{
		int nSkillId = static_cast<int>(skillId);
		int nLevel = static_cast<int>(level);
		CSkillInfo *pInfo = g_SkillDB.GetSkillInfo(nSkillId, nLevel);
		bool acquired = false;
		if(pInfo)
		{
			int acquiredLevel = pUser->GetAcquiredSkillLevel(nSkillId);
			
			if(acquiredLevel < 0)
				acquiredLevel = 0;

			if(acquiredLevel < nLevel)
			{
				acquiredLevel++;
				pUser->AcquireSkill(nSkillId, acquiredLevel);
				pUser->SendAcquireSkillToDB(nSkillId, acquiredLevel);
				acquired = true;
				
				if(acquiredLevel == 1)
				{
					pUser->SendSystemMessage(L"You have acquired new skill.");
				}else
				{
					pUser->SendSystemMessage(L"Skill level have been increased.");
				}
				
				g_Logger.Add(L"User[%s] acquired skill[%d][%d]", pUser->pSD->wszName, nSkillId, acquiredLevel);
			}else
			{
				pUser->SendSystemMessage(L"You already have maximum level of this skill.");
			}

			if(acquired)
			{
				pUser->ValidateParameters();
			}
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid acquire skill[%d][%d] - Skill[%S]", pSkillInfo->wszName);
		}
	}else
	{
		g_Log.Add(CLog::Error, "[%s] Invalid User! Creature[%S]", pCreature->pSD->wszName);
	}
	unguard;
}

void CSkillEffect_i_charm_of_courage::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	guard;
	if(User *pUser = pCreature->GetUser())
	{
		pUser->pED->charmOfCourageLevel = pSkillInfo->nLevel;
		pUser->SendETCBuffStatus();
	}
	unguard;
}

void CSkillEffect_i_additional_box::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	guard;
	if(User *pUser = pCreature->GetUser())
	{
		UINT timestamp = static_cast<UINT>(duration);
		if(timestamp > 0)
		{
			timestamp += time(0);
		}
		if(g_AccountDB.SetAdditionalBox(pUser->pSocket->accountId, timestamp))
		{
			if(timestamp > 0)
			{
				pUser->SendSystemMessage(L"Additional box have been acvitated.");
			}else
			{
				pUser->SendSystemMessage(L"Additional box have been deacvitated.");
			}
		}
	
	}
	unguard;
}

void CSkillEffect_i_dispel_buff_fixed::Instant(CCreature *pCreature, CCreature *pTarget, CSkillInfo *pSkillInfo, CSkillAction *pAction, double distance)
{
	guard;

	if(pCreature->ValidCreature() && pTarget->ValidCreature())
	{
		if(pTarget->isBlockBuff)
		{
			unguard;
			return;
		}

		double resistParam = pTarget->resistDispelByCategorySlotBuff;
		resistParam *= 100;	//0.7 -> 70, 1.3 -> 130

		double resistDice = g_Random.RandDouble(100.0);
		if(resistDice > resistParam)
		{
			unguard;
			return;
		}

		bool validate = false;

		UINT removeCount = static_cast<UINT>(maxCount);
		UINT countDice = g_Random.RandInt(100);
		if(countDice < 10)
		{
			removeCount = static_cast<UINT>(maxCount);
		}else if(countDice < 20)
		{
			double count = maxCount * 0.8;
			removeCount = static_cast<UINT>(count);
		}else if(countDice < 45)
		{
			double count = maxCount * 0.6;
			removeCount = static_cast<UINT>(count);
		}else if(countDice < 60)
		{
			double count = maxCount * 0.4;
			removeCount = static_cast<UINT>(count);
		}else if(countDice < 70)
		{
			double count = maxCount * 0.2;
			removeCount = static_cast<UINT>(count);
		}else
		{
			//30%
			removeCount = 0;
		}

		
		pTarget->abnormalStatusLock.Lock(__FILEW__, __LINE__);
		for(UINT counter = 0;counter < removeCount; counter++)
		{
			INT32 buffCount = pTarget->GetBuffAmount();
			if(buffCount > 0)
			{
				INT32 buffIndex = g_Random.RandInt(buffCount);
				INT32 index = 0;
				for( AbnormalDataVector::iterator Iter=pTarget->vAbnormalStatus.begin(); Iter != pTarget->vAbnormalStatus.end(); Iter++)
				{
					if(Iter->pSI && Iter->pSI->nResistCancel == FALSE)
					{
						if((Iter->pSI->nDebuff == 0 || Iter->pSI->nDebuff == 2) && Iter->pSI->nSkillID != 1323)
						{
							if(index == buffIndex)
							{
								//remove
								if(Iter->pSI->nAbnormalType > 63 &&  Iter->pSI->nAbnormalType < 78)
								{
									//song
									if(pTarget->songSlotsUsed > 0)
										pTarget->songSlotsUsed--;

								}else if(Iter->pSI->nAbnormalType > 77 && Iter->pSI->nAbnormalType < 91)
								{
									//dance
									if(pTarget->danceSlotsUsed > 0)
										pTarget->danceSlotsUsed--;
								}
								//Deleting from abnormal table
								CreatureAction::OnDeleteAbnormalStatus(pTarget->pSD, Iter->pSI->nAbnormalType);
								
								if(User *pUser = pTarget->GetUser())
								{
									pUser->pSocket->Send("cddddd", 0x64, 749, 1, 4, Iter->pSI->nSkillID, Iter->pSI->nLevel);
								}
								
								pTarget->vAbnormalStatus.erase(Iter);
								validate = true;
								break;
							}
							index++;
						}
					}
				}
			}
		}

		pTarget->abnormalStatusLock.Unlock();

		if(validate)
		{
			pTarget->lpSharedDataLock->Lock(__FILEW__, __LINE__);
			pTarget->ValidateAllOnSIM();
			pTarget->lpSharedDataLock->Unlock();

			pTarget->broadcastInfoPacket = true;

			pTarget->CheckAbnormalVisualEffect();
			if(User *pUser = pTarget->GetUser())
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