#include "StdAfx.h"
#include "PrivateStore.h"
#include "DB.h"

CPrivateStoreSystem g_PrivateStoreSystem;

void CPrivateStoreSystem::Init()
{
	LoadINI();
}

void CPrivateStoreSystem::LoadINI()
{
	const TCHAR* section = _T("PrivateStore");
	m_MinDistance = GetPrivateProfileDouble(section, _T("MinPrivateStoreDistance"), 0, g_ConfigFile);
	m_MinRequiredLevel = GetPrivateProfileInt(section, _T("MinPrivateStoreLevel"), 0, g_ConfigFile);
	m_StoreInDB = GetPrivateProfileInt(section, _T("StoreInDB"), 0, g_ConfigFile) ? true : false;
	
	TCHAR temp[0x8000] = { 0 };
	if(GetPrivateProfileString(section, _T("PrivateStoreItems"), 0, temp, 0x8000, g_ConfigFile))
	{
		wstring params = Parser::Replace(temp, L'{', L' ');
		params = Parser::Replace(params, L'}', L' ');
		params = Parser::Replace(params, L';', L' ');
		//{item_id;min_price;max_price;max_amount}
		wstringstream paramStream(params);
		StoreItemDef sid = { 0, 0, 0, 0 };
		while(paramStream >> sid.itemId >> sid.minPrice >> sid.maxPrice >> sid.maxAmount)
		{
			m_StoreItems[sid.itemId] = sid;
			sid.itemId = sid.maxAmount = sid.maxPrice = sid.minPrice = 0;
		}
	}

	//;Set price range for crafted recipes - recipe store format is {recipe_id;min_price;max_price}
	if(GetPrivateProfileString(section, _T("PrivateStoreRecipes"), 0, temp, 0x8000, g_ConfigFile))
	{
		wstring params = Parser::Replace(temp, L'{', L' ');
		params = Parser::Replace(params, L'}', L' ');
		params = Parser::Replace(params, L';', L' ');
		//{recipe_id;min_price;max_price}
		wstringstream paramStream(params);
		StoreRecipeDef srd = { 0, 0, 0};
		while(paramStream >> srd.recipeId >> srd.minPrice >> srd.maxPrice)
		{
			m_StoreRecipes[srd.recipeId] = srd;
			srd.recipeId = srd.maxPrice = srd.minPrice = 0;
		}
	}
}

bool CPrivateStoreSystem::IsValidStoreRecipe(int recipeId, int price)
{
	guard;

	bool valid = true;

	map<int, StoreRecipeDef>::iterator it = m_StoreRecipes.find(recipeId);
	if(it != m_StoreRecipes.end())
	{
		StoreRecipeDef& sid = it->second;
		if(price < sid.minPrice || price > sid.maxPrice)
		{
			valid = false;
		}
	}

	unguard;
	return valid;
}

bool CPrivateStoreSystem::IsValidStoreItem(int itemId, int price, int amount, UINT& error)
{
	guard;

	bool valid = true;

	error = 0;

	map<int, StoreItemDef>::iterator it = m_StoreItems.find(itemId);
	if(it != m_StoreItems.end())
	{
		StoreItemDef& sid = it->second;
		if(price >= sid.minPrice && price <= sid.maxPrice)
		{
			if(amount > sid.maxAmount)
			{
				error = 2;
			}
		}else
		{
			error = 1;	//invalid item price
		}
	}

	if(error)
	{
		valid = false;
	}

	unguard;
	return valid;
}

bool CPrivateStoreSystem::GotEnoughAdenaToBuy(User *pUser)
{
	guard;
	if(pUser->ValidUser())
	{
		if(pUser->pSD->nStoreMode == StoreBuy)
		{
			PrivateStoreItemMap* pItems = pUser->GetPrivateStoreBuy();
			if(pItems)
			{
				INT64 itemPrice = 0;
				for(PrivateStoreItemMap::iterator Iter = pItems->begin();Iter!=pItems->end();Iter++)
				{
					itemPrice += (Iter->second->Price * Iter->second->Count);
				}
				if(pUser->inventory.GetAdenaAmount() >= itemPrice)
				{
					unguard;
					return true;
				}
			}
			unguard;
			return false;
		}
	}
	unguard;
	return true;
}

