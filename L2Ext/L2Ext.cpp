#include "StdAfx.h"
#include "Exploits.h"
#include "License.h"

HANDLE hMutex;

DWORD g_ItemPacketReuse = 1000;
double g_MaxValidateLocationDiff = 0.0;
BOOL g_BlockSummonExp = FALSE;
BOOL g_SpiritShotExtension = FALSE;
BOOL g_ValidateAcquiredSkills = FALSE;
BOOL g_SubStackServer = FALSE;
BOOL g_ShowInventoryOnEnterWorld = TRUE;
BOOL g_ValidateStackableItems = FALSE;
BOOL g_DeleteInvalidStack = FALSE;
BOOL g_DisablePets = FALSE;
BOOL g_ShowEnterWorldHtml = FALSE;
INT32 g_MaxPartyLevelRange = 0;
BOOL g_BlockPartyInvite = 0;
wstring g_ExtReleaseVersion = L"1.0.0.0";
BOOL g_CustomAbnormalInfoPacket = FALSE;
BOOL g_GetLootFromPet = FALSE;
double g_PetExpRate = 1.0;
BOOL g_GlobalChat = FALSE;
INT32 g_ChatShoutLevel = 0;
BOOL g_PacketLog = FALSE;
TCHAR g_ConfigFile[260];
BOOL g_MenuHtml = FALSE;
UINT g_ServerStartTime = 0;
UINT g_SaveAbnormalsIntervalTime = 0;
UINT g_ShutdownTime = 30;

BOOL g_PetBlockGiveAllItemsToMaster = FALSE;
BOOL g_PetBlockTrade = FALSE;
vector<INT32> g_PetAllowedItems;

TCHAR g_LicenseName[256] = { 0 };

#pragma optimize("", off)

void L2Ext::BaseData()
{
	VIRTUALIZER_TIGER_WHITE_START;
	GetCurrentDirectory(MAX_PATH, g_ConfigFile);
	lstrcat(g_ConfigFile, _T("\\IlExt.ini"));

	g_ServerStartTime = time(0);

	TCHAR l2server_ini[260];
	GetCurrentDirectory(MAX_PATH, l2server_ini);
	lstrcat(l2server_ini, L"\\l2server.ini");

	const WCHAR altUserName[] = { L'A', L'L', L'T', L'U', L'S', L'E', L'R', L'N', L'A', L'M', L'E', 0 };

	GetEnvironmentVariable(altUserName, g_LicenseName, 255);

	License::CLicense::GetInstance().Init();

	UINT npcPort = GetPrivateProfileInt(L"Setting", L"NpcServerPort", 2002, l2server_ini);
	g_HookManager.WriteMemoryDWORD(0x643A63, npcPort);

	g_Config.ReadData();
	//ShowMessage("Offset", "Offset addr[%p]", offsetof(CreatureSharedData, CreatureSharedData::OfflineShop));
	g_PetBlockTrade = GetPrivateProfileInt(_T("L2Server"), _T("PetBlockTrade"), FALSE, g_ConfigFile);
	g_PetBlockGiveAllItemsToMaster = GetPrivateProfileInt(_T("L2Server"), _T("PetBlockGiveAllItemsToMaster"), FALSE, g_ConfigFile);
	g_SaveAbnormalsIntervalTime = GetPrivateProfileInt(_T("L2Server"), _T("SaveAbnormalsIntervalTime"), 0, g_ConfigFile);
	g_ShutdownTime = GetPrivateProfileInt(_T("L2Server"), _T("ShutdownTime"), 30, g_ConfigFile);
	g_HookManager.WriteMemoryDWORD(0x6270AE, g_ShutdownTime);

	TCHAR temp[8190] = { 0 };
	if(GetPrivateProfileString(_T("L2Server"), _T("PetAllowedItems"), 0, temp, 8190, g_ConfigFile))
	{
		wstringstream itemStream;
		itemStream << temp;
		int itemId = 0;
		while(itemStream >> itemId)
		{
			g_PetAllowedItems.push_back(itemId);
		}
	}
	g_GlobalChat = GetPrivateProfileInt(_T("L2Server"), _T("GlobalChat"), FALSE, g_ConfigFile);
	g_ChatShoutLevel = GetPrivateProfileInt(_T("L2Server"), _T("ChatShoutLevel"), 0, g_ConfigFile);
	g_PacketLog = GetPrivateProfileInt(_T("L2Server"), _T("PacketLog"), FALSE, g_ConfigFile);
	g_ItemPacketReuse = GetPrivateProfileInt(_T("L2Server"), _T("ItemPacketReuse"), 800, g_ConfigFile);
	g_MaxValidateLocationDiff = GetPrivateProfileDouble(_T("L2Server"), _T("MaxValidateLocationDiff"), 0, g_ConfigFile);
	g_BlockSummonExp = GetPrivateProfileInt(_T("L2Server"), _T("BlockSummonExp"), 0, g_ConfigFile);
	g_SpiritShotExtension = GetPrivateProfileInt(_T("L2Server"), _T("SpiritShotExtension"), 0, g_ConfigFile);
	g_ValidateAcquiredSkills = GetPrivateProfileInt(_T("L2Server"), _T("ValidateAcquiredSkills"), 0, g_ConfigFile);
	g_SubStackServer = GetPrivateProfileInt(_T("L2Server"), _T("SubStackServer"), 0, g_ConfigFile);
	g_ShowInventoryOnEnterWorld = GetPrivateProfileInt(_T("L2Server"), _T("ShowInventoryOnEnterWorld"), TRUE, g_ConfigFile);
	g_ValidateStackableItems = GetPrivateProfileInt(_T("L2Server"), _T("ValidateStackableItems"), FALSE, g_ConfigFile);
	g_DeleteInvalidStack = GetPrivateProfileInt(_T("L2Server"), _T("DeleteInvalidStack"), FALSE, g_ConfigFile);
	g_DisablePets = GetPrivateProfileInt(_T("L2Server"), _T("DisablePets"), FALSE, g_ConfigFile);
	g_ShowEnterWorldHtml = GetPrivateProfileInt(_T("L2Server"), _T("ShowEnterWorldHtml"), 0, g_ConfigFile);
	g_MaxPartyLevelRange = GetPrivateProfileInt(_T("L2Server"), _T("MaxPartyLevelRange"), 0, g_ConfigFile);
	g_BlockPartyInvite = GetPrivateProfileInt(_T("L2Server"), _T("BlockPartyInvite"), 0, g_ConfigFile);
	g_CustomAbnormalInfoPacket = GetPrivateProfileInt(_T("L2Server"), _T("CustomAbnormalInfoPacket"), 0, g_ConfigFile);
	g_GetLootFromPet = GetPrivateProfileInt(_T("L2Server"), _T("GetLootFromPet"), 0, g_ConfigFile);
	g_PetExpRate = GetPrivateProfileDouble(_T("L2Server"), _T("PetExpRate"), 1.0, g_ConfigFile);
	INT maxIOThread = GetPrivateProfileInt(_T("L2Server"), _T("MaxIOThread"), 8, g_ConfigFile);
	g_MenuHtml = GetPrivateProfileInt(_T("L2Server"), _T("EnableMenuHtml"), TRUE, g_ConfigFile);

	if(maxIOThread > 8)
		maxIOThread = 8;
	g_HookManager.WriteMemoryDWORD(0x61CFF4, maxIOThread);

	if(GetPrivateProfileInt(_T("L2Server"), _T("DisableSellItemPrice"), 0, g_ConfigFile))
	{
		g_HookManager.NopMemory(0x87A84F, 5);
	}

	VIRTUALIZER_TIGER_WHITE_END;

	MovLogFiles("LinErrBak");
	hMutex = CreateMutex(NULL, false, _T("Global\\IlExt"));
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, _T("Lineage II server is already running!"), _T("Interlude Extension"), MB_OK);
		ExitProcess(-1);
		return;
	}
	
