#include "StdAfx.h"
#include "OlympiadUser.h"
 
bool COlympiadUser::ValidateInventory()
{
        typedef bool (*VI)(COlympiadUser *pThis);
        VI xVI = (VI)(0x00703CE0L);
        return xVI(this);
}
 
bool COlympiadUser::OlympiadStatusReset()
{
        typedef bool (*OSR)(COlympiadUser *pThis);
        OSR xOSR = (OSR)(0x00705CB0L);
        return xOSR(this);
}
 
void COlympiadUser::ChangeObserverField(int nFieldId)
{
        typedef void (*COF)(COlympiadUser *pThis, int FieldId);
        COF xCOF = (COF)(0x00703E40L);
        xCOF(this, nFieldId);
}
 
void COlympiadUser::LeaveParty()
{
        typedef void (*LP)(COlympiadUser *pThis);
        LP xLP = (LP)(0x00704A20L);
        xLP(this);
}
 
void COlympiadUser::ObserverHide()
{
        typedef void (*OH)(COlympiadUser *pThis);
        OH xOH = (OH)(0x00704BD0L);
        xOH(this);
}
 
bool COlympiadUser::SetObserver(int nFieldId, LPVOID FVector)
{
        typedef bool (*SO)(COlympiadUser *pThis, int FieldId, LPVOID pFVector);
        SO xSO = (SO)(0x007056A0L);
        return xSO(this, nFieldId, FVector);
}
 
bool COlympiadUser::RegisterOlympiadEntry()
{
        typedef bool (*ROE)(COlympiadUser*);
        ROE xROE = (ROE)(0x00705950L);
        return xROE(this);
}
 
bool COlympiadUser::WaitingOlympiadPlaying()
{
        typedef bool (*WOP)(COlympiadUser *pThis);
        WOP xWOP = (WOP)(0x00705A70L);
        return xWOP(this);
}
 
bool COlympiadUser::WaitingOlympiadObserving()
{
        typedef bool (*WOO)(COlympiadUser *pThis);
        WOO xWOO = (WOO)(0x00705B90L);
        return xWOO(this);
}
 
COlympiadField *COlympiadUser::GetField()
{
        typedef COlympiadField *(*GF)(COlympiadUser*);
        GF xGF = (GF)(0x00703C00L);
		if(this)
			return xGF(this);
		else
			return NULL;
}
 
void COlympiadUser::EscapeOlympiad()
{
        typedef void (*EO)(COlympiadUser *pThis);
        EO xEO = (EO)(0x00705DE0L);
        xEO(this);
}
 
void COlympiadUser::UnequipForbbidenWeapon()
{
        typedef void (*UFW)(COlympiadUser *pThis);
        UFW xUFW = (UFW)(0x00706CE0L);
        xUFW(this);
}
 
void COlympiadUser::ReadyForMatch()
{
        typedef void (*RFM)(COlympiadUser *pThis);
        RFM xRFM = (RFM)(0x00707620L);
        xRFM(this);
}
 
bool COlympiadUser::SetPlayer(BYTE byPlayerOrder, int nFieldId, LPVOID FVector)
{
        typedef bool (*SP)(COlympiadUser *pThis, BYTE PlayerOrder, int FieldId, LPVOID pFVector);
        SP xSP = (SP)(0x00707CB0L);
        return xSP(this, byPlayerOrder, nFieldId, FVector);
}
 
void COlympiadUser::OnLeaveWorld()
{
    typedef void (*OLW)(COlympiadUser *pThis);
    OLW xOLW = (OLW)(0x0070A770L);
    xOLW(this);
}