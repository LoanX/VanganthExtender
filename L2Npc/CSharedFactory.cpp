#include "L2NPC.h"

void CSharedFactory::Initialize()
{
	ExtendCreatureSharedData();
	ReallocateAbnormalTable();
	ExtendPledgeSharedData();
	ReallocateQuests();
}

void CSharedFactory::ExtendCreatureSharedData()
{
	DWORD NewSize = sizeof(CreatureSharedData);
	WriteMemoryDWORD(0x42C7C3 , NewSize); //New Operator
	WriteMemoryDWORD(0x449DA4 , NewSize);
	WriteMemoryDWORD(0x449EC3 , NewSize);
	WriteMemoryDWORD(0x47D053 , NewSize);
	WriteMemoryDWORD(0x47D085 , NewSize);
	WriteMemoryDWORD(0x47D1B8 , NewSize);
	WriteMemoryDWORD(0x47D406 , NewSize);
	WriteMemoryDWORD(0x47D653 , NewSize);
	WriteMemoryDWORD(0x47D6B3 , NewSize);
	WriteMemoryDWORD(0x4CB06D , NewSize);
	WriteMemoryDWORD(0x4CC2E7 , NewSize);
	WriteMemoryDWORD(0x4CC3DC , NewSize);
	WriteMemoryDWORD(0x4D820A , NewSize);
	WriteMemoryDWORD(0x4ED95B , NewSize);
	WriteMemoryDWORD(0x4EE979 , NewSize);
	WriteMemoryDWORD(0x4EEE79 , NewSize);
	WriteMemoryDWORD(0x4F24AF , NewSize);
	WriteMemoryDWORD(0x4F67BB , NewSize);
	WriteMemoryDWORD(0x505438 , NewSize);
	WriteMemoryDWORD(0x50569D , NewSize);
	WriteMemoryDWORD(0x505A37 , NewSize);
	WriteMemoryDWORD(0x505C7B , NewSize);
	WriteMemoryDWORD(0x505F5B , NewSize);
	WriteMemoryDWORD(0x5061C5 , NewSize);
	WriteMemoryDWORD(0x50CE53 , NewSize);
	WriteMemoryDWORD(0x50E14B , NewSize);
	WriteMemoryDWORD(0x50E98B , NewSize);
	WriteMemoryDWORD(0x5CC7A6 , NewSize);
	WriteMemoryDWORD(0x5CCA26 , NewSize);
	WriteMemoryDWORD(0x5CCCA6 , NewSize);
	WriteMemoryDWORD(0x5CD02A , NewSize);
	WriteMemoryDWORD(0x5CD8A0 , NewSize);
	WriteMemoryDWORD(0x5CDBE6 , NewSize);
	WriteMemoryDWORD(0x5CE01A , NewSize);
	WriteMemoryDWORD(0x5CE2E6 , NewSize);
	WriteMemoryDWORD(0x5CE432 , NewSize);
	WriteMemoryDWORD(0x5CF347 , NewSize);
	WriteMemoryDWORD(0x5CF62A , NewSize);
	WriteMemoryDWORD(0x5CF8F0 , NewSize);
	WriteMemoryDWORD(0x5CFBB0 , NewSize);
	WriteMemoryDWORD(0x5CFE7A , NewSize);
	WriteMemoryDWORD(0x5D073A , NewSize);
	WriteMemoryDWORD(0x5D138B , NewSize);
	WriteMemoryDWORD(0x5D1983 , NewSize);
	WriteMemoryDWORD(0x5D264A , NewSize);
	WriteMemoryDWORD(0x5D26D0 , NewSize);
	WriteMemoryDWORD(0x5D2A48 , NewSize);
	WriteMemoryDWORD(0x5D2D0A , NewSize);
	WriteMemoryDWORD(0x5D2FDA , NewSize);
	WriteMemoryDWORD(0x5D32AA , NewSize);
	WriteMemoryDWORD(0x5D676A , NewSize);
	WriteMemoryDWORD(0x5D6A47 , NewSize);
	WriteMemoryDWORD(0x5D78A0 , NewSize);
	WriteMemoryDWORD(0x5D7B30 , NewSize);
	WriteMemoryDWORD(0x5D8D1A , NewSize);
	WriteMemoryDWORD(0x5D8FDA , NewSize);
	WriteMemoryDWORD(0x5D9290 , NewSize);
	WriteMemoryDWORD(0x5D954B , NewSize);
	WriteMemoryDWORD(0x5D988B , NewSize);
	WriteMemoryDWORD(0x5D9B90 , NewSize);
	WriteMemoryDWORD(0x5D9E2A , NewSize);
	WriteMemoryDWORD(0x5DA0C0 , NewSize);
	WriteMemoryDWORD(0x5DA350 , NewSize);
	WriteMemoryDWORD(0x5DA5E0 , NewSize);
	WriteMemoryDWORD(0x5E33B0 , NewSize);
	WriteMemoryDWORD(0x5E33C9 , NewSize);
	WriteMemoryDWORD(0x5E360C , NewSize);
	WriteMemoryDWORD(0x5E3628 , NewSize);
	WriteMemoryDWORD(0x600EA2 , NewSize);
}

