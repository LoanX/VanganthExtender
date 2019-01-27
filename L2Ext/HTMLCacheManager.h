#pragma once

class CHTMLCacheManager
{
	CHTMLCacheManager* instance;
public:
	CHTMLCacheManager();
	const WCHAR* GetHTMLFile(const WCHAR* wFileName, int nLang);
	int IsValid(const WCHAR* wFileName, const WCHAR* wHtmlData, UINT language = 0);
};

extern CHTMLCacheManager g_HTMLCacheManager;