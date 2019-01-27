#pragma once
#pragma pack( push, 1 )

struct NPacket
{
		union 
		{
			BYTE id;
			int _align;
		};
		int len; //size of data
		char* buff; //pointer to data
};

union HookMemory
{
	struct
	{
		BYTE byte[5];
	}bytes;
	struct
	{
		BYTE type;
		UINT address;
	}data;
};

union UtB
{
	UINT full;
	BYTE part[4];
};

#pragma pack( pop )