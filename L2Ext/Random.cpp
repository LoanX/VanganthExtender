#include "stdafx.h"
#include "Random.h"

CRandom g_Random;

CRandom::CRandom()
{
	UINT base = time(NULL);
	UINT currentTick = GetTickCount();
	for(int n=0;n<32;n++)
	{
		m_seed[n] = base;
		base += currentTick;
	}
}

CRandom::~CRandom()
{

}

UINT CRandom::RandInt(UINT max)
{
	UINT number = 0;
	rand_s(&number);

	double dice = static_cast<double>(number);
	dice /= static_cast<double>(UINT_MAX);
	dice *= static_cast<double>(max);

	return static_cast<UINT>(dice);
}

double CRandom::RandDouble(double max)
{
	UINT number = 0;
	rand_s(&number);

	double dice = static_cast<double>(number);
	dice /= static_cast<double>(UINT_MAX);
	dice *= max;

	return dice;
}

