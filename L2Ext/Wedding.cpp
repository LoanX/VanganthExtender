#include "stdafx.h"
#include "wedding.h"
#include "DB.h"
#include "PlayerAction.h"

CWedding g_Wedding;

void CWedding::Init()
{
	m_Enabled = FALSE;
	LoadINI();
	if(m_Enabled)
	{
		g_Log.Add(CLog::Blue, "[%s] system enabled.", __FUNCTION__);
	}
}

/*
[Wedding]
Enabled=0
MinLevel=0
Maxlevel=80
AllowHomo=0
;Price format: item_id;item_amount items separated with space , price will be take from both players when they get married
PriceList=57;100000
;Reward will be given to married players format same as price
RewardList=
;Player wont be able to get married again for x time (time in seconds)
Penalty=604800
;Reuse delay for .gotolove command
TeleportPenalty=3600
*/

void CWedding::LoadINI()
{
	const TCHAR* section = _T("Wedding");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	m_MinLevel = GetPrivateProfileInt(section, _T("MinLevel"), 0, g_ConfigFile);
	m_MaxLevel = GetPrivateProfileInt(section, _T("Maxlevel"), 0, g_ConfigFile);
	m_AllowHomo = GetPrivateProfileInt(section, _T("AllowHomo"), 0, g_ConfigFile);
	m_Penalty = GetPrivateProfileInt(section, _T("Penalty"), 0, g_ConfigFile);
	m_TeleportPenalty = GetPrivateProfileInt(section, _T("TeleportPenalty"), 0, g_ConfigFile);
	m_RequireFormalWear = GetPrivateProfileInt(section, _T("RequireFormalWear"), 0, g_ConfigFile);
	TCHAR temp[8190] = { 0 };
	if(GetPrivateProfileString(section, _T("PriceList"), 0, temp, 8190, g_ConfigFile))
	{
		wstringstream priceStream(Utils::ReplaceString(temp, L";", L" ", true));
		ItemReq ir;
		while(priceStream >> ir.itemId >> ir.amount)
		{
			m_Price.push_back(ir);
		}
		
	}
	if(GetPrivateProfileString(section, _T("RewardList"), 0, temp, 8190, g_ConfigFile))
	{
		wstringstream priceStream(Utils::ReplaceString(temp, L";", L" ", true));
		ItemReq ir;
		while(priceStream >> ir.itemId >> ir.amount)
		{
			m_Reward.push_back(ir);
		}
		
	}
	if(GetPrivateProfileString(section, _T("NormalAnnounce"), 0, temp, 8190, g_ConfigFile))
	{
		m_NormalAnnounce = temp;
	}
	if(GetPrivateProfileString(section, _T("HomoXAnnounce"), 0, temp, 8190, g_ConfigFile))
	{
		m_HomoXAnnounce = temp;
	}
	if(GetPrivateProfileString(section, _T("HomoYAnnounce"), 0, temp, 8190, g_ConfigFile))
	{
		m_HomoYAnnounce = temp;
	}
	if(GetPrivateProfileString(section, _T("MarriageSound"), 0, temp, 8190, g_ConfigFile))
	{
		m_WeddingSound = temp;
	}
}

void CWedding::OnLoad(User *pUser)
{
	guard;
	if(m_Enabled)
	{
		g_DB.RequestLoadWeddingStatus(pUser->nDBID);
	}
	unguard;
}

bool CWedding::CanBeMarried(User *pUser, bool sendMessage)
{
	CTL;
	if(pUser->pSD->nLevel < m_MinLevel)
	{
		if(sendMessage)
		{
			//2592	1	a,Your level is too low to be married!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->SendSystemMessage(2592);
		}
		unguard;
		return false;
	}
	if(pUser->pSD->nLevel > m_MaxLevel)
	{
		if(sendMessage)
		{
			//2593	1	a,Your level is too high to be married!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->SendSystemMessage(2593);
		}
		unguard;
		return false;
	}
	if(pUser->pED->weddingUser.state != WeddingUser::WeddingNone)
	{
		if(sendMessage)
		{
			//2594	1	a,You cannot engage/marry anyone at the moment!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->SendSystemMessage(2594);
		}
		unguard;
		return false;
	}
	if(pUser->pED->weddingUser.penaltyTimeout > time(0))
	{
		if(sendMessage)
		{
			//2595	1	a,The penalty for breaking your last relationship didn't pass yet.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->SendSystemMessage(2595);
		}
		unguard;
		return false;
	}
	unguard;
	return true;
}

