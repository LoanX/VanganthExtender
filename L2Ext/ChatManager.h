#pragma once

struct ChatSetting
{
	BOOL enabled;
	DWORD delay;
	INT32 minLevel;
	vector<pair<WORD, WORD>> charset;
	vector<pair<wstring, wstring>> wordToReplace;
	wstring invalidCharMsg;
};

class CChatManager
{
	BOOL m_Enabled;
	BOOL m_SendMessage;
	BOOL m_GlobalChat;
	INT32 m_MessageId;
	INT32 m_MinLevel;
	INT32 m_MaxLevel;
	BOOL m_AllClassAllowed;
	vector<UINT> m_AllowedClass;
	UINT m_MinUseTime;
	UINT m_MinUseTimeHWID;
	vector<wstring> m_ProhibitedPhrase;
	vector<wstring> m_ProhibitedPhraseMask;
	ChatSetting m_ChatSetting[18];
	vector<pair<WORD, WORD>> m_PrivateStoreCharset;
	UINT m_ReloadTimestamp;
	BOOL m_Reloading;
	CChatManager* m_BackupInstance;

	void LoadINI();
	bool CanChat(User *pUser);
	bool IsClassAllowed(UINT classId);
public:
	CChatManager();
	~CChatManager();
	CChatManager* GetInstance() { if(m_Reloading) { return m_BackupInstance; } return this; };
	void Init(bool backup = false);
	inline BOOL IsEnabled() { return m_Enabled; };
	bool ContainProhibitedPhrase(wstring str);
	bool ValidPrivateStoreMsg(wstring str);
	bool Handle(CUserSocket *pSocket, const unsigned char* packet, int type);
	inline const WCHAR* GetName() { return L"Chat Manager"; };
	inline void SetReloadTimestamp(UINT value) { m_ReloadTimestamp = value; m_Enabled = FALSE; };
	void TimerExpired();
};

extern CChatManager g_ChatManager;
