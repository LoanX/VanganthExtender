#include "StdAfx.h"
#include "AutoLoot.h"

CAutoLoot g_AutoLoot;

CAutoLoot::CAutoLoot() : m_enabled(FALSE)
{
}

CAutoLoot::~CAutoLoot()
{
}

void CAutoLoot::Init()
{
	const TCHAR* section = _T("AutoLoot");

	m_enabled = GetPrivateProfileInt(section, _T("Enabled"), 0, g_ConfigFile);
	m_activeByDefault = GetPrivateProfileInt(section, _T("ActiveByDefault"), 0, g_ConfigFile);
	m_affectParty = GetPrivateProfileInt(section, _T("AffectParty"), 0, g_ConfigFile);
	m_enableUserExCommand = GetPrivateProfileInt(section, _T("EnableUserExCommand"), 1, g_ConfigFile);
	m_affectZZoldagu = GetPrivateProfileInt(section, _T("AffectZZoldagu"), 0, g_ConfigFile);
	m_affectBoss = GetPrivateProfileInt(section, _T("AffectBoss"), 0, g_ConfigFile);
	TCHAR temp[0x4000] = { 0 };
	if(GetPrivateProfileString(section, _T("ExceptionItemList"), 0, temp, sizeof(temp), g_ConfigFile))
	{
		wstringstream itemStream(temp);
		int itemId;
		while(itemStream >> itemId)
		{
			m_ExceptionItems.push_back(itemId);
		}
	}
	if(GetPrivateProfileString(section, _T("AffectedItemList"), 0, temp, sizeof(temp), g_ConfigFile))
	{
		wstringstream itemStream(temp);
		int itemId;
		while(itemStream >> itemId)
		{
			m_AffectedItems.push_back(itemId);
		}
	}
	if(m_enabled)
	{
		g_Log.Add(CLog::Blue, "Auto Loot feature is enabled.");
	}
}

