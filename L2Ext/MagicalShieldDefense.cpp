#include "StdAfx.h"

void CMagicalShieldDefense::Initialize()
{
	//Hook skill effests at CCreature::GotDamageBy
	/* i_m_attack::instant:
	.text:00000000007A933C mov     rax, [rax+5F8h]
	.text:00000000007A9343 call    rax
	*/
}
double CMagicalShieldDefense::Calculate(CCreature *pCreature, CCreature *pAttacker, double Damage)
{
	if(pCreature->ValidCreature() && pAttacker->ValidCreature())
	{
		bool bShieldBlock = false;
		bShieldBlock = pCreature->IsShieldDefend(pAttacker, 1);
		if(bShieldBlock)
		{
			//Check for 1 dmg block
			double dRnd = g_MTRandom.RandDouble(1000);
			double dShieldPwr = pCreature->shieldDefencePower;
			dShieldPwr /= 100;
			dShieldPwr *= 15;
			if(dShieldPwr > dRnd)
			{
				//perfect block
				Damage = 1;
				//Send sys msg
				if(pCreature->ValidUser())
				{
					pCreature->GetUser()->GetSocket()->SendSystemMessage(1281);
				}
			}else
			{
				Damage /= 2;
				//send sys msg
				if(pCreature->ValidUser())
				{
					pCreature->GetUser()->GetSocket()->SendSystemMessage(111);
				}
			}
		}
	}
	return Damage;
}