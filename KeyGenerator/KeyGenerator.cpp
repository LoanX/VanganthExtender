// KeyGenerator.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

const size_t g_KeyLength = 25;


int32_t getCharIndex(char c)
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

char getCharFromIndex(int32_t index)
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

struct HardwareId
{
	char part[8];
	HardwareId(std::string hw)
	{
		size_t pos = hw.find('-');
		while (pos != hw.npos)
		{
			hw.erase(pos, 1);
			pos = hw.find('-');
		}
		if (hw.size() == 8)
		{
			for (size_t n = 0; n < 8; n++)
				part[n] = hw[n];
		}
	}
};

std::string generateKey(std::string name, const HardwareId& hw)
{
	std::string key(g_KeyLength, '*');
	int32_t keyPart = 0;
	int32_t partIndex = 0;
	//write name
	for (size_t n = 0; n < name.size(); n++)
	{
		if (partIndex < 3)
		{
			int32_t index = getCharIndex(name[n]);
			if (index < 0)
			{
				break;
			}
			//get position in key
			size_t position = (keyPart * 5 + partIndex) % g_KeyLength;
			if (keyPart == 4)
				partIndex++;

			keyPart = (keyPart + 1) % 5;

			int32_t keyIndex = index + ((rand() % 5) - 2);
			if (keyIndex > getCharIndex('z'))
				keyIndex = getCharIndex('z');
			if (keyIndex < 0)
				keyIndex = 0;

			key[position] = getCharFromIndex(keyIndex);
		}
	}
	keyPart = 0;
	partIndex = 3;
	//write hw
	for (size_t n = 0; n < 8; ++n)
	{
		int32_t index = getCharIndex(hw.part[n]);
		if (index >= 0)
		{
			int32_t keyIndex = index + ((rand() % 5) - 2);
			if (keyIndex > getCharIndex('z'))
				keyIndex = getCharIndex('z');
			if (keyIndex < 0)
				keyIndex = 0;

			size_t position = (keyPart * 5 + partIndex) % g_KeyLength;
			key[position] = getCharFromIndex(keyIndex);
		}
		if (keyPart == 4)
			partIndex++;
		keyPart = (keyPart + 1) % 5;
	}

	for (size_t n = 0; n < key.size(); n++)
	{
		if (key[n] == '*')
		{
			key[n] = getCharFromIndex(rand() % (getCharIndex('z') + 1));
		}
	}

	for (size_t n = 0; n < 4; n++)
	{
		key.insert((n * 5 + 5 + n), "-");
	}

	return key;
}

int _tmain(int argc, _TCHAR* argv[])
{
	cout << "Welcome to key generator." << std::endl;
	cout << "License name: ";
	std::string name;
	cin >> name;
	cout << "HardwareId: ";
	std::string hardware;
	cin >> hardware;

	cout << "Generating a key ..." << endl;

	std::string key = generateKey(name, HardwareId(hardware));


	cout << "Generated key: " << key << endl;

	system("pause");


	return 0;
}

