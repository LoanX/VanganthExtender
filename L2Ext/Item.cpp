#include "StdAfx.h"
#include "Item.h"
#include "Augmentation.h"

bool CItem::IsValidItem()
{
	if(this)
	{
        try
		{ 
			if(    (*(LPDWORD)this) == 0xA07CF8 
				|| (*(LPDWORD)this) == 0xA0B368 
				|| (*(LPDWORD)this) == 0xA0A448 
				|| (*(LPDWORD)this) == 0xA09558
				|| (*(LPDWORD)this) == 0xA09CC8
				|| (*(LPDWORD)this) == 0xA08DC8
				|| (*(LPDWORD)this) == 0xA0ABD8 )
			{
				return true;
			}
		}
        catch(...)
		{ 
			/* Invalid pointer */
			/* Preventing Crash ;) */
		}
	}
	return false;
}
CSharedItemData *CItem::GetSharedData()
{
	if(this)
	{
		try
		{
			int nSlotType = this->pSID->nSlotType;
			return this->pSID;
		}
		catch(...)
		{
			return NULL;
		}
	}
	return NULL;
}
bool CItem::IsLifeStone()
{
	if(this)
	{
		try
		{
			if(this->pSID->nItemID >= 8723 && this->pSID->nItemID <= 8762)
				return true;
		}
		catch(...)
		{
			return false;
		}
	}
	return false;
}
int CItem::GetLifeStoneGrade()
{
	if(this->IsLifeStone())
	{
		int nItemID = this->pSID->nItemID;
		if(nItemID >= 8723 && nItemID <= 8732)
			return CAugmentation::LS_LOW;
		if(nItemID >= 8733 && nItemID <= 8742)
			return CAugmentation::LS_MID;
		if(nItemID >= 8743 && nItemID <= 8752)
			return CAugmentation::LS_HIGH;
		if(nItemID >= 8753 && nItemID <= 8762)
			return CAugmentation::LS_TOP;
	}
	return CAugmentation::NOT_LS;
}
int CItem::GetLifeStoneLevel()
{
	if(this->IsLifeStone())
	{
		int nItemID = this->pSID->nItemID;
		if(nItemID == 8723 || nItemID == 8733 || nItemID == 8743 || nItemID == 8753)
			return 46;
		if(nItemID == 8724 || nItemID == 8734 || nItemID == 8744 || nItemID == 8754)
			return 49;
		if(nItemID == 8725 || nItemID == 8735 || nItemID == 8745 || nItemID == 8755)
			return 52;
		if(nItemID == 8726 || nItemID == 8736 || nItemID == 8746 || nItemID == 8756)
			return 55;
		if(nItemID == 8727 || nItemID == 8737 || nItemID == 8747 || nItemID == 8757)
			return 58;
		if(nItemID == 8728 || nItemID == 8738 || nItemID == 8748 || nItemID == 8758)
			return 61;
		if(nItemID == 8729 || nItemID == 8739 || nItemID == 8749 || nItemID == 8759)
			return 64;
		if(nItemID == 8730 || nItemID == 8740 || nItemID == 8750 || nItemID == 8760)
			return 67;
		if(nItemID == 8731 || nItemID == 8741 || nItemID == 8751 || nItemID == 8761)
			return 70;
		if(nItemID == 8732 || nItemID == 8742 || nItemID == 8752 || nItemID == 8762)
			return 76;
	}
	return -1;
}
CWeapon *CItem::GetWeapon()
{
	if(this)
	{
		try
		{
			if(IsValid(VT_ITEMWEAPON))
			{
				return (CWeapon*)this;
			}
			else
				return NULL;
		}
		catch(...)
		{
			return NULL;
		}
	}
	return NULL;
}

void CItem::Delete()
{
	typedef void (*f)(CItem*);
	f(0x62E180L)(this);
}

void CItem::FindItemSp(CItemSP& itemSp, int itemIndex)
{
	typedef void(*f)(UINT64, CItemSP&, int);
	f(0x4745F0L)(0x0E412130, itemSp, itemIndex);
}

CSPointer<CItem> CItem::FindItemSP(int nItemIndex)
{
	typedef void (*FFindItem)(__int64, CSPointer<CItem>*, int);
	__int64 pItemFactory = 0x0E412130;
	CSPointer<CItem> tmp;
	FFindItem(0x4745F0)(pItemFactory, &tmp, nItemIndex);
	return tmp;
}