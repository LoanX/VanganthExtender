#include "StdAfx.h"
#include "OlympiadField.h"

COlympiadObserverEntry* COlympiadField::GetObserverEntry()
{
	return (COlympiadObserverEntry*)((INT64)this + (INT64)0x88);
}

void COlympiadField::ChangeStatus(int nStatus)
{
	typedef void (__thiscall *__ChangeStatus)(COlympiadField*, int nStatus);
	__ChangeStatus _ChangeStatus = (__ChangeStatus) 0x006FE180;
	_ChangeStatus(this, nStatus);
}
bool COlympiadField::CheckGameExit()
{
	typedef bool (*_cge)(COlympiadField*);
	_cge cge = (_cge) 0x006FEB90;
	return cge(this);
}
bool COlympiadField::SummonUser()
{
	typedef bool (__thiscall *__SummonUser)(COlympiadField*);
	__SummonUser _SummonUser = (__SummonUser) 0x006FE310;
	return _SummonUser(this);
}

void COlympiadField::SetLoser(BYTE cLoser)
{
	// 1- First player 2- Second player 3- Both
	INT64 dLoser = (INT64)this;
	dLoser += 0x34;
	*(BYTE*)dLoser = cLoser;
}

void COlympiadField::StatusFinishMatch()                                                                                                                                                        //0x701A10
{
        typedef void (*SFM)(COlympiadField *);
        SFM xSFM = (SFM)(0x00701A10L);
        xSFM(this);
}

void COlympiadField::SendVToPlayers(const char *format, va_list args)                                                                                           //0x6FF8B0
{
        typedef void (*SVP)(COlympiadField *, const char *, va_list);
        SVP xSVP = (SVP)(0x006FF8B0L);
        xSVP(this, format, args);
}

