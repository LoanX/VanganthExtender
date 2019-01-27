#pragma once


class CIOCriticalSection
{
	/*000*/	CRITICAL_SECTION lock;
	/*028*/	bool Locked;
public:
	inline bool IsLocked() { return Locked; };
	CIOCriticalSection() 
	{
		typedef void (*f)(CIOCriticalSection*);
		f(0x00627E10L)(this);
	}
	~CIOCriticalSection()
	{
		typedef void (*f)(CIOCriticalSection*);
		f(0x00627F10L)(this);
	}
	void Lock(PWCHAR wFile = NULL, int nLine=NULL)
	{
		typedef void (*f)(CIOCriticalSection*, PWCHAR, int);
		f(0x00627D10L)(this, wFile, nLine);
	}
	void Unlock(PWCHAR wFile=NULL, int nLine=NULL)
	{
		typedef void (*f)(CIOCriticalSection*, PWCHAR, int);
		f(0x00628010L)(this, wFile, nLine);
	}
};

typedef CIOCriticalSection* LPCIOCriticalSection;