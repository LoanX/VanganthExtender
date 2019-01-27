#include "StdAfx.h"
#include "Packets.h"

int ItemListFix(PCHAR buf, int bufLen, CItem *pItem, WORD ItemType, DWORD ObjectID, DWORD ItemID, DWORD Count, WORD ItemType2, WORD CustomType1, WORD IsEquiped, DWORD BodyPart, WORD EnchantLevel, WORD CustomType2)
{
	//Old FMT:			 "hdddhhhdhh"
	const char *format = "hdddhhhdhhdd"; //New IL Format
						 
	DWORD AugmentationID = 0;
	DWORD Mana = 0;
	if(pItem->IsValidItem())
	{
		AugmentationID = pItem->nAugmentationID;
		Mana = pItem->nManaLeft / 60;
	}
	return Assemble(buf, bufLen, format, ItemType, ObjectID, ItemID, Count, ItemType2
		,CustomType1, IsEquiped, BodyPart, EnchantLevel, CustomType2, AugmentationID, Mana);
}