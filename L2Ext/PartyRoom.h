#pragma once


class CPartyRoomInfo
{
public:
	virtual void Destructor() {};
	int roomId;	//that's all i need
};

class CPartyRoom : public MemoryObject
{
public:
	CPartyRoomInfo pi;
	static CPartyRoom* FindByMember(User* pUser);
};

typedef CSPointer<CPartyRoom> CPartyRoomSP;