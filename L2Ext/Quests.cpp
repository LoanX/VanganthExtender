#include "StdAfx.h"
#include "DB.h"

void Quests::Initialize()
{
	g_HookManager.WriteCall( 0x46F027, BuilderCmdSetQuestFix, 0);	// setquest
	g_HookManager.WriteCall( 0x44AC86, BuilderCmdSetQuestFix, 0);	// delquest
}

void Quests::BuilderCmdSetQuestFix(LPVOID cdb, User *pUser)
{
	g_DB.SaveQuestInfo(pUser);
	pUser->SendQuestList();
}