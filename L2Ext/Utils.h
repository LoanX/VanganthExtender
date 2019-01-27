#pragma once
#include "MT.h"
#include "Splash.h"
#include "IPAddress.h"
#include "MacAddress.h"
#include "Parser.h"

class CSLock
{
	CRITICAL_SECTION m_lock;
	UINT m_lockCount;
public:
	CSLock() { InitializeCriticalSection(&m_lock); };
	~CSLock() { DeleteCriticalSection(&m_lock); };
	inline void Enter() { EnterCriticalSection(&m_lock); m_lockCount++; };
	inline void Leave() { m_lockCount--; LeaveCriticalSection(&m_lock); };
	inline UINT GetLockCount() { return m_lockCount; };
};

class CSAutoLock
{
	CSLock& m_Lock;
public:
	CSAutoLock(CSLock& lock) : m_Lock(lock) { m_Lock.Enter();};
	~CSAutoLock() { m_Lock.Leave(); };
};

#define AUTO_LOCK(lock) CSAutoLock autoLock(lock)

template<typename T>
class CAutoPointer
{
	T *m_Ptr;
public:
	CAutoPointer() : m_Ptr( new T() ) {}
	~CAutoPointer() { if(m_Ptr) delete m_Ptr; m_Ptr = NULL; };
	T* operator->() const
	{
		return m_Ptr;
	}
};

inline void CryptXor(LPBYTE lpData, UINT size)
{
	BYTE key = size % 255;
	for(UINT n=0;n<size;n++)
	{
		lpData[n] ^= key;
		if((n % 2) == 0)
		{
			key+=75;
		}else
		{
			key+=34;
		}
	}
};

inline UINT AdlerCheckSum(PBYTE pData, UINT len)
{
	UINT a = 1, b = 0;
	UINT index = 0;
	for(; index < len; ++index)
	{
			a = (a + pData[index]) % 65521;
			b = (b + a) % 65521;
	}

	return (b << 16) | a;
}

inline bool IsZeroMemory(LPBYTE lpMem, UINT size)
{
	for(UINT n=0;n<size;n++)
	{
		if(lpMem[n] != 0)
		{
			return false;
		}
	}
	return true;
}

void ShowMessage(const char* caption, const char* format, ...);

int Assemble(PCHAR buf, int bufLen, const char *format, ...);
const unsigned char* Disassemble(const unsigned char* packet, const char* format, ...);
bool CopyVMT(LPVOID *dst, LPVOID *src, size_t num);

static int AnsiToUnicode(PCHAR AnsiStr, int MaxAnsiLen, PWCHAR UniStr, int MaxUniLen)
{
	typedef int (*_f)(PCHAR, int, PWCHAR, int);
	_f f = (_f) 0x00899BA0;
	return f(AnsiStr, MaxAnsiLen, UniStr, MaxUniLen);
}
static int AnsiToUnicode(const char* AnsiStr, int MaxAnsiLen, PWCHAR UniStr, int MaxUniLen)
{
	typedef int (*_f)(const char*, int, PWCHAR, int);
	_f f = (_f) 0x00899BA0;
	return f(AnsiStr, MaxAnsiLen, UniStr, MaxUniLen);
}
static int UnicodeToAnsi(PWCHAR UniStr, int MaxUniLen, PCHAR AnsiStr, int MaxAnsiLen)
{
	typedef int (*_f)(PWCHAR, int, PCHAR, int);
	_f f = (_f) 0x00899A30;
	return f(UniStr, MaxUniLen, AnsiStr, MaxAnsiLen);
}
static int Pow2(int n)
{
	return n*n;
}
static bool ValidRange(int nNumber1, int nNumber2, int nRange)
{
	if( Pow2((nNumber1-nNumber2)) > Pow2(nRange)  )
		return false;
	return true;
}
/**********************************
* Converts hex in string to dword *
***********************************/
static DWORD AnsiHexToDword(const char* str)
{
	stringstream sHex; 
	DWORD value = 0;
	sHex << str;
	sHex >> hex >> value;
	return value;
}
/**********************************
* Converts hex in wstring to dword *
***********************************/
static DWORD UnicodeHexToDword(const wchar_t* wstr)
{
	wstringstream sHex; 
	DWORD value = 0;
	sHex << wstr;
	sHex >> hex >> value;
	return value;
}

