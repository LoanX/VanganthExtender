#include "StdAfx.h"

static int subst_yyaction = 0;
static map<wstring, int>	t_map; // map of words and their tokens
static int				t_map_reqs = 0;
static bool bFoundSkillEnchant = false;

extern "C"
{
	extern INT32 nExpTable[];
}

CLexerForSkillEnchanting *pEnchantLexer;

CLexerForSkillEnchanting::CLexerForSkillEnchanting(bool bFakeInstance)
{
	static bool _init_done = false;

	if (! _init_done)
	{
		_init_done = true;
		LPVOID *org_vmt = (LPVOID*)0xACABB8; // real VMT address
		LPVOID* my_vmt = *(LPVOID**)this;
		CopyVMT(&org_vmt[9], &my_vmt[9], 1); // yyaction()
	}
}

CParserForSkillEnchanting::CParserForSkillEnchanting(bool bFakeInstance)
{
	static bool _init_done = false;

	if (! _init_done)
	{
		_init_done = true;
		LPVOID *org_vmt = (LPVOID*)0xAE3258; // real VMT address
		LPVOID* my_vmt = *(LPVOID**)this;
		CopyVMT(&org_vmt[1], &my_vmt[1], 1); // yyaction()
	}
}

yywint_t CLexerForSkillEnchanting::yyaction(int action)
{
	typedef yywint_t(*f)(CLexerForSkillEnchanting*, int);
	yywint_t  token = f(0x008DCA30L)(this, action);
	
	if (token == SE_ERROR) 
	{
		YYSTYPE& yylval = *(YYSTYPE*)yyparserptr->yylvalptr;
		wstring cString = yylval.id_value;

		if ( wcsncmp(L"prob_79", cString.c_str(), cString.size()) == 0 )
		{
			token = SE_PROB_76;
			subst_yyaction = SE_PROB_79;
		}
		else if ( wcsncmp(L"prob_80", cString.c_str(), cString.size()) == 0 )
		{
			token = SE_PROB_76;
			subst_yyaction = SE_PROB_80;
		}
	}

	return token;
}

void CParserForSkillEnchanting::yyaction(int action)
{
	typedef void(*f)(CParserForSkillEnchanting*, int);

	#define CHECK_SUBST_AND_FALLBACK { if (!subst_yyaction) { f(0x008F7FB0L)(this,action); return; } }

	switch(action)
	{
		case 0x0A: 
			{
				CHECK_SUBST_AND_FALLBACK;

				int  arg1 = yyattribute(0).int_value;

				switch(subst_yyaction)
				{
					case SE_PROB_79:
						pSkillEnchantInfo->probArr[79-76] = arg1;
						break;

					case SE_PROB_80:
						pSkillEnchantInfo->probArr[80-76] = arg1;
						break;
				}

				subst_yyaction = 0;
			}
			break;


		default:
			f(0x008F7FB0L)(this,action);
			break;
	}
}


//-----------------------------------------------------------------

