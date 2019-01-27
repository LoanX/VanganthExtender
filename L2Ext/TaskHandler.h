#pragma once
#include "Task.h"

typedef list<CTask*> TaskList;

class CTaskHandler : private CThreadSafe
{
	TaskList lData;
public:
	CTaskHandler();
	~CTaskHandler();
	int GetTaskCount();
	void TimerExpired();
	void AddTask(CTask *pTask);
	CTask::ERROR_CODE Handle(CTask *pTask);
	void CreateTask(PCHAR Buff, SOCKET s);
};

extern CTaskHandler g_TaskHandler;