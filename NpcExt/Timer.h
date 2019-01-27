#pragma once

class CTimer
{
public:
	static void Init();
	static void TimerExpired(LPVOID pInstance, int id);
	static void TimerDispatch(PUINT64 pObject, int id, int param, int paramEx);
	static void TimerDispatchEx(PUINT64 pObject, int id);
};