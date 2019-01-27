#include "stdafx.h"
#include "CallStack.h"

CCallStack g_CallStack;

UINT g_CallStackIndex[32];
PCWSTR g_CallStackName[32][1000];
extern wstring g_ExtReleaseVersion;

extern BYTE g_LastUsedPacket[26];
extern BYTE g_LastUsedPacketEx[26];

UINT GetThreadIndex()
{
	typedef UINT(*f)();
	return f(0x7F01D0L)();
}

UINT GetThreadCount()
{
	typedef UINT(*f)();
	return f(0x7F0170L)();
}

CCallStack::CCallStack() : m_enabled(FALSE)
{
	memset(g_CallStackIndex, 0, sizeof(g_CallStackIndex));
	memset(g_CallStackName, 0, sizeof(g_CallStackName));
}

CCallStack::~CCallStack()
{
}

void CCallStack::Init()
{
	m_enabled = TRUE;
	if( GetFileAttributes(_T("ExtError.txt")) != INVALID_FILE_ATTRIBUTES)
	{
			tm ti;
			time_t rawtime;
			time ( &rawtime );
			localtime_s(&ti, &rawtime );

			tstringstream newName;
			newName << "ExtError.txt." << (ti.tm_year+1900) << "-" << (ti.tm_mon+1) << "-" << ti.tm_mday << "-" << ti.tm_hour << "-" << ti.tm_min << ".bak";
			MoveFile(TEXT("ExtError.txt"), newName.str().c_str());
	}
	g_HookManager.WriteCall(0x5B8F61, CCallStack::GenerateLinError, 0);
	g_HookManager.WriteCall(0x5B94B8, CCallStack::GenerateLinError, 0);
}

void CCallStack::GenerateLinError(LPVOID lpInstance)
{
	const WCHAR* wExtraLog = 0;
	try
	{
		typedef void(*f)(LPVOID);
		f(0x5B83B0L)(lpInstance);
	}catch(...)
	{
		wExtraLog = L"Exception when making linerror.txt!";
	}

	g_CallStack.GenerateLog(L"ExtError.txt", wExtraLog);
}

void CCallStack::GenerateLog(const WCHAR* wFileName, const WCHAR* wExtraLog)
{
	if(m_enabled)
	{
		HANDLE hFile = CreateFile(wFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile)
		{
			tm timeinfo;
			time_t rawtime;
			time ( &rawtime );
			localtime_s(&timeinfo, &rawtime );

			wstringstream dataStream;

			dataStream << L"Date: " << (timeinfo.tm_year+1900) << L"/" << (timeinfo.tm_mon+1) << L"/" << timeinfo.tm_mday << L" " << timeinfo.tm_hour << L":" << timeinfo.tm_min << L":" << timeinfo.tm_sec << L" " << endl;
			dataStream << L"Release: " << g_ExtReleaseVersion.c_str() << endl;
			dataStream << L"Crash thread: " << GetThreadIndex() << L" - IO: " << GetThreadCount() << endl;

			UINT threadCount = GetThreadCount();

			for(UINT n=0;n<16;n++)
			{
				dataStream << L"Thread[" << dec << n << L"] : Packet[0x" << hex << (UINT)g_LastUsedPacket[n] << L"] Ex[0x" << hex << (UINT)g_LastUsedPacketEx[n] << L"] : ";
				UINT funcCount = g_CallStackIndex[n];
				if(funcCount < 1000)
				{
					for(UINT m=0;m<funcCount;m++)
					{
						try
						{
							dataStream << L" -> " << g_CallStackName[n][m];
						}catch(...)
						{
							dataStream << L" -> " << L"Exception while getting name!";
						}
					}
					dataStream << endl;
				}else
				{
					dataStream << L" Invalid function count! [" << dec << funcCount << "]" << endl;
				}
			}

			if(wExtraLog)
			{
				dataStream << L"ExtraLog: " << endl << wExtraLog << endl;
			}

			dataStream << endl << L"Log end. " << endl;
			
			
			DWORD written = 0;

			wstring output(dataStream.str());

			WriteFile(hFile, output.c_str(), (DWORD)(output.size() * 2), &written, NULL);

			CloseHandle(hFile);
		}else
		{
			MessageBox(0, _T("Cannot create ExtError.txt!"), _T("CallStack"), MB_OK);
		}
	}
}