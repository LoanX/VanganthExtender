#pragma once


class CItemDataParser
{
public:
	static void Init();
	static INT64 FindInTable(ObjectFieldType field, PWCHAR wStr);
};