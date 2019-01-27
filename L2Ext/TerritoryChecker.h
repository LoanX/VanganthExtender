#pragma once
#include "Territory.h"

class CTerritoryChecker
{
	map<int, CTerritory*> mData;
public:
	CTerritoryChecker();
	~CTerritoryChecker();
	void Initialize();
	void ReadData(string sFile);
	void GetTerritories(CCreature *pCreature, TerritoryData& td);
	int IsInTerritory(CCreature *pCreature, TERRITORY_TYPE TerritoryType);
	int IsInTerritory(FVector& Pos, TERRITORY_TYPE TerritoryType);
	void ValidateRideTerritory(CCreature *pCreature);
	CTerritory* GetTerritory(int ID);
};

extern CTerritoryChecker g_TerritoryChecker;