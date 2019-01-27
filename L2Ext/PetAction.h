#pragma once


class PetAction
{
public:
	static void Initialize();
	static void PickUpItem(CInventory *pInventory, CItem *pItem, int nChangedType);
	static void TimerExpired(CPet *pPet, int id);
	static void GetItemFromPet(LPVOID lpInstance, User *pUser, CItem *pItem, int count);
	static void GiveItemToPet(LPVOID lpInstance, User *pUser, CItem *pItem, int count);
	static bool LinkPet(LPVOID lpInstance, User *pUser, CPet *pPet);
	static void GiveAllItemToMaster(CPet *pPet);
	static void DespawnPet(User *pUser, bool bByLeaveWorld);
};