//	CAntiDebugger::Init();
	CAntiExploit::Init();
	CTurboBoost::Init();
	string protocol = g_Config.Protocol.GetProtocolVersionS();
	g_HookManager.WriteMemory(0x9BA9B3, protocol.c_str(), protocol.size());

	//AbnormalStatus packet
	g_HookManager.WriteMemoryBYTE(0x82801F, 40);
	//Mail Report
	g_HookManager.NopMemory(0x5B8F87, 5);
	g_HookManager.NopMemory(0x5B9540, 5);

	//henna id is invalid
	g_HookManager.NopMemory(0x86985B, 5);

	g_HookManager.WriteMemoryBYTE(0x4010C0, 0xC3);	//invalid log func fix
	g_HookManager.NopMemory(0x4010C1, 14);
	
	g_HookManager.WriteJump( 0x6432F6, (PVOID)0x643307, 0);	//win2k8 crash fix

	if(g_Config.IsSet(CConfig::DISABLE_PARTY_LEVEL_GAP))
	{
		g_HookManager.WriteJump( 0x6F7F21, (PVOID)0x6F7F2B, 0);
	}
	//Pvp Flag Time
	g_HookManager.WriteMemoryDWORD(0x846036, g_Config.L2ServerConfig.GetPvpFlagTimeShort());
	g_HookManager.WriteMemoryDWORD(0x84603B, g_Config.L2ServerConfig.GetPvpFlagTimeLong());

	//Timer Dispath fix
	g_HookManager.WriteCall(0x62311A, L2Server::TimerDispatchFix , 0);
	g_HookManager.NopMemory(0x626227, 5);	//call stack
	//Spoil Fix (when chance got over 100 l2server decreases it to lower value (ex 120% -> 20%))
	if(g_Config.L2ServerConfig.GetSpoilFix() == 2)
	{
		g_HookManager.WriteJump(0x7D0962, OldSpoilFix, 1);
	}else if(g_Config.L2ServerConfig.GetSpoilFix() == 1)
	{
		g_HookManager.WriteJump(0x7D0944, (PVOID)0x7D0968, 31);
		g_HookManager.WriteJump(0x7D0972, SpoilFixEx, 24);
		//g_HookManager.WriteJump(0x7D0962, (DWORD)SpoilFix, 1);
	}

	g_HookManager.NopMemory(0x4FAC20, 5);	//SYS msg
	g_HookManager.NopMemory(0x4FAC6E, 5);	//SYS msg
	

	//Seven Sign system message 1263 change