void CWedding::EngageReply(User *pUser, UINT answer)
{
	guard;
	if(m_Enabled)
	{
		WeddingUser& wu = pUser->pED->weddingUser;
		if(wu.state == WeddingUser::WeddingNone)
		{
			if(wu.partnerId)
			{
				if(User *pPartner = g_UserDB.GetUserByDBID(wu.partnerId))
				{
					WeddingUser& wuEx = pPartner->pED->weddingUser;
					if(wuEx.state == WeddingUser::WeddingAskEngage)
					{
						if(wuEx.partnerId == pUser->nDBID)
						{
							if(answer)
							{
								//engage
								wu.state = WeddingUser::WeddingEngaged;
								wuEx.state = WeddingUser::WeddingEngaged;
								//2596	1	a,Congratulation! Your engagement has been accepted!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
								pPartner->SendSystemMessage(2596);
								//2597	1	a,Congratulation! You're now engaged.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
								pUser->SendSystemMessage(2597);
								g_DB.RequestSaveWeddingStatus(pUser->nDBID, wu.state, wu.partnerId, wu.partnerName.c_str(), wu.penaltyTimeout, wu.teleportTimeout, wu.weddingTime);
								g_DB.RequestSaveWeddingStatus(pPartner->nDBID, wuEx.state, wuEx.partnerId, wuEx.partnerName.c_str(), wuEx.penaltyTimeout, wuEx.teleportTimeout, wuEx.weddingTime);
							}else
							{
								//clear data for both
								wu.clear();
								wuEx.clear();
								//2598	1	a,Your friend rejected your engagement!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
								pPartner->SendSystemMessage(2598);
							}
						}else
						{
							g_Log.Add(CLog::Error, "[%s] Invalid parnter[%d] vs [%d] - User[%S]", __FUNCTION__, wuEx.partnerId, pUser->nDBID, pUser->pSD->wszName);
							wu.clear();
						}
					}else
					{
						g_Log.Add(CLog::Error, "[%s] Invalid partner[%S] status[%d] - User[%S]!", __FUNCTION__, pPartner->pSD->wszName, wuEx.state, pUser->pSD->wszName);
						wu.clear();
					}
				}else
				{
					//2599	1	a,Your partner have to be online!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->SendSystemMessage(2599);
					wu.clear();
				}
			}
		}
	}
	unguard;
}

bool CWedding::Engage(User *pUser, const WCHAR* wTarget)
{
	guard;
	if(m_Enabled)
	{
		if(CanBeMarried(pUser))
		{
			if(User *pTarget = g_UserDB.GetUserByName(wTarget))
			{
				if(pUser->IsFriend(pTarget))
				{
					if(pTarget->pSD->nSex != pUser->pSD->nSex || m_AllowHomo)
					{
						if( CanBeMarried(pTarget, false) )
						{
							//send ask
							WeddingUser& wu = pUser->pED->weddingUser;
							wu.state = WeddingUser::WeddingAskEngage;
							wu.partnerId = pTarget->nDBID;
							wu.partnerName = pTarget->pSD->wszName;
							WeddingUser& wuEx = pTarget->pED->weddingUser;
							wuEx.partnerId = pUser->nDBID;
							wuEx.partnerName = pUser->pSD->wszName;

							CPacket pck;
							pck.WriteC(0xED);
							pck.WriteD(2461); //2461	1	a,Would you like to be engaged with $s1 ?\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pck.WriteD(1); //number of parameters
							pck.WriteD(0); //text
							pck.WriteS(pUser->pSD->wszName); // x
							pck.WriteD(9000); //timeout
							pck.WriteD(0); //dunno
							pTarget->pSocket->Send("b", pck.GetSize(), pck.GetBuff());
						}else
						{
							//2600	1	a,Your partner doesn't meet requirements!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->SendSystemMessage(2600);
						}
					}else
					{
						//2601	1	a,Homo marriages are not allowed in this server!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->SendSystemMessage(2601);
					}
				}else
				{
					//2602	1	a,You can engage only players from your friendlist!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->SendSystemMessage(2602);
				}
			}else
			{
				//2603	1	a,Your partner have to be online in order to get engaged!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->SendSystemMessage(2603);
			}
		}
	}
	unguard;
	return false;
}

