#include "StdAfx.h"
#include "HTMLCacheManager.h"

CHTMLCacheManager g_HTMLCacheManager;

CHTMLCacheManager::CHTMLCacheManager()
{
	instance = (CHTMLCacheManager*)0x6F8BF40;
}

const WCHAR* CHTMLCacheManager::GetHTMLFile(const WCHAR* wFileName, int nLang)
{
	typedef PWCHAR (*_f)(CHTMLCacheManager*, const WCHAR*, int);
	_f f = (_f) 0x005F7E60;
	return f(instance, wFileName, nLang);
};

int CHTMLCacheManager::IsValid(const WCHAR *wFileName, const WCHAR *wHtmlData, UINT language)
{
	typedef int(*f)(CHTMLCacheManager*, const WCHAR*, const WCHAR*, UINT);
	return f(0x5F99F0L)(instance, wFileName, wHtmlData, language);
}