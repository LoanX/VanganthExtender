#include "stdafx.h"
#include "Job.h"
#include "NetworkHandler.h"
#include "MemoryProtector.h"

CJobParam::CJobParam() : m_Type(JobParamTypeNone)
{
};

CJobParamAdrenalinBot* CJobParam::SafeCastAdrenalinBot()
{
	if(this && m_Type == JobParamTypeAdrenalinBot)
		return reinterpret_cast<CJobParamAdrenalinBot*>(this);
	return 0;
}

CJobParamAdrenalinBot::CJobParamAdrenalinBot()
{
	SetType(JobParamTypeAdrenalinBot);
}

CJob::CJob() : m_Type(JobTypeNone), m_ThreadId(0), m_Thread(0), m_Event(0), m_Param(0), m_Function(0)
{
}


#pragma optimize("", off)

CJob::~CJob()
{
	VIRTUALIZER_START;
	if(m_Param)
	{
		//release params
		switch(m_Param->GetType())
		{
		case JobParamTypeAdrenalinBot:
			{
				delete m_Param->SafeCastAdrenalinBot();
				m_Param = 0;
				break;
			}
		default:
			{
			}
		}
	}
	if(m_Thread)
	{
		CloseHandle(m_Thread);
	}
	if(m_Event)
	{
		CloseHandle(m_Event);
	}
	VIRTUALIZER_END;
}

void CJob::Start()
{
	VIRTUALIZER_START;
	if(m_Function)
	{
		m_Event = CreateEvent(0, TRUE, FALSE, 0);
		m_StartTime = time(0);
		m_Thread = CreateThread(0, 0, m_Function, this, 0, &m_ThreadId);
	}
	VIRTUALIZER_END;
}

void CJob::Terminate()
{
	VIRTUALIZER_START;
	if(m_Thread)
	{
		TerminateThread(m_Thread, 0);
		SetEvent(m_Event);
	}
	VIRTUALIZER_END;
}

DWORD WINAPI DetectAdrenalinBot(PVOID pParams)
{
	VIRTUALIZER_START;

	CJob* pJob = reinterpret_cast<CJob*>(pParams);
	if(CJobParamAdrenalinBot* pData = pJob->GetParam()->SafeCastAdrenalinBot())
	{
		HANDLE hPipe = INVALID_HANDLE_VALUE;
		WCHAR wPipe[256] = { 0 };
		const WCHAR wPipeC[] = { L'\\', L'\\', L'.', L'\\', L'p', L'i', L'p', L'e', L'\\', 0 };

		lstrcat(wPipe, wPipeC);
		lstrcat(wPipe, pData->GetPipeName());
		//const WCHAR wPipe[] = L"\\\\.\\pipe\\CF3614CF075AA0CB";
		hPipe = CreateFile(wPipe, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
		if(hPipe != INVALID_HANDLE_VALUE)
		{
			const WCHAR adrenalin[] = { L'A', L'd', L'r', L'e', L'n', L'a', L'l', L'i', L'n', 0 };
			g_NetworkHandler.SetRequestQuit(DCReasonAdrenaline, adrenalin);
			CloseHandle(hPipe);
		}
	}
	pJob->SetDone();

	VIRTUALIZER_END;
	return 0;
}


#pragma optimize("", on)