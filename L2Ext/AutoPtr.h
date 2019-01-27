#pragma once
#include "Log.h"

template<class T>
class CAutoPtr
{
	T *m_Ptr;
public:
	CAutoPtr() : m_Ptr(NULL) {}
	explicit CAutoPtr(T *ptr) : m_Ptr(ptr) {}
	~CAutoPtr()
	{
		delete m_Ptr;
		m_Ptr = NULL;
	}
	T& operator*() const
	{
		if(!m_Ptr)
		{
			g_Log.Add(CLog::LogError, "[%s:%d] extender referencing NULL object!", __FILE__, __LINE__);
			g_Log.StackTrace(CLog::LogError);
		}
		return *m_Ptr;
	}
	T* operator->() const
	{
		if(!m_Ptr)
		{
			g_Log.Add(CLog::LogError, "[%s:%d] extender referencing NULL object!", __FILE__, __LINE__);
			g_Log.StackTrace(CLog::LogError);
		}
		return m_Ptr;
	}
};