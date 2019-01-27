#pragma once

enum DCReasonType
{
	DCReasonNone = 0,
	DCReasonProcessTick = 1,
	DCReasonBOT1 = 2,
	DCReasonBOT2 = 3,
	DCReasonBOT3 = 4,
	DCReasonShadowExe = 5,
	DCReasonLoaderExe = 6,
	DCReasonL2Tower = 7,
	DCReasonL2UI = 8,
	DCReasonL2Walker = 9,
	DCReasonCPTracker = 10,
	DCReasonL2QuickTools = 11,
	DCReasonUoPilot = 12,
	DCReasonAutoIt = 13,
	DCReasonPointMacro = 14,
	DCReasonInfocus = 15,
	DCReasonL2Control = 16,
	DCReasonAdrenaline = 17,
	DCReason4Bot = 18,
	DCReasonShadowH5 = 19,
	DCReasonL2Ext = 20,
	DCReasonCPanel = 21,
	DCReasonClickermann = 22,
	DCExtremeInjector = 23,
	DCReasonAutoHotKey = 24
};

struct ModuleInfo
{
	wstring moduleName;
	UINT moduleBase;
	UINT moduleSize;
};

union ProcessIdentifier
{
	UINT64 globalId;
	struct
	{
		UINT processId;
		UINT startTime;
	}part;
};

class CMemoryProtector
{
	UINT m_EngineSize;
	UINT m_EngineAddressStart;
	UINT m_EngineAddressEnd;
	time_t m_ValidateModulesTimeout;
	UINT m_ScanAddress;
	UINT m_ScanAddressEnd;
	UINT m_ScanTick;
	UINT m_CliExtAddress;
	UINT m_CliExtSize;
	UINT m_ModuleIndex;
	vector<ModuleInfo> m_Modules;
	CSLock m_ValidatedLock;
	vector<ProcessIdentifier> m_ValidatedProcess;
	vector<ProcessIdentifier> m_ValidatedWindows;
public:
	void Init();
	void InitCliExt();
	void SetValidatedProcess(UINT processId);
	bool IsValidatedProcess(UINT processId);
	void SetValidatedWindows(UINT processId);
	bool IsValidatedWindows(UINT processId);
	static LPBYTE FindMemory(LPBYTE lpMemory, UINT memorySize, LPBYTE lpData, UINT dataSize, bool& invalid);
	bool ValidateModules();
	void ValidateRunningApps();
	void ValidateRunningAppsEx();
	void ValidateMemory();
	void ValidateMemoryEx();
	static BOOL CALLBACK EnumerateLoadedModulesProc( PSTR moduleName, ULONG moduleBase, ULONG moduleSize, PVOID userContext );
	static BOOL CALLBACK EnumWindowsCallback(HWND hWnd, LPARAM lParam);
	inline time_t GetValidateModulesTimeout() { return m_ValidateModulesTimeout; };
	inline UINT GetScanTick() { return m_ScanTick; };
};

extern CMemoryProtector g_MemoryProtector;