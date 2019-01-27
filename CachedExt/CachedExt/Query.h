#pragma once
#include <windows.h>
#include <iostream>
#include <sstream>

using namespace std;

class CQuery
{
	wstring m_query;
public:
	CQuery(wstring query);
	void Set(wstring query);
	bool AddParam(INT32 param);
	bool AddParam(double param);
	bool AddParam(UINT param);
	bool AddParam(wstring param);
	const WCHAR* Get();
	void Clear();
};