void InitializeSkillEnchanting()
{
	// .text:00000000008F7FE9                 mov     ecx, 70h        ; jumptable 008F7FE7 case 0
	g_HookManager.WriteMemoryDWORD(0x8F7FEA, sizeof(SkillEnchantInfo));
	//CSkillEnchant::IsEnchantSuccess
	g_HookManager.WriteMemoryBYTE(0x79E26E, 81);
	//CSkillEnchant::GetProbIndex fix
	g_HookManager.WriteMemoryBYTE(0x79DC0E, 4);
	//CSkillEnchantChecker::IsLearnableLevel
	g_HookManager.WriteMemoryBYTE(0x79DAC1, 4);
	//void CSkillEnchant::NpcEnchantSkillSuggestion(int NpcIndex, int TalkerID, int TalkerSMIndex, int SkillNameID)
	g_HookManager.WriteMemoryBYTE(0x7A0ECB, 4);
	//bool CSkillEnchantChecker::CheckLearnable(CUserSocket *pUserSocket, const SkillEnchantInfo *pInfo, int nSp, int nExp, int nLevel)
	g_HookManager.WriteMemoryBYTE(0x79E049, 4);

    static CLexerForSkillEnchanting  *lexer  = NULL;
    lexer = new CLexerForSkillEnchanting(true);
    static CParserForSkillEnchanting *parser = NULL;
    parser = new CParserForSkillEnchanting(true);

	CSkillEnchantDB* pDB = &g_SkillEnchantDB;

	g_HookManager.WriteRelativeAddress(0x64300F, 3, pDB, 0);
	g_HookManager.WriteRelativeAddress(0x79EC5A, 3, pDB, 0);
	g_HookManager.WriteRelativeAddress(0x79F199, 3, pDB, 0);
	g_HookManager.WriteRelativeAddress(0x7A0927, 3, pDB, 0);
	g_HookManager.WriteRelativeAddress(0x8469F6, 3, pDB, 0);
	g_HookManager.WriteRelativeAddress(0x846F06, 3, pDB, 0);
	g_HookManager.WriteRelativeAddress(0x8471C9, 3, pDB, 0);
	g_HookManager.WriteRelativeAddress(0x8F8043, 3, pDB, 0);
	g_HookManager.WriteRelativeAddress(0x960049, 3, pDB, 0);
	g_HookManager.WriteRelativeAddress(0x960069, 3, pDB, 0);
	g_HookManager.WriteRelativeAddress(0x970F60, 3, pDB, 0);

	//packet fixes
	g_HookManager.WriteCall( 0x846B51, SendEnchantSkillList_Assemble, 0);
	g_HookManager.WriteCall( 0x7A0FBB, SendEnchantSkillInfo, 0);
	g_HookManager.WriteCall( 0x7A1018, SendEnchantSkillInfo, 0);

	//exp fixes
	// Asm Hooks
	g_HookManager.WriteJump( 0x79E0D4, SkillEnchantSystem_ExpFixHook1, 0);
	g_HookManager.WriteJump( 0x79F9A0, SkillEnchantSystem_ExpFixHook2, 0);
	g_HookManager.WriteJump( 0x79FDA0, SkillEnchantSystem_ExpFixHook3, 0);
}

SkillEnchantInfo* GetSkillEnchantInfo(int skillId, int level)
{
	return g_SkillEnchantDB.GetSkillEnchantInfo(skillId, level);
}

void InitializeSkillEnchanting2()
{
	//amped hook
	INT64 nAmpedX64Address = (INT64)GetModuleHandle(_T("AmpeDx64.dll"));
	if(nAmpedX64Address)
	{
		g_HookManager.WriteCall((nAmpedX64Address+0xAD0A), GetSkillEnchantInfo, 0);
	}
}

int SendEnchantSkillList_Assemble(char* buf, int len, const char* format, ...)
{
	
	va_list tag;
	INT32 nSkillID, nSkillLevel;
	INT32 nSP, nExp;
	va_start (tag, format);
	nSkillID = va_arg (tag, INT32);
	nSkillLevel = va_arg (tag, INT32);
	nSP = va_arg (tag, INT32);
	nExp = va_arg (tag, INT32);
	va_end (tag);

	//format is "dddd" - c4 and "dddQ" - c5
	return Assemble(buf, len, "dddf", nSkillID, nSkillLevel, nSP, CL2LevelUP::GetExperience(nExp));
}

void SendEnchantSkillInfo(CSocket *pSocket, const char* format, ...)
{
	va_list va;
	va_start (va, format);
	BYTE opCode = va_arg(va, BYTE);
	WORD exOpCode = va_arg(va, WORD);
	INT32 skillId = va_arg(va, INT32);
	INT32 level = va_arg(va, INT32);
	INT32 spCost = va_arg(va, INT32);
	INT32 expCost = va_arg(va, INT32);
	INT32 rate = va_arg(va, INT32);
	INT32 reqCount = va_arg(va, INT32);
	INT32 reqBufSize = 0;
	PCHAR reqBuf = NULL;
	if(reqCount > 0)
	{
		reqBufSize  = va_arg(va, INT32);
		reqBuf = va_arg(va, PCHAR);
	}
	va_end(va);

	if(reqBufSize > 0)
		pSocket->Send("chdddfddb", opCode, exOpCode, skillId, level, spCost, expCost, rate, reqCount, reqBufSize, reqBuf);
	else
		pSocket->Send("chdddfdd", opCode, exOpCode, skillId, level, spCost, expCost, rate, reqCount);
}

INT32 SkillEnchantSystem_DivideReqWrapper(INT32 nRequiredExp, INT32 nPlayerExp)
{
	INT32 nReqExp = nRequiredExp;
	
	if ( nPlayerExp >= nExpTable[78] )
	{
		// Fixing the Required Exp For Players Higher Than Level 78 0.00%
		nReqExp /= 64;
	}
	
	return nReqExp;
}