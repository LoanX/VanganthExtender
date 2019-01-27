#pragma once
#include "class.h"

class CNpcMarker
{
public:
	PWCHAR GetName()
	{
		return (PWCHAR)((INT64)this + (INT64)0x1D0);
	}
	void OnNpcPosEvent(bool bEnabled)
	{
		typedef void (*f)(CNpcMarker*, bool);
		f(0x54D310)(this, bEnabled);
	}
};