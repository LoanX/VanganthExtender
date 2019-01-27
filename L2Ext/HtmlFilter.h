#pragma once

struct BypassParam
{
	wstring paramName;
	wstring paramValue;
};

enum BypassType
{
	BypassTypeNone = 0,
	BypassTypeBypass = 1,
	BypassTypeLink = 2
};

struct HtmlBypass
{
	BypassType type;
	wstring name;
	wstring linkHtml;
	vector<BypassParam> params;
	HtmlBypass() : type(BypassTypeNone) {}
};

class CHtmlFilter
{
	BOOL m_Enabled;
	UINT GetClassId(const WCHAR* wBypass);
	void LoadINI();
public:
	CHtmlFilter();
	~CHtmlFilter();
	void Init();
	void OnSendHtml(User* pUser, const WCHAR* wFileName, const WCHAR* wHtml, UINT itemClassId);
	bool IsValid(User* pUser, const WCHAR* wBypass, bool link = false, bool tutorialHtml = false);
	void ParseAction(wstring html, std::vector<HtmlBypass> &bypasses);
	void ParseBypass(const WCHAR *wBypass, HtmlBypass& hb, bool link = false);
	void Split(vector<wstring>& data, wstring bypass);
	inline BOOL IsEnabled() { return m_Enabled; };
};

extern CHtmlFilter g_HtmlFilter;