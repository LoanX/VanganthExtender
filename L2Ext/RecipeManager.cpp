#include "stdafx.h"
#include "RecipeManager.h"
#include "License.h"
#include "ObjectDB.h"

using namespace License;

CRecipeManager& CRecipeManager::Get()
{
	static CRecipeManager instance;
	return instance;
}

#pragma optimize("", off)

void CRecipeManager::Init()
{
	CTL;
	m_Enabled = false;
	VIRTUALIZER_TIGER_BLACK_START
	if(CLicense::GetInstance().CanUse(LicenseRecipeManager))
	{
		m_Bonus.clear();
		LoadData();
		if(!m_Bonus.empty())
			m_Enabled = true;
		else
			m_Enabled = false;
		g_Log.Add(CLog::Blue, "[%s] Loaded [%d] bonus recipe(s).", __FUNCTION__, m_Bonus.size());
	}
	VIRTUALIZER_TIGER_BLACK_END
}

void CRecipeManager::LoadData()
{
	CTL;
	VIRTUALIZER_TIGER_BLACK_START
	if(CLicense::GetInstance().CanUse(LicenseRecipeManager))
	{
		LPBYTE lpFile = 0;
		SIZE_T size = ReadFileBinary(L"..\\Script\\Recipe.txt", lpFile);
		if(lpFile)
		{
			wstringstream fileStream((PWCHAR)&lpFile[2]);
			wstring line;
			while(getline(fileStream, line))
			{
				//Format: //*recipe_bonus	recipe_id=775	bonus_rate=50	product_multipler=2	system_message=2621 *//
//*recipe_bonus	recipe_id=90	base_item_rate=96	rare_items={{[slamander_skin_mail_rare];1;4}}	system_message=2621 *//
				line = Parser::Trim(line);	//remove white spaces on the begining ' ' and '\t'
				if(line.find(L"//*recipe_bonus") == 0)
				{
					CRecipeBonusInfo rbi;
					rbi.recipeId = Parser::ParseOptionInt(line, L"recipe_id");
					rbi.baseItemRate = Parser::ParseOptionInt(line, L"base_item_rate");
					rbi.systemMessage = Parser::ParseOptionInt(line, L"system_message");
					//parse items
					wstring itemStr = Parser::ParseOptionString(line, L"rare_items");
					itemStr = Parser::Replace(itemStr, L'{', L' ');
					itemStr = Parser::Replace(itemStr, L'}', L' ');
					itemStr = Parser::Replace(itemStr, L'[', L' ');
					itemStr = Parser::Replace(itemStr, L']', L' ');
					itemStr = Parser::Replace(itemStr, L';', L' ');
					wstringstream itemStream(itemStr);
					wstring itemName;
					CRecipeBonusInfo::ItemRate ir;
					while(itemStream >> itemName >> ir.item.amount >> ir.rate)
					{
						ir.item.itemId = g_ObjectDB.GetClassIdFromName(itemName.c_str());
						if(ir.rate > 0 && ir.item.itemId > 0 && ir.item.amount > 0)
						{
							rbi.items.push_back(ir);
						}
					}					

					if(rbi.recipeId > 0)
					{
						m_Bonus[rbi.recipeId] = rbi;
					}
				}

			}
			delete [] lpFile;
		}
	}
	VIRTUALIZER_TIGER_BLACK_END
}

#pragma optimize("", on)

bool CRecipeManager::OnCraft(User *pUser, INT32 recipeId, ItemReq& item, INT32& systemMessage) const
{
	CTL;
	systemMessage = 0;

	BonusMap::const_iterator iter = m_Bonus.find(recipeId);
	if(iter != m_Bonus.end())
	{
		const CRecipeBonusInfo& rbi = iter->second;
		INT32 dice = g_Random.RandInt(100);
		g_Logger.Add(L"OnCraft User[%s][%d] recipeId[%d] dice[%d] baseItemRate[%d]", pUser->pSD->wszName, pUser->nDBID, recipeId, dice, rbi.baseItemRate);
		INT32 rate = rbi.baseItemRate;
		if(dice > rate)
		{
			for(UINT n=0;n<rbi.items.size();n++)
			{
				const CRecipeBonusInfo::ItemRate& ir = rbi.items[n];
				rate += ir.rate;
				if(dice < rate)
				{
					item = ir.item;
					systemMessage = rbi.systemMessage;
					return true;
				}
			}
		}
	}

	return false;
}