bool CWedding::Marry(User *pUser)
{
	guard;
	if(m_Enabled)
	{
		WeddingUser& wu = pUser->pED->weddingUser;
		if(wu.state == WeddingUser::WeddingEngaged)
		{
			if(User *pTarget = g_UserDB.GetUserByDBID(wu.partnerId))
			{
				if(pUser->IsFriend(pTarget))
				{
					if(pTarget->pED->weddingUser.state == WeddingUser::WeddingEngaged && pTarget->pED->weddingUser.partnerId == pUser->nDBID)
					{
						if(pTarget->pSD->nSex != pUser->pSD->nSex || m_AllowHomo)
						{
							if( pUser->IsInRange(pTarget, 150) )
							{
								//check item requirements
								bool userHas = true;
								bool partnerHas = true;
								for(UINT n=0;n<m_Price.size();n++)
								{
									ItemReq& ir = m_Price[n];
									if(pUser->inventory.HaveItemByClassID(ir.itemId, ir.amount) == false)
									{
										userHas = false;
										break;
									}
									if(pTarget->inventory.HaveItemByClassID(ir.itemId, ir.amount) == false)
									{
										partnerHas = false;
										break;
									}
								}

								if(userHas)
								{
									if(partnerHas)
									{
										if(pUser->equippedFormalWear || !m_RequireFormalWear)
										{
											if(pTarget->equippedFormalWear || !m_RequireFormalWear)
											{
												wu.state = WeddingUser::WeddingAskMarry;
												
												CPacket pck;
												pck.WriteC(0xED);
												pck.WriteD(2462); //2462	1	a,Would you like to be married with $s1 ?\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
												pck.WriteD(1); //number of parameters
												pck.WriteD(0); //text
												pck.WriteS(pUser->pSD->wszName); // x
												pck.WriteD(9000); //timeout
												pck.WriteD(0); //dunno
												pTarget->pSocket->Send("b", pck.GetSize(), pck.GetBuff());
												//2604	1	a,You're now asking your partner if he/she will marry you...\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
												pUser->SendSystemMessage(2604);
											}else
											{
												//2605	1	a,Your partner have to equip Formal Wear!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
												pUser->SendSystemMessage(2605);
											}
										}else
										{
											//2606	1	a,In order to continue please equip Formal Wear!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
											pUser->SendSystemMessage(2606);
										}
									}else
									{
										//2607	1	aYour partner doesn't have enough required items!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
										pUser->SendSystemMessage(2607);
									}
								}else
								{
									//2608	1	a,You don't have enough required items!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
									pUser->SendSystemMessage(2608);
								}
							}else
							{
								//2609	1	a,Your partner is too far away from you!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
								pUser->SendSystemMessage(2609);
							}
						}else
						{
							//2601	1	a,Homo marriages are not allowed in this server!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->SendSystemMessage(2601);
						}
					}else
					{
						//2610	1	a,Seems like your partner isn't engaged with you at the moment.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->SendSystemMessage(2610);
					}
				}else
				{
					//2611	1	a,You can marry only players from your friendlist!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->SendSystemMessage(2611);
				}
			}else
			{
				//2612	1	a,Your partner have to be online in order to get married!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->SendSystemMessage(2612);
			}
		}else
		{
			//2613	1	a,In order to get married you have to engage first!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->SendSystemMessage(2613);
		}
	}
	unguard;
	return false;
}