void COlympiadField::Broadcast(const char *format, ...)                                                                                                                 //0x7005E0
{
	va_list va;
	va_start(va, format);
	COlympiadObserverEntry *pOE = this->GetObserverEntry();
	if(pOE)
		pOE->Broadcast(format, va);
	SendVToPlayers(format, va);
	va_end(va);
}
/*
#include "COlympiadField.h"
 
// Hooks
void COlympiadField::TimerExpiredHook(COlympiadField *pField, int id)
{
        // Calling Original Timer Expired Handler
        pField->TimerExpired(id);
}
 
void COlympiadField::Init()                                                                                                                                                                                     //0x6FDEE0
{
        typedef void (*I)(COlympiadField *);
        I xI = (I)(0x006FDEE0L);
        xI(this);
}
 
void COlympiadField::AsyncPlayerDied(int nUserServerId)                                                                                                                         //0x6FE010
{
        typedef void (*APD)(COlympiadField *, int);
        APD xAPD = (APD)(0x006FE010L);
        xAPD(this, nUserServerId);
}
 
bool COlympiadField::SummonUser()                                                                                                                                                                       //0x6FE310
{
        typedef bool (*SU)(COlympiadField *);
        SU xSU = (SU)(0x006FE310L);
        return xSU(this);
}
 
bool COlympiadField::CheckGameExit()                                                                                                                                                            //0x6FEB90
{
        typedef bool (*SU)(COlympiadField *);
        SU xSU = (SU)(0x006FE310L);
        return xSU(this);
}
 
void COlympiadField::SetWinner()                                                                                                                                                                        //0x6FEE20
{
        typedef void (*SW)(COlympiadField *);
        SW xSW = (SW)(0x006FEE20L);
        xSW(this);
}
 
void COlympiadField::StatusProcessMatch()                                                                                                                                                       //0x6FF0E0
{
        typedef void (*SPM)(COlympiadField *);
        SPM xSPM = (SPM)(0x006FF0E0L);
        xSPM(this);
}
 
void COlympiadField::AsyncAddEntry(unsigned char byGameRule, int nPlayer1SID, int nPlayer2SID)                                          //0x6FF240
{
        typedef void (*AAE)(COlympiadField *, unsigned char, int, int);
        AAE xAAE = (AAE)(0x006FF240L);
        xAAE(this, byGameRule, nPlayer1SID, nPlayer2SID);
}
 
void COlympiadField::AsyncEscapePlayer(int nUserServerId)                                                                                                                       //0x6FF680
{
        typedef void (*AEP)(COlympiadField *);
        AEP xAEP = (AEP)(0x006FF680L);
        xAEP(this);
}

 
void COlympiadField::SendToPlayers(const char *format, ...)                                                                                                                     //0x6FFAF0
{
        typedef void (*STP)(COlympiadField *, const char *, ...);
        STP xSTP = (STP)(0x006FFAF0L);
        xSTP(this, format);
}
 
void COlympiadField::StatusPrepareMatch()                                                                                                                                                       //0x700010
{
        typedef void (*SPM)(COlympiadField *);
        SPM xSPM = (SPM)(0x00700010L);
        xSPM(this);
}
 
void COlympiadField::StatusClearMatch()                                                                                                                                                         //0x700340
{
        typedef void (*SCM)(COlympiadField *);
        SCM xSCM = (SCM)(0x00700340L);
        xSCM(this);
}
 
void COlympiadField::Broadcast(const char *format, ...)                                                                                                                 //0x7005E0
{
        typedef void (*B)(COlympiadField *, const char *, ...);
        B xB = (B)(0x007005E0L);
        xB(this, format);
}
 
void COlympiadField::StatusWaitMatch()                                                                                                                                                          //0x700930
{
        typedef void (*SWM)(COlympiadField *);
        SWM xSWM = (SWM)(0x00700930L);
        xSWM(this);
}
 
void COlympiadField::DoFinishJob(int nWinnerSID, SharedNoblessInfo *pPlayer1Info, SharedNoblessInfo *pPlayer2Info)      //0x700AD0
{
        typedef void (*DFJ)(COlympiadField *, int, SharedNoblessInfo *, SharedNoblessInfo *);
        DFJ xDFJ = (DFJ)(0x00700AD0L);
        xDFJ(this, nWinnerSID, pPlayer1Info, pPlayer2Info);
}
 
void COlympiadField::StatusFinishMatch()                                                                                                                                                        //0x701A10
{
        typedef void (*SFM)(COlympiadField *);
        SFM xSFM = (SFM)(0x00701A10L);
        xSFM(this);
}
 
void COlympiadField::AsyncEscapeObserver(int nUserServerId)                                                                                                                     //0x7020F0
{
        typedef void (*AEO)(COlympiadField *, int);
        AEO xAEO = (AEO)(0x007020F0L);
        xAEO(this, nUserServerId);
}
 
void COlympiadField::AsyncUpdateUserStatus(int nUserServerId, int nUpdateType, int nObserverId)                                         //0x702360
{
        typedef void (*AUUS)(COlympiadField *, int, int, int);
        AUUS xAUUS = (AUUS)(0x00702360L);
        xAUUS(this, nUserServerId, nUpdateType, nObserverId);
}
 
void COlympiadField::AsyncUpdateSpelledInfo(int nUserServerId, int nObserverId)                                                                         //0x7026F0
{
        typedef void (*AUSI)(COlympiadField *, int, int);
        AUSI xAUSI = (AUSI)(0x007026F0L);
        xAUSI(this, nUserServerId, nObserverId);
}
 
void COlympiadField::AsyncChangeObserverField(int nObserverId, int nFieldId)                                                                            //0x7029F0
{
        typedef void (*ACOF)(COlympiadField *, int, int);
        ACOF xACOF = (ACOF)(0x007029F0L);
        xACOF(this, nObserverId, nFieldId);
}
 
void COlympiadField::AddEntry(unsigned char byGameRule, int nPlayer1SID, int nPlayer2SID)                                                       //0x702C70
{
        typedef void (*AE)(COlympiadField *, unsigned char, int, int);
        AE xAE = (AE)(0x00702C70L);
        xAE(this, byGameRule, nPlayer1SID, nPlayer2SID);
}
 
void COlympiadField::AddObserver(int nUserServerId)                                                                                                                                     //0x703000
{
        typedef void (*AO)(COlympiadField *, int);
        AO xAO = (AO)(0x00703000L);
        xAO(this, nUserServerId);
}
 
void COlympiadField::StatusStartMatch()                                                                                                                                                         //0x703210
{
        typedef void (*STM)(COlympiadField *);
        STM xSTM = (STM)(0x00703210L);
        xSTM(this);
}
 
void COlympiadField::AsyncAddObserver(int nUserServerId)                                                                                                                        //0x703410
{
        typedef void (*AAO)(COlympiadField *, int);
        AAO xAAO = (AAO)(0x00703410L);
        xAAO(this, nUserServerId);
}
 
void COlympiadField::TimerExpired(int id)                                                                                                                                                       //0x703A10
{
        typedef void (*TE)(COlympiadField *, int);
        TE xTE = (TE)(0x00703A10L);
        xTE(this, id);
}
*/