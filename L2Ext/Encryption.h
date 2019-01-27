#pragma once

extern "C"
{
	void CloseIOSocketHook();
	void InlineEncryptHook();
	void InlineDecryptHook();
	void EncryptWrapper(CSocket *pSocket, CIOBuffer *pBuffer);
	void DecryptWrapper(CUserSocket *pSocket, int nOffset);
};

class CPacketEncryption
{
public:
	static void Init();
	static void KeyPacket(CUserSocket *pSocket, const char* format, BYTE opCode, BYTE validProtocol, int nSKey1, int nSKey2, int nSKey3);
	static void EncryptCall(LPBYTE dest, LPBYTE src, LPVOID pKey, int size);	
};