#pragma once
#include "class.h"

template<class Object> class CSPointer
{
public:
	Object *pObject;
	int nObjectType;
	
	CSPointer()
	{
		pObject = NULL;
		nObjectType = NULL;
	}
	CSPointer(const CSPointer& cspointer)
	{
		typedef void (__thiscall *_f)(CSPointer*, const CSPointer&);
		_f f = (_f) 0x402210;
		f(this, cspointer);
	};	

	CSPointer Set(CSPointer NewPointer)
	{
		typedef CSPointer (__thiscall *_f)(CSPointer*, CSPointer);
		_f f = (_f) 0x443900;
		return f(this, NewPointer);
	};
	CSPointer Set(Object *pObject, int nObjectType)
	{
		typedef CSPointer (__thiscall *_f)(CSPointer*, Object*, int);
		_f f = (_f) 0x44E2A0;
		return f(this, pObject, nObjectType);
	};
	Object *operator -> ()
	{
		typedef Object* (__thiscall * _f)(CSPointer*);
		_f f = (_f) 0x4064F0;
		return f(this);
	};
	~CSPointer()
	{
		typedef void (__thiscall *_f)(CSPointer*);
		_f f = (_f) 0x4072B0;
		f(this);
	};
};

typedef CSPointer<CUser> CUserSP;
typedef CSPointer<CItem> CItemSP;
typedef CSPointer<CWareHouse> CWareHouseSP;