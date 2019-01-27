#include "StdAfx.h"

extern UINT g_ServerStartTime;

UINT GetServerUpTime()
{
	return static_cast<UINT>(time(0) - g_ServerStartTime);
}

string WINAPI SendPostRequest(string sHost, string sPath, string params)
{
	VIRTUALIZER_TIGER_RED_START;
	string reply = "";
	try
	{
		hostent *remoteHost = gethostbyname(sHost.c_str());
		if(remoteHost)
		{
			hostent host = *remoteHost;
			in_addr addr;
			stringstream webStream;
			if(remoteHost)
			{
				if(host.h_length)
				{
					addr.s_addr = (*(u_long *)host.h_addr_list[0]);
					SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
					if(s != INVALID_SOCKET)
					{
						SOCKADDR_IN sockAddr;
						memset(&sockAddr,0, sizeof(sockAddr));
						sockAddr.sin_family = AF_INET;
						sockAddr.sin_port = htons(80);
						sockAddr.sin_addr = *((LPIN_ADDR)*host.h_addr_list);

						if(!connect(s,(SOCKADDR *)&sockAddr,sizeof(SOCKADDR_IN)))
						{
							char buffer[8192];
							memset(buffer, 0, sizeof(buffer));
							int len = 0;

							string content = params;
							char temp[16];
							_itoa_s((INT32)content.size(), temp, sizeof(temp), 10);

							char _post[] = { 'P', 'O', 'S', 'T', ' ', 0 };
							char _http[] = { ' ', 'H', 'T', 'T', 'P', '/', '1', '.', '1', '\r', '\n', 0 };
							char _host[] = {'H', 'o', 's', 't', ':', ' ', 0};
							char _rn[] = { '\r', '\n', 0 };
							char _userAgent[] = { 'U', 's', 'e', 'r', '-', 'A', 'g', 'e', 'n', 't', ':', ' ',
								'M', 'o', 'z', 'i', 'l', 'l', 'a', '/', '5', '.', '0', ' ',
								'(', 'W', 'i', 'n', 'd', 'o', 'w', 's', ' ', 'N', 'T', ' ', '6', '.', '1', ';',
								' ', 'W', 'O', 'W', '6', '4', ';', ' ', 'r', 'v', ':', '2', '3', '.', '0', ')',
								' ', 'G', 'e', 'c', 'k', 'o', '/', '2', '0', '1', '0', '0', '1', '0', '1', ' ',
								'F', 'i', 'r', 'e', 'f', 'o', 'x', '/', '2', '3', '.', '0', 0 };
							char _acceptLanguage[] = { 'A', 'c', 'c', 'e', 'p', 't', '-', 'L', 'a', 'n', 'g', 'u', 'a', 'g', 'e', ':', ' ', 'p', 'l', '-', 'P', 'L', ',', 'p', 'l', ';', 'q', '=', '0', '.', '8', ',', 'e', 'n', '-', 'U', 'S', ';', 'q', '=', '0', '.', '6', ',', 'e', 'n', ';', 'q', '=', '0', '.', '4', 0 };
							char _acceptCharset[] = { 'A', 'c', 'c', 'e', 'p', 't', '-', 'C', 'h', 'a', 'r', 's', 'e', 't', ':', ' ', 'I', 'S', 'O', '-', '8', '8', '5', '9', '-', '2', ',', 'u', 't', 'f', '-', '8', ';', 'q', '=', '0', '.', '7', ',', '*', ';', 'q', '=', '0', '.', '7', 0 };
							char _cacheControl[] = { 'C', 'a', 'c', 'h', 'e', '-', 'C', 'o', 'n', 't', 'r', 'o', 'l', ':', ' ', 'm', 'a', 'x', '-', 'a', 'g', 'e', '=', '0', 0 };
							char _contentType[] = { 'C', 'o', 'n', 't', 'e', 'n', 't', '-', 'T', 'y', 'p', 'e', ':', ' ', 'a', 'p', 'p', 'l', 'i', 'c', 'a', 't', 'i', 'o', 'n', '/', 'x', '-', 'w', 'w', 'w', '-', 'f', 'o', 'r', 'm', '-', 'u', 'r', 'l', 'e', 'n', 'c', 'o', 'd', 'e', 'd', 0 };
							char _contentLength[] = { 'C', 'o', 'n', 't', 'e', 'n', 't', '-', 'L', 'e', 'n', 'g', 't', 'h', ':', ' ', 0 };
							char _connectionClose[] = { 'C', 'o', 'n', 'n', 'e', 'c', 't', 'i', 'o', 'n', ':', ' ', 'c', 'l', 'o', 's', 'e', 0 };
							char _transferEncoding[] = { 'T', 'r', 'a', 'n', 's', 'f', 'e', 'r', '-', 'E', 'n', 'c', 'o', 'd', 'i', 'n', 'g', ':', ' ', 'c', 'h', 'u', 'n', 'k', 'e', 'd', 0 };
							
							string request = _post + sPath + _http;
							request += _host + sHost + _rn;
							request += _userAgent + string(_rn);
							request += _acceptLanguage + string(_rn);
							request += _acceptCharset + string(_rn);
							request += _cacheControl + string(_rn);
							request += _contentType + string(_rn);
							request += _contentLength;
							request += temp;
							request += _rn;
							request += _connectionClose + string(_rn) + string(_rn);
							request += content;

							char sendBuffer[8192];
							memset(sendBuffer, 0, sizeof(sendBuffer));
							strcpy_s(sendBuffer, 8192, request.c_str());
							send(s, sendBuffer, static_cast<int>(request.size()+1), 0);
							string header;
							int bytes = 0;
							try
							{
								while((bytes = recv(s, buffer, 8190, 0)) > 0 )
								{
									len += bytes;

									string code(buffer);
									size_t pos = code.find(string(_rn) + string(_rn));
									if(pos != string::npos)
									{
										header = code.substr(0, pos);
										pos += 4;
										if(header.find(_transferEncoding) != string::npos)
										{
											pos = code.find(_rn, pos);
											pos += 2;
										}
										code = code.substr(pos);
									}
									
									webStream << code;

									if(buffer[bytes-1] == '\0')
										break;

									memset(buffer, 0, sizeof(buffer));
								}
							}catch(...)
							{
							//	EXCEPTION_LOG;
							}

							reply = webStream.str();
						}
						closesocket(s);
					}
				}
			}
		}
	}catch(...)
	{
	//	EXCEPTION_LOG;
	}

	VIRTUALIZER_TIGER_RED_END;

	return reply;
};

