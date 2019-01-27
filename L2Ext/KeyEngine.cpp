#include "stdafx.h"
#include "KeyEngine.h"

using namespace KeyEngine;

namespace
{
	const size_t g_KeyLength = 5*5;
}

CKeyEngine::CKeyEngine() : m_State(KeyEngine::StateNone), m_Validated(false)
{
	//ctor
}

CKeyEngine& CKeyEngine::getInstance()
{
	static CKeyEngine m_Instance;
	return m_Instance;
}

void CKeyEngine::init()
{
	VIRTUALIZER_TIGER_BLACK_START;
	loadINI();
	m_Hardware.generate();

	if(isValid(m_Name, m_Key, m_Hardware))
	{
		m_State = KeyEngine::StateValid;
	}
	else
	{
		m_State = KeyEngine::StateInvalid;
	}

	VIRTUALIZER_TIGER_BLACK_END;
}

void CKeyEngine::loadINI()
{
	VIRTUALIZER_TIGER_BLACK_START;
	TCHAR file[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, file);
	lstrcat(file, _T("\\License.ini"));

	const WCHAR* sectionName = L"Data";
	TCHAR temp[8190] = { 0 };
	if(GetPrivateProfileString(sectionName, _T("Name"), 0, temp, 8190, file))
	{
		size_t len = wcslen(temp);
		for(size_t n=0;n<len;n++)
		{
			m_Name.push_back((char)temp[n]);
		}
	}
	if(GetPrivateProfileString(sectionName, _T("Key"), 0, temp, 8190, file))
	{
		size_t len = wcslen(temp);
		for(size_t n=0;n<len;n++)
		{
			m_Key.push_back((char)temp[n]);
		}
	}
	VIRTUALIZER_TIGER_BLACK_END;
}

void CKeyEngine::writeINI()
{
	VIRTUALIZER_TIGER_BLACK_START;
	TCHAR file[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, file);
	lstrcat(file, _T("\\License.ini"));

	const WCHAR* sectionName = L"Data";
	TCHAR temp[8190] = { 0 };
	for(size_t n=0;n<m_Name.size(); ++n)
		temp[n] = m_Name[n];

	WritePrivateProfileString(sectionName, _T("Name"), temp, file);
	
	std::string hw = m_Hardware.toString().c_str();
	memset(temp, 0, sizeof(temp));
	for(size_t n=0;n<hw.size(); ++n)
		temp[n] = hw[n];

	WritePrivateProfileString(sectionName, _T("HardwareId"), temp, file);

	memset(temp, 0, sizeof(temp));
	for(size_t n=0;n<m_Key.size(); ++n)
		temp[n] = m_Key[n];

	WritePrivateProfileString(sectionName, _T("Key"), temp, file);

	
	VIRTUALIZER_TIGER_BLACK_END;
}

void CKeyEngine::showInvalidKeyMessage()
{
	VIRTUALIZER_TIGER_BLACK_START;
	std::wstringstream msgStream;
	msgStream << L"The license file (License.ini) seems to contain invalid serial key.\nYour HardwareId is: " << m_Hardware.toString().c_str();
	msgStream << L"\nPlease contact with server administrator in order to get correct serial key.";

	MessageBox(0, msgStream.str().c_str(), L"License Engine", MB_OK | MB_TOPMOST);

	writeINI();

	VIRTUALIZER_TIGER_BLACK_END;
}

bool CKeyEngine::isValid(std::string name, std::string key, const CHardwareId& hw) const
{
	VIRTUALIZER_TIGER_BLACK_START;
	int32_t keyPart = 0;
	int32_t partIndex = 0;
	size_t pos = key.find('-');
	while (pos != key.npos)
	{
		key.erase(pos, 1);
		pos = key.find('-');
	}

	m_Validated = false;

	bool result = (key.size() == g_KeyLength && name.size() > 3) ? true : false;

	if (result)
	{
		//validate name
		for (size_t n = 0; n < name.size(); n++)
		{
			if (partIndex < 3)
			{
				int32_t index = getCharIndex(name[n]);
				if (index < 0)
				{
					result = false;
					break;
				}
				//get position in key
				size_t position = (keyPart * 5 + partIndex) % g_KeyLength;
				int32_t keyIndex = getCharIndex(key[position]);
				if (keyIndex < 0)
				{
					result = false;
					break;
				}

				int32_t diff = index > keyIndex ? (index - keyIndex) : (keyIndex - index);

				if (diff > 2)
				{
					result = false;
					break;
				}

				if (keyPart == 4)
					partIndex++;
				keyPart = (keyPart + 1) % 5;
			}
		}
	}
	if (result)
	{
		//validate hardware
		keyPart = 0;
		partIndex = 3;
		//write hw
		for (size_t n = 0; n < 8; ++n)
		{
			int32_t index = getCharIndex(hw[n]);
			if (index < 0)
			{
				result = false;
				break;
			}

			size_t position = (keyPart * 5 + partIndex) % g_KeyLength;
			int32_t keyIndex = getCharIndex(key[position]);

			if (keyIndex < 0)
			{
				result = false;
				break;
			}

			int32_t diff = index > keyIndex ? (index - keyIndex) : (keyIndex - index);

			if (diff > 2)
			{
				result = false;
				break;
			}

			if (keyPart == 4)
				partIndex++;
			keyPart = (keyPart + 1) % 5;
		}

	}
	m_Validated = true;
	VIRTUALIZER_TIGER_BLACK_END;
	return result;
}

