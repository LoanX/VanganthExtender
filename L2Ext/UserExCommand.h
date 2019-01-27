#pragma once

typedef bool (*UserExCommandCallback)(User*, wstring);

class CUserExCommand
{
	map<wstring, UserExCommandCallback> m_commands;
public:
	CUserExCommand();
	~CUserExCommand();
	void Init();
	bool IsUserExCommand(WCHAR* wCommand);
	void Bind(wstring name, UserExCommandCallback func);
	bool Handle(User *pUser, WCHAR* wCommand);
};

extern CUserExCommand g_UserExCommand;

bool UserExCommandActivateOfflineShop(User *pUser, wstring params);
bool UserExCommandVip(User *pUser, wstring params);
bool UserExCommandVote(User *pUser, wstring params);
bool UserExCommandShowVote(User *pUser, wstring params);
bool UserExCommandCheckVote(User *pUser, wstring params);
bool UserExCommandExp(User *pUser, wstring params);
bool UserExCommandAutoLoot(User *pUser, wstring params);
bool UserExCommandMenu(User *pUser, wstring params);
bool UserExCommandRune(User *pUser, wstring params);
bool UserExCommandLifeTimeItemsInfo(User *pUser, wstring params);
bool UserExCommandRestart(User *pUser, wstring params);
bool UserExCommandGoldBar(User *pUser, wstring params);
bool UserExCommandVisualArmor(User *pUser, wstring params);
bool UserExCommandEngage(User *pUser, wstring params);
bool UserExCommandDivorce(User *pUser, wstring params);
bool UserExCommandGoToLove(User *pUser, wstring params);
bool UserExCommandPing(User *pUser, wstring params);
bool UserExCommandOnline(User *pUser, wstring params);
bool UserExCommandJoinTvT(User *pUser, wstring params);
bool UserExCommandRebirth(User *pUser, wstring params);