void CWedding::MarryReply(User *pUser, UINT answer)
{
	guard;
	if(m_Enabled)
	{
		WeddingUser& wu = pUser->pED->weddingUser;
		if(wu.state == WeddingUser::WeddingEngaged)
		{
			if(User *pTarget = g_UserDB.GetUserByDBID(wu.partnerId))
			{
				if(pUser->IsFriend(pTarget))
				{
					WeddingUser& wuEx = pTarget->pED->weddingUser;
					if(wuEx.state == WeddingUser::WeddingAskMarry && wuEx.partnerId == pUser->nDBID)
					{
						if(answer)
						{
							if( pUser->IsInRange(pTarget, 150) )
							{
								//check item requirements
								bool userHas = true;
								bool partnerHas = true;
								for(UINT n=0;n<m_Price.size();n++)
								{
									ItemReq& ir = m_Price[n];
									if(pUser->inventory.HaveItemByClassID(ir.itemId, ir.amount) == false)
									{
										userHas = false;
										break;
									}
									if(pTarget->inventory.HaveItemByClassID(ir.itemId, ir.amount) == false)
									{
										partnerHas = false;
										break;
									}
								}

								if(userHas)
								{
									if(partnerHas)
									{
										//delete required items
										for(UINT n=0;n<m_Price.size();n++)
										{
											ItemReq& ir = m_Price[n];
											pUser->DeleteItemInInventory(ir.itemId, ir.amount);
											pTarget->DeleteItemInInventory(ir.itemId, ir.amount);
										}
										wu.state = WeddingUser::WeddingMarried;
										wuEx.state = WeddingUser::WeddingMarried;
										wu.penaltyTimeout = 0;
										wuEx.penaltyTimeout = 0;
										wu.teleportTimeout = 0;
										wuEx.teleportTimeout = 0;
										wu.weddingTime = time(0);
										wuEx.weddingTime = wu.weddingTime;
										//2614	1	a,Congratulation! You're now married!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
										pTarget->SendSystemMessage(2614);
										pUser->SendSystemMessage(2614);
										g_DB.RequestSaveWeddingStatus(pUser->nDBID, wu.state, wu.partnerId, wu.partnerName.c_str(), wu.penaltyTimeout, wu.teleportTimeout, wu.weddingTime);
										g_DB.RequestSaveWeddingStatus(pTarget->nDBID, wuEx.state, wuEx.partnerId, wuEx.partnerName.c_str(), wuEx.penaltyTimeout, wuEx.teleportTimeout, wuEx.weddingTime);
										//give reward

										for(UINT n=0;n<m_Reward.size();n++)
										{
											ItemReq& ir = m_Reward[n];
											pUser->AddItemToInventory(ir.itemId, ir.amount);
											pTarget->AddItemToInventory(ir.itemId, ir.amount);
										}

										//announce
										wstring announce;
										if(pUser->pSD->nSex == pTarget->pSD->nSex)
										{
											if(pUser->pSD->nSex == SexFemale)
											{
												announce = m_HomoYAnnounce;
											}else
											{
												announce = m_HomoXAnnounce;
											}
										}else
										{
											announce = m_NormalAnnounce;
										}
										announce = Utils::ReplaceString(announce, L"$s1", pTarget->pSD->wszName, true);
										announce = Utils::ReplaceString(announce, L"$s2", pUser->pSD->wszName, true);
										Utils::BroadcastToAllUser_Announce(announce.c_str());
										// play sound
										if(m_WeddingSound.size() > 0)
										{
											//pSocket->Send("cdSdddddd", 0x98, 0, wszName, nCount, nObjectID, x, y, z, 0);
											CHAR soundBuff[8190];
											int len = Assemble(soundBuff, 8190, "cdSdddddd", 0x98, 0, m_WeddingSound.c_str(), 1, (int)pUser->nObjectID, pUser->pSD->Pos.x, (int)pUser->nObjectID, pUser->pSD->Pos.y, (int)pUser->nObjectID, pUser->pSD->Pos.z, 0);
											if(len > 0)
											{
												L2Server::BroadcastToAllUser(len, soundBuff);
											}
										}
									}else
									{
										//2607	1	a,Your partner doesn't have enough required items!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
										pUser->SendSystemMessage(2607);
										wuEx.state = WeddingUser::WeddingEngaged;
									}
								}else
								{
									//2608	1	a,You don't have enough required items!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
									pUser->SendSystemMessage(2608);
									wuEx.state = WeddingUser::WeddingEngaged;
								}
							}else
							{
								//2609	1	a,Your partner is too far away from you!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
								pUser->SendSystemMessage(2609);
								wuEx.state = WeddingUser::WeddingEngaged;
							}
						}else
						{
							wuEx.state = WeddingUser::WeddingEngaged;
							//2615	1	a,Your partner isn't ready yet to be married!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pTarget->SendSystemMessage(2615);
						}
					}else
					{
						//2610	1	a,Seems like your partner isn't engaged with you at the moment.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->SendSystemMessage(2610);
					}
				}else
				{
					//2611	1	a,You can marry players from your friendlist only!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pUser->SendSystemMessage(2611);
				}
			}else
			{
				//2612	1	a,Your partner have to be online in order to get married!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->SendSystemMessage(2612);
			}
		}else
		{
			//2613	1	a,In order to get married you have to engage first!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->SendSystemMessage(2613);
		}
	}
	unguard;
}