void ShowMessage(const char* caption, const char* format, ...)
{
	va_list va;
	va_start(va, format);
	char Msg[2048];
	vsprintf_s(Msg, sizeof(Msg), format, va);
	BYTE arrMessageBoxA[24] = { 77, 185, 101, 0, 115, 135, 115, 62, 97, 0, 103, 0, 101, 194, 66, 197, 111, 70, 120, 0, 65, 56, 0, 0 };
	char sMessageBoxA[12];
	UnpackString(arrMessageBoxA, sMessageBoxA, 24);
	typedef int (* _messageBoxA)(    __in_opt HWND hWnd,    __in_opt LPCSTR lpText,    __in_opt LPCSTR lpCaption,    __in UINT uType);
	
	BYTE arrUser32_dll[22] = { 85, 30, 115, 101, 101, 0, 114, 0, 51, 150, 50, 0, 46, 147, 100, 243, 108, 219, 108, 51, 0, 0 };
	char sUser32_dll[11];
	UnpackString(arrUser32_dll, sUser32_dll, 22);
	HMODULE hModule = GetModuleHandleA(sUser32_dll);
	_messageBoxA messageBoxA = (_messageBoxA)GetProcAddress(hModule, sMessageBoxA);
	messageBoxA(NULL, Msg, caption, MB_OK);
	va_end(va);
}

const unsigned char* Disassemble(const unsigned char* packet, const char* format, ...)
{
	typedef PUCHAR (*f)(const unsigned char*, const char*, va_list va);

	va_list va;
	va_start(va, format);
	const unsigned char* ret = f(0x61F1E0)(packet,format, va);
	va_end(va);

	return ret;
};

int Assemble(PCHAR buf, int bufLen, const char *format, ...)
{
	
	typedef int (*f)(PCHAR, int, const char*, va_list);
	
	va_list va;
	va_start(va, format);
	int nRet = f(0x61D8A0L)(buf, bufLen, format, va);
	va_end(va);

	return nRet;
};

bool CopyVMT(LPVOID *dst, LPVOID *src, size_t num)
{
	DWORD oldProtect = NULL;
	size_t size = num * sizeof(LPVOID);

	if (VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect)!=0)
	{
		memcpy_s(dst, size, src, size);
		VirtualProtect(dst, size, oldProtect, &oldProtect);
		return true;
	}
	else
	{
		MessageBoxA(NULL, "CopyVMT() failed!", "(Error) L2Server_addon", MB_OK);
		return false;
	}
}

