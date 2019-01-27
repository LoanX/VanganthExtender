#pragma once


class CHtmlCache
{
	BOOL m_Enabled;
	BOOL m_Caching;
	map<wstring, wstring> m_Htmls;
	void LoadINI();
public:
	CHtmlCache();
	~CHtmlCache();
	void Init();
	void LoadHtmls();
	const WCHAR* Get(wstring name);
	const WCHAR* Load(wstring name);
	static const WCHAR* GetHTMLFileHook(LPVOID lpInstance, const WCHAR* name, int lang);
	inline BOOL IsEnabled() { return m_Enabled; };
	static void ToggleCachingHook(LPVOID lpInstance);
};

extern CHtmlCache g_HtmlCache;