#pragma once

typedef INT64 Native;

class CExecutionArgument
{
public:
	static const INT64 DoNothingArgument = 0x009E6598;
	CExecutionArgument(INT64 p) : pointer(p) {};
	INT64 pointer;
	LPVOID _unkn8;
	LPVOID _unkn10;
	LPVOID _unkn18;
};