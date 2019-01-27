#pragma once


class NpcServer
{
	static const INT64 nServer = 0x1557EE0;
public:
	static void Send(int nBuffSize, PCHAR buff);
	static bool IsConnected();
	static void SpawnNpc(UINT npcClassId, int x, int y, int z);
	static UINT SpawnNpcEx(UINT npcClassId, int x, int y, int z, map<wstring, wstring>& aiParams);
};