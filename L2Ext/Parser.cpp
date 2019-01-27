#include "StdAfx.h"
#include "Parser.h"

wstring CParser::Replace(wstring str, wchar_t sign, wchar_t new_sign)
{
	size_t pos = str.find(sign);
	while(pos!=wstring::npos)
	{
		str[pos] = new_sign;
		pos = str.find(sign);
	}
	return str;
}
string CParser::Replace(string str, char sign, char new_sign)
{
	size_t pos = str.find(sign);
	while(pos!=string::npos)
	{
		str[pos] = new_sign;
		pos = str.find(sign);
	}
	return str;
}
wstring CParser::Trim(wstring str)
{
	size_t pos = str.find_first_of(L"\t\n\r\v ");
	while(pos!=wstring::npos)
	{
		str.erase(pos, 1);
		pos = str.find_first_of(L"\t\n\r\v ");
	}
	return str;
}
string CParser::Trim(string str)
{
	size_t pos = str.find_first_of("\t\n\r\v ");
	while(pos!=string::npos)
	{
		str.erase(pos, 1);
		pos = str.find_first_of("\t\n\r\v ");
	}
	return str;
}
wstring CParser::GetBypassValue(wstring wBypass, wstring wSub)
{
	wstring wRet;
	//Remove white spaces from bypass
	wBypass = Trim(wBypass);
	//Find wSub
	size_t pos = wBypass.find(wSub);
	if(pos!=wstring::npos)
	{
		wBypass = wBypass.substr(pos, (wBypass.size() - pos));
		pos = wBypass.find_first_of(L"&#");
		if(pos!=wstring::npos)
		{
			wBypass = wBypass.substr(0, pos);
		}

		pos = wBypass.find(L"=");
		if(pos == wstring::npos)
		{
			//not found
			return L"";
		}
		pos++;

		wRet = wBypass.substr(pos, (wBypass.size() - pos));
	}
	return wRet;
}

int CParser::GetBypassValueInt(wstring wBypass, wstring wSub, int defaultValue)
{
	wstring wRet;
	//Remove white spaces from bypass
	wBypass = Trim(wBypass);
	//Find wSub
	size_t pos = wBypass.find(wSub);
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

	wstringstream wstr;
	wstr << wRet;
	int ret = defaultValue;
	wstr >> ret;
	return ret;
}

string CParser::GetColumn(string sSub, int nIndex)
{
	stringstream sstr;
	sstr << sSub;
	string sColumn;
	while(sstr >> sColumn)
	{
		if(nIndex == 0)
			return sColumn;
		nIndex--;
	}
	sColumn = "";
	return sColumn;
}
string CParser::GetValueString(string sSub, string sName)
{
	string sValue;
	size_t pos = sSub.find(sName);
	if(pos != string::npos)
	{
		string sTmp = sSub.substr(pos+sName.size(), sSub.size());
		pos = sTmp.find('=');
		if(pos!= string::npos)
			sValue = sTmp.substr(pos+1, sSub.size());
	}
	
	stringstream sstr;
	sstr << sValue;
	sstr >> sValue;
	return sValue;
}
wstring CParser::GetName(wstring wStr)
{
	wstring wRet;
	for(int n=0;n<wStr.size();n++)
	{
		if(wStr[n] != L'[' && wStr[n] != L']')
			wRet += wStr[n];
	}
	return wRet;
}
wstring CParser::GetValueString(wstring sSub, wstring sName)
{
	wstring sValue;
	size_t pos = sSub.find(sName);
	if(pos != wstring::npos)
	{
		wstring sTmp = sSub.substr(pos+sName.size(), sSub.size());
		pos = sTmp.find('=');
		if(pos!= wstring::npos)
			sValue = sTmp.substr(pos+1, sSub.size());
	}
	
	wstringstream sstr;
	sstr << sValue;
	sstr >> sValue;
	return sValue;
}
int CParser::GetValueInt(string sSub, string sName, int defaultValue)
{
	string sValue;
	size_t pos = sSub.find(sName);
	if(pos != string::npos)
	{
		string sTmp = sSub.substr(pos+sName.size(), sSub.size());
		pos = sTmp.find('=');
		if(pos!= string::npos)
			sValue = sTmp.substr(pos+1, sSub.size());
	}else
	{
		return defaultValue;
	}

	int nValue = 0;
	stringstream sstr;
	sstr << sValue;
	sstr >> nValue;
	return nValue;
}
int CParser::GetValueInt(wstring wSub, wstring wName)
{
	wstring wValue;
	size_t pos = wSub.find(wName);
	if(pos != wstring::npos)
	{
		wstring wTmp = wSub.substr(pos+wName.size(), wSub.size());
		pos = wTmp.find('=');
		if(pos!= string::npos)
			wValue = wTmp.substr(pos+1, wSub.size());
	}
	int nValue = 0;
	wstringstream wstr;
	wstr << wValue;
	wstr >> nValue;
	return nValue;
}
double CParser::GetValueDouble(string sSub, string sName)
{
	string sValue;
	size_t pos = sSub.find(sName);
	if(pos != string::npos)
	{
		string sTmp = sSub.substr(pos+sName.size(), sSub.size());
		pos = sTmp.find('=');
		if(pos!= string::npos)
			sValue = sTmp.substr(pos+1, sSub.size());
	}
	double dValue = 0;
	stringstream sstr;
	sstr << sValue;
	sstr >> dValue;
	return dValue;
}
void CParser::ConvToLower(PWCHAR wsTxt, int nMaxSize)
{
	try 
	{
		LPWORD buffer = (LPWORD) wsTxt;
		int n = 0;
		while(buffer[n]!= 0x0000 && n < nMaxSize)
		{
			//65 A 90 Z + 32 = lowercase
			if(buffer[n] > 64 && buffer[n] < 91)
			{
				buffer[n] += 32;
			}
			n++;
		}
	}
	catch(...)
	{
		g_Log.Add(CLog::Error, "[%s] Exception Detected", __FUNCTION__);
	}
}