void CPrivateStoreSystem::OnCreateStore(User *pUser)
{
	if(pUser->pSD->nStoreMode == 3 || pUser->pSD->nStoreMode == 5 || pUser->pSD->nStoreMode == 1)
	{
		StoreItemList lData;
		int nStoreType = pUser->pSD->nStoreMode;
		switch(nStoreType)
		{
		case StoreBuy:
			{
				PrivateStoreItemMap* pItems = pUser->GetPrivateStoreBuy();
				if(pItems)
				{
					for(PrivateStoreItemMap::iterator Iter = pItems->begin();Iter!=pItems->end();Iter++)
					{
						CStoreItemData ItemData;
						ItemData.nClassID = Iter->second->ClassID;
						ItemData.nCount = Iter->second->Count;
						ItemData.nPrice = Iter->second->Price;
						ItemData.nEnchant = Iter->second->Enchant;
						ItemData.nObjectSID = Iter->second->ObjectID;
						lData.push_back(ItemData);
					}
				}
				break;
			}
		case StoreSell:
			{
				PrivateStoreItemMap *pItems = pUser->GetPrivateStoreSell();
				if(pItems)
				{
					for(PrivateStoreItemMap::iterator Iter = pItems->begin();Iter!=pItems->end();Iter++)
					{
						CStoreItemData ItemData;
						ItemData.nClassID = Iter->second->ClassID;
						ItemData.nCount = Iter->second->Count;
						ItemData.nPrice = Iter->second->Price;
						ItemData.nEnchant = Iter->second->Enchant;
						ItemData.nObjectSID = Iter->second->ObjectID;
						lData.push_back(ItemData);
					}
				}
				break;
			}
		case StoreRecipe:
			{
				PrivateStoreRecipeMap* plStore = pUser->GetPrivateStoreRecipe();
				if(plStore)
				{
					for(PrivateStoreRecipeMap::iterator Iter=plStore->begin(); Iter!=plStore->end();Iter++)
					{
						CRecipeInfo *pInfo = Iter->second->pInfo;
						if(pInfo)
						{
							CStoreItemData ItemData;
							ItemData.nClassID = pInfo->RecipeID;
							ItemData.nEnchant = pInfo->SuccessRate;
							ItemData.nPrice = Iter->second->Price;
							lData.push_back(ItemData);
						}
					}
				}
				break;
			}
		default:
			{
				//OnDeleteStore(pUser);
				break;
			}
		}
		int nOffline = 0;
		if(pUser->pSD->activateOfflineShopTime)
			nOffline = 1;
		g_DB.RequestSavePrivateStoreInfo(pUser->nDBID, nStoreType, lData, nOffline, 1);
		pUser->pSD->nPrivateStoreTimer = 1;
	}
}

void CPrivateStoreSystem::TimerExpired(User *pUser)
{
	if(pUser->pSD->nStoreMode == 1 || pUser->pSD->nStoreMode == 3 || pUser->pSD->nStoreMode == 5)
	{
		pUser->pSD->nPrivateStoreTimer += 1;
		if(pUser->pSD->nPrivateStoreTimer >= m_CheckTime)
		{
			StoreItemList lData;
			int nStoreType = pUser->pSD->nStoreMode;
			pUser->pSD->nPrivateStoreTimer = 1;
			switch(nStoreType)
			{
			case StoreBuy:
				{
					PrivateStoreItemMap* pItems = pUser->GetPrivateStoreBuy();
					if(pItems)
					{
						for(PrivateStoreItemMap::iterator Iter=pItems->begin();Iter!=pItems->end();Iter++)
						{
							CStoreItemData ItemData;
							ItemData.nClassID = Iter->second->ClassID;
							ItemData.nCount = Iter->second->Count;
							ItemData.nPrice = Iter->second->Price;
							ItemData.nEnchant = Iter->second->Enchant;
							ItemData.nObjectSID = Iter->second->ObjectID;
							lData.push_back(ItemData);
						}
					}
					break;
				}
			case StoreSell:
				{
					PrivateStoreItemMap *pItems = pUser->GetPrivateStoreSell();
					if(pItems)
					{
						for(PrivateStoreItemMap::iterator Iter=pItems->begin();Iter!=pItems->end();Iter++)
						{
							CStoreItemData ItemData;
							ItemData.nClassID = Iter->second->ClassID;
							ItemData.nCount = Iter->second->Count;
							ItemData.nPrice = Iter->second->Price;
							ItemData.nEnchant = Iter->second->Enchant;
							ItemData.nObjectSID = Iter->second->ObjectID;
							lData.push_back(ItemData);
						}
					}
					break;
				}
			case StoreRecipe:
				{
					PrivateStoreRecipeMap* plStore = pUser->GetPrivateStoreRecipe();
					if(plStore)
					{
						for(PrivateStoreRecipeMap::iterator Iter=plStore->begin(); Iter!=plStore->end();Iter++)
						{
							CRecipeInfo *pInfo = Iter->second->pInfo;
							if(pInfo)
							{
								CStoreItemData ItemData;
								ItemData.nClassID = pInfo->RecipeID;
								ItemData.nEnchant = pInfo->SuccessRate;
								ItemData.nPrice = Iter->second->Price;
								lData.push_back(ItemData);
							}
						}
					}
					break;
				}
			default:
				{
					OnDeleteStore(pUser);
					break;
				}
			}
			int nOffline = 0;
			if(pUser->pSD->activateOfflineShopTime)
				nOffline = 1;
			g_DB.RequestSavePrivateStoreInfo(pUser->nDBID, nStoreType, lData, nOffline, 0);
		}
	}else if (pUser->pSD->nPrivateStoreTimer > 0)
	{
		OnDeleteStore(pUser);
		pUser->pSD->nPrivateStoreTimer = 0;
	}
}

void CPrivateStoreSystem::OnDeleteStore(User *pUser)
{
	StoreItemList lData;
	g_DB.RequestSavePrivateStoreInfo(pUser->nDBID, 0, lData, 0, 0);
}