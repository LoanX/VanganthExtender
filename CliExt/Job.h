#pragma once

enum JobType
{
	JobTypeNone = 0,
	JobTypeAdrenalinBot = 1
};

enum JobParamType
{
	JobParamTypeNone,
	JobParamTypeAdrenalinBot
};

class CJobParamAdrenalinBot;

class CJobParam
{
	JobParamType m_Type;
public:
	CJobParam();
	inline void SetType(JobParamType type) { m_Type = type; };
	inline JobParamType GetType() { return m_Type; };
	CJobParamAdrenalinBot* SafeCastAdrenalinBot();
};

class CJobParamAdrenalinBot : public CJobParam
{
	WCHAR m_PipeName[256];
public:
	CJobParamAdrenalinBot();
	inline void SetPipeName(const WCHAR* wName)
	{
		UINT index = 0;
		while(wName[index] != 0 && index < 255)
		{
			m_PipeName[index] = wName[index++];
		}
		m_PipeName[index] = 0;
	}
	inline const WCHAR* GetPipeName() { return m_PipeName; }
};

class CJob
{
	HANDLE m_Thread;
	DWORD m_ThreadId;
	JobType m_Type;
	CJobParam* m_Param;
	time_t m_StartTime;
	LPTHREAD_START_ROUTINE m_Function;
	HANDLE m_Event;
public:
	CJob();
	virtual ~CJob();
	virtual void Start();
	virtual void Terminate();
	inline void SetParam(CJobParam* pParam) { m_Param = pParam; }
	inline void SetFunction(LPTHREAD_START_ROUTINE function) { m_Function = function; }
	inline CJobParam* GetParam() { return m_Param; }
	inline void SetDone() { SetEvent(m_Event); };
	inline bool IsDone() { if(WAIT_TIMEOUT == WaitForSingleObject(m_Event, 0)) return false; return true; };
};

DWORD WINAPI DetectAdrenalinBot(PVOID pParams);