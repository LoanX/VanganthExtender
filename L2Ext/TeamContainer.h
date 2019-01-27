#pragma once


class CTeamContainer : private CThreadSafe
{
private:
	map<int, UserList> mData;
public:
	void Initialize();
	int GenerateNewTeam();
	void ViewTeamList(CSocket *pSocket);
	void ViewUserList(CSocket *pSocket, int nTeam);
	void SendSystemMessage(int nTeam, int nMsgID, int nParam);
	void SendSystemMessage(int nTeam, int nMsgID);
	void Broadcast(int nTeam, size_t nSize, PCHAR packet);
	void DelTeam(CSocket *pSocket, int nTeam);
	void ClearTeam(CSocket *pSocket, int nTeam);
	void DelUser(CSocket *pSocket, User *pTarget);
	void DelUser(CSocket *pSocket, int nTeam, User *pTarget);
	void AddUser(CSocket *pSocket, int nTeam, User *pTarget);
};

extern CTeamContainer g_TeamContainer;