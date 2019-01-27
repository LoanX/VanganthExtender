#pragma once


class CObjectMap
{
public:
	map<int, CObject*> m_Objects;
	CRITICAL_SECTION cs_Lock;
};