bool CAutoLoot::Handle(CNPC *pNpc, CCreature *pKiller, INT32 itemId, INT32 amount)
{
	guard;
	if(m_enabled)
	{
		if(pNpc->ValidCreature() && pKiller->ValidCreature())
		{
			for(UINT n=0;n<m_ExceptionItems.size();n++)
			{
				if(itemId == m_ExceptionItems[n])
				{
					unguard;
					return false;
				}
			}
			if(m_AffectedItems.size() > 0)
			{
				//check if item is on the list
				bool inList = false;
				for(UINT n=0;n<m_AffectedItems.size();n++)
				{
					if(itemId == m_AffectedItems[n])
					{
						inList = true;
						break;
					}
				}
				if(!inList)
				{
					unguard;
					return false;
				}
			}

			if( (pNpc->IsBoss() && !m_affectBoss) || (pNpc->IsZZoldagu() && !m_affectZZoldagu) )
			{
				unguard;
				return false;
			}

			if(amount < 1)
			{
				g_Log.Add(CLog::Error, "[%s] Invalid item count[%d][%d] npc[%S] killer[%S]", __FUNCTION__, itemId, amount, pNpc->pSD->wszName, pKiller->pSD->wszName);
				unguard;
				return false;
			}

			if(pNpc->pContributeData)
			{
				if(pNpc->pContributeData->contributorLevel > 0 && pNpc->pContributeData->contributorId > 0)
				{
					User *pUser = User::GetUserBySID(&pNpc->pContributeData->contributorId);
					if(pUser->ValidUser())
					{
						if(!pNpc->IsInRange(pUser, 1500))
						{
							pUser = 0;
						}
					}

					if(!pUser->ValidUser())
					{
						pUser = pKiller->GetUser();
						if(pKiller->IsSummon())
						{
							pUser = pKiller->CastSummon()->GetMaster();
						}
						if(pUser->ValidUser())
						{
							if(!pNpc->IsInRange(pUser, 1500))
							{
								pUser = 0;
							}
						}
					}

					if(pUser->ValidUser())
					{
						if(CParty *pParty = pUser->GetParty())
						{
							if(m_affectParty)
							{
								UserVector members;
								pParty->GetAllMembersInRange(members, pNpc->pSD->Pos, 1500);

								bool isAutoLoot = false;
								for(UINT n=0;n<members.size();n++)
								{
									if(User *pMember = members[n]->SafeCastUser())
									{
										if(pMember->pED->autoLoot)
										{
											isAutoLoot = true;
											break;
										}
									}
								}

								if(!isAutoLoot)
								{
									unguard;
									return false;
								}
									
								//adena distribution
								if(itemId == 57)
								{
									if(members.size() > 0)
									{
										amount /= static_cast<int>(members.size());
									}

									for(UINT index = 0;index < members.size();index++)
									{
										if(User *pMember = members[index]->SafeCastUser())
										{
											if(pMember->IsNowTrade())
											{
												pMember->TradeCancel();
											}
											if(pMember->inventory.CheckAddable(itemId, amount))
											{
												pMember->AddItemToInventory(itemId, amount);
											}
										}
									}
									unguard;
									return true;
								}

								//299	1	a,$s1 has obtained $s3 $s2.\0	0	0	FF	FF	FF	a,	a,	0	0	0	0	0	a,	a,none\0
								//300	1	a,$s1 has obtained $s2.\0	0	0	FF	FF	FF	a,	a,	0	0	0	0	0	a,	a,none\0



								//distribute other items
								if(pParty->itemRoutingType == IR_FINDERS_KEEPERS)
								{
									if(pUser->pED->autoLoot)
									{
										if(pUser->IsNowTrade())
										{
											pUser->TradeCancel();
										}
										if(pUser->inventory.CheckAddable(itemId, amount))
										{
											pUser->AddItemToInventory(itemId, amount);
											if(amount > 1)
											{
												CSystemMessage msg(299);
												msg.AddText(pUser->pSD->wszName);
												msg.AddItem(itemId);
												msg.AddNumber(amount);
												int len = msg.GetSize();
												PCHAR buff = msg.GetBuff();
												for(UINT l=0;l<members.size();l++)
												{
													if(User *pMember = members[l]->SafeCastUser())
													{
														if(pMember->nObjectID != pUser->nObjectID)
														{
															pMember->pSocket->Send("b", len, buff);
														}
													}
												}
											}else
											{
												CSystemMessage msg(300);
												msg.AddText(pUser->pSD->wszName);
												msg.AddItem(itemId);
												int len = msg.GetSize();
												PCHAR buff = msg.GetBuff();
												for(UINT l=0;l<members.size();l++)
												{
													if(User *pMember = members[l]->SafeCastUser())
													{
														if(pMember->nObjectID != pUser->nObjectID)
														{
															pMember->pSocket->Send("b", len, buff);
														}
													}
												}
											}
											unguard;
											return true;
										}
									}
									unguard;
									return false;
								}else if(pParty->itemRoutingType == IR_RANDOM || pParty->itemRoutingType == IR_RANDOM_INCLUDING_SPOIL)
								{
									UINT diceIndex = g_Random.RandInt(static_cast<int>(members.size()));

									for(UINT index = 0;index < members.size();index++)
									{
										if(User *pMember = members[diceIndex]->SafeCastUser())
										{
											if(pMember->pED->autoLoot)
											{
												if(pMember->IsNowTrade())
												{
													pMember->TradeCancel();
												}
												if(pMember->inventory.CheckAddable(itemId, amount))
												{
													pMember->AddItemToInventory(itemId, amount);
													if(amount > 1)
													{
														CSystemMessage msg(299);
														msg.AddText(pMember->pSD->wszName);
														msg.AddItem(itemId);
														msg.AddNumber(amount);
														int len = msg.GetSize();
														PCHAR buff = msg.GetBuff();
														for(UINT l=0;l<members.size();l++)
														{
															if(User *pMemberEx = members[l]->SafeCastUser())
															{
																if(pMemberEx->nObjectID != pMember->nObjectID)
																{
																	pMemberEx->pSocket->Send("b", len, buff);
																}
															}
														}
													}else
													{
														CSystemMessage msg(300);
														msg.AddText(pMember->pSD->wszName);
														msg.AddItem(itemId);
														int len = msg.GetSize();
														PCHAR buff = msg.GetBuff();
														for(UINT l=0;l<members.size();l++)
														{
															if(User *pMemberEx = members[l]->SafeCastUser())
															{
																if(pMemberEx->nObjectID != pMember->nObjectID)
																{
																	pMemberEx->pSocket->Send("b", len, buff);
																}
															}
														}
													}
													unguard;
													return true;
												}
											}
										}
										diceIndex++;
										diceIndex %= members.size();
									}
									unguard;
									return false;
								}else if(pParty->itemRoutingType == IR_BY_TURN || pParty->itemRoutingType == IR_BY_TURN_INCLUDING_SPOIL)
								{
									for(UINT counter=0;counter<members.size();counter++)
									{
										UINT index = pParty->indexByOrder % pParty->memberCount;
										pParty->indexByOrder = (pParty->indexByOrder + 1) % pParty->memberCount;
										index %= members.size();

										if(User *pMember = members[index]->SafeCastUser())
										{
											if(pMember->pED->autoLoot)
											{
												if(pMember->IsNowTrade())
												{
													pMember->TradeCancel();
												}
												if(pMember->inventory.CheckAddable(itemId, amount))
												{
													pMember->AddItemToInventory(itemId, amount);
													if(amount > 1)
													{
														CSystemMessage msg(299);
														msg.AddText(pMember->pSD->wszName);
														msg.AddItem(itemId);
														msg.AddNumber(amount);
														int len = msg.GetSize();
														PCHAR buff = msg.GetBuff();
														for(UINT l=0;l<members.size();l++)
														{
															if(User *pMemberEx = members[l]->SafeCastUser())
															{
																if(pMemberEx->nObjectID != pMember->nObjectID)
																{
																	pMemberEx->pSocket->Send("b", len, buff);
																}
															}
														}
													}else
													{
														CSystemMessage msg(300);
														msg.AddText(pMember->pSD->wszName);
														msg.AddItem(itemId);
														int len = msg.GetSize();
														PCHAR buff = msg.GetBuff();
														for(UINT l=0;l<members.size();l++)
														{
															if(User *pMemberEx = members[l]->SafeCastUser())
															{
																if(pMemberEx->nObjectID != pMember->nObjectID)
																{
																	pMemberEx->pSocket->Send("b", len, buff);
																}
															}
														}
													}
													unguard;
													return true;
												}
											}
										}
									}
									unguard;
									return false;
								}
							}else
							{
								unguard;
								return false;
							}
						}else
						{
							//without party
							if(pUser->pED->autoLoot)
							{
								if(pUser->IsNowTrade())
								{
									pUser->TradeCancel();
								}
								if(pUser->inventory.CheckAddable(itemId, amount))
								{
									pUser->AddItemToInventory(itemId, amount);
									unguard;
									return true;
								}
							}
							unguard;
							return false;
						}
					}
				}
			}
		}
	}
	unguard;
	return false;
}

