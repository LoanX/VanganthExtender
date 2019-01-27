#include "CachedExt.h"

wchar_t *new_lin_SetQuest = L"EXEC lin_SetQuest \
%d, %d, %d, %d,  %d, %d, %d, %d,  %d, %d, %d, %d,  %d, %d, %d, %d, \
%d, %d, %d, %d,  %d, %d, %d, %d,  %d, %d, %d, %d,  %d, %d, %d, %d, \
%d, %d, %d, %d,  %d, %d, %d, %d,  %d, %d, %d, %d,  %d, %d, %d, %d, \
%d, %d, %d, %d,  %d, %d, %d, %d,  %d, %d, %d, %d,  %d, %d, %d, %d, \
%d, %d, %d, %d,  %d, %d, %d, %d,  %d, %d, %d, %d,  %d, %d, %d, %d, \
%d, %d, %d, %d,  %d, %d, %d, %d,  %d, %d, %d, %d,  %d, %d, %d, %d, \
%d, %d, %d, %d,  %d, %d, %d, %d, \
%d";

void Quests::Initialize()
{
	CUser* pTest = NULL;
	Quests::MoveQuestData((__int32)&pTest->quests,
		sizeof(pTest->quests) / sizeof(QuestData),
		(int)&Quests::FixSaveMemo);
}


void Quests::MoveQuestData(int index, __int8 maxQuests, int newSaveMemo)
{	
// 0x49E840 CUser::LoadQuestFromDB, 0x525174		EXEC lin_LoadQuest %d
	// .text:0049E855                 sub     esp, 418h
	CachedExt::WriteMemoryDWORD(0x49E857, 0x418 + 0xA0);
	// .text:0049E9E8                 add     esp, 424h
	CachedExt::WriteMemoryDWORD(0x49E9EA, 0x424 + 0xA0);
	// .text:0049E8D3                 lea     esi, [ebx+434h]
	CachedExt::WriteMemoryDWORD(0x49E8D5, index + 4);
	// .text:0049E914                 mov     edi, 10h
	CachedExt::WriteMemoryDWORD(0x49E915, maxQuests);

// 524F20		EXEC lin_SetQuest
	CachedExt::SetHook(0xE9, 0x49E607, newSaveMemo, 0);
	// .text:0049E7EA                 add     esp, 10Ch
	CachedExt::WriteMemoryDWORD(0x49E7EC, (maxQuests*4+3)*4);
	// .text:0049E7DD                 lea     eax, [esp+538h+var_420]
	CachedExt::WriteMemoryDWORD(0x49E7E0, (maxQuests*4+3)*4 + 0xC);
// 0x4AEC70 CUser::CUser
	// .text:004AEEDE                 lea     eax, [ebx+434h]
	CachedExt::WriteMemoryDWORD(0x4AEEE0, index + 4);
	// .text:004AEEE4                 mov     esi, 10h
	CachedExt::WriteMemoryDWORD(0x4AEEE5, maxQuests);

// CUser::SetSelectedQuest
	// .text:004A2482                 lea     edx, [esi+430h]
	CachedExt::WriteMemoryDWORD(0x4A2484, index);
	// .text:004A249A                 cmp     eax, 10h
	CachedExt::WriteMemoryBYTE(0x4A249C, maxQuests);
	// .text:004A24AC                 cmp     eax, 10h
	CachedExt::WriteMemoryBYTE(0x4A24AE, maxQuests);
	// .text:004A24C5                 mov     [eax+esi+434h], ecx
	CachedExt::WriteMemoryDWORD(0x4A24C8, index + 4);

//  CUser::DeleteSelectedQuest
	// .text:004A2593                 lea     ecx, [esi+430h]
	CachedExt::WriteMemoryDWORD(0x4A2595, index);
	// .text:004A25AA                 cmp     eax, 10h
	CachedExt::WriteMemoryBYTE(0x4A25AC, maxQuests);
	// .text:004A25BD                 mov     [eax+esi+434h], ebx
	CachedExt::WriteMemoryDWORD(0x4A25C0, index + 4);

// 0x4A8650 CUser::LoadQuest
	// .text:004A8756                 lea     esi, [ebx+438h]
	CachedExt::WriteMemoryDWORD(0x4A8758, index + 8);
	// .text:004A875C                 mov     ebp, 10h
	CachedExt::WriteMemoryDWORD(0x4A875D, maxQuests);

// 0x4A8300 CUser::SetQuest
	// .text:004A83AC                 lea     edi, [ebp+43Ch]
	CachedExt::WriteMemoryDWORD(0x4A83AE, index + 0x0C);
	// .text:004A84A4                 cmp     eax, 10h
	CachedExt::WriteMemoryBYTE(0x4A84A6, maxQuests);
}


