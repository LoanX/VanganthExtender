#include "StdAfx.h"
#include "DB.h"
#include "VIPSystem.h"
#include "DnN.h"
#include "SchemeBuffer.h"

union PackedDouble {DOUBLE d; INT64 n;};
extern INT32 g_MaxPartyLevelRange;
extern BOOL g_IsDistributingExpSp[32];
extern CCreature* g_DistributingCreature[32];
extern double g_PartyExpRate;

extern "C" 
{
	INT32 nExpTable[] = 
	{
			 0,
			 0,
			68,
		   363,
		  1168,
		  2884,
		  6038,
		 11287,
		 19423,
		 31378,
		 48229,  //level 10
		 71201,
		101676,
		141192,
		191452,
		254327,
		331864,
		426284,
		539995,
		675590,
		835854,  //level 20
		1023775,
		1242536,
		1495531,
		1786365,
		2118860,
		2497059,
		2925229,
		3407873,
		3949727,
		4555766,  //level 30
		5231213,
		5981539,
		6812472,
		7729999,
		8740372,
		9850111,
		11066012,
		12395149,
		13844879,
		15422851,  //level 40
		17137002,
		18995573,
		21007103,
		23180442,
		25524751,
		28049509,
		30764519,
		33679907,
		36806133,
		40153995, //level 50
		45524865,
		51262204,
		57383682,
		63907585,
		70852742,
		80700339,
		91162131,
		102265326,
		114038008,
		126509030,  //level 60
		146307211,
		167243291,
		189363788,
		212716741,
		237351413,
		271973532,
		308441375,
		346825235,
		387197529,
		429632402,  //level 70
		474205751,
		532692055,
		606319094,
		696376867,
		804219972,
		931275828,
		1151275834,
		1511275834, //78 - 2099275834 //79 4200000000	//80 6300000000	//81 8820000000
		1520463334, // 2099275834 - Last Level / 64 = (1511275834 + 9187500 = 1520463334)  [Level 79]
		1553287149,	// 4200000000 - Last Level / 64 = (1520463334 + 32823815 = 1553287149) [Level 80]
		1586099570  // 6299994999 - Last Level / 64 = (1553287149 + 32812421 = 1586099570) [Level 81]
	};
	INT32 nMaxLevel = 78;
}

extern double g_ExpCurve[85];

