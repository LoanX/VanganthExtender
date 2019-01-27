#pragma once
#include "MemoryObject.h"

struct FriendInfo
{
	/* 00 */ WCHAR wName[25];
	/* 50 */

};


//vt 0x9EE758 size: 0x80
class CFriends : public MemoryObject
{
public:
	/* 018 */ RWLock m_Lock;
	/* 050 */ map<UINT, FriendInfo> m_FriendList;
	/* 068 */ map<UINT, UINT> m_OnlineFriends;

	int GetFriendList(int buffSize, PCHAR buff, int& buffLen)	//dSdd loop
	{
		typedef int(*f)(CFriends*, int, PCHAR, int&);
		return f(0x5C34E0L)(this, buffSize, buff, buffLen);
	}
};