#include "CachedExt.h"

void __declspec(naked) CItemClear()
{
/*
.text:00442A57                 mov     [esi+18h], edx
.text:00442A5A                 mov     byte ptr [esi+40h], 1
.text:00442A5E                 mov     ecx, [ecx+8]
*/
	__asm
	{
		mov [esi+18h], edx
		mov byte ptr[esi+40h], 1
		mov [esi+58h], ebx
		mov [esi+5Ch], ebx
		mov edi, 442A5Eh
		jmp edi
	}
}

void ClassExtender::Initialize()
{
	ExtendCUser();
//	ExtendCItem();
}

void ClassExtender::ExtendCUser()
{
	int newSize = sizeof(CUser);

	// CUserDbHelper::MakeNewUserObject
	// .text:004AF5D3                 push    5A8h
	CachedExt::WriteMemoryDWORD(0x4AF5D4, newSize);
	// .text:004AF60E                 mov     dword_9804A8[eax*4], 5A8h
	CachedExt::WriteMemoryDWORD(0x4AF615, newSize);
}

void ClassExtender::ExtendCItem()
{
	int newSize = sizeof(CItem);

	CachedExt::SetHook(0xE9, 0x442A57, (DWORD)CItemClear, 2);
	// .text:0044267D                 mov     dword_9804A8[eax*4], 58h
	CachedExt::WriteMemoryDWORD(0x442684, newSize);

	// .text:00444352                 push    58h
	CachedExt::WriteMemoryBYTE(0x444353, newSize);

	// .text:004B1E6B                 push    58h
	CachedExt::WriteMemoryBYTE(0x4B1E6C, newSize);

	// .text:004B208E                 push    58h
	CachedExt::WriteMemoryBYTE(0x4B208F, newSize);

	// .text:004B755E                 push    58h
	CachedExt::WriteMemoryBYTE(0x4B755F, newSize);

	// .text:00443B3F                 push    58h
	CachedExt::WriteMemoryBYTE(0x443B40, newSize);

	// .text:00443B78                 mov     dword_9804A8[eax*4], 58h
	CachedExt::WriteMemoryDWORD(0x443B7F, newSize);

	// .text:0045274E                 push    58h
	CachedExt::WriteMemoryBYTE(0x45274F, newSize);

	// .text:0045277B                 mov     dword_9804A8[eax*4], 58h
	CachedExt::WriteMemoryDWORD(0x452782, newSize);

	// .text:004B157D                 push    58h
	CachedExt::WriteMemoryBYTE(0x4B157E, newSize);

	// .text:004B15AA                 mov     dword_9804A8[eax*4], 58h
	CachedExt::WriteMemoryDWORD(0x4B15B1, newSize);
}
