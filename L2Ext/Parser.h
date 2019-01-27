#pragma once

void WritePrivateProfileInt(LPTSTR lpAppName, LPTSTR lpKeyName, int Value, LPTSTR lpFileName);

class CParser
{
public:
	static wstring Replace(wstring str, wchar_t sign, wchar_t new_sign);
	static string Replace(string str, char sign, char new_sign);
	static wstring Trim(wstring str);
	static string Trim(string str);
	static wstring GetBypassValue(wstring wBypass, wstring wSub);
	static int GetBypassValueInt(wstring wBypass, wstring wSub, int defaultValue = -1);
	static string GetColumn(string sSub, int nIndex);
	static string GetValueString(string sSub, string sName);
	static wstring GetName(wstring wStr);
	static wstring GetValueString(wstring sSub, wstring sName);
	static int GetValueInt(string sSub, string sName, int defaultValue = 0);
	static int GetValueInt(wstring wSub, wstring wName);
	static double GetValueDouble(string sSub, string sName);
	static void ConvToLower(PWCHAR wsTxt, int nMaxSize);
};

namespace Parser
{
	INT32 GetBypassValueInt(const WCHAR* wBypass, const WCHAR* wName, int defaultValue = 0);
	INT32 ParseOptionInt(string line, string option, INT32 defaultValue = 0);
	INT32 ParseOptionInt(wstring line, wstring option, INT32 defaultValue = 0);
	double ParseOptionDouble(wstring line, wstring option, double defaultValue = 0);
	double ParseOptionDouble(string line, string option, double defaultValue = 0);
	string ParseOptionString(string line, string option, string sDefault = "");
	wstring ParseOptionString(wstring line, wstring option, wstring sDefault = L"");
	wstring ParseOptionMessage(wstring line, wstring option, wstring sDefault = L"");
	wstring Replace(wstring str, wchar_t sign, wchar_t new_sign);
	string Replace(string str, char sign, char new_sign);
	vector<wstring> Split(wstring line, wstring separators);
	vector<string> Split(string line, string separators);
	void SplitEx(wstring line, wstring separators, vector<wstring>& words, wstring magicChar, bool toLower);
	bool IsValidSqlString(const WCHAR* wInput);
	WCHAR ToWLower(WCHAR c);
	wstring Trim(wstring str);
};