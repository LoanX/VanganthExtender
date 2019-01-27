#include "StdAfx.h"
#include "Packets.h"

int CPacketFix::MultisellIngredientsFix(PCHAR buff, int bufLen, const char *format, ...)
{
	va_list tag;
	va_start (tag, format);
	//hhdh
	WORD ItemID = va_arg(tag, WORD);
	WORD ItemType = va_arg (tag, WORD);
	DWORD Count = va_arg (tag, DWORD);
	WORD Enchant = va_arg (tag, WORD);
	va_end (tag);
	format = "hhdhdd"; //New IL Format
	DWORD dUnkn = 0;
	DWORD dUnkn2 = 0;
	return Assemble(buff, bufLen, format, ItemID, ItemType, Count, Enchant, dUnkn, dUnkn2);
}
int CPacketFix::MultisellProductsFix(PCHAR buff, int bufLen, const char *format, ...)
{
	va_list tag;
	va_start (tag, format);
	//hdhdh
	WORD ItemID = va_arg(tag, WORD);
	DWORD dDw1 = va_arg(tag, DWORD);
	WORD ItemType = va_arg (tag, WORD);
	DWORD Count = va_arg (tag, DWORD);
	WORD Enchant = va_arg (tag, WORD);
	va_end (tag);
	format = "hdhdhdd"; //New IL Format
	DWORD dUnkn = 0;
	DWORD dUnkn2 = 0;
	return Assemble(buff, bufLen, format, ItemID, dDw1, ItemType, Count, Enchant, dUnkn, dUnkn2);
}
int CPacketFix::MultisellFix(PCHAR buff, int bufLen, const char *format, ...)
{
	va_list tag;
	va_start (tag, format);
	//dchhbb
	DWORD EntryID = va_arg(tag, DWORD);
	BYTE bByte1 = va_arg(tag, BYTE);
	WORD ProductSize = va_arg(tag, WORD);
	WORD IngredientSize = va_arg(tag, WORD);
	DWORD dProdSizeBuff = va_arg(tag, DWORD);
	char *cProdBuff = va_arg(tag, char*);
	DWORD dIngredSizeBuff = va_arg(tag, DWORD);
	char *cIngredBuff = va_arg(tag, char*);
	va_end (tag);
	format = "dddchhbb"; //New IL Format
	DWORD dUnkn = 0;
	DWORD dUnkn2 = 0;
	return Assemble(buff, bufLen, format, EntryID, dUnkn, dUnkn2, bByte1, ProductSize, IngredientSize, dProdSizeBuff,
		cProdBuff, dIngredSizeBuff, cIngredBuff);
}