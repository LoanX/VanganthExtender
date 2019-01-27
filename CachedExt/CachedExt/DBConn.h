#pragma once

#include <windows.h>
#include <sqlext.h>

class DBConn
{
	/* 000 */ SQLHSTMT handle;
	/* 004 */ __int16 collumnNumber;
	/* 006 */ __int8 unk[0x40A];
	/* 410 */
public:
	DBConn();
	~DBConn();

	void BindS(void* pTargetValue, int nBuffLen); 
	void Bind(void* pTargetValue);
	bool Execute(const wchar_t *cpstmt, ...);
	bool Fetch();
	void ResetParam();
};