void CL2LevelUP::StartSystem()
{
	nMaxLevel = GetPrivateProfileInt(_T("L2Server"), _T("MaxLevel"), 80, g_ConfigFile);
	DWORD dwFuncAddr = (DWORD)CL2LevelUP::ExpInc;
	// ExpInc Hooks
	g_HookManager.WriteMemory(0xA87528, &dwFuncAddr, 4);
	//UserInfo Fix
	g_HookManager.WriteJump( 0x856B6A, UserInfoExpFix1, 6);
	g_HookManager.WriteJump( 0x85760D, UserInfoExpFix2, 6);
	// Validate Level Hooks
	dwFuncAddr = (INT32)CL2LevelUP::ValidateLevel;
	g_HookManager.WriteMemory(0x9B0D30, &dwFuncAddr, 4);
	g_HookManager.WriteMemory(0xA28E50, &dwFuncAddr, 4);
	g_HookManager.WriteMemory(0xA87630, &dwFuncAddr, 4);
	g_HookManager.WriteMemory(0xAA0E40, &dwFuncAddr, 4);
	// Set Param Array (Level) Limit [Level 80 - 0.00%]
	INT32 nVal = nMaxLevel-1;
	g_HookManager.WriteMemory(0x444C9A, &nVal, 1);									// Array Last Level
	// GetKarmaIncreaseAmount (Level) Limit
	g_HookManager.WriteMemory(0x74580A, &nVal, 1);
	// Array (Exp Table) Memory Address Patch
	// Addresses in L2Server (656[NA])
	// 0x98B4D0 (Exp Table 1) Set Param Level								[Code Ref: 0x58B4CC] (Max Level 78 - 100%) [Replaced]
	// 0x9B11E0 (Exp Table 2) Validate Level								[Code Ref: 0x5B11DC] (Max Level 78 - 100%) [Replaced]
	// 0x9CCB50 (Exp Table 3) Pledge Dismiss Functions / Lost Die Exp		[Code Ref: 0x5CCB4C] (Max Level 78 - 100%) [Replaced]
	// 0xA60020 (Exp Table 4) CSkillEnchantChecker / CSkillEnchant			[Code Ref: 0x66001C] (Max Level 78 - 100%) [Replaced]
	// 0xA7B0A0 (Exp Table 5) User::ExpDown()								[Code Ref: 0x67B09C] (Max Level 78 - 100%) [Replaced]
	// 0xA87FF0 (Exp Table 6) User::InitSubJob()							[Code Ref: 0x687FEC] (Max Level 78 - 100%) [Replaced]
	// Rewritting Experience Table Address In the Needed Places
	nVal = ((INT32)&nExpTable)-0x400000;
	g_HookManager.WriteMemory(0x444CB4, &nVal, 4);			// Set Param
	nVal = ((INT32)&nExpTable)-0x400000+4;
	g_HookManager.WriteMemory(0x55654B, &nVal, 4);			// ReplyDeletePledge (Offset +4)
	nVal = ((INT32)&nExpTable)-0x400000;
	g_HookManager.WriteMemory(0x556553, &nVal, 4);			// ReplyDeletePledge
	nVal = ((INT32)&nExpTable)-0x400000+4;
	g_HookManager.WriteMemory(0x55916B, &nVal, 4);			// ReplyQueryCastleSiegeForDismiss (Offset +4)
	nVal = ((INT32)&nExpTable)-0x400000;
	g_HookManager.WriteMemory(0x559173, &nVal, 4);			// ReplyQueryCastleSiegeForDismiss
	nVal = ((INT32)&nExpTable)-0x400000+4;
	g_HookManager.WriteMemory(0x594033, &nVal, 4);			// ReplyDismissAlliance (Offset +4)
	nVal = ((INT32)&nExpTable)-0x400000;
	g_HookManager.WriteMemory(0x59403A, &nVal, 4);			// ReplyDismissAlliance
	nVal = ((INT32)&nExpTable)-0x5452BC;
	g_HookManager.WriteMemory(0x5452BC, &nVal, 4);			// Compute Lost Experience (User::Die()) (Offset +4)
	nVal = ((INT32)&nExpTable)-0x400000+4;
	g_HookManager.WriteMemory(0x8041F5, &nVal, 4);			// User::ExpDown (Offset +4)
	nVal = ((INT32)&nExpTable)-0x400000;
	g_HookManager.WriteMemory(0x8041FD, &nVal, 4);			// User::ExpDown
	nVal = ((INT32)&nExpTable)-0x400000;
	g_HookManager.WriteMemory(0x851994, &nVal, 4);			// User::InitSubJob()
	// Karma Calculation Assembly Fix
//	g_HookManager.WriteJump( 0x7FF550, (INT32)LevelUpSystemFixHook1, 0);
	g_HookManager.WriteCall(0x7FF561, CL2LevelUP::CPCParamDB_GetKarmaIncreaseAmount, 0);
	// CPet Class Hex Hooks
	nVal = nMaxLevel+1;
	g_HookManager.WriteMemory(0x749B2A, &nVal, 1);
	// CPet SetExp Fix
	g_HookManager.WriteJump( 0x74999C, LevelUpSystemFixHook2, 0);
	// Packed 128 Bit Floating Point Integers Patch
	// PackedDouble x;
	g_Log.Add(CLog::Blue, "[%s] MaxLevel[%d] - Done.", __FUNCTION__, nMaxLevel);
}

double CL2LevelUP::CPCParamDB_GetKarmaIncreaseAmount(LPVOID pInstance, int Level)
{
	typedef double (*f)(LPVOID, int);
	double ret = f(0x745770L)(pInstance, Level);

	if(Level >= 78)
	{
		ret /= 64;		//divide by 64
	}

	return ret;
}

