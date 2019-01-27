#pragma once

class COnlineMultipler
{
	BOOL m_Enabled;
	double m_MaxMultipler;
	double m_MaxFixed;
	double m_CurrentMultipler;
	double m_CurrentFixed;
	double m_PlayingMultipler;
	double m_PlayingFixed;
	double m_PrivateStoreMultipler;
	double m_PrivateStoreFixed;
	LPINT m_lpCurrentUsers;
	LPINT m_lpPlayingUsers;
	LPINT m_lpMaxUsers;	//Limit
	LPINT m_lpLetBuilder;
	void LoadINI();
public:
	COnlineMultipler(void);
	virtual ~COnlineMultipler(void);
	int GetCurrentUsers();
	void SetCurrentUsers(int Value);
	int GetPlayingUsers();
	void SetPlayingUsers(int Value);
	int GetMaxUsers();	//GetLimit
	void SetMaxUsers(int Value);	//SetLimit
	void Init();
	static void AuthUserNum(LPVOID pInstance, int nCurrent, int Max);
	static void SendRemoteWho(CSocket *pSocket, const char* format, BYTE opCode, int param1, int max, int current, int playing, int privateStore, PWCHAR wStr, int param7, int param8, int param9, int param10, int param11);
};

extern COnlineMultipler g_OnlineMultipler;