wstring StringToWString(const string& s)
{
	wstring temp(s.length(),L' ');
	copy(s.begin(), s.end(), temp.begin());
	return temp; 
}


string WStringToString(const wstring& s)
{
	string temp(s.length(), ' ');
	copy(s.begin(), s.end(), temp.begin());
	return temp; 
}

string HexToString(int len, LPBYTE lpArr)
{
	string str;
	char cTmp[8];
	for(int n=0;n<len;n++)
	{
		memset(cTmp, 0, 8);
		sprintf_s(cTmp, 8, "%x", lpArr[n]);
		str += cTmp;
	}
	return str;
}

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}

std::string base64_decode(std::string const& encoded_string) {
  int in_len = (int)encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = (unsigned char)base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = (unsigned char)base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}

void MovLogFiles(const char* sLogDir)
{
	list<string> lLogs;
	CreateDirectoryA(sLogDir, NULL);
	WIN32_FIND_DATAA info;
	HANDLE hFile = 0;
	char szDir[265];
	GetCurrentDirectoryA(260, szDir);
	strcat(szDir, "\\*");
	hFile = FindFirstFileA(szDir, &info);
	if(hFile)
	{
		do
		{
			if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			 
			}
			else
			{
				if(strcmp(info.cFileName, "LinError.txt"))
				{
					if(strstr(info.cFileName, "LinError") || strstr(info.cFileName, ".dmp"))
					{
						lLogs.push_back(info.cFileName);
					}
				}
				if(strcmp(info.cFileName, "ExtError.txt"))
				{
					if(strstr(info.cFileName, "ExtError.txt."))
					{
						lLogs.push_back(info.cFileName);
					}
				}
			}
		}
		while (FindNextFileA(hFile, &info) != 0);
		FindClose(hFile);
	}
	if(lLogs.size() > 0)
	{
		for(list<string>::iterator Iter = lLogs.begin();Iter!=lLogs.end();Iter++)
		{
			char sNewFile[512];
			strcpy(sNewFile, sLogDir);
			strcat(sNewFile, "\\");
			strcat(sNewFile, Iter->c_str());
			MoveFileA(Iter->c_str(), sNewFile);
		}
	}
};

time_t ConvertTime(int year, int month, int day, int hour, int minute, int second)
{
	time_t rawtime = time(0);
	tm *timeinfo = localtime ( &rawtime );
	timeinfo->tm_year = year - 1900;
	timeinfo->tm_mon = month - 1;
	timeinfo->tm_mday = day;
	timeinfo->tm_hour = hour;
	timeinfo->tm_min = minute;
	timeinfo->tm_sec = second;

	return mktime ( timeinfo );
}

bool AppendToFileW(wstring wFile, wstring wData)
{
	HANDLE hAppend; 
	DWORD dwBytesToWrite = (DWORD)(wData.size()*2);
    DWORD dwBytesWritten = 0;
    BOOL bErrorFlag = FALSE;

	hAppend = CreateFileW(wFile.c_str(),
                       FILE_APPEND_DATA,          // open for writing
                       FILE_SHARE_READ,                      // do not share
                       NULL,                   // default security
                       OPEN_ALWAYS,             // create new file only
                       FILE_ATTRIBUTE_NORMAL,  // normal file
                       NULL);                  // no attr. template

    if (hAppend == INVALID_HANDLE_VALUE) 
    { 
		return false;
    }

	DWORD dwPos = SetFilePointer(hAppend, 0, NULL, FILE_END);
    LockFile(hAppend, dwPos, 0, dwBytesToWrite, 0);
	WriteFile(hAppend, wData.c_str(), dwBytesToWrite, &dwBytesWritten, NULL);
    UnlockFile(hAppend, dwPos, 0, dwBytesToWrite, 0);

    CloseHandle(hAppend);
	return true;
}

