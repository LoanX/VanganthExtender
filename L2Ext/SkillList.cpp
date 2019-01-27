#include "StdAfx.h"
#include "Packets.h"

int CPacketFix::SkillListFix(PCHAR buf, int bufLen, const char *format, ...)
{
	va_list tag;
	va_start (tag, format);
	DWORD dPassive = va_arg (tag, DWORD);
	DWORD dLevel = va_arg (tag, DWORD);
	DWORD dSkillID = va_arg (tag, DWORD);
	va_end (tag);
	format = "dddc"; //New C5 format
	BYTE bNull = 0x00;
	return Assemble(buf, bufLen, format, dPassive, dLevel, dSkillID, bNull);
}