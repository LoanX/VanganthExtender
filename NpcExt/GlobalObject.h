#pragma once

class CGlobalObject
{
public:
	static void TeleportMPCC(LPVOID lpGlobalObject, PUINT64 lpParams);
	static void TeleportMPCCByIndex(LPVOID lpGlobalObject, PUINT64 lpParams);
	static void SetPCLevel(LPVOID lpGlobalObject, PUINT64 lpParams);
	static void AddPCSocial(LPVOID lpGlobalObject, PUINT64 lpParams);
	static void UpdatePledgeNameValue(LPVOID lpGlobalObject, PUINT64 lpParams);
};