wstring CreateSpawnEntry(wstring territoryName, INT32 range, IVector vPos, UINT heading, wstring npcName, UINT count)
{
	/*
	territory_begin	[oren16_npc2218_011]	{{85075;17375;-3559;-3359};{85577;17375;-3559;-3359};{85584;17575;-3559;-3359};{85055;17575;-3559;-3359}}	territory_end
	npcmaker_ex_begin	[oren16_npc2218_011]	name=[oren16_npc2218_01101]	ai=[default_maker]	maximum_npc=1
	npc_ex_begin	[guard_yening]	pos={85149;17458;-3511;16384}	total=1	respawn=1min	npc_ex_end
	npcmaker_ex_end
	*/
	range /= 2;

	wstringstream wData;
	wData << L"territory_begin	[" << territoryName << L"]	{{"
		<< vPos.x - range << L";" << vPos.y - range << L";" << vPos.z - range << L";" << vPos.z + range << L"};{"
		<< vPos.x + range << L";" << vPos.y - range << L";" << vPos.z - range << L";" << vPos.z + range << L"};{"
		<< vPos.x + range << L";" << vPos.y + range << L";" << vPos.z - range << L";" << vPos.z + range << L"};{"
		<< vPos.x - range << L";" << vPos.y + range << L";" << vPos.z - range << L";" << vPos.z + range << L"}}	territory_end" << endl;
	wData << L"npcmaker_ex_begin	[" << territoryName << L"]	name=[" << territoryName << L"]	ai=[default_maker]	maximum_npc=" << count << endl;
	wData << L"npc_ex_begin	[" << npcName << L"]	pos={" << vPos.x << L";" << vPos.y << L";" << vPos.z << L";" << heading << L"}	total=" << count << L"	respawn=1min	npc_ex_end" << endl;
	wData << L"npcmaker_ex_end" << endl;
	return wData.str();
}

DWORD ReadFileBinary(LPCWSTR lpFileName, LPBYTE& lpOut)
{
	UINT64 FileSize = 0;
	_WIN32_FIND_DATAW FileData;
	HANDLE hFile = FindFirstFileW(lpFileName, &FileData);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		FileSize = MAXDWORD;
		FileSize += 1;
		FileSize *= FileData.nFileSizeHigh;
		FileSize += FileData.nFileSizeLow;
	}
	FindClose(hFile);
	DWORD readed = 0;
	hFile = CreateFileW(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		lpOut = new BYTE[FileSize+2];
		memset(lpOut, 0, FileSize+2);
		if(ReadFile(hFile, lpOut, static_cast<DWORD>(FileSize), &readed, NULL))
		{
			WaitForSingleObject(hFile, 5000);
		}
	}
	CloseHandle(hFile);
	return readed;
}

DWORD ReadFileBinary(LPCSTR lpFileName, LPBYTE lpOut, DWORD maxLen)
{
	INT64 FileSize = 0;
	_WIN32_FIND_DATAA FileData;
	HANDLE hFile = FindFirstFileA(lpFileName, &FileData);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		FileSize = MAXDWORD;
		FileSize += 1;
		FileSize *= FileData.nFileSizeHigh;
		FileSize += FileData.nFileSizeLow;
	}
	FindClose(hFile);
	if(maxLen > FileSize)
	{
		hFile = CreateFileA(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile != INVALID_HANDLE_VALUE)
		{
			memset(lpOut, 0, maxLen);
			if(ReadFile(hFile, lpOut, static_cast<DWORD>(FileSize), &maxLen, NULL))
			{
				WaitForSingleObject(hFile, 5000);
			}
		}
		CloseHandle(hFile);
		return maxLen;
	}

	return 0;
}

void Crypt(LPBYTE lpIn, LPBYTE lpOut, int len)
{
	BYTE crypt = 0;
	for(int n=0;n<len;n++)
	{
		crypt += n;
		BYTE xorKey = crypt % 223;
		lpOut[n] = lpIn[n] ^ xorKey;
	}
}


void Utils::ShowMessage(LPTSTR lpCaption, LPTSTR lpFormat, ...)
{
	TCHAR buff[8192];
	va_list va;
	va_start(va, lpFormat);
	_vstprintf_s(buff, 8190, lpFormat, va);
	MessageBox(NULL, buff, lpCaption, MB_OK);
	va_end(va);
}

int Utils::VAssemble(char* buff, int len, const char* format, va_list va)
{
	typedef int (*f)(char*, int, const char*, va_list);
	return f(0x61D8A0L)(buff, len, format, va);
}

const unsigned char* Utils::Disassemble(const unsigned char* packet, const char* format, ...)
{
	typedef PUCHAR (*f)(const unsigned char*, const char*, va_list va);

	va_list va;
	va_start(va, format);
	const unsigned char* ret = f(0x61F1E0)(packet,format, va);
	va_end(va);

	return ret;
};

