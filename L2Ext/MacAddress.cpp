#include "StdAfx.h"

CMacAddress::CMacAddress(void)
{
	memset(p, 0, sizeof(p));
}

CMacAddress::~CMacAddress(void)
{
}

CMacAddress::CMacAddress(std::string sMac)
{
	int np[6];
	sscanf_s(sMac.c_str(), "%x-%x-%x-%x-%x-%x", &np[0], &np[1], &np[2], &np[3], &np[4], &np[5]);
	for(int n = 0; n < 6; n++)
		p[n] = (BYTE)np[n];
}

string CMacAddress::ToString()
{
	char sMac[256]; 
	memset(sMac, 0, sizeof(sMac));
	sprintf_s(sMac, 256, "%x-%x-%x-%x-%x-%x", p[0], p[1], p[2], p[3], p[4], p[5]);
	return sMac;
}

bool CMacAddress::operator ==(const CMacAddress &other) const
{
	for(int n = 0; n < 6; n++)
	{
		if(this->p[n] != other.p[n])
			return false;
	}
	return true;
}

bool CMacAddress::operator !=(const CMacAddress &other) const
{
	return !(*this == other);
}

