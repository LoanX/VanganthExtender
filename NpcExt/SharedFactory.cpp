#include "stdafx.h"
#include "SharedFactory.h"


void CSharedFactory::Init()
{
	ExtendSharedCreatureData();
	ReallocateAbnormalArray();
	ReallocateQuestArray();
	ExtendSharedPledgeData();
}

void CSharedFactory::ExtendSharedCreatureData()
{
	DWORD NewSize = sizeof(CSharedCreatureData);

	WriteDWORD(0x42C7C3 , NewSize); //New Operator
	WriteDWORD(0x449DA4 , NewSize);
	WriteDWORD(0x449EC3 , NewSize);
	WriteDWORD(0x47D053 , NewSize);
	WriteDWORD(0x47D085 , NewSize);
	WriteDWORD(0x47D1B8 , NewSize);
	WriteDWORD(0x47D406 , NewSize);
	WriteDWORD(0x47D653 , NewSize);
	WriteDWORD(0x47D6B3 , NewSize);
	WriteDWORD(0x4CB06D , NewSize);
	WriteDWORD(0x4CC2E7 , NewSize);
	WriteDWORD(0x4CC3DC , NewSize);
	WriteDWORD(0x4D820A , NewSize);
	WriteDWORD(0x4ED95B , NewSize);
	WriteDWORD(0x4EE979 , NewSize);
	WriteDWORD(0x4EEE79 , NewSize);
	WriteDWORD(0x4F24AF , NewSize);
	WriteDWORD(0x4F67BB , NewSize);
	WriteDWORD(0x505438 , NewSize);
	WriteDWORD(0x50569D , NewSize);
	WriteDWORD(0x505A37 , NewSize);
	WriteDWORD(0x505C7B , NewSize);
	WriteDWORD(0x505F5B , NewSize);
	WriteDWORD(0x5061C5 , NewSize);
	WriteDWORD(0x50CE53 , NewSize);
	WriteDWORD(0x50E14B , NewSize);
	WriteDWORD(0x50E98B , NewSize);
	WriteDWORD(0x5CC7A6 , NewSize);
	WriteDWORD(0x5CCA26 , NewSize);
	WriteDWORD(0x5CCCA6 , NewSize);
	WriteDWORD(0x5CD02A , NewSize);
	WriteDWORD(0x5CD8A0 , NewSize);
	WriteDWORD(0x5CDBE6 , NewSize);
	WriteDWORD(0x5CE01A , NewSize);
	WriteDWORD(0x5CE2E6 , NewSize);
	WriteDWORD(0x5CE432 , NewSize);
	WriteDWORD(0x5CF347 , NewSize);
	WriteDWORD(0x5CF62A , NewSize);
	WriteDWORD(0x5CF8F0 , NewSize);
	WriteDWORD(0x5CFBB0 , NewSize);
	WriteDWORD(0x5CFE7A , NewSize);
	WriteDWORD(0x5D073A , NewSize);
	WriteDWORD(0x5D138B , NewSize);
	WriteDWORD(0x5D1983 , NewSize);
	WriteDWORD(0x5D264A , NewSize);
	WriteDWORD(0x5D26D0 , NewSize);
	WriteDWORD(0x5D2A48 , NewSize);
	WriteDWORD(0x5D2D0A , NewSize);
	WriteDWORD(0x5D2FDA , NewSize);
	WriteDWORD(0x5D32AA , NewSize);
	WriteDWORD(0x5D676A , NewSize);
	WriteDWORD(0x5D6A47 , NewSize);
	WriteDWORD(0x5D78A0 , NewSize);
	WriteDWORD(0x5D7B30 , NewSize);
	WriteDWORD(0x5D8D1A , NewSize);
	WriteDWORD(0x5D8FDA , NewSize);
	WriteDWORD(0x5D9290 , NewSize);
	WriteDWORD(0x5D954B , NewSize);
	WriteDWORD(0x5D988B , NewSize);
	WriteDWORD(0x5D9B90 , NewSize);
	WriteDWORD(0x5D9E2A , NewSize);
	WriteDWORD(0x5DA0C0 , NewSize);
	WriteDWORD(0x5DA350 , NewSize);
	WriteDWORD(0x5DA5E0 , NewSize);
	WriteDWORD(0x5E33B0 , NewSize);
	WriteDWORD(0x5E33C9 , NewSize);
	WriteDWORD(0x5E360C , NewSize);
	WriteDWORD(0x5E3628 , NewSize);
	WriteDWORD(0x600EA2 , NewSize);
}

