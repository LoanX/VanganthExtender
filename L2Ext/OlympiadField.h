#pragma once
#include "OlympiadObserverEntry.h"

class COlympiadField
{
public:
	enum GAME_RULE
	{
		CLASS_FREE_ENTRY = 1,
		CLASS_ENTRY = 2
	};
/* 000 */	int nSomething1;
/* 004 */	int nSomething2;
/* 008 */	int nSomething3;
/* 00C */	int nSomething4;
/* 010 */	int nSomething5;
/* 014 */	int nSomething6;
/* 018 */	int nGameRule;	//ClassFreeEntry = 1, ClassEntry = 2, 
/* 01C */	int nPlayer1SID;
/* 020 */	int nPlayer2SID;
/* 024 */	int nPlayer1DBID;
/* 028 */	int nPlayer2DBID;
/* 02C */	int nPlayer1ServerIndex;
/* 030 */	int nPlayer2ServerIndex;
/* 034 */	int nWinner; //1- First user, 2 - second, 3 - draw
/* 038 */	int nFieldStatus; // 4- process match
/* 03C */	long nProcessingMessage; //interlocked
/* A0 CIOCRITICALSECTION */
public:
	COlympiadObserverEntry* GetObserverEntry();
	void ChangeStatus(int nStatus);
	bool CheckGameExit();
	bool SummonUser();
	void SetLoser(BYTE cLoser);
	void StatusFinishMatch();
	void SendVToPlayers(const char *format, va_list args);
	void Broadcast(const char *format, ...);
};