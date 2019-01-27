#pragma once

#define CRYPT32W(str) { CRYPT32_(str L"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0") }
#define CRYPT32W_(str) (str)[0] + 1, (str)[1] + 2, (str)[2] + 3, (str)[3] + 4, (str)[4] + 5, (str)[5] + 6, (str)[6] + 7, (str)[7] + 8, (str)[8] + 9, (str)[9] + 10, (str)[10] + 11, (str)[11] + 12, (str)[12] + 13, (str)[13] + 14, (str)[14] + 15, (str)[15] + 16, (str)[16] + 17, (str)[17] + 18, (str)[18] + 19, (str)[19] + 20, (str)[20] + 21, (str)[21] + 22, (str)[22] + 23, (str)[23] + 24, (str)[24] + 25, (str)[25] + 26, (str)[26] + 27, (str)[27] + 28, (str)[28] + 29, (str)[29] + 30, (str)[30] + 31, (str)[31] + 32, L'\0'

#define CRYPT32(str) { CRYPT32_(str "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0") }
#define CRYPT32_(str) (str)[0] + 1, (str)[1] + 2, (str)[2] + 3, (str)[3] + 4, (str)[4] + 5, (str)[5] + 6, (str)[6] + 7, (str)[7] + 8, (str)[8] + 9, (str)[9] + 10, (str)[10] + 11, (str)[11] + 12, (str)[12] + 13, (str)[13] + 14, (str)[14] + 15, (str)[15] + 16, (str)[16] + 17, (str)[17] + 18, (str)[18] + 19, (str)[19] + 20, (str)[20] + 21, (str)[21] + 22, (str)[22] + 23, (str)[23] + 24, (str)[24] + 25, (str)[25] + 26, (str)[26] + 27, (str)[27] + 28, (str)[28] + 29, (str)[29] + 30, (str)[30] + 31, (str)[31] + 32, '\0'

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

inline const char* DECRYPT32(const char* src)
{
	static char temp[32];
	memset(temp, 0, 32);

	for(UINT n=0;n<32;n++)
	{
		temp[n] = src[n] - (n+1);
	}
	return temp;
}

inline const WCHAR* DECRYPT32W(const WCHAR* src)
{
	static WCHAR temp[32];
	memset(temp, 0, 32);

	for(UINT n=0;n<32;n++)
	{
		temp[n] = src[n] - (n+1);
	}
	return temp;
}

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
