#pragma once 
#include "SetObject.h"

class CObjectDB
{
	UINT64 m_RealInstance;
public:
	CObjectDB();
	~CObjectDB();
	const WCHAR* GetName(int nClassID);
	CNPC* CreateNPC(int nClassID);
	const WCHAR* GetAI(int nClassID);
	CObject* GetTemplateObject(int nClassID);
	CItem* CreateItem(int nClassID);
	int GetClassIdFromName(const WCHAR* wName);
	CSetObject* GetSetObject(int ClassID);
};

extern CObjectDB g_ObjectDB;