//	int nMsgID = 2443;
//	g_HookManager.WriteMemoryDWORD(0x0084D9E5, nMsgID);
	//Disable health on lvl up
//	g_HookManager.NopMemory(0x515E80, 63);
}

#pragma optimize("", on)

void L2Ext::EtcHook()
{
	//Not active user try to save bla bla useless log
	g_HookManager.NopMemory(0x53A37C, 5);
	//Chrono weapon fix
	g_HookManager.WriteJump( 0x517A8D, ChronoWeaponFix1, 1);
	g_HookManager.WriteJump( 0x8B86A4, ChronoWeaponFix2, 2);

	//Item distribution fix - for herbs/zariche
	g_HookManager.WriteJump( 0x8BF783, ItemDistribution, 0);
}

WCHAR g_ItemHtmlData[30000][260];
bool g_IsRegisteredNpc[40000] = { 0 };
UINT g_SpecialNpcArray[1040000] = { 0 };

void L2Ext::FixNPCnItemID()
{
	//.text:0000000000642EBA mov     r8d, 5200000
	g_HookManager.WriteMemoryDWORD(0x642EBC, sizeof(g_ItemHtmlData));
	//.text:0000000000642EC0 lea     rcx, cs:39A8730h                ; void *
	g_HookManager.WriteRelativeAddress(0x642EC0, 3, g_ItemHtmlData, 0);
	//.text:000000000062CC3F lea     rax, cs:39A8730h
	g_HookManager.WriteRelativeAddress(0x62CC3F, 3, g_ItemHtmlData, 0);
	//.text:00000000004FAFDE lea     r12, cs:39A8730h
	g_HookManager.WriteRelativeAddress(0x4FAFDE, 3, g_ItemHtmlData, 0);

	g_HookManager.WriteMemoryDWORD(0x95F574, 30000);	//new item count
	g_HookManager.WriteMemoryDWORD(0xBF71C4, 30000);	//Max loaded items

	g_HookManager.WriteMemoryDWORD(0x95F5AA, 1000000);
	g_HookManager.WriteMemoryDWORD(0x95F5A4, 1040000);

	//reallocate register array
	//.text:00000000004659A7 lea     r14, cs:7D557F0h
	g_HookManager.WriteMemoryDWORD(0x4659AA, ((UINT)&g_IsRegisteredNpc[0] - 0x4659AE));
	//.text:0000000000465AF0 cmp     r13d, 1015000
	g_HookManager.WriteMemoryDWORD(0x465AF3, 1040000);
	//.text:00000000006BFF44 lea     rbx, cs:7D557F1h
	g_HookManager.WriteMemoryDWORD(0x6BFF47, ((UINT)&g_IsRegisteredNpc[1] - 0x6BFF4B));
	//.text:00000000006BFFBD lea     rsi, cs:7D557F1h
	g_HookManager.WriteMemoryDWORD(0x6BFFC0, ((UINT)&g_IsRegisteredNpc[1] - 0x6BFFC4));
	//.text:00000000006FCB26 mov     byte ptr [rax+r15+79557F0h], 1
	g_HookManager.WriteMemoryDWORD(0x6FCB2A, ((UINT)&g_IsRegisteredNpc[0] - 0x400000));
	//.text:000000000046569E cmp     byte ptr [rax+r14+79557F0h], 0
	g_HookManager.WriteMemoryDWORD(0x4656A2, ((UINT)&g_IsRegisteredNpc[0] - 0x400000));

	//teleport_to_npc
	//.text:0000000000453DBC cmp     eax, 15000
	g_HookManager.WriteMemoryDWORD(0x453DBD, 1040000);
	//.text:0000000000453DC5 mov     ecx, [rsi+rax*4+7959290h]
	g_HookManager.WriteMemoryDWORD(0x453DC8, ((UINT)&g_SpecialNpcArray[0] - 0x400000));

	//CNPC::OnCreate
	//.text:00000000006C890A mov     [rsi+rcx*4+7959290h], eax
	g_HookManager.WriteMemoryDWORD(0x6C890D, ((UINT)&g_SpecialNpcArray[0] - 0x400000));
	//CNPC::Delete
	//.text:00000000006C8CAC mov     [rbp+rax*4+7959290h], edi
	g_HookManager.WriteMemoryDWORD(0x6C8CAF, ((UINT)&g_SpecialNpcArray[0] - 0x400000));
}