#pragma once


class ClassExtender
{
public:
	static void Initialize();
	static void ExtendCItem();
	static void ExtendCWeapon();
	static void ExtendCEtcItem();
	static void ExtendCAsset();
	static void ExtendCArmor();
	static void ExtendCAccessary();
	static void ExtendCQuestItem();
	static CItem *CreateItemObject(INT64 nAddr, int nSize, BYTE bOpt, PWCHAR wszName);
	static CItem *CopyItem(CItem *pItem);
};