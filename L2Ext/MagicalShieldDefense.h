#pragma once


class CMagicalShieldDefense
{
public:
	static void Initialize();
	static double Calculate(CCreature *pCreature, CCreature *pAttacker, double Damage);
};