#pragma once

namespace DnN
{
	enum RateType
	{
		RateExp = 0,
		RateSp = 1,
		RateDrop = 2,
		RateSpoil = 3,
		RateAdena = 4,
		RateTypeCount = 5
	};

	enum TimeType
	{
		TimeDay = 0,
		TimeNight = 1,
		TimeTypeCount = 2
	};

	class CDnNSystem
	{
		BOOL m_Enabled;
		double m_Rate[TimeTypeCount][RateTypeCount];
		void LoadINI();
	public:
		CDnNSystem();
		~CDnNSystem();
		void Init();
		double GetRate(RateType type);
		BOOL IsEnabled() { return m_Enabled; };
		void TimerExpired();
		
	};
};

extern DnN::CDnNSystem g_DnNSystem;