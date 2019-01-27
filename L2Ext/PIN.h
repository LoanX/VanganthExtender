#pragma once

class CPINSystem
{
	enum DigType
	{
		DigCreate = 0,
		DigCreateConfirm = 1,
		DigEnter = 2,
		DigChangeCurrent = 3,
		DigChangeNew = 4,
		DigChangeNewConfirm = 5
	};
	BOOL m_Enabled;
	INT32 m_AttemptLimit;
	INT32 m_BanTime;
	BOOL m_UserCommand;
	UINT m_ForceChangeTime;
	map<UINT, PINData*> m_Pins;	//<charId, PIN>
	CSLock m_Lock;
	void LoadINI();
public:
	CPINSystem();
	~CPINSystem();
	void Init();
	inline BOOL IsUserCommand() { return m_UserCommand; };
	inline BOOL IsEnabled() { return m_Enabled; };
	bool DBLoad(UINT charId, UINT pin, UINT timestamp);
	void UpdatePINData(UINT charId, PINData* lpPd);
	void ResetTempPIN(UINT charId);
	bool CanEnter(CUserSocket *pSocket, bool& kick);
	bool RequestCreatePIN(CUserSocket *pSocket, wstring message = L"", UINT lang = 0);
	bool OnCreatePIN(CUserSocket *pSocket, UINT lang);
	bool RequestCreateConfirmPIN(CUserSocket *pSocket, UINT lang);
	bool OnCreateConfirmPIN(CUserSocket *pSocket, UINT lang);
	bool CreatePINDone(CUserSocket *pSocket, UINT lang);
	bool RequestEnterPIN(CUserSocket *pSocket, wstring message = L"", UINT lang = 0);
	bool OnEnterPIN(CUserSocket *pSocket, UINT lang);
	bool EnterPINDone(CUserSocket *pSocket, UINT lang);
	bool RequestChangePIN(CUserSocket *pSocket, wstring message = L"", UINT lang = 0);
	bool OnChangePIN(CUserSocket *pSocket, UINT lang);
	bool RequestChangeNewPIN(CUserSocket *pSocket, wstring message = L"", UINT lang = 0);
	bool OnChangeNewPIN(CUserSocket *pSocket, UINT lang);
	bool RequestChangeNewConfirmPIN(CUserSocket *pSocket, wstring message = L"", UINT lang = 0);
	bool OnChangeNewConfirmPIN(CUserSocket *pSocket, UINT lang);
	bool ChangePINDone(CUserSocket *pSocket, UINT lang);
	bool RequestHandleBypass(CUserSocket *pSocket, wstring& wTxt);
	bool RequestAddDig(CUserSocket *pSocket, UINT dig, UINT type, UINT lang);
};

extern CPINSystem g_PINSystem;