__declspec(naked) void Quests::FixSaveMemo()
{
	__asm
	{
		mov     eax, [esi+0C4h]
		push	eax

		// quest 26
		mov		eax, [esi+5A8h+0Ch+16*25]
		push	eax
		mov		eax, [esi+5A8h+08h+16*25]
		push	eax
		mov		eax, [esi+5A8h+04h+16*25]
		push	eax
		mov		eax, [esi+5A8h+00h+16*25]
		push	eax

		// quest 25
		mov		eax, [esi+5A8h+0Ch+16*24]
		push	eax
		mov		eax, [esi+5A8h+08h+16*24]
		push	eax
		mov		eax, [esi+5A8h+04h+16*24]
		push	eax
		mov		eax, [esi+5A8h+00h+16*24]
		push	eax

		// quest 24
		mov		eax, [esi+5A8h+0Ch+16*23]
		push	eax
		mov		eax, [esi+5A8h+08h+16*23]
		push	eax
		mov		eax, [esi+5A8h+04h+16*23]
		push	eax
		mov		eax, [esi+5A8h+00h+16*23]
		push	eax

		// quest 23
		mov		eax, [esi+5A8h+0Ch+16*22]
		push	eax
		mov		eax, [esi+5A8h+08h+16*22]
		push	eax
		mov		eax, [esi+5A8h+04h+16*22]
		push	eax
		mov		eax, [esi+5A8h+00h+16*22]
		push	eax

		// quest 22
		mov		eax, [esi+5A8h+0Ch+16*21]
		push	eax
		mov		eax, [esi+5A8h+08h+16*21]
		push	eax
		mov		eax, [esi+5A8h+04h+16*21]
		push	eax
		mov		eax, [esi+5A8h+00h+16*21]
		push	eax

		// quest 21
		mov		eax, [esi+5A8h+0Ch+16*20]
		push	eax
		mov		eax, [esi+5A8h+08h+16*20]
		push	eax
		mov		eax, [esi+5A8h+04h+16*20]
		push	eax
		mov		eax, [esi+5A8h+00h+16*20]
		push	eax

		// quest 20
		mov		eax, [esi+5A8h+0Ch+16*19]
		push	eax
		mov		eax, [esi+5A8h+08h+16*19]
		push	eax
		mov		eax, [esi+5A8h+04h+16*19]
		push	eax
		mov		eax, [esi+5A8h+00h+16*19]
		push	eax

		// quest 19
		mov		eax, [esi+5A8h+0Ch+16*18]
		push	eax
		mov		eax, [esi+5A8h+08h+16*18]
		push	eax
		mov		eax, [esi+5A8h+04h+16*18]
		push	eax
		mov		eax, [esi+5A8h+00h+16*18]
		push	eax

		// quest 18
		mov		eax, [esi+5A8h+0Ch+16*17]
		push	eax
		mov		eax, [esi+5A8h+08h+16*17]
		push	eax
		mov		eax, [esi+5A8h+04h+16*17]
		push	eax
		mov		eax, [esi+5A8h+00h+16*17]
		push	eax

		// quest 17
		mov		eax, [esi+5A8h+0Ch+16*16]
		push	eax
		mov		eax, [esi+5A8h+08h+16*16]
		push	eax
		mov		eax, [esi+5A8h+04h+16*16]
		push	eax
		mov		eax, [esi+5A8h+00h+16*16]
		push	eax

		// quest 16
		mov		eax, [esi+5A8h+0Ch+16*15]
		push	eax
		mov		eax, [esi+5A8h+08h+16*15]
		push	eax
		mov		eax, [esi+5A8h+04h+16*15]
		push	eax
		mov		eax, [esi+5A8h+00h+16*15]
		push	eax

		// quest 15
		mov		eax, [esi+5A8h+0Ch+16*14]
		push	eax
		mov		eax, [esi+5A8h+08h+16*14]
		push	eax
		mov		eax, [esi+5A8h+04h+16*14]
		push	eax
		mov		eax, [esi+5A8h+00h+16*14]
		push	eax

		// quest 14
		mov		eax, [esi+5A8h+0Ch+16*13]
		push	eax
		mov		eax, [esi+5A8h+08h+16*13]
		push	eax
		mov		eax, [esi+5A8h+04h+16*13]
		push	eax
		mov		eax, [esi+5A8h+00h+16*13]
		push	eax

		// quest 13
		mov		eax, [esi+5A8h+0Ch+16*12]
		push	eax
		mov		eax, [esi+5A8h+08h+16*12]
		push	eax
		mov		eax, [esi+5A8h+04h+16*12]
		push	eax
		mov		eax, [esi+5A8h+00h+16*12]
		push	eax

		// quest 12
		mov		eax, [esi+5A8h+0Ch+16*11]
		push	eax
		mov		eax, [esi+5A8h+08h+16*11]
		push	eax
		mov		eax, [esi+5A8h+04h+16*11]
		push	eax
		mov		eax, [esi+5A8h+00h+16*11]
		push	eax

		// quest 11
		mov		eax, [esi+5A8h+0Ch+16*10]
		push	eax
		mov		eax, [esi+5A8h+08h+16*10]
		push	eax
		mov		eax, [esi+5A8h+04h+16*10]
		push	eax
		mov		eax, [esi+5A8h+00h+16*10]
		push	eax

		// quest 10
		mov		eax, [esi+5A8h+0Ch+16*9]
		push	eax
		mov		eax, [esi+5A8h+08h+16*9]
		push	eax
		mov		eax, [esi+5A8h+04h+16*9]
		push	eax
		mov		eax, [esi+5A8h+00h+16*9]
		push	eax

		// quest 09
		mov		eax, [esi+5A8h+0Ch+16*8]
		push	eax
		mov		eax, [esi+5A8h+08h+16*8]
		push	eax
		mov		eax, [esi+5A8h+04h+16*8]
		push	eax
		mov		eax, [esi+5A8h+00h+16*8]
		push	eax

		// quest 08
		mov		eax, [esi+5A8h+0Ch+16*7]
		push	eax
		mov		eax, [esi+5A8h+08h+16*7]
		push	eax
		mov		eax, [esi+5A8h+04h+16*7]
		push	eax
		mov		eax, [esi+5A8h+00h+16*7]
		push	eax

		// quest 07
		mov		eax, [esi+5A8h+0Ch+16*6]
		push	eax
		mov		eax, [esi+5A8h+08h+16*6]
		push	eax
		mov		eax, [esi+5A8h+04h+16*6]
		push	eax
		mov		eax, [esi+5A8h+00h+16*6]
		push	eax

		// quest 06
		mov		eax, [esi+5A8h+0Ch+16*5]
		push	eax
		mov		eax, [esi+5A8h+08h+16*5]
		push	eax
		mov		eax, [esi+5A8h+04h+16*5]
		push	eax
		mov		eax, [esi+5A8h+00h+16*5]
		push	eax

		// quest 05
		mov		eax, [esi+5A8h+0Ch+16*4]
		push	eax
		mov		eax, [esi+5A8h+08h+16*4]
		push	eax
		mov		eax, [esi+5A8h+04h+16*4]
		push	eax
		mov		eax, [esi+5A8h+00h+16*4]
		push	eax

		// quest 04
		mov		eax, [esi+5A8h+0Ch+16*3]
		push	eax
		mov		eax, [esi+5A8h+08h+16*3]
		push	eax
		mov		eax, [esi+5A8h+04h+16*3]
		push	eax
		mov		eax, [esi+5A8h+00h+16*3]
		push	eax

		// quest 03
		mov		eax, [esi+5A8h+0Ch+16*2]
		push	eax
		mov		eax, [esi+5A8h+08h+16*2]
		push	eax
		mov		eax, [esi+5A8h+04h+16*2]
		push	eax
		mov		eax, [esi+5A8h+00h+16*2]
		push	eax

		// quest 02
		mov		eax, [esi+5A8h+0Ch+16*1]
		push	eax
		mov		eax, [esi+5A8h+08h+16*1]
		push	eax
		mov		eax, [esi+5A8h+04h+16*1]
		push	eax
		mov		eax, [esi+5A8h+00h+16*1]
		push	eax

		// quest 01
		mov		eax, [esi+5A8h+0Ch+16*0]
		push	eax
		mov		eax, [esi+5A8h+08h+16*0]
		push	eax
		mov		eax, [esi+5A8h+04h+16*0]
		push	eax
		mov		eax, [esi+5A8h+00h+16*0]
		push	eax

		mov		eax, new_lin_SetQuest
		push	eax

		mov		eax, 049E7DDh
		jmp		eax
	}
}
