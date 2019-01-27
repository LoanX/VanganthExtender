#pragma once

// namespaces
#ifdef YYSTDCPPLIB
using namespace std;
#endif
#ifdef YYNAMESPACE
using namespace yl;
#endif

#ifndef YYSTYPE
union tagYYSTYPE {
	wchar_t*	id_value;
	int			int_value;
	double		num_value;
	LPVOID		ptr_value;
	long double	lnum_value;
};

#define YYSTYPE union tagYYSTYPE
#endif

#define yyattribute(index) (((YYSTYPE*)yyattributestackptr)[yytop + (index)])

enum TokensForSkillEnchanting
{
	SE_SKILL_NAME = 0x10001,
	SE_ERROR = 0x10002,
	SE_NUMBER = 0x10003,
	SE_ENCHANT_SKILL_BEGIN = 0x10004,
	SE_ENCHANT_SKILL_END = 0x10005,
	SE_ORIGINAL_SKILL = 0x10006,
	SE_ROUTE_ID = 0x10007,
	SE_ENCHANT_ID = 0x10008,
	SE_SKILL_LEVEL = 0x10009,
	SE_EXP = 0x1000A,
	SE_SP = 0x1000B,
	SE_ITEM_NEEDED = 0x1000C,
	SE_PROB_76 = 0x1000D,
	SE_PROB_77 = 0x1000E,
	SE_PROB_78 = 0x1000F,
	SE_PROB_79 = 0x10010,
	SE_PROB_80 = 0x10011
};

class CLexerForSkillEnchanting : public yywflexer
{
public:
	virtual yywint_t yylex() { return 0; };  // fake
	virtual yywint_t yyaction(int action);

	CLexerForSkillEnchanting(bool bFakeInstance);
	CLexerForSkillEnchanting() {};
};

void InitializeSkillEnchanting();
void InitializeSkillEnchanting2();

class CParserForSkillEnchanting : public yywfparser
{
public:
	virtual void yyaction(int action);

//protected:
	/* 0F0 */ INT8 unk001[0xD0];
	/* 1C0 */ SkillEnchantInfo *pSkillEnchantInfo;
	/* 1C8 */

	CParserForSkillEnchanting(bool bFakeInstance);
	CParserForSkillEnchanting() {};
};

//packet fixes
int SendEnchantSkillList_Assemble(char* buf, int len, const char* format, ...);
void SendEnchantSkillInfo(CSocket *pSocket, const char* format, ...);

extern "C"
{
	// Hooks
	void SkillEnchantSystem_ExpFixHook1();
	void SkillEnchantSystem_ExpFixHook2();
	void SkillEnchantSystem_ExpFixHook3();
	// Wrappers
	INT32 SkillEnchantSystem_DivideReqWrapper(INT32 nRequiredExp, INT32 nPlayerExp);
}