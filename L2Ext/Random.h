#pragma once

class CRandom
{
	unsigned int m_seed[32];
public:
	CRandom();
	~CRandom();
	UINT RandInt(UINT max);
	double RandDouble(double max);
};

extern CRandom g_Random;
/*
inline double RandDouble(double max)
{
	double chance = (double)(rand());
	chance /= RAND_MAX;
	chance *= max;

	return chance;
};

inline int RandInt(int max)
{
	double chance = rand();
	chance /= RAND_MAX;
	chance *= max;

	return (int)chance;
};
*/