void CL2LevelUP::ExpInc(User *pUser, INT32 nExp, bool bRessurection)
{
	if(!bRessurection)
	{
		if(nExp > 0)
		{
			if(pUser->pSD->pExData->stopIncExp)
			{
				return;
			}

			if(g_VIPSystem.IsEnabled())
			{
				if(pUser->pSD->vipLevel)
				{
					nExp = g_VIPSystem.GetExp(nExp, pUser->pSD->vipLevel);
				}
			}

			UINT currentThreadIdx = GetThreadIndex();
			if(currentThreadIdx < 32)
			{
				if(g_IsDistributingExpSp[currentThreadIdx])
				{
					if(pUser->CanGetPartyBonus(g_DistributingCreature[currentThreadIdx]))
					{
						double modExp = static_cast<double>(nExp);
						modExp *= g_PartyExpRate;
						nExp = static_cast<INT32>(modExp);
					}
				}
			}

			//exp curve
			{
				double exp = static_cast<double>(nExp);
				exp *= g_ExpCurve[pUser->pSD->nLevel];
				nExp = static_cast<INT32>(exp);
			}

			double exp = static_cast<double>(nExp);
			exp *= pUser->pSD->pExData->SkillMod.bonusExp;
			exp *= g_DnNSystem.GetRate(DnN::RateExp);
			nExp = static_cast<INT32>(exp);
		}
	}

	INT64 n4Percent = pUser->pSD->nExp;
	n4Percent *= 5;
	n4Percent /= 100;
	n4Percent *= -1;

	if(nExp < n4Percent)
	{
		n4Percent *= -1;
		g_Log.Add(CLog::Error, "Exp overflow user[%S] [%d] chagned to [%d]", pUser->pSD->wszName, nExp, n4Percent);
		nExp = n4Percent; 
	}

	INT32 nExpGained = nExp;
	// Functions
	typedef void (__fastcall *_ExpInc) (User *pUser, INT32 nExp, bool bRessurection);
	_ExpInc xExpInc = (_ExpInc)0x007FF470L;
	if(pUser->ValidUser())
	{
		// Variables
		CreatureSharedData *pSD = pUser->pSD;
		INT32 nLevel     = pSD->nLevel;
		if ( nLevel >= 78 && pSD->nAlive )
		{
			// Dividing the Gained Amount by 64 Times !
			nExpGained /= 64;
		}
		// Checking if the Gained Exp + Actual Exp will Pass Level 78 0.00% Experience, if so we have to divide it too.
		if ( nLevel <= 77 && (pSD->nExp + nExpGained) > nExpTable[78] )
		{
			// Computing (Difference Between Level 77 and Level 78 Exp)
			nExpGained = nExpTable[78] - pSD->nExp;
			// Dividing the Gained Amount (After 78) by 64 Times !
			nExpGained  +=  (nExp-nExpGained) / 64;
		}
		//LOG->Add(1,"Gain : %d - %d", nExp, nExpGained);
	}else
		nExpGained /=64;

	if(nExpGained > 0)
	{
		//check if there is level restriction
		if(g_VIPSystem.IsEnabled())
		{
			//blocking exp for not vips
			if(pUser->pSD->vipLevel == 0)
			{
				if(UINT levelRestriction = g_VIPSystem.GetNotVIPLevelRestriction())
				{
					INT32 delta = nExpTable[levelRestriction + 1] - pUser->pSD->nExp;
					INT32 expDiff = nExpTable[levelRestriction + 1] - (pUser->pSD->nExp + nExpGained);
					if(expDiff < 2)
					{
						if(delta > 1)
						{
							nExpGained = delta;
						}else
						{
							//block the exp
							return;
						}
					}
				}
			}
		}
	}

	xExpInc(pUser, nExpGained, bRessurection);
}