void CSharedFactory::ReallocateAbnormalArray()
{
	DWORD NewAddr = offsetof(CSharedCreatureData, CSharedCreatureData::AbnormalTable);
	
	BYTE MaxAbnormals = 40;

	//CreatureSharedData::GetAbnormalLevel(AbnormalStatusSlotType type)
	WriteDWORD(0x53304E, NewAddr);
	WriteBYTE(0x533062, MaxAbnormals);
	WriteDWORD(0x53306E, (NewAddr+4));
}

void CSharedFactory::ReallocateQuestArray()
{
	DWORD index = offsetof(CSharedCreatureData, CSharedCreatureData::Quests);
	BYTE count = 26;


	// 4EB360 - CNPC::GetMemoCount
	// .text:00000000004EB430                 lea     rax, [rbx+1E4h]
	WriteDWORD(0x4EB433, index);
	// .text:00000000004EB437                 lea     ecx, [r9+10h]
	// 48 C7 C1 FF FF FF FF		mov
	WriteBYTE(0x4EB437, 0x48);
	WriteBYTE(0x4EB438, 0xC7);
	WriteBYTE(0x4EB439, 0xC1);
	WriteDWORD(0x4EB43A, count);
	WriteNOP(0x4EB43E, 2);
	// 4EB4C0 - CNPC::HaveMemo
	// .text:00000000004EB5CC                 lea     rax, [rdi+1E4h]
	WriteDWORD(0x4EB5CF, index);
	// .text:00000000004EB5DE                 cmp     rcx, 10h
	WriteBYTE(0x4EB5E1, count);
	// 4EBFD0 - CNPC::GetMemoState
	// .text:00000000004EC105                 lea     rax, [rdi+1E4h]
	WriteDWORD(0x4EC108, index);
	// .text:00000000004EC11D                 cmp     rcx, 10h
	WriteBYTE(0x4EC120, count);
	// .text:00000000004EC12B                 mov     eax, [rdi+rax*8+1E8h]
	WriteDWORD(0x4EC12E, index + 4);
	// 4EC210 - CNPC::GetMemoStateEx
	// .text:00000000004EC34D                 lea     rbx, [r13+1E4h]
	WriteDWORD(0x4EC350, index);
	// .text:00000000004EC39A                 cmp     rdi, 10h
	WriteBYTE(0x4EC39D, count);
	// .text:00000000004EC3AB                 mov     eax, [r13+rax*8+1E8h]
	WriteDWORD(0x4EC3AF, index + 4);
	// .text:00000000004EC3F4                 mov     eax, [r13+rax*8+1ECh]
	WriteDWORD(0x4EC3F8, index + 8);
}

void CSharedFactory::ExtendSharedPledgeData()
{
	//Extending CPledgeSharedData 
	DWORD nSize = sizeof(CSharedPledgeData);
	WriteDWORD(0x42C901, nSize);
	WriteDWORD(0x4C3620, nSize);
	WriteDWORD(0x4F748F, nSize);
	WriteDWORD(0x4F769E, nSize);
	WriteDWORD(0x4FAB33, nSize);
	WriteDWORD(0x4FB170, nSize);
	WriteDWORD(0x4FB909, nSize);
	WriteDWORD(0x501AB9, nSize);
	WriteDWORD(0x505891, nSize);
	WriteDWORD(0x505ECE, nSize);
	WriteDWORD(0x506161, nSize);
	WriteDWORD(0x50671E, nSize);
	WriteDWORD(0x5068A1, nSize);
	WriteDWORD(0x506D8A, nSize);
	WriteDWORD(0x522313, nSize);
	WriteDWORD(0x5244F3, nSize);
	WriteDWORD(0x5D4C8F, nSize);
	WriteDWORD(0x5E3450, nSize);
	WriteDWORD(0x5E3469, nSize);
	WriteDWORD(0x5E36AC, nSize);
	WriteDWORD(0x5E36C8, nSize);
}