tstring Utils::ReplaceString(tstring baseStr, tstring oldStr, tstring newStr, bool all)
{
	size_t pos = baseStr.find(oldStr);
	while(pos != tstring::npos)
	{
		baseStr = baseStr.replace(pos, oldStr.size(), newStr);
		if(all)
			pos = baseStr.find(oldStr);
		else
			pos = tstring::npos;
	}
	return baseStr;
}

void Utils::BroadcastToAllUser_Announce(const WCHAR* wAnnounce)
{
	typedef void (*f)(const WCHAR*);
	f(0x823660L)(wAnnounce);
}

void Utils::WriteDWORD(UINT64 address, DWORD value)
{
	LPDWORD lpAddr = reinterpret_cast<LPDWORD>(address);
	DWORD old;
	if(VirtualProtect(lpAddr, 4, PAGE_EXECUTE_READWRITE, &old))
	{
		(*lpAddr) = value;
		VirtualProtect(lpAddr, 4, old, &old);
	}
}

LPVOID Utils::WriteCall(UINT64 address, LPVOID function, UINT nops, LPBYTE lpOrgMemory, BOOL extended)
{
	LPVOID lpRet = 0;
	try
	{
		UINT size = 5 + nops;
		if(extended)
		{
			LPBYTE pMemory = static_cast<LPBYTE>(VirtualAlloc(0, (size + 5), MEM_COMMIT, PAGE_EXECUTE_READWRITE));
			if(pMemory)
			{
				HookMem hm;
				LPBYTE lpAddress = reinterpret_cast<LPBYTE>(address);
				for(UINT n=0;n<size;n++)
				{
					pMemory[n] = lpAddress[n];
					if( n < 5 )
					{
						hm.byte.p[n] = lpAddress[n];
					}
				}

				if(hm.full.opCode == 0xE8 || hm.full.opCode == 0xE9)
				{
					//reallocate addr
					UINT64 addr = hm.full.addr;
					addr += (address + 5);
					
					hm.full.addr = static_cast<UINT>(addr - (reinterpret_cast<UINT64>(pMemory) + 5));

					for(int n=0;n<5;n++)
					{
						pMemory[n] = hm.byte.p[n];
					}
				}
				
				WriteJump(reinterpret_cast<UINT64>(&pMemory[5]), reinterpret_cast<LPVOID>(address + size));
				WriteJump(address, function);
				lpRet = pMemory;
			}else
			{
				ShowMessage(_T("VoteMod"), _T("Cannot allocate memory! (%p)"), address);
			}
		}else
		{
			LPBYTE lpAddress = reinterpret_cast<LPBYTE>(address);
			HookMem mem;
			mem.full.opCode = 0xE8;	//CALL
			UINT64 fundAddr = reinterpret_cast<UINT64>(function);
			fundAddr -= (address + 5);
			mem.full.addr = static_cast<UINT>(fundAddr);

			DWORD oldProtect = 0;
			if(VirtualProtect(lpAddress, size, PAGE_EXECUTE_READWRITE, &oldProtect))
			{
				for(SIZE_T n = 0; n < size ; n++)
				{
					if(n < 5)
					{
						lpAddress[n] = mem.byte.p[n];
					}else
					{
						lpAddress[n] = 0x90;
					}
				}
				VirtualProtect(lpAddress, size, oldProtect, &oldProtect);
			}
		}
	}catch(...)
	{
		EXCEPTION_LOG_PARAM(address);
	}
	return lpRet;
}