static void ReadMemoryBYTES(LPVOID Address, VOID *bytes, INT32 len)
{
	SIZE_T BR;
	HANDLE Client = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_READ | PROCESS_VM_WRITE, false, GetCurrentProcessId());
	if ( Client )
		ReadProcessMemory(Client,(LPVOID)Address,bytes,len,&BR);
	CloseHandle(Client);
}

static void ReadMemoryQWORD(LPVOID Address, INT64 *Value) {	ReadMemoryBYTES(Address, Value, 8); }
static void ReadMemoryDWORD(LPVOID Address, INT32 *Value) {	ReadMemoryBYTES(Address, Value, 4); }
static void ReadMemoryWORD(LPVOID Address, INT16 *Value)  {	ReadMemoryBYTES(Address, Value, 2); }
static void ReadMemoryBYTE(LPVOID Address, INT8 *Value)   {	ReadMemoryBYTES(Address, Value, 1); }
double GetPrivateProfileDouble(LPCTSTR lpAppName, LPCTSTR lpKeyName, double Default, LPCTSTR lpFileName);

string SendPostRequest(string sHost, string sPath, string params);

static void GetMacAddress(vector<string> &vMacAddresses)
{
    vMacAddresses.clear();
    IP_ADAPTER_INFO AdapterInfo[32];       // Allocate information for up to 32 NICs
    DWORD dwBufLen = sizeof(AdapterInfo);  // Save memory size of buffer
    DWORD dwStatus = GetAdaptersInfo(      // Call GetAdapterInfo
    AdapterInfo,                 // [out] buffer to receive data
    &dwBufLen);                  // [in] size of receive data buffer

    //No network card? Other error?
    if(dwStatus != ERROR_SUCCESS)
        return;

    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
    char szBuffer[512];
    while(pAdapterInfo)
    {
        if(pAdapterInfo->Type == MIB_IF_TYPE_ETHERNET)
        {
                sprintf_s(szBuffer, sizeof(szBuffer), "%.2x-%.2x-%.2x-%.2x-%.2x-%.2x"
                        , pAdapterInfo->Address[0]
                        , pAdapterInfo->Address[1]
                        , pAdapterInfo->Address[2]
                        , pAdapterInfo->Address[3]
                        , pAdapterInfo->Address[4]
                        , pAdapterInfo->Address[5]
                        );
                vMacAddresses.push_back(szBuffer);
        }
        pAdapterInfo = pAdapterInfo->Next;

    }
};


 __forceinline string Crypt(string Str)
{
	string Crypted;
	if(Str.size() > 0)
	{
		char pr = (char)Str.size();
		for(int n=0;n<Str.size();n++)
		{
			char r = rand() % 255;
			char b = Str[n] ^ r;
			char c = rand() % 255;
			r^= pr;
			pr = r^pr;
			c ^= pr;
			Crypted += r;
			Crypted += b;
			if((n % 2) == 0)
				Crypted += c;

		}
	}
	return Crypted;
}

 __forceinline string Decrypt(string Str)
{
	string Decrypted;
	if(Str.size() > 0)
	{
		char pr = (char)(Str.size() / 2.5);
		for(int n=0;n<Str.size();n+=2)
		{
			char r = Str[n] ^ pr;
			char b = Str[n+1] ^ r;
			Decrypted += b;
			pr = r;
			if( (n % 5) == 0)
			{
				n++;
			}
		}
	}
	return Decrypted;
}

__forceinline void GetTimeInfo(tm& timeinfo, time_t baseTime = 0)
{
	time_t rawtime;
	if(baseTime == 0)
	{
		time ( &rawtime );
	}else
	{
		rawtime = baseTime;
	}
	localtime_s(&timeinfo, &rawtime );
}

static bool CopyToClipboard(string sData)
{
	BOOL bRes = OpenClipboard(NULL);
	if ( !bRes )
		return false;
	HGLOBAL clipBuffer;
	char * buffer;

	EmptyClipboard();
	clipBuffer = GlobalAlloc(GMEM_DDESHARE, (sData.size() + 1));
	if ( !clipBuffer )
	{
		CloseClipboard();
		return false;
	}
	buffer = (char*)GlobalLock(clipBuffer);
	if ( !buffer )
	{
		GlobalFree( clipBuffer );
		CloseClipboard();
		return false;
	}
	strcpy_s(buffer, (sData.size() + 1), sData.c_str());
	GlobalUnlock(clipBuffer);
	SetClipboardData(CF_TEXT,clipBuffer);
	CloseClipboard();
	return true;
}

