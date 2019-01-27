#pragma once

class CItemEnchant
{
public:
	static void Initialize();
	static bool EnchantItem(CItem *pScroll, CItem *pItem, User *pUser);
	static void LoadINI();
	static void CancelEnchant(User *pUser);
	static void CustomEnchantItem(User *pUser, CItem *pItem, int nEnchant);
	static bool StackableEnchantWeapon(CItem *pScroll, CItem *pItem, User *pUser);
	static bool StackableEnchantArmor(CItem *pScroll, CItem *pItem, User *pUser);
};