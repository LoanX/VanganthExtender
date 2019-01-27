#pragma once
#pragma warning(disable:4800)
#include "class.h"

class Config
{
private:
	static Config* pConfig;
	TCHAR File[260];
public:
	enum SYSTEM
	{
		ERA
	};
	Config();
	bool IsSet(SYSTEM System)
	{
		switch(System)
		{
		case ERA:
			return EraSystem;
			break;
		}
		return false;
	};
	void SetSystem(SYSTEM System, bool Value)
	{
		switch(System)
		{
		case ERA:
			EraSystem = Value;
			break;
		}
	}
private:
	bool EraSystem;
public:
	class CEraInfo
	{
		int Duration;
		int Count;
		DWORD LastChange;
		int CurrentEra;
		int UseMoon;
		time_t StartTime;
		wstring sEraNames;
		wstring sMoonStates;
	public:
		int GetUseMoon() { return UseMoon; };
		void SetUseMoon(int Value) { UseMoon = Value; };
		int GetCurrentEra() { return CurrentEra; };
		void SetCurrentEra( int Value ) { CurrentEra = Value; };
		DWORD GetLastChange() { return LastChange; };
		void SetLastChange(DWORD Value) { LastChange = Value; };
		int GetDuration() { return Duration; };
		void SetDuration(DWORD Value) { Duration = Value; };
		int GeCount() { return Count;};
		void SetCount(int Value) { Count = Value; };
		time_t GetStartTime() { return StartTime; };
		wstring GetMoonStates() { return sMoonStates; };
		wstring GetEraNames() { return sEraNames; };
		void Load();
		void Save();
	} EraInfo;
private:
	void ReadData()
	{
		EraInfo.Load();
	}
};

extern Config gConfig;