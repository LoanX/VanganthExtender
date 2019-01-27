#pragma once

class Quests
{
public:
	static void Initialize();
private:
	static void FixSaveMemo();
	static void MoveQuestData(int index, __int8 maxQuests, int newSaveMemo);
};
