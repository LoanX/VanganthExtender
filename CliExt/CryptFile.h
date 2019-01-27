#pragma once

class CCryptFile
{
	wstring m_Name;
	wstring GenerateRandomName();
public:
	void Release();
	wstring Handle(LPCWSTR lpFileName);
	void XorCrypt(LPBYTE lpBuff, UINT len);
	DWORD ReadFileBinary(LPCWSTR lpFileName, LPBYTE& lpOut);
	BOOL WriteFileBinary(LPCWSTR lpFileName, LPBYTE lpBuffer, UINT length);
	const WCHAR* GetName() { return m_Name.c_str(); };
};