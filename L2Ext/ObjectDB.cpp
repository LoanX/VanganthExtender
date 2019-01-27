#include "stdafx.h"
#include "ObjectDB.h"

CObjectDB g_ObjectDB;

CObjectDB::CObjectDB() : m_RealInstance(0x0BC5D810)
{

}

CObjectDB::~CObjectDB()
{

}

int CObjectDB::GetClassIdFromName(const WCHAR* wName)
{
	typedef int(*f)(UINT64, const WCHAR*);
	return f(0x006FB7F0L)(m_RealInstance, wName);
}

const WCHAR* CObjectDB::GetName(int nClassID)
{
	typedef const WCHAR*(*f)(UINT64, int);
	return f(0x006FA690L)( m_RealInstance, nClassID);
};

CNPC* CObjectDB::CreateNPC(int nClassID)
{
	typedef CNPC*(*f)(UINT64, int);
	return f(0x006FBE90L)(m_RealInstance, nClassID);
};

const WCHAR* CObjectDB::GetAI(int nClassID)
{
	typedef const WCHAR*(*f)(UINT64, int);
	return f(0x6FA420L)(m_RealInstance, nClassID);
}

CObject* CObjectDB::GetTemplateObject(int nClassID)
{
	typedef CObject*(*f)(UINT64, int);
	return f(0x006FA900L)(m_RealInstance, nClassID);
}

CItem* CObjectDB::CreateItem(int nClassID)
{
	typedef CItem*(*f)(UINT64, int);
	return f(0x006FAB60L)(m_RealInstance, nClassID);
}

CSetObject* CObjectDB::GetSetObject(int ClassID)
{
	typedef CSetObject*(*f)(UINT64, int);
	return f(0x6FB0C0L)(m_RealInstance, ClassID);
}