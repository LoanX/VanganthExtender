#pragma once

class CHWID
{
	UINT m_id;
	BYTE m_hash[32];
	BOOL m_initialized;
	UINT m_checkSum;

	UINT CRC32(wstring hash);
	UINT CountDig(wstring str);
public:
	void Init();
	CHWID();
	~CHWID();
	inline UINT Get() { return m_id; };
	inline LPBYTE GetHash() { return m_hash; };
	inline UINT GetCheckSum() { return m_checkSum; };
};

extern CHWID g_HWID;