bool CL2LevelUP::ValidateLevel(CCreature *pCreature)
{
	// Limits
	INT32 nMinLevel = 1;
	// Logic Variables
	bool bResult = false;
	bool bSave = false;
	bool bLevelUp = false;
	bool bLevelDown = false;
	// Checking Valid Pointer
	if ( !pCreature->ValidCreature() )
		return false;
	// Functions
	typedef void (__fastcall *_LUP) (CCreature *pCreature, INT32 nLevels);
	_LUP xLevelUP = (_LUP)0x00515A10L;
	typedef void (__fastcall *_LDOWN) (CCreature *pCreature, INT32 nLevels);
	_LDOWN xLevelDown = (_LDOWN)0x00515370L;
	// Variables
	CreatureSharedData *pSD = pCreature->pSD;
	INT32 nLevel = pSD->nLevel;
	INT32 nExperience = pSD->nExp;
	// Fixing Max Level
	if ( nLevel > nMaxLevel )
	{
		nLevel = nMaxLevel;
		pSD->nLevel = nMaxLevel;
		bSave = true;
	}
	// Fixing Min Level
	if ( nLevel < nMinLevel )
	{
		nLevel = nMinLevel;
		pSD->nLevel = nMinLevel;
		bSave = true;
	}
	// Fixing Max Experience
	if ( nExperience > nExpTable[nMaxLevel+1] )
	{
		nExperience = nExpTable[nMaxLevel+1]-1;
		pSD->nExp = nExperience;
	}
	// Fixing Min Experience
	if ( nExperience < nExpTable[nMinLevel] )
	{
		nExperience = nExpTable[nMinLevel];
		pSD->nExp = nExperience;
	}
	// LOG->Add(1, "Experience Necessary for Level : %d[%ld/%ld]", nLevel, nExpTable[nLevel-1], nExperience);
	// If the Current Experience is Equal or Higher The Current Level Requirment
	// Check for a possible Level UP !
	if ( nExperience >= nExpTable[nLevel+1] && !bResult && nLevel < nMaxLevel)
	{
		INT32 nLevelUp = 0;
		bool  bLevelFound = false;
		while ( !bLevelFound )
		{
			// LOG->Add(1, "Comparing Current Experience With One Level After This : %d/%d", nLevel+nLevelUp, nLevel);
			if ( nLevel+nLevelUp <= nMaxLevel )
			{
				if ( nExperience < nExpTable[nLevel+nLevelUp+1] )
					bLevelFound = true;
				else
					nLevelUp++;
			}
			else
				bLevelFound = true;
		}
		bSave = true;
		bResult = true;
		bLevelUp = true;
		nLevel  +=  nLevelUp;
		xLevelUP(pCreature, nLevelUp);
		
		if(User *pUser = pCreature->GetUser())
		{
			if(g_Config.IsSet(CConfig::SPIRIT_SYSTEM))
			{
				g_SpiritSystem.ValidateSkill(pUser);
			}
		}
	}
	// If the Current Experience is Lower than The Current Level Requiriment, Issue a Level Down !
	if ( nExperience < nExpTable[nLevel] && !bResult && nLevel > nMinLevel)
	{
		INT32 nLevelDown = 0;
		bool  bLevelFound = false;
		while ( !bLevelFound )
		{
			if ( nLevel+nLevelDown >= nMinLevel )
			{
				if ( nExperience >= nExpTable[nLevel+nLevelDown] )
					bLevelFound = true;
				else
					nLevelDown--;
			}
			else
				bLevelFound = true;
		}
		bSave = true;
		bResult = true;
		bLevelDown = true;
		nLevel  +=  nLevelDown;
		xLevelDown(pCreature, (nLevelDown * -1));
	}
	// Recharging Status
	if ( bLevelUp )
	{
		pSD->fHP = pSD->MaxHP;
		pSD->fMP = pSD->MaxMP;
		pSD->fCP = pSD->MaxCP;
	}
	// Validating Status
	pCreature->ValidateAllOnSIM();
	if ( bLevelUp || bLevelDown )
	{
		if ( User *pUser = pCreature->SafeCastUser())
		{
			// Performing Extra Validations
			pUser->DoExtraValidation();
			g_DB.RequestSaveCharacterInfo(pUser, false, true);
			// Requesting Newbie Check
			g_DB.RequestNewbieCheck(pUser);
			// Sending ETC Buff Status Bar Update
			pUser->SendETCBuffStatus();
			g_SchemeBuffer.ValidateBuffList(pUser, false, false);
			g_SchemeBuffer.ValidateBuffList(pUser, false, true);
			g_SchemeBuffer.ValidateBuffList(pUser, true, false);
			g_SchemeBuffer.ValidateBuffList(pUser, true, true);
		}
	}
	return bResult;
}
// Used to Convert The Exp The Clientside Should Receive (UserInfo)
INT64 CL2LevelUP::GetExperience(INT32 nExperience)
{
	INT64 nNewExperience = 0;
	if ( nMaxLevel > 78 )
	{
		if ( nExperience > nExpTable[78] )
			nNewExperience = ((INT64)nExpTable[78] + (((INT64)nExperience-(INT64)nExpTable[78])*64));
		else
			nNewExperience = (INT64)nExperience;
	}
	else
	{
		nNewExperience = nExperience;
	}
	return nNewExperience;
}
INT64 CL2LevelUP_GetExperienceWrapper(INT32 nExperience)
{
	return CL2LevelUP::GetExperience(nExperience);
}
