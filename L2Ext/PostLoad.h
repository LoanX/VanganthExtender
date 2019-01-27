#pragma once


class CPostLoad
{
public:
	static void Initialize();
	static void InitKey(LPVOID pInstance, CLog::LogType Type, wchar_t* Msg);
	static void FirstLoad(LPVOID fnAddr);
	static void SecondLoad(LPVOID fnAddr);
	static void ThirdLoad(LPVOID pInstance);
	static bool IsHardwareIdHack();
};