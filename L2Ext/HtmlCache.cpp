#include "stdafx.h"
#include "HtmlCache.h"
#include "HTMLCacheManager.h"

CHtmlCache g_HtmlCache;

CHtmlCache::CHtmlCache() : m_Enabled(FALSE), m_Caching(TRUE)
{
};

CHtmlCache::~CHtmlCache()
{

}

void CHtmlCache::Init()
{
	guard;
	m_Enabled = FALSE;
	LoadINI();
	if(m_Enabled)
	{
		g_HookManager.WriteCall(0x4379AF, CHtmlCache::GetHTMLFileHook);
		g_HookManager.WriteCall(0x464D02, CHtmlCache::GetHTMLFileHook);
		g_HookManager.WriteCall(0x46BA8B, CHtmlCache::GetHTMLFileHook);
		g_HookManager.WriteCall(0x4FB040, CHtmlCache::GetHTMLFileHook);
		g_HookManager.WriteCall(0x550E20, CHtmlCache::GetHTMLFileHook);
		g_HookManager.WriteCall(0x551AB7, CHtmlCache::GetHTMLFileHook);
		g_HookManager.WriteCall(0x5B9D48, CHtmlCache::GetHTMLFileHook);
		g_HookManager.WriteCall(0x60EA25, CHtmlCache::GetHTMLFileHook);
		g_HookManager.WriteCall(0x6DD60E, CHtmlCache::GetHTMLFileHook);
		g_HookManager.WriteCall(0x6E5884, CHtmlCache::GetHTMLFileHook);
		g_HookManager.WriteCall(0x71604B, CHtmlCache::GetHTMLFileHook);
		g_HookManager.WriteCall(0x797227, CHtmlCache::GetHTMLFileHook);
		g_HookManager.WriteCall(0x871889, CHtmlCache::GetHTMLFileHook);
		g_HookManager.WriteCall(0x894649, CHtmlCache::GetHTMLFileHook);

		g_HookManager.WriteCall(0x63F4F8, CHtmlCache::ToggleCachingHook);

		g_HookManager.WriteMemoryBYTE(0x5FC130, 0xC3);
		g_HookManager.WriteMemoryBYTE(0x5FC131, 0x90);
		g_HookManager.WriteMemoryBYTE(0x5FC132, 0x90);
		g_Log.Add(CLog::Blue, "[%s] Feature is enabled.", __FUNCTION__);
	}
	unguard;
}

void CHtmlCache::LoadINI()
{
	guard;
	const TCHAR* section = _T("HtmlCache");
	m_Enabled = GetPrivateProfileInt(section, _T("Enabled"), FALSE, g_ConfigFile);

	unguard;
}

void CHtmlCache::LoadHtmls()
{
	guard;
	if(m_Enabled)
	{
		TCHAR path[260];
		GetCurrentDirectory(MAX_PATH, path);
		lstrcat(path, TEXT("\\..\\html\\*.htm"));

		g_Log.Add(CLog::Blue, "[%s] Initializing path[%S].", __FUNCTION__, path);

		WIN32_FIND_DATA findFileData;
		HANDLE hFind = FindFirstFile(path, &findFileData);
		if(hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if((findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == false)
				{
					wstringstream filePath;
					filePath << L"..\\html\\" << findFileData.cFileName;
					wstring file = ReadFileW(filePath.str());
#ifdef EXTEND_IOBUFFER
					if(file.size() < 8100)
#else
					if(file.size() < 4030)
#endif
					{
						wstring name(findFileData.cFileName);
						transform(name.begin(), name.end(), name.begin(), towlower);
						m_Htmls.insert(pair<wstring, wstring>(name, file));
					}else
					{
						g_Log.Add(CLog::Error, "[%s] Too big html[%s] [%d]", __FUNCTION__, findFileData.cFileName, file.size()*2);
					}
				}

			}while( FindNextFile(hFind, &findFileData) != FALSE );
			g_Log.Add(CLog::Blue, "[%s] Loaded [%d] html(s).", __FUNCTION__, m_Htmls.size());
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Cannot find first file - path[%S] !", __FUNCTION__, path);
		}
		FindClose(hFind);
		m_Caching = FALSE;
		g_HtmlCache.ToggleCachingHook((LPVOID)0x6F8BF40L);
	}
	unguard;
}

const WCHAR* CHtmlCache::Load(wstring name)
{
	guard;
	const WCHAR* wHtml = 0;
	wstring path = L"..\\html\\";
	path += name;
	if(path.find(L".htm") == (path.size() - 4))
	{
		wstring file = ReadFileW(path);
#ifdef EXTEND_IOBUFFER
		if(file.size() < 8000)
#else
		if(file.size() < 4000)
#endif
		{
			transform(name.begin(), name.end(), name.begin(), towlower);
			m_Htmls[name] = file;
			wHtml = m_Htmls[name].c_str();
			g_Log.Add(CLog::Black, "[%s] path[%S]", __FUNCTION__, path.c_str());
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Too big html[%s] [%d]", __FUNCTION__, name.c_str(), file.size()*2);
		}
	}
	unguard;
	return wHtml;
}

const WCHAR* CHtmlCache::Get(wstring name)
{
	guard;
	const WCHAR* wHtml = 0;
	if(m_Enabled)
	{
		if(m_Caching)
		{
			transform(name.begin(), name.end(), name.begin(), towlower);
			map<wstring, wstring>::iterator it = m_Htmls.find(name);
			if(it!=m_Htmls.end())
			{
				wHtml = it->second.c_str();
			}else
			{
				g_Log.Add(CLog::Error, "[%s] Cannot find html[%S]!", __FUNCTION__, name.c_str());
				wHtml = L"<html><body>NO HTML</body></html>";
			}
		}else
		{
			wHtml = Load(name);
		}
	}else
	{
		wHtml = g_HTMLCacheManager.GetHTMLFile(name.c_str(), 0);
	}

	unguard;
	return wHtml;
}

const WCHAR* CHtmlCache::GetHTMLFileHook(LPVOID lpInstance, const WCHAR *name, int lang)
{
	guard;
	const WCHAR* wHtml = L"<html><body>NO HTML</body></html>";
	if(g_HtmlCache.m_Caching)
	{
		try
		{
			if(name)
			{
				wHtml = g_HtmlCache.Get(name);
			}
		}catch(...)
		{
			EXCEPTION_LOG;
		}
	}else
	{
		if(name)
		{
			wHtml = g_HtmlCache.Load(name);
		}
	}

	unguard;
	return wHtml;
}

void CHtmlCache::ToggleCachingHook(LPVOID lpInstance)
{
	guard;

	if(g_HtmlCache.m_Caching)
	{
		g_HtmlCache.m_Caching = FALSE;
	}else
	{
		g_HtmlCache.m_Caching = TRUE;
	}

	typedef void(*f)(LPVOID);
	f(0x5F8040L)(lpInstance);

	unguard;
}