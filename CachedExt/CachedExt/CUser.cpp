#include "CUser.h"

__declspec(naked) void CUser::SaveMemo()
{
	__asm
	{
		mov		eax, 49E560h
		jmp		eax
	}
}

__declspec(naked) void CUser::SetQuest(__int32 c1)
{
	__asm
	{
		mov		eax, 4A8300h
		jmp		eax
	}
}

__declspec(naked) wchar_t* CUser::GetCharName()
{
	__asm
	{
		mov		eax, 4A00A0h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetId()
{
	__asm
	{
		mov		eax, 49D850h
		jmp		eax
	}
}

__declspec(naked) wchar_t* CUser::GetAccountName()
{
	__asm
	{
		mov		eax, 4A0140h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetAccountID()
{
	__asm
	{
		mov		eax, 4A01E0h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetPledgeID()
{
	__asm
	{
		mov		eax, 4A0280h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetBuilder()
{
	__asm
	{
		mov		eax, 4A0320h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetGender()
{
	__asm
	{
		mov		eax, 4A03C0h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetRace()
{
	__asm
	{
		mov		eax, 4A0460h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetClass()
{
	__asm
	{
		mov		eax, 4A0500h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetWorld()
{
	__asm
	{
		mov		eax, 4A05A0h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetX()
{
	__asm
	{
		mov		eax, 4A0640h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetY()
{
	__asm
	{
		mov		eax, 4A06E0h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetZ()
{
	__asm
	{
		mov		eax, 4A0780h
		jmp		eax
	}
}

__declspec(naked) double CUser::GetHP()
{
	__asm
	{
		mov		eax, 4A0820h
		jmp		eax
	}
}

__declspec(naked) double CUser::GetMP()
{
	__asm
	{
		mov		eax, 4A08C0h
		jmp		eax
	}
}

__declspec(naked) double CUser::GetCP()
{
	__asm
	{
		mov		eax, 4A0960h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetSP()
{
	__asm
	{
		mov		eax, 4A0A00h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetExp()
{
	__asm
	{
		mov		eax, 4A0AA0h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetLevel()
{
	__asm
	{
		mov		eax, 4A0B40h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetAlign()
{
	__asm
	{
		mov		eax, 4A0BE0h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetPK()
{
	__asm
	{
		mov		eax, 4A0C80h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetDuel()
{
	__asm
	{
		mov		eax, 4A0D20h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetPKPardon()
{
	__asm
	{
		mov		eax, 4A0DC0h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetUnderware()
{
	__asm
	{
		mov		eax, 49EA00h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetRight_ear()
{
	__asm
	{
		mov		eax, 49EAA0h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetLeft_ear()
{
	__asm
	{
		mov		eax, 49EB40h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetNeck()
{
	__asm
	{
		mov		eax, 49EBE0h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetRight_finger()
{
	__asm
	{
		mov		eax, 49EC80h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetLeft_finger()
{
	__asm
	{
		mov		eax, 49ED20h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetHead()
{
	__asm
	{
		mov		eax, 49EDC0h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetRight_hand()
{
	__asm
	{
		mov		eax, 49EE60h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetLeft_hand()
{
	__asm
	{
		mov		eax, 49EF00h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetGloves()
{
	__asm
	{
		mov		eax, 49EFA0h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetChest()
{
	__asm
	{
		mov		eax, 49F040h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetLegs()
{
	__asm
	{
		mov		eax, 49F0E0h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetFeet()
{
	__asm
	{
		mov		eax, 49F180h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetBack()
{
	__asm
	{
		mov		eax, 49F220h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetBoth_hand()
{
	__asm
	{
		mov		eax, 49F2C0h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetHair()
{
	__asm
	{
		mov		eax, 49F360h
		jmp		eax
	}
}

__declspec(naked) void CUser::WriteLock(const wchar_t* file, int line)
{
	__asm
	{
		mov		eax, 4A16C0h
		jmp		eax
	}
}

__declspec(naked) void CUser::WriteUnlock()
{
	__asm
	{
		mov		eax, 4A1780h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetExpDiff()
{
	__asm
	{
		mov		eax, 4A2B40h
		jmp		eax
	}
}

__declspec(naked) int CUser::GetLevelByExp(int exp)
{
	__asm
	{
		mov		eax, 4A13A0h
		jmp		eax
	}
}

// Broken, returns 0
// CWarehouse* CUser::GetWarehouse()
// {
// // 	.text:00491CD2 push    1
// // 	.text:00491CD4 mov     [esp+104h+var_C0], eax
// // 	.text:00491CD8 push    edi
// // 	.text:00491CD9 lea     eax, [esp+108h+Warehouse]
// // 	.text:00491CDD push    eax
// // 	.text:00491CDE mov     ecx, esi
// // 	.text:00491CE0 call    CUser__GetWarehouse
// // 	.text:00491CE5 mov     ecx, [esp+100h+Warehouse]
// 	u32 nWarehousePTR = 0;
// 	__asm
// 	{
// 		push 1						// unknown
// 		push 0						// unknown
// 		lea eax, [nWarehousePTR]	// store wh ptr to
// 		push eax
// 		mov ecx, this				// CUser
// 		mov eax, 4A9D30h			// CUser::GetWarehouse
// 		call eax
// 	}
// 	return (CWarehouse*)nWarehousePTR;
// }
