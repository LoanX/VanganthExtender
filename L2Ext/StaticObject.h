#pragma once

struct StaticObjectData;

class CStaticObject : public CObject
{
public:
	/*030*/ StaticObjectData* pSD; //A8h size
};