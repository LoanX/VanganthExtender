#pragma once

struct ObsceneWordData
{
	wstring word;
	BOOL partialSearch;
	BOOL kick;
	BOOL sendWarning;
	INT32 chatBanTime;
	INT32 actionType;	/* 0 - drop packet, 1 - replace chars with * */
	INT32 banAccountTime;
};

class CObsceneFilter
{
	BOOL enabled;
	UINT reloadTimer;
	UINT messageId;
	vector<ObsceneWordData> lWords;

	void LoadData();
public:
	CObsceneFilter();
	~CObsceneFilter();
	void Init();
	void CheckString(User *pUser, WCHAR* wStr, bool& drop, bool& kick);
	void RequestReload();
	void TimerExpired();
};

extern CObsceneFilter g_ObsceneFilter;