#pragma once


struct TeleportData
{
	int x;
	int y;
	int z;
	int price;
	int min_level;
	int max_level;
	int required_item_id;
	int required_item_count;
	int castle_id;
	void Clear()
	{
		x = 0;
		y = 0;
		z = 0;
		price = 0;
		min_level = 0;
		max_level = 0;
		required_item_id = 0;
		required_item_count = 0;
		castle_id = 0;
	}
};

class CTeleportBypass
{
	map<wstring, TeleportData> mData;
public:
	static const int MsgID = 2413;
	CTeleportBypass();
	void Init();
	void ReadData();
	void RequestTeleport(User *pUser, wstring wName);
	void Teleport(User *pUser, wstring wName);
};

extern CTeleportBypass g_TeleportBypass;