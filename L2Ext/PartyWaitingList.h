#pragma once

struct WaitingListMember
{
	UINT objectId;
	int classId;
	int level;
	int inRoom;
	WCHAR wName[25];
	User *pUser;
};

typedef list<WaitingListMember> WaitingList;

class CPartyWaitingList : CThreadSafe
{
	WaitingList lData;
	bool IsInList(User *pUser);
public:
	CPartyWaitingList();
	~CPartyWaitingList();
	void AddMember(User *pUser);
	void DelMember(User *pUser);
	void ClearList(User *pUser);
	void TimerExpired();
	void RequestList(User *pUser, int page, int nMinLevel, int nMaxLevel, int nInPartyRoom);
	void AskJoinRoom(User *pUser, PWCHAR wTarget);
	void JoinRoom(User *pUser, int roomId);
};

extern CPartyWaitingList g_PartyWaitingList;