LPVOID Utils::WriteJump(UINT64 address, LPVOID function, UINT nops, LPBYTE lpOrgMemory, BOOL extended)
{
	LPVOID lpRet = 0;
	try
	{
		UINT size = 5 + nops;
		if(extended)
		{
			LPBYTE pMemory = static_cast<LPBYTE>(VirtualAlloc(0, (size + 5), MEM_COMMIT, PAGE_EXECUTE_READWRITE));
			if(pMemory)
			{
				HookMem hm;
				LPBYTE lpAddress = reinterpret_cast<LPBYTE>(address);
				for(UINT n=0;n<size;n++)
				{
					pMemory[n] = lpAddress[n];
					if( n < 5 )
					{
						hm.byte.p[n] = lpAddress[n];
					}
				}

				if(hm.full.opCode == 0xE8 || hm.full.opCode == 0xE9)
				{
					//reallocate addr
					UINT64 addr = hm.full.addr;
					addr += (address + 5);
					
					hm.full.addr = static_cast<UINT>(addr - (reinterpret_cast<UINT64>(pMemory) + 5));

					for(int n=0;n<5;n++)
					{
						pMemory[n] = hm.byte.p[n];
					}
				}
				
				WriteJump(reinterpret_cast<UINT64>(&pMemory[5]), reinterpret_cast<LPVOID>(address + size));
				WriteJump(address, function);
				lpRet = pMemory;
			}else
			{
				ShowMessage(_T("VoteMod"), _T("Cannot allocate memory! (%p)"), address);
			}
		}else
		{
			LPBYTE lpAddress = reinterpret_cast<LPBYTE>(address);
			HookMem mem;
			mem.full.opCode = 0xE9;	//Jump
			UINT64 fundAddr = reinterpret_cast<UINT64>(function);
			fundAddr -= (address + 5);
			mem.full.addr = static_cast<UINT>(fundAddr);

			DWORD oldProtect = 0;
			if(VirtualProtect(lpAddress, size, PAGE_EXECUTE_READWRITE, &oldProtect))
			{
				for(SIZE_T n = 0; n < size ; n++)
				{
					if(n < 5)
					{
						lpAddress[n] = mem.byte.p[n];
					}else
					{
						lpAddress[n] = 0x90;
					}
				}
				VirtualProtect(lpAddress, size, oldProtect, &oldProtect);
			}
		}
	}catch(...)
	{
		EXCEPTION_LOG_PARAM(address);
	}
	return lpRet;
}

wstring Utils::Trim(wstring str)
{
	size_t pos = str.find_first_of(L"\t\n\r\v ");
	while(pos!=wstring::npos)
	{
		str.erase(pos, 1);
		pos = str.find_first_of(L"\t\n\r\v ");
	}
	return str;
}

int Utils::GetBypassValueInt(wstring wBypass, wstring wSub, int defaultValue)
{
	wstring wRet;
	//Remove white spaces from bypass
	wBypass = Trim(wBypass);
	//Find wSub
	size_t pos = wBypass.find(L"?");
	if(pos!=wstring::npos)
	{
		pos = wBypass.find(wSub, pos);
		if(pos!=wstring::npos)
		{
			wBypass = wBypass.substr(pos, (wBypass.size() - pos));
			pos = wBypass.find(L"&");
			if(pos!=wstring::npos)
			{
				wBypass = wBypass.substr(0, pos);
			}

			pos = wBypass.find(L"=");
			if(pos == wstring::npos)
			{
				//not found
				return defaultValue;
			}
			pos++;

			wRet = wBypass.substr(pos, (wBypass.size() - pos));
		}
	}

	wstringstream wstr;
	wstr << wRet;
	int ret = defaultValue;
	wstr >> ret;
	return ret;
}

double GetPrivateProfileDouble(LPCTSTR lpAppName, LPCTSTR lpKeyName, double defaultValue, LPCTSTR lpFileName)
{
	TCHAR buf[64] = { 0 }, *ptr = 0;
    if(GetPrivateProfileString(lpAppName,lpKeyName, 0, buf, 63, lpFileName))
	{
		double outval=_tcstod(buf, &ptr);
		if(ptr!=buf)
			return outval;
	}
	return defaultValue; 
}

wstring Utils::ReplaceStringW(wstring baseStr, wstring oldStr, wstring newStr, bool all)
{
	size_t pos = baseStr.find(oldStr);
	while(pos != string::npos)
	{
		baseStr = baseStr.replace(pos, oldStr.size(), newStr);
		if(all)
			pos = baseStr.find(oldStr);
		else
			pos = wstring::npos;
	}
	return baseStr;
}

wstring Utils::ReplaceString(wstring baseStr, wstring oldStr, UINT nNew, bool all)
{
	WCHAR newStr[16] = { 0 };
	_itow_s(nNew, newStr, 16, 10);
	size_t pos = baseStr.find(oldStr);
	while(pos != wstring::npos)
	{
		baseStr = baseStr.replace(pos, oldStr.size(), newStr);
		if(all)
			pos = baseStr.find(oldStr);
		else
			pos = wstring::npos;
	}
	return baseStr;
}

string Utils::ReplaceStringA(string baseStr, string oldStr, string newStr, bool all)
{
	size_t pos = baseStr.find(oldStr);
	while(pos != string::npos)
	{
		baseStr = baseStr.replace(pos, oldStr.size(), newStr);
		if(all)
			pos = baseStr.find(oldStr);
		else
			pos = string::npos;
	}
	return baseStr;
}