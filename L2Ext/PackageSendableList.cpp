#include "StdAfx.h"
#include "Packets.h"

int CPacketFix::PackageSendableList(PCHAR buff, int nBuffLen, const char *format, ...)
{
	//
	va_list tag;
	va_start (tag, format);
/*h*/ WORD ItemType = va_arg(tag, WORD);
/*d*/ UINT ObjectID = va_arg(tag, UINT);
/*d*/ int ItemID = va_arg(tag, int);
/*d*/ int Count = va_arg(tag, int);
/*h*/ WORD ItemType2 = va_arg(tag, WORD);
/*h*/ WORD wUnkn = va_arg(tag, WORD);
/*d*/ int BodyPart = va_arg(tag, int);
/*h*/ WORD Enchant = va_arg(tag, WORD);
/*h*/ WORD wUnkn2 = va_arg(tag, WORD);
/*h*/ WORD wUnkn3 = va_arg(tag, WORD);
/*d*/ UINT DatabaseID = va_arg(tag, UINT);
	va_end (tag);
	CItem *pItem = CObject::GetObjectBySID(ObjectID)->CastItem();
	if(pItem->IsValidItem())
	{
		if(pItem->nAugmentationID || pItem->nManaLeft || pItem->nProtectionTimeout > time(0))
			return 0;
	}
	
	return Assemble(buff, nBuffLen, format, ItemType, ObjectID, ItemID, Count, ItemType2, wUnkn,
		BodyPart, Enchant, wUnkn2, wUnkn3, DatabaseID);
}