void WritePrivateProfileInt(LPTSTR lpAppName, LPTSTR lpKeyName, int Value, LPTSTR lpFileName)
{
	TCHAR str[128];
	_itot_s(Value, str, 128, 10);
	WritePrivateProfileString(lpAppName, lpKeyName, str, lpFileName);
};

WCHAR Parser::ToWLower(WCHAR c)
{
	if(c >= 0x410 && c <= 0x42F)
	{
		c += 0x20;
	}else if( c <= 0x24F)
	{
		c = towlower(c);
	}
	
	return c;
}


int Parser::GetBypassValueInt(const WCHAR* wBypass, const WCHAR* wName, int defaultValue)
{
	guard;

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
				 unguard;
				 return ret;
			 }
		}
	}
	unguard;
	return defaultValue;
}

INT32 Parser::ParseOptionInt(string line, string option, INT32 defaultValue)
{
	guard;

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

	unguard;
	return defaultValue;
}

INT32 Parser::ParseOptionInt(wstring line, wstring option, INT32 defaultValue)
{
	guard;

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

	unguard;
	return defaultValue;
}

double Parser::ParseOptionDouble(string line, string option, double defaultValue)
{
	guard;

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

	unguard;
	return defaultValue;
}

double Parser::ParseOptionDouble(wstring line, wstring option, double defaultValue)
{
	guard;

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

	unguard;
	return defaultValue;
}

wstring Parser::ParseOptionMessage(wstring line, wstring option, wstring sDefault)
{
	CTL;

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
			//get text between "message"
			param = line.substr((pos + 1), (line.size() - pos  -1));
			pos = param.find(L"\"");
			if(pos != wstring::npos)
			{
				size_t posEx = param.find(L"\"", pos+1);
				if(posEx != wstring::npos)
				{
					value = param.substr(pos+1, (posEx - pos - 1));
				}
			}
		}
		break;
	}

	return value;
}

string Parser::ParseOptionString(string line, string option, string sDefault)
{
	guard;

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

	unguard;
	return value;
}

wstring Parser::ParseOptionString(wstring line, wstring option, wstring sDefault)
{
	guard;

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

	unguard;
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

void Parser::SplitEx(wstring line, wstring separators, vector<wstring>& words, wstring magicChars, bool toLower)
{
	wstring word = L"";
	wstring magicWord = L"";
	for(UINT n=0;n<line.size();n++)
	{
		bool newWord = false;
		bool newMagicWord = false;
		for(UINT m=0;m<separators.size();m++)
		{
			if(line[n] == separators[m])
			{
				newWord = true;
				break;
			}
		}
		for(UINT m=0;m<magicChars.size();m++)
		{
			if(line[n] == magicChars[m])
			{
				newWord = true;
				newMagicWord = true;
				magicWord = line[n];
				break;
			}
		}
		if(newWord)
		{
			if(word.size() > 0)
			{
				words.push_back(word);
				word = L"";
			}
			if(newMagicWord)
			{
				words.push_back(magicWord);
				magicWord = L"";
			}
		}else
		{
			if(toLower)
			{
				word += towlower(line[n]);
			}else
			{
				word += line[n];
			}
		}
	}
	if(word.size() > 0)
	{
		words.push_back(word);
	}
}

bool Parser::IsValidSqlString(const WCHAR* wInput)
{
	guard;
	bool ret = true;

	const WCHAR* invalidChars = L"\'\"\\;`";

	//check for not allowed signs
	size_t len = wcslen(wInput);
	for(size_t n=0;n<len;n++)
	{
		for(UINT m=0;m<5;m++)
		{
			if(wInput[n] == invalidChars[m])
			{
				ret = false;
				break;
			}
		}
		if(!ret)
		{
			break;
		}
	}

	unguard;

	return ret;
}

wstring Parser::Trim(wstring str)
{
	while(str.size() > 0)
	{
		if(str[0] == L' ' || str[0] == L'\t')
			str.erase(0, 1);
		else
			break;
	}

	while(str.size() > 0)
	{
		if(str[str.size() - 1] == L' ' || str[str.size() - 1] == L'\t')
			str.erase(str.size() - 1, 1);
		else
			break;
	}

	return str;
}