int32_t KeyEngine::getCharIndex(char c)
{
	int32_t result = -1;
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	else if (c >= 'A' && c <= 'Z')
	{
		result = '9' - '0' + 1 + c - 'A';
	}
	else if (c >= 'a' && c <= 'z')
	{
		result = '9' - '0' + 1 + 'Z' - 'A' + 1 + c - 'a';
	}
	return result;
}

char KeyEngine::getCharFromIndex(int32_t index)
{
	char result = 0;

	if (index >= 0 && index <= ('9' - '0'))
	{
		result = '0' + index;
	}
	else if (index >= ('9' - '0' + 1) && index <= ('9' - '0' + 1 + 'Z' - 'A'))
	{
		result = 'A' + index - ('9' - '0' + 1);
	}
	else if (index >= ('9' - '0' + 1 + 'Z' - 'A' + 1) && index <= ('9' - '0' + 1 + 'Z' - 'A' + 1 + 'z' - 'a'))
	{
		result = 'a' + index - ('9' - '0' + 1 + 'Z' - 'A' + 1);
	}
	return result;
}

KeyEngine::CHardwareId::CHardwareId()
{
	for(size_t n=0;n<CHardwareId::SIZE;++n)
		m_Part[n] = 0;
}

KeyEngine::CHardwareId::CHardwareId(std::string hw)
{
	size_t pos = hw.find('-');
	while(pos != hw.npos)
	{
		hw.erase(pos, 1);
		pos = hw.find('-');
	}

	if(hw.size() == CHardwareId::SIZE)
	{
		for(size_t n=0;n<CHardwareId::SIZE;n++)
			m_Part[n] = hw[n];
	}
}

std::string KeyEngine::CHardwareId::toString()
{
	std::string key;
	key.insert(0, m_Part, CHardwareId::SIZE);
	key.insert(4, "-");
	return key;
}

const char& KeyEngine::CHardwareId::operator [](size_t pos) const
{
	static char m_Default = 0;
	if(pos < CHardwareId::SIZE)
	{
		return m_Part[pos];
	}
	return m_Default;
}

void KeyEngine::CHardwareId::generate()
{
	VIRTUALIZER_TIGER_BLACK_START;
	std::wstringstream hash;
	//get computer name
	TCHAR temp[8190] = { 0 };
	DWORD size = sizeof(temp);
	GetComputerName(temp, &size);
	hash << temp;

	//network card
	std::vector<std::string> mac;
	GetMacAddress(mac);
	if(mac.size() > 0)
	{
		std::wstring macW;
		for(size_t n=0;n<mac[0].size();++n)
			macW.push_back(mac[0][n]);
		hash << macW;
	}

	//cpu info
	SYSTEM_INFO si = { 0 };
	GetSystemInfo(&si);
	hash << hex << si.dwNumberOfProcessors << hex << si.dwProcessorType << hex << si.wProcessorArchitecture << hex << si.wProcessorLevel << hex << si.wProcessorRevision;

	//memoryinfo
	MEMORYSTATUSEX statex = { 0 };
	statex.dwLength = sizeof (statex);
	if(GlobalMemoryStatusEx (&statex))
	{
		UINT64 totalRam = statex.ullTotalPhys;
		totalRam /= 1024;
		hash << hex << totalRam;
	}

	std::wstring baseStr = hash.str();
	std::string noZeroStr;
	for(size_t n=0;n<baseStr.size(); ++n)
		if(baseStr[n] != L'0' && baseStr[n] != L' ' && baseStr[n] != L'-')
			noZeroStr.push_back((char)baseStr[n]);

	UINT checkSum = AdlerCheckSum((PBYTE)noZeroStr.c_str(), static_cast<UINT>(noZeroStr.size()));

	wstringstream strm;
	strm << hex << checkSum << std::endl;
	strm << noZeroStr.c_str();

	std::stringstream ss;
    ss << uppercase << hex << checkSum;

	std::string data = ss.str();
	if(data.size() >= CHardwareId::SIZE)
	{
		for(size_t n=0;n<CHardwareId::SIZE;n++)
			m_Part[n] = data[n];
	}

	VIRTUALIZER_TIGER_BLACK_END;
}