#pragma once

class CSetObject
{
public:
	/* 0000 */      void* _vmt;
	/* 0008 */      int nSetID;
	/* 000C */      int nEquipmentClassId[18];
	/* 0054 */      int nAdditionalSlotType; // additional_slot=[x] in itemdata.txt
	/* 0058 */      double fPDefPerBonus1;
	/* 0060 */      double fMDefPerBonus1;
	/* 0068 */      double fMaxHPBonusDiff1;
	/* 0070 */      double fMaxMPBonusDiff1;
	/* 0078 */      double fBreathDiff1;
	/* 0080 */      double _unk0080;
	/* 0088 */      double fCastSpdPerBonus1;
	/* 0090 */      double fSpeedDiff1;
	/* 0098 */      double fPDefPerBonus2;
	/* 00A0 */      double fMDefPerBonus2;
	/* 00A8 */      double fMaxHPBonusDiff2;
	/* 00B0 */      double fMaxMPBonusDiff2;
	/* 00B8 */      double fBreathDiff2;
	/* 00C0 */      double _unk00C0;
	/* 00C8 */      double fSpeedDiff2;
	/* 00D0 */      double fCastSpdPerBonus2;
	/* 00D8 */      double fPAtkPerBonus1;
	/* 00E0 */      double fMAtkPerBonus1;
	/* 00E8 */      double fHPRegenPerBonus1;
	/* 00F0 */      double fMPRegenPerBonus1;
	/* 00F8 */      double fAvoidRateDiff1;
	/* 0100 */      double fShieldDefRatePerBonus1;
	/* 0108 */      double fWeightPenaltyDiff1;
	/* 0110 */      double fCreature1290hPerBonus1;
	/* 0118 */      double fCreature1240hPerBonus1;
	/* 0120 */      int nStrBonus;
	/* 0124 */      int nConBonus;
	/* 0128 */      int nDexBonus;
	/* 012C */      int nIntBonus;
	/* 0130 */      int nMenBonus;
	/* 0134 */      int nWitBonus;
	/* 0138 */      double fPAtkPerBonus2;
	/* 0140 */      double fMAtkPerBonus2;
	/* 0148 */      double fHPRegenPerBonus2;
	/* 0150 */      double fMPRegenPerBonus2;
	/* 0158 */      double fAvoidRateDiff2;
	/* 0160 */      double fShieldDefRatePerBonus2;
	/* 0168 */      double fWeightPenaltyDiff2;
	/* 0170 */      double fCreature1290hPerBonus2;
	/* 0178 */      double fCreature1240hPerBonus2;
	/* 0180 */      int nStrBonus2; // for when additional slot is equiped
	/* 0184 */      int nConBonus2;
	/* 0188 */      int nDexBonus2;
	/* 018C */      int nIntBonus2;
	/* 0190 */      int nMenBonus2;
	/* 0194 */      int nWitBonus2;
	/* 0198 */      wstring wstrSetSkillName;
	/* 01C0 */      wstring wstrSetEffectSkillName;
	/* 01E8 */      wstring wstrSetAdditionalSkillName;
	/* 0210 */  
};
