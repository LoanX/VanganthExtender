#pragma once


class CSharedNoblessInfo
{
public:
/* 000 */	int nObjectID;
/* 004 */	int nDBID;
/* 008 */	int nNoblessType; 
/* 00C */	int nHeroType;
/* 010 */	int nWinCount;
/* 014 */	int nPreviousCount;
/* 018 */	int nOlympiadPoints;
/* 01C */	int nOlympiadMatchCount;
/* 020 */	int nOlympiadWinCount;
/* 024 */	int nOlympiadLoseCount;
};
typedef CSharedNoblessInfo* LPCSharedNoblessInfo;

class CNoblessSystem
{
	static const INT64 nInstance = 0x7D55770;
public:
	static int GetIndexByCharId(int char_id)
	{
		typedef int (*t)(INT64, int);
		t f = (t)0x475E60;
		return f(nInstance, char_id);
	};
	static LPCSharedNoblessInfo GetInfoByCharId(int char_id)
	{
		typedef LPCSharedNoblessInfo (*t)(INT64, int);
		t f = (t) 0x00475FB0;
		return f(nInstance, char_id);
	}
};