static void WriteFileA(string sFile, string sData)
{
	HANDLE hFile; 
	DWORD dwBytesToWrite = (DWORD)sData.size();
    DWORD dwBytesWritten = 0;
    BOOL bErrorFlag = FALSE;

	hFile = CreateFileA(sFile.c_str(),                // name of the write
                       GENERIC_WRITE,          // open for writing
                       0,                      // do not share
                       NULL,                   // default security
                       CREATE_ALWAYS,             // create new file only
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);                  // no attr. template

    if (hFile == INVALID_HANDLE_VALUE) 
    { 
        ShowMessage("Error", "Unable to create file %s! GetLastError(%d)", sFile.c_str(), GetLastError());
        return;
    }

    bErrorFlag = WriteFile( 
                    hFile,           // open file handle
					sData.c_str(),      // start of data to write
                    dwBytesToWrite,  // number of bytes to write
                    &dwBytesWritten, // number of bytes that were written
                    NULL);            // no overlapped structure

    if (FALSE == bErrorFlag)
    {
        ShowMessage("Error", "Unable to write to file! GetLastError(%d)", GetLastError());
    }
    else
    {
        if (dwBytesWritten != dwBytesToWrite)
        {
            ShowMessage("Error", "Unable to write all data to file!");
        }
    }

    CloseHandle(hFile);
}

static string ReadFileA(string sName)
{
	INT64 FileSize = 0;
	_WIN32_FIND_DATAA FileData;
	PCHAR sStr = NULL;
	string str;
	HANDLE hFile = FindFirstFileA(sName.c_str(), &FileData);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		FileSize = MAXDWORD;
		FileSize += 1;
		FileSize *= FileData.nFileSizeHigh;
		FileSize += FileData.nFileSizeLow;
	}
	FindClose(hFile);
	hFile = CreateFileA(sName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		char *pBuffer = new char[FileSize];
		memset(pBuffer, 0, FileSize);
		DWORD Readed = NULL;
		if(ReadFile(hFile, pBuffer, FileSize, &Readed, NULL))
		{
			WaitForSingleObject(hFile, INFINITE);
			sStr = (PCHAR)(pBuffer);
			str.insert(0, pBuffer, Readed);
		}		
		delete [] pBuffer;
	}
	CloseHandle(hFile);
	return str;
}

static wstring ReadFileW(wstring wName)
{
	UINT64 FileSize = 0;
	_WIN32_FIND_DATAW FileData;
	PWCHAR wStr = NULL;
	wstring str;
	HANDLE hFile = FindFirstFileW(wName.c_str(), &FileData);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		FileSize = MAXDWORD;
		FileSize += 1;
		FileSize *= FileData.nFileSizeHigh;
		FileSize += FileData.nFileSizeLow;
	}
	FindClose(hFile);
	hFile = CreateFileW(wName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		BYTE *pBuffer = new BYTE[FileSize];
		memset(pBuffer, 0, FileSize);
		DWORD Readed = NULL;
		if(ReadFile(hFile, pBuffer, (DWORD)FileSize, &Readed, NULL))
		{
			WaitForSingleObject(hFile, INFINITE);
			wStr = (PWCHAR)(pBuffer+2);
			if(Readed > 2)
				str.insert(0, (wchar_t*)wStr, ((Readed / 2) -1));
		}		
		delete [] pBuffer;
	}
	CloseHandle(hFile);
	return str;
}

