#include "DBConn.h"
#include <wchar.h>

DBConn::DBConn()
{
	typedef void (__fastcall *TDBConn) (DBConn*);
	TDBConn _DBConn = (TDBConn)0x00428A80;
	_DBConn(this);
}

DBConn::~DBConn()
{
	typedef void (__fastcall *TDBConn_) (DBConn*);
	TDBConn_ DBConn_ = (TDBConn_)0x00428E00;
	DBConn_(this);
}

void DBConn::BindS(void* pTargetValue, int nBuffLen)
{
	__asm
	{
	push nBuffLen
	push pTargetValue
	mov ecx, this
	mov  eax, 42A4E0h
	call eax
	}
}

void DBConn::Bind(void* pTargetValue)
{
	__asm
	{
	push pTargetValue
	mov ecx, this
	mov  eax, 42A720h
	call eax
	}
}

bool DBConn::Execute(const wchar_t *stmt, ...)
{
	WCHAR buffer[8190];
	memset(buffer, 0, sizeof(buffer));
	va_list va;
	va_start(va, stmt);
	vswprintf_s(buffer, stmt, va);
	va_end(va);
	typedef bool (__cdecl *f) (DBConn* ,wchar_t* );
	return f(0x0042AFF0L)(this, buffer);
}
bool DBConn::Fetch()
{
	typedef bool (__fastcall *TFetch) (DBConn*);
	TFetch _Fetch = (TFetch)0x0042A140;
	return _Fetch(this);
}

void DBConn::ResetParam()
{
	typedef void(__thiscall *f)(DBConn*);
	f(0x42AA20L)(this);
}