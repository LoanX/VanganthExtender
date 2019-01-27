#pragma once

//60h size
class COlympiadUser
{
	/* 000 */	UINT m_State;	/* 4 - registered , 3 - observer,  1 and 2 - fighting , 0 none*/
	/* 004 */	UINT _align004;
	/* 008 */	CIOCriticalSection m_Lock;
	/* 038 */	UINT m_FieldIndex;
	/* 03C */	UINT _align3C;
	/* 040 */	FVector m_ReturnPos;
	/* 058 */	User *m_pUser;
public:
	bool ValidateInventory();
	bool OlympiadStatusReset();
	void ChangeObserverField(int nFieldId);
	void LeaveParty();
	 
	void ObserverHide();
	bool SetObserver(int nFieldId, LPVOID FVector);
	bool RegisterOlympiadEntry();
	bool WaitingOlympiadPlaying();
	 
	bool WaitingOlympiadObserving();
	 
	COlympiadField *GetField();
	 
	void EscapeOlympiad();
	 
	void UnequipForbbidenWeapon();
	 
	void ReadyForMatch(); 
	bool SetPlayer(BYTE byPlayerOrder, int nFieldId, LPVOID FVector);
	void OnLeaveWorld();

	inline UINT GetState() { return m_State; };
	inline User* GetUser() { return m_pUser; };
	inline BOOL IsInside()
	{
		if(m_State == 1 || m_State == 2)
		{
			return TRUE;
		}
		return FALSE;
	}
};