void CSharedFactory::ExtendPledgeSharedData()
{
	//Extending CPledgeSharedData 
	DWORD nSize = sizeof(CPledgeSharedData);
	WriteMemoryDWORD(0x42C901, nSize);
	WriteMemoryDWORD(0x4C3620, nSize);
	WriteMemoryDWORD(0x4F748F, nSize);
	WriteMemoryDWORD(0x4F769E, nSize);
	WriteMemoryDWORD(0x4FAB33, nSize);
	WriteMemoryDWORD(0x4FB170, nSize);
	WriteMemoryDWORD(0x4FB909, nSize);
	WriteMemoryDWORD(0x501AB9, nSize);
	WriteMemoryDWORD(0x505891, nSize);
	WriteMemoryDWORD(0x505ECE, nSize);
	WriteMemoryDWORD(0x506161, nSize);
	WriteMemoryDWORD(0x50671E, nSize);
	WriteMemoryDWORD(0x5068A1, nSize);
	WriteMemoryDWORD(0x506D8A, nSize);
	WriteMemoryDWORD(0x522313, nSize);
	WriteMemoryDWORD(0x5244F3, nSize);
	WriteMemoryDWORD(0x5D4C8F, nSize);
	WriteMemoryDWORD(0x5E3450, nSize);
	WriteMemoryDWORD(0x5E3469, nSize);
	WriteMemoryDWORD(0x5E36AC, nSize);
	WriteMemoryDWORD(0x5E36C8, nSize);
}

void CSharedFactory::ReallocateAbnormalTable()
{
	CreatureSharedData Data;
	DWORD NewAddr = (DWORD)((INT64)&Data.AbnormalTable - (INT64)&Data);
	char MaxAbnormals = 40;

	//Fix CreatureSharedData::GetAbnormalLevel(AbnormalStatusSlotType type)
	WriteMemoryDWORD(0x53304E, NewAddr);
	WriteMemoryBYTE(0x533062, MaxAbnormals);
	WriteMemoryDWORD(0x53306E, (NewAddr+4));
}

void CSharedFactory::ReallocateQuests()
{
	CreatureSharedData test;

	int index = (INT64)&test.Quests - (INT64)&test;
	char count = sizeof(test.Quests) / sizeof(QuestData);


	// 4EB360 - CNPC::GetMemoCount
	// .text:00000000004EB430                 lea     rax, [rbx+1E4h]
	WriteMemoryDWORD(0x4EB433, index);
	// .text:00000000004EB437                 lea     ecx, [r9+10h]
	// 48 C7 C1 FF FF FF FF		mov
	WriteMemoryBYTE(0x4EB437, 0x48);
	WriteMemoryBYTE(0x4EB438, 0xC7);
	WriteMemoryBYTE(0x4EB439, 0xC1);
	WriteMemoryDWORD(0x4EB43A, count);
	WriteMemoryNOP(0x4EB43E, 2);
	// 4EB4C0 - CNPC::HaveMemo
	// .text:00000000004EB5CC                 lea     rax, [rdi+1E4h]
	WriteMemoryDWORD(0x4EB5CF, index);
	// .text:00000000004EB5DE                 cmp     rcx, 10h
	WriteMemoryBYTE(0x4EB5E1, count);
	// 4EBFD0 - CNPC::GetMemoState
	// .text:00000000004EC105                 lea     rax, [rdi+1E4h]
	WriteMemoryDWORD(0x4EC108, index);
	// .text:00000000004EC11D                 cmp     rcx, 10h
	WriteMemoryBYTE(0x4EC120, count);
	// .text:00000000004EC12B                 mov     eax, [rdi+rax*8+1E8h]
	WriteMemoryDWORD(0x4EC12E, index + 4);
	// 4EC210 - CNPC::GetMemoStateEx
	// .text:00000000004EC34D                 lea     rbx, [r13+1E4h]
	WriteMemoryDWORD(0x4EC350, index);
	// .text:00000000004EC39A                 cmp     rdi, 10h
	WriteMemoryBYTE(0x4EC39D, count);
	// .text:00000000004EC3AB                 mov     eax, [r13+rax*8+1E8h]
	WriteMemoryDWORD(0x4EC3AF, index + 4);
	// .text:00000000004EC3F4                 mov     eax, [r13+rax*8+1ECh]
	WriteMemoryDWORD(0x4EC3F8, index + 8);
}