class CBytePointer
{
public:
	BYTE bVar;
};
static HRESULT _AnsiToUnicode(LPCSTR pszA, LPWSTR ppszW)
{

    ULONG cCharacters;
    DWORD dwError;

    // If input is null then just return the same.
    if (NULL == pszA)
    {
        *ppszW = NULL;
        return NOERROR;
    }

    // Determine number of wide characters to be allocated for the
    // Unicode string.
    cCharacters =  (ULONG)strlen(pszA)+1;

    if (NULL == ppszW)
        return E_OUTOFMEMORY;

    // Covert to Unicode.
    if (0 == MultiByteToWideChar(CP_ACP, 0, pszA, cCharacters,
                  ppszW, cCharacters))
    {
        dwError = GetLastError();
        return HRESULT_FROM_WIN32(dwError);
    }

    return NOERROR;
}
/*
 * UnicodeToAnsi converts the Unicode string pszW to an ANSI string
 * and returns the ANSI string through ppszA. Space for the
 * the converted string is allocated by UnicodeToAnsi.
 */ 

static HRESULT _UnicodeToAnsi(LPCWSTR pszW, LPSTR ppszA)
{

    ULONG cbAnsi, cCharacters;
    DWORD dwError;

    // If input is null then just return the same.
    if (pszW == NULL)
    {
        *ppszA = NULL;
        return NOERROR;
    }

    cCharacters = (ULONG)wcslen(pszW)+1;
    // Determine number of bytes to be allocated for ANSI string. An
    // ANSI string can have at most 2 bytes per character (for Double
    // Byte Character Strings.)
    cbAnsi = cCharacters*2;

    if (NULL == ppszA)
        return E_OUTOFMEMORY;

    // Convert to ANSI.
    if (0 == WideCharToMultiByte(CP_ACP, 0, pszW, cCharacters, ppszA,
                  cbAnsi, NULL, NULL))
    {
        dwError = GetLastError();
        return HRESULT_FROM_WIN32(dwError);
    }
    return NOERROR;

}

static double HeadingToDegree(double Heading)
{
	return (double)(Heading/65536 * 360);
}

static double DegreeToRadian(double Degree)
{
	return (double)(Degree * (3.14159265 / 180));
}

std::wstring StringToWString(const std::string& s);
std::string WStringToString(const std::wstring& s);
string HexToString(int len, LPBYTE lpArr);
std::string base64_encode(unsigned char const* , unsigned int len);
std::string base64_decode(std::string const& s);
void MovLogFiles(const char* sLogDir);
inline void UnpackString(PBYTE coded, PCHAR out, int size)
{
	int m=0;
	for(int n=0;n<size;n++)
	{
		if((n%2)==0)
		{
			out[m] = coded[n];
			m++;
		}
	}
}

time_t ConvertTime(int year, int month, int day = 0, int hour = 0, int minute = 0, int second = 0);
bool AppendToFileW(wstring wFile, wstring wData);
wstring CreateSpawnEntry(wstring territoryName, INT32 range, IVector vPos, UINT heading, wstring npcName, UINT count);

DWORD ReadFileBinary(LPCSTR lpFileName, LPBYTE lpOut, DWORD maxLen);
DWORD ReadFileBinary(LPCWSTR lpFileName, LPBYTE& lpOut);
void Crypt(LPBYTE lpIn, LPBYTE lpOut, int len);

UINT GetServerUpTime();

namespace Utils
{
	void ShowMessage(LPTSTR lpCaption, LPTSTR lpFormat, ...);
	LPVOID WriteCall(UINT64 address, LPVOID function, UINT nops = 0, LPBYTE lpOrgMemory = 0, BOOL extended = FALSE);
	LPVOID WriteJump(UINT64 address, LPVOID function, UINT nops = 0, LPBYTE lpOrgMemory = 0, BOOL extended = FALSE);
	void WriteDWORD(UINT64 address, DWORD value);
	void BroadcastToAllUser_Announce(const WCHAR* wAnnounce);
	int VAssemble(char* buff, int len, const char* format, va_list va);
	const unsigned char* Disassemble(const unsigned char* packet, const char* format, ...);
	wstring Trim(wstring str);
	int GetBypassValueInt(wstring wBypass, wstring wSub, int defaultValue);
	tstring ReplaceString(tstring baseStr, tstring oldStr, tstring newStr, bool all = false);
	wstring ReplaceString(wstring wStr, wstring wOld, UINT nNew, bool all = false);
	wstring ReplaceStringW(wstring wStr, wstring wOld, wstring wNew, bool all = false);
	string ReplaceStringA(string wStr, string wOld, string wNew, bool all = false);
};