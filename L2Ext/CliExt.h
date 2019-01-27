#pragma once

typedef bool (*CPHandler)(CUserSocket*, const unsigned char*);

struct BannedHardwareId
{
	BYTE hash[32];
	wstring accountName;
	wstring GMName;
	UINT timeout;
	BannedHardwareId() : timeout(0) { memset(hash, 0, sizeof(hash)); };
};


class CCliExt
{
	BOOL m_Enabled;
	BOOL m_DisablePingPacket;
	UINT m_Version;
	INT32 m_BoxLimit;
	WCHAR m_BoxLimitHTML[0x4000];
	WCHAR m_BannedHWIDHTML[0x4000];
	WCHAR m_LockedCharacterHTML[0x4000];
	WCHAR m_LockedAccountHTML[0x4000];
	CPHandler m_CustomHandler[256];
	vector<HWID> m_AllowedHWID;
	WCHAR m_AllowedHWIDHtml[0x4000];
	UINT m_AutoBanHWID;
	BYTE m_MD5Checksum[32];
	WCHAR m_OutdatedClientHTML[0x4000];

	CSLock m_BannedLock;
	vector<BannedHardwareId> m_Banned;
	void LoadINI();
public:
	void Init();
	void Bind(UINT id, CPHandler func);
	void ReloadAllowedHWID();
	inline WCHAR* GetBannedHtml() { return m_BannedHWIDHTML; };
	inline WCHAR* GetBoxLimitHtml() { return m_BoxLimitHTML; };
	inline WCHAR* GetLockedCharacterHtml() { return m_LockedCharacterHTML; };
	inline WCHAR* GetLockedAccountHtml() { return m_LockedAccountHTML; };
	inline INT32 GetBoxLimit() { return m_BoxLimit; };
	inline void SetBoxLimit(int value) { m_BoxLimit = value; };
	void OnBanDBLoad(LPBYTE lpHwid, wstring account, wstring gmName, UINT timeout);
	bool IsBanned(LPBYTE lpHwid);
	bool IsAllowed(LPBYTE lpHwid);
	bool UnBan(LPBYTE lpHWID, wstring gmName);
	void Ban(LPBYTE lpHWID, wstring account, wstring gmName, UINT timeout);
	void HandleCheckSum(CUserSocket *pSocket, const unsigned char* packet, UINT packetSize);
	void DecryptPacket(CUserSocket *pSocket, unsigned char* packet, UINT packetSize);
	bool HandleCustomPacket(CUserSocket *pSocket, const unsigned char* packet);
	void TimerExpired(CUserSocket *pSocket);
	void GetAllowedHWIDHtml(HWID& hwid, WCHAR* wHtml);
	inline UINT IsAutoBanHWID() { return m_AutoBanHWID; };
	inline BOOL IsEnabled() { return m_Enabled; };
	inline UINT GetVersion() { return m_Version; };
	inline const LPBYTE GetMD5CHecksum() { return m_MD5Checksum; };
	inline const WCHAR* GetOutdatedClientHtml() { return m_OutdatedClientHTML; };
};

bool CPHDummyPacket(CUserSocket *pSocket, const unsigned char* pacekt);
bool CPHPingExPacket(CUserSocket *pSocket, const unsigned char* packet);
bool CPHHWIDPacket(CUserSocket *pSocket, const unsigned char* packet);
bool CPHPingPacket(CUserSocket *pSocket, const unsigned char* packet);
bool CPHReplyL2NetworkVoteTime(CUserSocket *pSocket, const unsigned char* packet);
bool CPHReplyJobPacket(CUserSocket *pSocket, const unsigned char* packet);
bool CPHReplyMD5Packet(CUserSocket *pSocket, const unsigned char* packet);

extern CCliExt g_CliExt;