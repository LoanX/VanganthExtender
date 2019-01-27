#include "Query.h"
#include "CLog.h"

CQuery::CQuery(wstring query)
{
	m_query = query;
}

void CQuery::Set(wstring query)
{
	m_query = query;
}

void CQuery::Clear()
{
	m_query.clear();
}

bool CQuery::AddParam(double param)
{
	size_t pos = m_query.find(L'?');
	if(pos!=wstring::npos)
	{
		wstringstream paramStream;
		paramStream << param;
		m_query = m_query.replace(pos, 1, paramStream.str());
		return true;
	}else
	{
		g_Log.AddU(CLog::Error, L"[%s] Param error [%f] - query[%s]", __FUNCTIONW__, param, m_query.c_str());
		return false;
	}
}

bool CQuery::AddParam(INT32 param)
{
	size_t pos = m_query.find(L'?');
	if(pos!=wstring::npos)
	{
		wstringstream paramStream;
		paramStream << param;
		m_query = m_query.replace(pos, 1, paramStream.str());
		return true;
	}else
	{
		g_Log.AddU(CLog::Error, L"[%s] Param error [%d] - query[%s]", __FUNCTIONW__, param, m_query.c_str());
		return false;
	}
}

bool CQuery::AddParam(UINT param)
{
	size_t pos = m_query.find(L'?');
	if(pos!=wstring::npos)
	{
		wstringstream paramStream;
		paramStream << param;
		m_query = m_query.replace(pos, 1, paramStream.str());
		return true;
	}else
	{
		g_Log.AddU(CLog::Error, L"[%s] Param error [%d] - query[%s]", __FUNCTIONW__, param, m_query.c_str());
		return false;
	}
}

bool CQuery::AddParam(wstring param)
{
	size_t pos = m_query.find(L'?');
	if(pos!=wstring::npos)
	{
		//remove white space and slashesh
		size_t invalidChar = param.find_first_of(L"'\"-\\/\0\n\t");
		while(invalidChar != wstring::npos)
		{
			param = param.replace(invalidChar, 1, L"N");
			invalidChar = param.find_first_of(L"'\"-\\/\0\n\t", invalidChar);
		}

		m_query = m_query.replace(pos, 1, param);
		return true;
	}else
	{
		g_Log.AddU(CLog::Error, L"[%s] Param error [%s] - query[%s]", __FUNCTIONW__, param.c_str(), m_query.c_str());
		return false;
	}
}

const WCHAR* CQuery::Get()
{
	return m_query.c_str();
}