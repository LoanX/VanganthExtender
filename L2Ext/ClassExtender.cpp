#include "StdAfx.h"
#include "ObjectDB.h"

void ClassExtender::Initialize()
{
	ExtendCItem();
	ExtendCWeapon();
	ExtendCEtcItem();
	ExtendCArmor();
	ExtendCAsset();
	ExtendCAccessary();
	ExtendCQuestItem();
	g_HookManager.WriteCall(0x6FAD8E, CopyItem, 4);
}

CItem *ClassExtender::CopyItem(CItem *pItem)
{
	CItem *pReturn = NULL;
	if(pItem)
	{
		INT64 nAddr = *(INT64*)(*(INT64*)pItem + 0xC0);
		typedef CItem* (*_vtt)(CItem*);
		_vtt vtt = (_vtt) nAddr;
		pReturn = vtt(pItem);
		if(pReturn)
		{
			if(CItemDataEx *pData = g_ItemDBEx.GetItemDataEx(pReturn->pSID->nItemID))
			{
				pReturn->nManaLeft = pData->GetShadowTime();
			}
			UINT LifeTime = g_ItemDBEx.GetLifeTime(pReturn->pSID->nItemID);
			if(LifeTime > 0)
			{
				pReturn->nLifeTime = time(NULL) + LifeTime;
			}
			if(pReturn->IsValidItem() && pItem->IsValidItem())
			{
				pReturn->nAugmentationID = pItem->nAugmentationID;
			}
			pReturn->nProtectionTimeout = 0;
		}
	}
	return pReturn;
}
CItem *ClassExtender::CreateItemObject(INT64 nAddr, int nSize, BYTE bOpt, PWCHAR wszName)
{
	CItem *pItem = (CItem*)CObject::CreateObject(nSize, bOpt, wszName);
	if(pItem)
	{
		try
		{
			pItem->nAugmentationID = 0;
			pItem->nManaLeft = 0;
			pItem->nLifeTime = 0;
			pItem->nProtectionTimeout = 0;
		}
		catch(...)
		{
			g_Log.Add(CLog::Error,"[%s] Access Violation."__FUNCTION__);
		}
	}
	return pItem;
}

void ClassExtender::ExtendCItem()
{
	g_HookManager.WriteMemoryDWORD(0x62B904, sizeof(CItem)); //CObject *CItem::Copy
	g_HookManager.WriteCall( 0x62B90F, CreateItemObject, 0); // Clearing new variables
	g_HookManager.WriteMemoryDWORD(0x62B940, sizeof(CItem)); //CObject* CItem::Copy
}

void ClassExtender::ExtendCWeapon()
{
	g_HookManager.WriteMemoryBYTE(0x8E8C73, sizeof(CWeapon));	//ItemData Parser
	g_HookManager.WriteCall( 0x630328, CreateItemObject, 0); // Clearing new variables
	g_HookManager.WriteMemoryDWORD(0x630362, sizeof(CWeapon)); //ItemData Parser creating object
	g_HookManager.WriteMemoryDWORD(0x63373F, sizeof(CWeapon)); //CObject *CWeapon::Copy
	g_HookManager.WriteCall( 0x63374A, CreateItemObject, 0); // Clearing new variables
	g_HookManager.WriteMemoryDWORD(0x63377E, sizeof(CWeapon)); //CObject *CWeapon::Copy
}

void ClassExtender::ExtendCEtcItem()
{
	g_HookManager.WriteMemoryBYTE(0x8E8D6F, (sizeof(CEtcItem)-4));	//ItemData Parser
	g_HookManager.WriteCall( 0x630108, CreateItemObject, 0); // Clearing new variables
	g_HookManager.WriteMemoryDWORD(0x630142, sizeof(CEtcItem)); //ItemData Parser creating object
	g_HookManager.WriteMemoryDWORD(0x633243, sizeof(CEtcItem)); //CObject *CEtcItem::Copy
	g_HookManager.WriteCall( 0x63324E, CreateItemObject, 0); // Clearing new variables
	g_HookManager.WriteMemoryDWORD(0x633282, sizeof(CEtcItem)); //CObject *CEtcItem::Copy
}

void ClassExtender::ExtendCAsset()
{
	g_HookManager.WriteMemoryBYTE(0x8E8D30, (sizeof(CAsset)-3));	//ItemData Parser
	g_HookManager.WriteCall( 0x62FF48, CreateItemObject, 0); // Clearing new variables
	g_HookManager.WriteMemoryDWORD(0x62FF82, sizeof(CAsset)); //ItemData Parser creating object
	g_HookManager.WriteMemoryDWORD(0x632D3F, sizeof(CAsset)); //CObject *CAsset::Copy
	g_HookManager.WriteCall( 0x632D4A, CreateItemObject, 0); // Clearing new variables
	g_HookManager.WriteMemoryDWORD(0x632D7E, sizeof(CAsset)); //CObject *CAsset::Copy
}

void ClassExtender::ExtendCArmor()
{
	g_HookManager.WriteMemoryBYTE(0x8E8CF1, (sizeof(CArmor)-2));	//ItemData Parser
	g_HookManager.WriteCall( 0x630028, CreateItemObject, 0); // Clearing new variables
	g_HookManager.WriteMemoryDWORD(0x630062, sizeof(CArmor)); //ItemData Parser creating object
	g_HookManager.WriteMemoryDWORD(0x632FBF, sizeof(CArmor)); //CObject *CArmor::Copy
	g_HookManager.WriteCall( 0x632FCA, CreateItemObject, 0); // Clearing new variables
	g_HookManager.WriteMemoryDWORD(0x632FFE, sizeof(CArmor)); //CObject *CArmor::Copy
}

void ClassExtender::ExtendCAccessary()
{
	g_HookManager.WriteMemoryBYTE(0x8E8CB2, (sizeof(CAccessary)-1));	//ItemData Parser
	g_HookManager.WriteCall( 0x62FE68, CreateItemObject, 0); // Clearing new variables
	g_HookManager.WriteMemoryDWORD(0x62FEA2, sizeof(CAccessary)); //ItemData Parser creating object
	g_HookManager.WriteMemoryDWORD(0x632ABF, sizeof(CAccessary)); //CObject *CAccessary::Copy
	g_HookManager.WriteCall( 0x632ACA, CreateItemObject, 0); // Clearing new variables
	g_HookManager.WriteMemoryDWORD(0x632AFE, sizeof(CAccessary)); //CObject *CAccessary::Copy
}

void ClassExtender::ExtendCQuestItem()
{
	g_HookManager.WriteMemoryBYTE(0x8E8DAF, (sizeof(CQuestItem)-5));	//ItemData Parser
	g_HookManager.WriteCall( 0x630248, CreateItemObject, 0); // Clearing new variables
	g_HookManager.WriteMemoryDWORD(0x630282, sizeof(CQuestItem)); //ItemData Parser creating object
	g_HookManager.WriteMemoryDWORD(0x6334BF, sizeof(CQuestItem)); //CObject *CQuestItem::Copy
	g_HookManager.WriteCall( 0x6334CA, CreateItemObject, 0); // Clearing new variables
	g_HookManager.WriteMemoryDWORD(0x6334FE, sizeof(CQuestItem)); //CObject *CQuestItem::Copy
}