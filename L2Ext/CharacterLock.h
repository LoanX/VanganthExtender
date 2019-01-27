#pragma once

struct CharacterHWIDBan
{
	HWID hwid;
	UINT charId;
	UINT timeout;
};

class CCharacterLock
{
	BOOL m_Enabled;
	UINT m_AttemptLimit;
	UINT m_BanTimeout;
	UINT m_MinCodeLength;
	UINT m_MaxCodeLength;
	vector<pair<WCHAR, WCHAR>> m_AllowedCharacters;
	BOOL m_Force;
	WCHAR m_BanHTML[0x4000];
	vector<CharacterHWIDBan> m_BannedHWID;
	CSLock m_BannedLock;
	void LoadINI();
public:
	void Init();
	bool IsBanned(UINT charId, HWID& hwid);
	inline const WCHAR* GetBanHTML() { return m_BanHTML; };
	inline BOOL IsEnabled() { return m_Enabled; };
	void OnDBLoad(UINT charId, LPBYTE lpHash);
	bool OnSay(User *pUser, wstring text);
	void BanHWID(UINT charId, HWID& hwid);
};

extern CCharacterLock g_CharacterLock;