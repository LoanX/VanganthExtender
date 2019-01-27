#pragma once

class CCoder
{
	BYTE inKey[16];
	BYTE outKey[16];
	BYTE cryptionByte1;
	BYTE cryptionByte2;
	bool isEnabled;
public:
	void Constructor();
	static LPBYTE GenerateKey();
	void SetKey(LPBYTE key);
	inline bool IsEnabled() { return isEnabled; };
	void Decrypt(unsigned char* packet, int offset, int size);
	void Encrypt(unsigned char* packet, int offset, int size);
};