bool CWedding::Divorce(User *pUser)
{
	guard;

	if(m_Enabled)
	{
		WeddingUser& wu = pUser->pED->weddingUser;
		if(wu.state == WeddingUser::WeddingMarried || wu.state == WeddingUser::WeddingEngaged)
		{
			//send ask
			CPacket pck;
			pck.WriteC(0xED);
			pck.WriteD(2463); //2463	1	a,Would you like to break your relationship with $s1 ?\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pck.WriteD(1); //number of parameters
			pck.WriteD(0); //text
			pck.WriteS(wu.partnerName.c_str()); // x
			pck.WriteD(9000); //timeout
			pck.WriteD(0); //dunno
			pUser->pSocket->Send("b", pck.GetSize(), pck.GetBuff());
		}
	}

	unguard;
	return false;
}

void CWedding::DivorceReply(User *pUser, UINT answer)
{
	guard;
	if(m_Enabled)
	{
		if(answer)
		{
			WeddingUser& wu = pUser->pED->weddingUser;
			if(wu.state == WeddingUser::WeddingMarried || wu.state == WeddingUser::WeddingEngaged)
			{
				wu.penaltyTimeout = time(0) + m_Penalty;
				if(User *pPartner = g_UserDB.GetUserByDBID(wu.partnerId))
				{
					//2616	1	a,You're no longer in relationship with your partner!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
					pPartner->SendSystemMessage(2616);
					WeddingUser& wuEx = pPartner->pED->weddingUser;
					wuEx.clear();
					wuEx.penaltyTimeout = wu.penaltyTimeout;
				}
				pUser->SendSystemMessage(2616);
				g_DB.RequestSaveWeddingStatus(wu.partnerId, WeddingUser::WeddingNone, 0, L"", wu.penaltyTimeout, 0, 0);
				wu.clear();
				g_DB.RequestSaveWeddingStatus(pUser->nDBID, WeddingUser::WeddingNone, 0, L"", wu.penaltyTimeout, 0, 0);
			}
		}
	}
	unguard;
}

void CWedding::GoToLove(User *pUser)
{
	guard;
	if(m_Enabled)
	{
		WeddingUser& wu = pUser->pED->weddingUser;
		if(wu.state == WeddingUser::WeddingMarried)
		{
			if(pUser->pSD->nKarma == 0 && pUser->IsInCombatMode() == false && pUser->pSD->bGuilty == false && pUser->olympiadUser.GetState() == 0 && pUser->pSD->pExData->territoryData.territoryId[ANTI_RECALL_ZONE] == 0 && pUser->pSD->pExData->territoryData.territoryId[FORBIDDEN_RECALL_ZONE] == 0)
			{
				if(User *pPartner = g_UserDB.GetUserByDBID(wu.partnerId))
				{
					if(pPartner->pSD->nKarma == 0 && pPartner->IsInCombatMode() == false && pPartner->pSD->bGuilty == false && pPartner->olympiadUser.GetState() == 0 && pPartner->pSD->pExData->territoryData.territoryId[ANTI_RECALL_ZONE] == 0 && pPartner->pSD->pExData->territoryData.territoryId[FORBIDDEN_RECALL_ZONE] == 0)
					{
						if(wu.teleportTimeout < time(0))
						{
							IVector pos(pPartner->pSD->Pos);
							wu.teleportTimeout = time(0) + m_TeleportPenalty;
							g_DB.RequestSaveWeddingStatus(pUser->nDBID, wu.state, wu.partnerId, wu.partnerName.c_str(), wu.penaltyTimeout, wu.teleportTimeout, wu.weddingTime);
							PlayerAction::OnTeleportToLocation(pUser, pos.x, pos.y, pos.z, 0);
						}else
						{
							//2617	1	a,Go to love reuse delay didn't pass yet!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
							pUser->SendSystemMessage(2617);
						}
					}else
					{
						//2618	1	a,Your love is engaged in fight or in a zone that forbids teleportation!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
						pUser->SendSystemMessage(2618);
					}
				}
			}else
			{
				//2619	1	a,You cannot teleport to your love in current state!\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
				pUser->SendSystemMessage(2619);
			}
		}else
		{
			//2620	1	a,You have to be married in order to get .gotolove command working.\0	0	79	9B	B0	FF	a,	a,	0	0	0	0	0	a,	a,none\0
			pUser->SendSystemMessage(2620);
		}
	}
	unguard;
}