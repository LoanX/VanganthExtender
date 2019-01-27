#include "stdafx.h"
#include "Utils.h"

const unsigned char* Disassemble(const unsigned char* packet, const char* format, ...)
{
	typedef const unsigned char* (*f)(const unsigned char*, const char*, va_list va);

	va_list va;
	va_start(va, format);
	const unsigned char* ret = f(0x4849B0)(packet,format, va);
	va_end(va);

	return ret;
};

int Assemble(PCHAR buf, int bufLen, const char *format, ...)
{
	typedef int (*f)(PCHAR, int, const char*, va_list);
	
	va_list va;
	va_start(va, format);
	int nRet = f(0x484EB0)(buf, bufLen, format, va);
	va_end(va);

	return nRet;
};

wstring StringToWString(const string& s)
{
	wstring temp(s.size(), L' ');
	for(UINT n=0;n<s.size();n++)
	{
		temp[n] = static_cast<wchar_t>(s[n]);
	}
	return temp; 
}


string WStringToString(const wstring& s)
{
	string temp(s.length(), ' ');
	for(UINT n=0;n<s.size();n++)
	{
		temp[n] = static_cast<char>(s[n]);
	}
	return temp; 
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


void MovLogFiles(LPCTSTR sLogDir)
{
	list<wstring> lLogs;
	CreateDirectory(sLogDir, NULL);
	WIN32_FIND_DATA info;
	HANDLE hFile = 0;
	TCHAR sDir[265];
	GetCurrentDirectory(260, sDir);
	_tcscat_s(sDir, 260, TEXT("\\*"));
	hFile = FindFirstFile(sDir, &info);
	if(hFile)
	{
		do
		{
			if (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			 
			}
			else
			{
				if(_tcscmp(info.cFileName, TEXT("LinError.txt")))
				{
					if(_tcsstr(info.cFileName, TEXT("LinError")) || _tcsstr(info.cFileName, TEXT(".dmp")))
					{
						lLogs.push_back(info.cFileName);
						continue;
					}
				}
			}
		}
		while (FindNextFile(hFile, &info) != 0);
		FindClose(hFile);
	}
	if(lLogs.size() > 0)
	{
		for(list<wstring>::iterator Iter = lLogs.begin();Iter!=lLogs.end();Iter++)
		{
			TCHAR sNewFile[512];
			_tcscpy_s(sNewFile, 512, sLogDir);
			_tcscat_s(sNewFile, TEXT("\\"));
			_tcscat_s(sNewFile, Iter->c_str());
			MoveFile(Iter->c_str(), sNewFile);
		}
	}
};


int Parser::GetBypassValueInt(const WCHAR* wBypass, const WCHAR* wName, int defaultValue)
{
	if(wBypass && wName)
	{
		size_t bypassLen = wcslen(wBypass);
		size_t nameLen = wcslen(wName);
		if(bypassLen > nameLen && nameLen)
		{
			WCHAR wStr[4096]; memset(wStr, 0, sizeof(wStr));
			//trim str
			for(int n=0, m=0; n < bypassLen; n++)
			{
				if( wBypass[n] != L' ' && wBypass[n] != L'\t' )
				{
					wStr[m] = wBypass[n];
					m++;
				}
			}

			 const WCHAR* wPos = wcsstr(wStr, wName);
			 bool found = false;
			 while(wPos && !found)
			 {
				 if( wPos != wStr && wPos[-1] == L'?' || wPos[-1] == L'&')
				 {
					 if( wPos[nameLen] == L'=' )
					 {
						 found = true;
						 break;
					 }
				 }
				 wPos = wcsstr(&wPos[1], wName);
			 }

			 if(found)
			 {

				 INT32 ret = _wtoi(&wPos[nameLen+1]);
				 return ret;
			 }
		}
	}
	return defaultValue;
}

INT32 Parser::ParseOptionInt(string line, string option, INT32 defaultValue)
{
	string param;
	size_t optionSize = option.size();
	size_t pos = line.find(option);
	while( pos!=string::npos )
	{
		if( pos > 0 )
		{
			if( line[pos-1] != ' ' && line[pos-1] != '\t' )
			{
				pos++;
				pos = line.find(option, pos);
				continue;
			}
		}
		
		if( line[pos + optionSize] != '=' && line[pos + optionSize] != ' ' && line[pos + optionSize] != '\t' )
		{
			pos++;
			pos = line.find(option, pos);
			continue;
		}

		pos = line.find("=", pos);
		if( pos != string::npos )
		{
			param = line.substr((pos + 1), (line.size() - pos  -1));
			stringstream sstr(param);
			sstr >> defaultValue;
		}
		break;
	}
	return defaultValue;
}

INT32 Parser::ParseOptionInt(wstring line, wstring option, INT32 defaultValue)
{
	wstring param;
	size_t optionSize = option.size();
	size_t pos = line.find(option);
	while( pos!=wstring::npos )
	{
		if( pos > 0 )
		{
			if( line[pos-1] != L' ' && line[pos-1] != L'\t' )
			{
				pos++;
				pos = line.find(option, pos);
				continue;
			}
		}
		
		if( line[pos + optionSize] != L'=' && line[pos + optionSize] != L' ' && line[pos + optionSize] != L'\t' )
		{
			pos++;
			pos = line.find(option, pos);
			continue;
		}

		pos = line.find(L"=", pos);
		if( pos != string::npos )
		{
			param = line.substr((pos + 1), (line.size() - pos  -1));
			wstringstream sstr(param);
			sstr >> defaultValue;
		}
		break;
	}
	return defaultValue;
}

double Parser::ParseOptionDouble(string line, string option, double defaultValue)
{
	string param;
	size_t optionSize = option.size();
	size_t pos = line.find(option);
	while( pos!=string::npos )
	{
		if( pos > 0 )
		{
			if( line[pos-1] != ' ' && line[pos-1] != '\t' )
			{
				pos++;
				pos = line.find(option, pos);
				continue;
			}
		}
		
		if( line[pos + optionSize] != '=' && line[pos + optionSize] != ' ' && line[pos + optionSize] != '\t' )
		{
			pos++;
			pos = line.find(option, pos);
			continue;
		}

		pos = line.find("=", pos);
		if( pos != string::npos )
		{
			param = line.substr((pos + 1), (line.size() - pos  -1));
			stringstream sstr(param);
			sstr >> defaultValue;
		}
		break;
	}
	return defaultValue;
}

double Parser::ParseOptionDouble(wstring line, wstring option, double defaultValue)
{
	wstring param;
	size_t optionSize = option.size();
	size_t pos = line.find(option);
	while( pos!=wstring::npos )
	{
		if( pos > 0 )
		{
			if( line[pos-1] != L' ' && line[pos-1] != L'\t' )
			{
				pos++;
				pos = line.find(option, pos);
				continue;
			}
		}
		
		if( line[pos + optionSize] != L'=' && line[pos + optionSize] != L' ' && line[pos + optionSize] != L'\t' )
		{
			pos++;
			pos = line.find(option, pos);
			continue;
		}

		pos = line.find(L"=", pos);
		if( pos != string::npos )
		{
			param = line.substr((pos + 1), (line.size() - pos  -1));
			wstringstream sstr(param);
			sstr >> defaultValue;
		}
		break;
	}

	return defaultValue;
}

string Parser::ParseOptionString(string line, string option, string sDefault)
{
	string param;
	
	string value(sDefault);

	size_t optionSize = option.size();
	size_t pos = line.find(option);
	while( pos!=string::npos )
	{
		if( pos > 0 )
		{
			if( line[pos-1] != ' ' && line[pos-1] != '\t' )
			{
				pos++;
				pos = line.find(option, pos);
				continue;
			}
		}
		
		if( line[pos + optionSize] != '=' && line[pos + optionSize] != ' ' && line[pos + optionSize] != '\t' )
		{
			pos++;
			pos = line.find(option, pos);
			continue;
		}

		pos = line.find("=", pos);
		if( pos != string::npos )
		{
			param = line.substr((pos + 1), (line.size() - pos  -1));
			stringstream sstr(param);
			sstr >> value;
		}
		break;
	}
	return value;
}

wstring Parser::ParseOptionString(wstring line, wstring option, wstring sDefault)
{
	wstring param;
	wstring value(sDefault);

	size_t optionSize = option.size();
	size_t pos = line.find(option);
	while( pos!=wstring::npos )
	{
		if( pos > 0 )
		{
			if( line[pos-1] != L' ' && line[pos-1] != L'\t' )
			{
				pos++;
				pos = line.find(option, pos);
				continue;
			}
		}
		
		if( line[pos + optionSize] != L'=' && line[pos + optionSize] != L' ' && line[pos + optionSize] != L'\t' )
		{
			pos++;
			pos = line.find(option, pos);
			continue;
		}

		pos = line.find(L"=", pos);
		if( pos != string::npos )
		{
			param = line.substr((pos + 1), (line.size() - pos  -1));
			wstringstream sstr(param);
			sstr >> value;
		}
		break;
	}
	return value;
}

wstring Parser::Replace(wstring str, wchar_t sign, wchar_t new_sign)
{
	size_t pos = str.find(sign);
	while(pos!=wstring::npos)
	{
		str[pos] = new_sign;
		pos = str.find(sign);
	}
	return str;
}
string Parser::Replace(string str, char sign, char new_sign)
{
	size_t pos = str.find(sign);
	while(pos!=string::npos)
	{
		str[pos] = new_sign;
		pos = str.find(sign);
	}
	return str;
}

vector<string> Parser::Split(string line, string separator)
{
	vector<string> data;
	string ex = "";
	for(UINT n=0;n<line.size();n++)
	{
		bool newL = false;
		for(UINT m = 0; m < separator.size(); m++)
		{
			if(line[n] == separator[m])
			{
				newL = true;
				break;
			}
		}
		if(newL)
		{
			data.push_back(ex);
			ex = "";
			continue;
		}
		ex += line[n];
	}
	
	if(ex.size() > 0)
	{
		data.push_back(ex);
	}
	return data;
}

vector<wstring> Parser::Split(wstring line, wstring separator)
{
	vector<wstring> data;
	wstring ex = L"";
	for(UINT n=0;n<line.size();n++)
	{
		bool newL = false;
		for(UINT m = 0; m < separator.size(); m++)
		{
			if(line[n] == separator[m])
			{
				newL = true;
				break;
			}
		}
		if(newL)
		{
			data.push_back(ex);
			ex = L"";
			continue;
		}
		ex += line[n];
	}
	if(ex.size() > 0)
	{
		data.push_back(ex);
	}
	return data;
}