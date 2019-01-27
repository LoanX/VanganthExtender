#include "StdAfx.h"

#define IMAGEBASE 0x400000
#define arg_0  0x08
#define arg_8  0x10
#define arg_10 0x18
#define arg_18 0x20

LPINT lpCreatureDataCount = (LPINT)0xBF7170;
CCreatureSharedFactory *g_CreatureSharedFactory = (CCreatureSharedFactory*)0xE4120A0;

void CSharedFactory::Initialize()
{
	ExtendCreatureSharedData();
	ExtendPledgeSharedData();
	ReallocateAbnormalTable();
	ReallocateQuests();
}

CreatureSharedData* CCreatureSharedFactoryAllocateWrapper(CCreatureSharedFactory* pInstance, CCreature *pCreature)
{
	return pInstance->Allocate(pCreature);
}

BOOL CCreatureSharedFactoryFreeWrapper(CCreatureSharedFactory* pInstance, int serverIndex)
{
	return pInstance->Free(serverIndex);
}

void CSharedFactory::ExtendCreatureSharedData()
{
	g_HookManager.WriteCall(0x4D68AA, CCreatureSharedFactoryAllocateWrapper, 0);
	g_HookManager.WriteCall(0x4D7CF8, CCreatureSharedFactoryAllocateWrapper, 0);
	g_HookManager.WriteCall(0x4D5267, CCreatureSharedFactoryFreeWrapper, 0);

	//Extending CSharedData in CSharedFactory
	DWORD NewSize = sizeof(CreatureSharedData);
		//Constructor
	g_HookManager.WriteMemoryDWORD(0x4D2EC9 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x4D2EF7 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x4D2F07 , NewSize);
		//Destructor
	g_HookManager.WriteMemoryDWORD(0x4D335C , NewSize);
	g_HookManager.WriteMemoryDWORD(0x4D3348 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x4D333D , NewSize);
		//Changes in other funcs
	g_HookManager.WriteMemoryDWORD(0x4E149E, NewSize);
	g_HookManager.WriteMemoryDWORD(0x4E60F5 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x611953 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x611985 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x611AB9 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x611C71 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x6D8494 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x6DB6B3 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x7890E7 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x78915F , NewSize);
	g_HookManager.WriteMemoryDWORD(0x7892D0 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x78933D , NewSize);
	g_HookManager.WriteMemoryDWORD(0x7893DE , NewSize);
	g_HookManager.WriteMemoryDWORD(0x7893F5 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x78B14C , NewSize);
	g_HookManager.WriteMemoryDWORD(0x78C14E , NewSize);
	g_HookManager.WriteMemoryDWORD(0x78C93D , NewSize);
	g_HookManager.WriteMemoryDWORD(0x78CA6A , NewSize);
	g_HookManager.WriteMemoryDWORD(0x78CA81 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x841142 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x841B00 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x87959A , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8A04DF , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8A17FD , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8A210D , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8A29B4 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8A3059 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8B1BDC , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8B241A , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8B2C47 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8B36CB , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8B3EF6 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8B4721 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8B4C8C , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8B5566 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8B633D , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8B6C45 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8BDFE9 , NewSize);
	g_HookManager.WriteMemoryDWORD(0x8C0530 , NewSize);
	// .text:000000000078CA89                 mov     dword ptr [rcx+rax-8B8h], 0FFFFFFFFh
	g_HookManager.WriteMemoryDWORD(0x78CA8C, (DWORD)((DWORD)0x58 - NewSize) );
}

void CSharedFactory::ExtendPledgeSharedData()
{
	//Extending CPledgeSharedData
	DWORD dSize = sizeof(CPledgeSharedData);
	g_HookManager.WriteMemoryDWORD(0x762DD9, dSize);
	g_HookManager.WriteMemoryDWORD(0x762E07, dSize);
	g_HookManager.WriteMemoryDWORD(0x762E17, dSize);
	g_HookManager.WriteMemoryDWORD(0x76324D, dSize);
	g_HookManager.WriteMemoryDWORD(0x763258, dSize);
	g_HookManager.WriteMemoryDWORD(0x76326C, dSize);
	g_HookManager.WriteMemoryDWORD(0x78D3CD, dSize);  
	g_HookManager.WriteMemoryDWORD(0x78D4FA, dSize); 
	g_HookManager.WriteMemoryDWORD(0x78D511, dSize); 
	dSize = dSize - 0x124;
	g_HookManager.WriteMemoryDWORD(0x76B73B, dSize); //Clearing new memory size
}

void CSharedFactory::ReallocateAbnormalTable()
{
	DWORD NewAddr = offsetof(CreatureSharedData, CreatureSharedData::AbnormalTable);
	BYTE MaxAbnormals = 40;
	
	//Fix AddAbnormalStatus 
	g_HookManager.WriteMemoryDWORD(0x4D96E7, NewAddr);
	g_HookManager.WriteMemoryBYTE(0x4D9701, MaxAbnormals);
	g_HookManager.WriteMemoryDWORD(0x4D970C, NewAddr);
	g_HookManager.WriteMemoryDWORD(0x4D9714, (NewAddr+4));
	//Fix DeleteAllAbnormalStatus
	g_HookManager.WriteMemoryDWORD(0x4D234F, NewAddr);
	g_HookManager.WriteMemoryBYTE(0x4D2354, MaxAbnormals);
	//Fix DeleteAbnormalStatus
	g_HookManager.WriteMemoryDWORD(0x4D95AE, NewAddr);
	g_HookManager.WriteMemoryBYTE(0x4D95C2, MaxAbnormals);
	g_HookManager.WriteMemoryDWORD(0x4D95CD, NewAddr);
	//Fix InfectedSkillBy
		//AddAbnormal
	g_HookManager.WriteMemoryDWORD(0x5065E7, NewAddr);
	g_HookManager.WriteMemoryBYTE(0x506601, MaxAbnormals);
	g_HookManager.WriteMemoryDWORD(0x50660D, NewAddr);
	g_HookManager.WriteMemoryDWORD(0x506615, (NewAddr+4));
		//Delete Abnormal
	g_HookManager.WriteMemoryDWORD(0x505AD5, NewAddr);
	g_HookManager.WriteMemoryBYTE(0x505AF0, MaxAbnormals);
	g_HookManager.WriteMemoryDWORD(0x505AFC, NewAddr);
	//AddAbnormal2 - Check Amount of buffs
	//g_HookManager.WriteJump( 0x5057FB, (DWORD)CheckBuffAmount, 4);
	//Fix SendAbnormalStatus
	g_HookManager.WriteMemoryBYTE(0x827D54, MaxAbnormals);
	//Fix SpelledInfoChanged 
	g_HookManager.WriteMemoryBYTE(0x850F7B, MaxAbnormals);
	//Fix CCreature::CCreature() - Delete All Abnormals
	g_HookManager.WriteMemoryDWORD(0x4D6C46, NewAddr);
	g_HookManager.WriteMemoryBYTE(0x4D6C4B, MaxAbnormals);
	//Fix CCreature::CCreature(&other) - Delete All Abnormals
	g_HookManager.WriteMemoryDWORD(0x4D817F, NewAddr);
	g_HookManager.WriteMemoryBYTE(0x4D8184, MaxAbnormals);
	//Fix CCreature::DispelByCategory
	g_HookManager.WriteMemoryDWORD(0x5018A3, NewAddr);
	g_HookManager.WriteMemoryBYTE(0x5018B6, MaxAbnormals);
	g_HookManager.WriteMemoryDWORD(0x5018BE, NewAddr);
	//Fix CCreature::DispelAll
	g_HookManager.WriteMemoryDWORD(0x502555, NewAddr);
	g_HookManager.WriteMemoryBYTE(0x50256F, MaxAbnormals);
	g_HookManager.WriteMemoryDWORD(0x502578, NewAddr);
	//Fix CCreature::DispelAllByGM
	g_HookManager.WriteMemoryDWORD(0x502ECD, NewAddr);
	g_HookManager.WriteMemoryBYTE(0x502EE0, MaxAbnormals);
	g_HookManager.WriteMemoryDWORD(0x502EE8, NewAddr);
	//Fix CCreature::DispelByName
	g_HookManager.WriteMemoryDWORD(0x5039B8, NewAddr);
	g_HookManager.WriteMemoryBYTE(0x5039CF, MaxAbnormals);
	g_HookManager.WriteMemoryDWORD(0x5039D8, NewAddr);
	//Fix CCreature::DispelBySlot
	g_HookManager.WriteMemoryDWORD(0x504553, NewAddr);
	g_HookManager.WriteMemoryBYTE(0x504566, MaxAbnormals);
	g_HookManager.WriteMemoryDWORD(0x50456F, NewAddr);
	//Fix User::DeleteYongmaAbnormalStatus
	g_HookManager.WriteMemoryDWORD(0x831964, NewAddr);
	g_HookManager.WriteMemoryBYTE(0x831977, MaxAbnormals);
	g_HookManager.WriteMemoryDWORD(0x831980, NewAddr);
	//Fix User::OnChangeSubjobEnd - Delete All Abnormals
	g_HookManager.WriteMemoryDWORD(0x854191, NewAddr);
	g_HookManager.WriteMemoryBYTE(0x854196, MaxAbnormals);
}

void CSharedFactory::ReallocateQuests()
{
	CreatureSharedData test;
	int count = sizeof(test.Quests) / sizeof(QuestData);
	int index = (int)&test.Quests - (int)&test;
	int countMem = count * 16;
	int countMemSmall = count*8;
	int indexEnd = index + countMem;
	// inline bool CSharedCreatureData::SetJournal(int nQuestID , int nJournal)
	// .text:00000000004298D5                 lea     rax, [rdi+1E4h]
	g_HookManager.WriteMemoryDWORD(0x4298D8, index);
	// .text:00000000004298DC                 lea     rcx, [rdi+2E4h]
	g_HookManager.WriteMemoryDWORD(0x4298DF, indexEnd);
	// inline bool  CSharedCreatureData::SetFlagJournal(int nQuestID , int nFlag) 
	// .text:0000000000429A85                 lea     rdx, [rsi+1E4h]
	g_HookManager.WriteMemoryDWORD(0x429A88, index);
	// .text:0000000000429A8C                 lea     rax, [rsi+2E4h]
	g_HookManager.WriteMemoryDWORD(0x429A8F, indexEnd);
	// inline bool  CSharedCreatureData::ResetFlagJournal(int nQuestID , int nFlag)
	// .text:0000000000429C45                 lea     rdx, [rsi+1E4h]
	g_HookManager.WriteMemoryDWORD(0x429C48, index);
	// .text:0000000000429C4C                 lea     rax, [rsi+2E4h]
	g_HookManager.WriteMemoryDWORD(0x429C4F, indexEnd);
	// inline bool CSharedCreatureData::RemoveMemo(int nQuestID )
	// .text:0000000000429DFE                 lea     rax, [rdi+1E4h]
	g_HookManager.WriteMemoryDWORD(0x429E01, index);
	// .text:0000000000429E05                 lea     rcx, [rdi+2E4h]
	g_HookManager.WriteMemoryDWORD(0x429E08, indexEnd);
	// inline bool  CSharedCreatureData::HaveMemo(int nQuestID)
	// .text:0000000000431C60                 lea     rdx, [rdi+1E4h]
	g_HookManager.WriteMemoryDWORD(0x431C63, index);
	// .text:0000000000431C6A                 lea     r8, [rdi+2E4h]
	g_HookManager.WriteMemoryDWORD(0x431C6D, indexEnd);
	// inline bool  CSharedCreatureData::SetMemoState(int nQuestID , int nSlot, int nState)
	// .text:0000000000431F27                 lea     rbx, [r13+1E4h]
	g_HookManager.WriteMemoryDWORD(0x431F2A, index);
	// .text:0000000000431F31                 lea     rdi, [r13+2E4h]
	g_HookManager.WriteMemoryDWORD(0x431F34, indexEnd);
	// bool CSCharacterInfoPacket(CSocket* pSocket, const PUCHAR packet)
	// .text:000000000047B08A                 mov     eax, [rax+22Ch]
	g_HookManager.WriteMemoryDWORD(0x47B08C, index + 0x48);
	// .text:000000000047B097                 mov     eax, [rcx+228h]
	g_HookManager.WriteMemoryDWORD(0x47B099, index + 0x44);
	// .text:000000000047B0A4                 mov     eax, [rdx+224h]
	g_HookManager.WriteMemoryDWORD(0x47B0A6, index + 0x40);
	
	// .text:000000000047B0AE                 mov     eax, [rdi+21Ch]
	g_HookManager.WriteMemoryDWORD(0x47B0B0, index + 0x38);
	// .text:000000000047B0B8                 mov     eax, [r8+218h]
	g_HookManager.WriteMemoryDWORD(0x47B0BB, index + 0x34);
	// .text:000000000047B0C3                 mov     eax, [r9+214h]
	g_HookManager.WriteMemoryDWORD(0x47B0C6, index + 0x30);
	// .text:000000000047B0CE                 mov     eax, [r10+20Ch]
	g_HookManager.WriteMemoryDWORD(0x47B0D1, index + 0x28);
	// .text:000000000047B0D9                 mov     eax, [r11+208h]
	g_HookManager.WriteMemoryDWORD(0x47B0DC, index + 0x24);
	// .text:000000000047B0E4                 mov     eax, [rbx+204h]
	g_HookManager.WriteMemoryDWORD(0x47B0E6, index + 0x20);
	// .text:000000000047B0EE                 mov     eax, [rsi+1FCh]
	g_HookManager.WriteMemoryDWORD(0x47B0F0, index + 0x18);
	// .text:000000000047B0F8                 mov     eax, [rbp+1F8h]
	g_HookManager.WriteMemoryDWORD(0x47B0FA, index + 0x14);
	// .text:000000000047B102                 mov     eax, [r12+1F4h]
	g_HookManager.WriteMemoryDWORD(0x47B106, index + 0x10);
	// .text:000000000047B10E                 mov     eax, [r13+1ECh]
	g_HookManager.WriteMemoryDWORD(0x47B111, index + 0x08);
	// .text:000000000047B119                 mov     eax, [r14+1E8h]
	g_HookManager.WriteMemoryDWORD(0x47B11C, index + 0x04);
	// .text:000000000047B124                 mov     r9d, [r15+1E4h]
	g_HookManager.WriteMemoryDWORD(0x47B127, index + 0x00);
	// void g_DB.SaveQuestInfo(User *pUser)
	// .text:0000000000538980                 mov     eax, [rbx+r8+1F0h]
	g_HookManager.WriteMemoryDWORD(0x538984, index + 0x0C);
	// .text:000000000053898C                 mov     eax, [rbx+r8+1ECh]
	g_HookManager.WriteMemoryDWORD(0x538990, index + 0x08);
	// .text:0000000000538998                 mov     eax, [rbx+r8+1E8h]
	g_HookManager.WriteMemoryDWORD(0x53899C, index + 0x04);
	// .text:00000000005389A4                 mov     r9d, [rbx+r8+1E4h]
	g_HookManager.WriteMemoryDWORD(0x5389A8, index + 0x00);
	// .text:00000000005389C2                 cmp     rbx, 100h
	g_HookManager.WriteMemoryDWORD(0x5389C5, countMem);
	// static bool LoadQuestPacket(CDBSocket *socket, const PUCHAR packet)
	// .text:0000000000552840                 mov     edi, 1F0h
	g_HookManager.WriteMemoryDWORD(0x552841, index + 0x0C);
	// .text:0000000000552848                 lea     ebp, [rbx+10h]
	g_HookManager.WriteMemoryBYTE(0x552848, 0xBD);
	g_HookManager.WriteMemoryDWORD(0x552849, count);
	g_HookManager.NopMemory(0x55284D, 2);
	// .text:0000000000552857                 lea     rax, [rbx+rdx+1F0h]
	g_HookManager.WriteMemoryDWORD(0x55285B, index + 0x0C);
	// .text:000000000055285F                 lea     rcx, [rbx+rdx+1ECh]
	g_HookManager.WriteMemoryDWORD(0x552863, index + 0x08);
	// .text:0000000000552867                 lea     r9, [rbx+rdx+1E8h]
	g_HookManager.WriteMemoryDWORD(0x55286B, index + 0x04);
	// .text:000000000055286F                 lea     r8, [rbx+rdx+1E4h]
	g_HookManager.WriteMemoryDWORD(0x552873, index + 0x00);
	// CSharedCreatureData::GetMemoState(int nQuestID, int nSlot) 
	// .text:00000000006D04FF                 lea     rax, [rbx+1E4h]
	g_HookManager.WriteMemoryDWORD(0x6D0502, index);
	// .text:00000000006D0506                 lea     rcx, [rbx+2E4h]
	g_HookManager.WriteMemoryDWORD(0x6D0509, indexEnd);
	// void User::SendQuestList()
	// quest list
	// .text:000000000081EBF8                 mov     r9d, [rbx+rax+1E4h]
	g_HookManager.WriteMemoryDWORD(0x81EBFC, index);
	// .text:000000000081EC00                 mov     r8d, [rbx+rax+1F0h]
	g_HookManager.WriteMemoryDWORD(0x81EC04, index+0x0C);
	// .text:000000000081EC0D                 mov     edx, 80h
	g_HookManager.WriteMemoryDWORD(0x81EC0E, countMemSmall);				// increase buffer, which is used for assembled packet with list of quests
	// .text:000000000081EC36                 cmp     rbx, 100h
	g_HookManager.WriteMemoryDWORD(0x81EC39, countMem);
	// move rest of the list
	// .text:000000000081EC43                 mov     [rsp+0DB8h+var_CC8], r15b
	g_HookManager.WriteMemoryDWORD(0x81EC47, 0x70 + countMemSmall);
	// .text:000000000081EC4D                 mov     r8d, 0C7Fh      ; size_t
	g_HookManager.WriteMemoryDWORD(0x81EC4F, 0xCFF - countMemSmall);
	// .text:000000000081EC53                 lea     rcx, [rsp+0DB8h+var_CC7]
	g_HookManager.WriteMemoryDWORD(0x81EC57, 0x71 + countMemSmall);
	// quest items list
	// .text:000000000081EDAB                 mov     edx, 0C80h
	g_HookManager.WriteMemoryDWORD(0x81EDAC, 0xD00-countMemSmall);		// whole original buffer 0xD00 (0xC80 + 0x80)
	// .text:000000000081EDB6                 lea     rcx, [rsp+rax+0DB8h+var_CC8]
	g_HookManager.WriteMemoryDWORD(0x81EDBA, 0x70 + countMemSmall);
	// CSocket->Send
	// .text:000000000081F012                 lea     rdx, [rsp+0DB8h+var_CC8]
	g_HookManager.WriteMemoryDWORD(0x81F016, 0x70 + countMemSmall);
	// .text:000000000081F059                 lea     rcx, [rsp+0DB8h+var_CC8]
	g_HookManager.WriteMemoryDWORD(0x81F05D, 0x70 + countMemSmall);
	// void User::OnChangeSubJobEnd(bool bCreate)
	// .text:0000000000855077                 lea     rax, [rbp+1E4h]
	g_HookManager.WriteMemoryDWORD(0x85507A, index);
	// .text:000000000085507E                 lea     rcx, [rbp+2E4h]
	g_HookManager.WriteMemoryDWORD(0x855081, indexEnd);
}

/*
void CSharedFactory::ReallocatePaperDoll()
{
	CreatureSharedData Data;
	DWORD StartAddr = (DWORD)((INT64)&Data.PaperDoll - (INT64)&Data);
	BYTE Size = (BYTE)ITEM_SLOT_COUNT;
	DWORD EndAddr = (DWORD)((DWORD)(Size*4) + StartAddr);

	static DWORD SlotOffsetTable[ITEM_SLOT_COUNT+4];
	for(int n=0;n<ITEM_SLOT_COUNT;n++)
		SlotOffsetTable[n] = (1 << n);
	PINT64 End = (PINT64)&SlotOffsetTable[ITEM_SLOT_COUNT];
	End[0] = 0;
	End[1] = 0x3FF3333333333333;

	UINT64 SlotOffsetTableStart = (INT64)&SlotOffsetTable[0];
	UINT64 SlotOffsetTableEnd = (INT64)&SlotOffsetTable[ITEM_SLOT_COUNT];
	if(SlotOffsetTableStart > UINT_MAX)
		MessageBox(NULL, "Too big SlotOffsetTableStart! > DWORD", "ERROR!", MB_OK);
	if(SlotOffsetTableEnd > UINT_MAX)
		MessageBox(NULL, "Too big SlotOffsetTableStart! > DWORD", "ERROR!", MB_OK);

//CCreature::CCreature
	//.text:00000000004D6961 mov     ecx, 128h
	g_HookManager.WriteMemoryDWORD(0x4D6962, StartAddr);
	//.text:00000000004D6982 cmp     rcx, 170h
	g_HookManager.WriteMemoryDWORD(0x4D6985, EndAddr);

//CCreature::CCreature(const CCreature& other): CObject(other)
	//.text:00000000004D7DAF mov     ecx, 128h
	g_HookManager.WriteMemoryDWORD(0x4D7DB0, StartAddr);
	//.text:00000000004D7DD2 cmp     rcx, 170h
	g_HookManager.WriteMemoryDWORD(0x4D7DD5, EndAddr);

//bool CCreature::IsEquiped
	//.text:00000000004DA81F add     rax, 128h
	g_HookManager.WriteMemoryDWORD(0x4DA821, StartAddr);
	//.text:00000000004DA830 cmp     rcx, 12h
	g_HookManager.WriteMemoryBYTE(0x4DA833, Size);

//bool CCreature::ValidateBaseData()
	//.text:00000000004DAF8F mov     edx, [rax+rbp*4+128h]
	g_HookManager.WriteMemoryDWORD(0x4DAF92, StartAddr);

//SlotType CCreature::IsEquiped(int nItemServerId, SlotType slot)
	//.text:00000000004E126C mov     r8d, 12h
	g_HookManager.WriteMemoryDWORD(0x4E126E, Size);
	//.text:00000000004E1294 cmp     [rax+150h], ebx
	g_HookManager.WriteMemoryDWORD(0x4E1296, ((0x150 - 0x128) + StartAddr));
	//.text:00000000004E12A7 cmp     [rax+154h], ebx
	g_HookManager.WriteMemoryDWORD(0x4E12A9, ((0x154 - 0x128) + StartAddr));
	//.text:00000000004E12BA cmp     [rax+164h], ebx
	g_HookManager.WriteMemoryDWORD(0x4E12BC, ((0x164 - 0x128) + StartAddr));
	//.text:00000000004E12D8 cmp     [rax+144h], ebx
	g_HookManager.WriteMemoryDWORD(0x4E12DA, ((0x144 - 0x128) + StartAddr));
	//.text:00000000004E12EB cmp     [rax+148h], ebx
	g_HookManager.WriteMemoryDWORD(0x4E12ED, ((0x148 - 0x128) + StartAddr));
	//.text:00000000004E12FB cmp     [rax+160h], ebx
	g_HookManager.WriteMemoryDWORD(0x4E12FD, ((0x160 - 0x128) + StartAddr));
	//.text:00000000004E1312 cmp     [rax+138h], ebx
	g_HookManager.WriteMemoryDWORD(0x4E1314, ((0x138 - 0x128) + StartAddr));
	//.text:00000000004E1322 cmp     [rax+13Ch], ebx
	g_HookManager.WriteMemoryDWORD(0x4E1324, ((0x13C - 0x128) + StartAddr));
	//.text:00000000004E1339 cmp     [rax+12Ch], ebx
	g_HookManager.WriteMemoryDWORD(0x4E133B, ((0x12C - 0x128) + StartAddr));
	//.text:00000000004E1349 cmp     [rax+130h], ebx
	g_HookManager.WriteMemoryDWORD(0x4E134B, ((0x130 - 0x128) + StartAddr));
	//.text:00000000004E1360 cmp     [rcx+rsi*4+128h], ebx
	g_HookManager.WriteMemoryDWORD(0x4E1363, StartAddr);

//double CCreature::sub_4E3180() - something with equip penalty
	//.text:00000000004E31A4 mov     ebx, 128h
	g_HookManager.WriteMemoryDWORD(0x4E31A5, StartAddr);
	//.text:00000000004E3213 cmp     rbx, 170h
	g_HookManager.WriteMemoryDWORD(0x4E3216, EndAddr);

//double CCreature::EquipedArmorSpeedByCrystal() const
	//.text:00000000004E32FA mov     edi, 128h
	g_HookManager.WriteMemoryDWORD(0x4E32FB, StartAddr);
	//.text:00000000004E3363 cmp     rdi, 170h
	g_HookManager.WriteMemoryDWORD(0x4E3366, EndAddr);

//double CCreature::EquipedArmorAvoidByCrystal() const
	//.text:00000000004E34B6 mov     edi, 128h
	g_HookManager.WriteMemoryDWORD(0x4E34B7, StartAddr);
	//.text:00000000004E3523 cmp     rdi, 170h
	g_HookManager.WriteMemoryDWORD(0x4E3526, EndAddr);

//int CCreature::EquipedArmorsMpBonus() const
	//.text:00000000004E3676 mov     ebx, 128h
	g_HookManager.WriteMemoryDWORD(0x4E3677, StartAddr);
	//.text:00000000004E36D1 cmp     rbx, 170h
	g_HookManager.WriteMemoryDWORD(0x4E36D4, EndAddr);

//double CCreature::AvoidCorrection()
	//.text:00000000004E3813 mov     ebx, 128h
	g_HookManager.WriteMemoryDWORD(0x4E3814, StartAddr);
	//.text:00000000004E3869 cmp     rbx, 170h
	g_HookManager.WriteMemoryDWORD(0x4E386C, EndAddr);

//double CCreature::WeaponHitCorrection()
	//.text:00000000004E39B1 lea     rdx, [rax+128h]
	g_HookManager.WriteMemoryDWORD(0x4E39B4, StartAddr);
	//.text:00000000004E39ED cmp     r8, 12h
	g_HookManager.WriteMemoryBYTE(0x4E39F0, Size);

//double CCreature::WeaponHitCorrectionByCrystal()
	//.text:00000000004E3B3D add     rdx, 128h
	g_HookManager.WriteMemoryDWORD(0x4E3B40, StartAddr);
	//.text:00000000004E3B79 cmp     r8, 12h
	g_HookManager.WriteMemoryBYTE(0x4E3B7C, Size);

//CCreature::DeleteAllItemsInInventory()
	//.text:00000000004EAB25 mov     ecx, 128h
	g_HookManager.WriteMemoryDWORD(0x4EAB26, StartAddr);
	//.text:00000000004EAB43 cmp     rcx, 170h
	g_HookManager.WriteMemoryDWORD(0x4EAB46, EndAddr);

//double CCreature::EquipedArmorsPDefend() const
	//.text:00000000004ED62B movsxd  rcx, dword ptr [rax+164h]
	g_HookManager.WriteMemoryDWORD(0x4ED62E, ((0x164 - 0x128) + StartAddr));
	//.text:00000000004ED65D mov     ebp, 128h
	g_HookManager.WriteMemoryDWORD(0x4ED65E, StartAddr);
	//.text:00000000004EDA15 cmp     esi, 12h
	g_HookManager.WriteMemoryBYTE(0x4EDA17, Size);

//double CCreature::EquipedArmorsMDefend() const
	//.text:00000000004EDC56 mov     r12d, 128h
	g_HookManager.WriteMemoryDWORD(0x4EDC58, StartAddr);
	//.text:00000000004EDE4B cmp     edi, 12h
	g_HookManager.WriteMemoryBYTE(0x4EDE4D, Size);
	
//CCreature::UseItem()
	//.text:00000000004FA02A add     rax, 128h
	g_HookManager.WriteMemoryDWORD(0x4FA02C, StartAddr);
	//.text:00000000004FA03D cmp     r11, 12h
	g_HookManager.WriteMemoryBYTE(0x4FA040, Size);

//bool CCreature::ValidateSetItem(SlotType slot)
	//.text:00000000005169EE add     r8, 128h
	g_HookManager.WriteMemoryDWORD(0x5169F1, StartAddr);
	//.text:0000000000516E30 mov     edx, [rax+rdi*4+128h]
	g_HookManager.WriteMemoryDWORD(0x516E33, StartAddr);

//void CCreature::ValidateAttachedSkills()
	//.text:00000000005174B3 mov     esi, 128h
	g_HookManager.WriteMemoryDWORD(0x5174B4, StartAddr);
	//.text:0000000000517592 cmp     rsi, 170h
	g_HookManager.WriteMemoryDWORD(0x517595, EndAddr);

//CCreature::EquipItem()
	//.text:0000000000519915 lea     r8, dword_9B1340
	g_HookManager.WriteMemoryDWORD(0x519918, ((DWORD)SlotOffsetTableStart - 0x51991C));
	//.text:000000000051991C cmp     edi, 12h
	g_HookManager.WriteMemoryBYTE(0x51991E, Size);
	//.text:000000000051992D lea     rdx, SlotOffsetTable3End  //9B1388
	g_HookManager.WriteMemoryDWORD(0x519930, ((DWORD)SlotOffsetTableEnd - 0x519934));

	//.text:0000000000519A4D mov     eax, [rcx+150h]
	g_HookManager.WriteMemoryDWORD(0x519A4F, ((0x150 - 0x128) + StartAddr));
	//.text:0000000000519A53 cmp     [rcx+16Ch], eax
	g_HookManager.WriteMemoryDWORD(0x519A55, ((0x16C - 0x128) + StartAddr));
	//.text:0000000000519A5B mov     dword ptr [rcx+150h], -1
	g_HookManager.WriteMemoryDWORD(0x519A5D, ((0x150 - 0x128) + StartAddr));
	//.text:0000000000519A6C mov     dword ptr [rax+16Ch], -1
	g_HookManager.WriteMemoryDWORD(0x519A6E, ((0x16C - 0x128) + StartAddr));
	//.text:0000000000519A9E mov     eax, [rcx+150h]
	g_HookManager.WriteMemoryDWORD(0x519AA0, ((0x150 - 0x128) + StartAddr));
	//.text:0000000000519AA4 cmp     [rcx+164h], eax
	g_HookManager.WriteMemoryDWORD(0x519AA6, ((0x164 - 0x128) + StartAddr));
	//.text:0000000000519AAC mov     dword ptr [rcx+150h], -1
	g_HookManager.WriteMemoryDWORD(0x519AAE, ((0x150 - 0x128) + StartAddr));
	//.text:0000000000519ABD mov     dword ptr [rax+164h], -1
	g_HookManager.WriteMemoryDWORD(0x519ABF, ((0x164 - 0x128) + StartAddr));
	//.text:0000000000519AD5 mov     [rcx+r12*4+128h], eax
	g_HookManager.WriteMemoryDWORD(0x519AD9, StartAddr);
	//.text:0000000000519AF0 mov     [rcx+150h], eax
	g_HookManager.WriteMemoryDWORD(0x519AF2, ((0x150 - 0x128) + StartAddr));
	//.text:0000000000519AFD mov     dword ptr [rax+154h], -1
	g_HookManager.WriteMemoryDWORD(0x519AFF, ((0x154 - 0x128) + StartAddr));
	//.text:0000000000519B15 mov     [rcx+164h], eax
	g_HookManager.WriteMemoryDWORD(0x519B17, ((0x164 - 0x128) + StartAddr));
	//.text:0000000000519B27 mov     dword ptr [rax+140h], -1
	g_HookManager.WriteMemoryDWORD(0x519B29, ((0x140 - 0x128) + StartAddr));
	//.text:0000000000519B38 mov     dword ptr [rax+144h], -1
	g_HookManager.WriteMemoryDWORD(0x519B3A, ((0x144 - 0x128) + StartAddr));
	//.text:0000000000519B49 mov     dword ptr [rax+148h], -1
	g_HookManager.WriteMemoryDWORD(0x519B4B, ((0x148 - 0x128) + StartAddr));
	//.text:0000000000519B5A mov     dword ptr [rax+14Ch], -1
	g_HookManager.WriteMemoryDWORD(0x519B5C, ((0x14C - 0x128) + StartAddr));
	//.text:0000000000519B6B mov     dword ptr [rax+150h], -1
	g_HookManager.WriteMemoryDWORD(0x519B6D, ((0x150 - 0x128) + StartAddr));
	//.text:0000000000519B7C mov     dword ptr [rax+154h], -1
	g_HookManager.WriteMemoryDWORD(0x519B7E, ((0x154 - 0x128) + StartAddr));
	//.text:0000000000519B8D mov     dword ptr [rax+158h], -1
	g_HookManager.WriteMemoryDWORD(0x519B8F, ((0x158 - 0x128) + StartAddr));
	//.text:0000000000519B9E mov     dword ptr [rax+15Ch], -1
	g_HookManager.WriteMemoryDWORD(0x519BA0, ((0x15C - 0x128) + StartAddr));
	//.text:0000000000519BAF mov     dword ptr [rax+160h], -1
	g_HookManager.WriteMemoryDWORD(0x519BB1, ((0x160 - 0x128) + StartAddr));
	//.text:0000000000519BC0 mov     dword ptr [rax+164h], -1
	g_HookManager.WriteMemoryDWORD(0x519BC2, ((0x164 - 0x128) + StartAddr));
	//.text:0000000000519BD8 mov     [rcx+150h], eax
	g_HookManager.WriteMemoryDWORD(0x519BDA, ((0x150 - 0x128) + StartAddr));
	//.text:0000000000519BEC mov     [rcx+16Ch], eax
	g_HookManager.WriteMemoryDWORD(0x519BEE, ((0x16C - 0x128) + StartAddr));
	//.text:0000000000519C08 movsxd  rcx, dword ptr [rax+160h]
	g_HookManager.WriteMemoryDWORD(0x519C0B, ((0x160 - 0x128) + StartAddr));
	//.text:0000000000519C49 mov     [rcx+148h], eax
	g_HookManager.WriteMemoryDWORD(0x519C4B, ((0x148 - 0x128) + StartAddr));
	//.text:0000000000519C63 mov     eax, [rcx+144h]
	g_HookManager.WriteMemoryDWORD(0x519C65, ((0x144 - 0x128) + StartAddr));
	//.text:0000000000519C69 cmp     [rcx+160h], eax
	g_HookManager.WriteMemoryDWORD(0x519C6B, ((0x160 - 0x128) + StartAddr));
	//.text:0000000000519C71 mov     dword ptr [rcx+144h], -1
	g_HookManager.WriteMemoryDWORD(0x519C73, ((0x144 - 0x128) + StartAddr));
	//.text:0000000000519C82 mov     dword ptr [rax+160h], -1
	g_HookManager.WriteMemoryDWORD(0x519C84, ((0x160 - 0x128) + StartAddr));
	//.text:0000000000519C9A mov     [rcx+r12*4+128h], eax
	g_HookManager.WriteMemoryDWORD(0x519C9E, StartAddr);
	//.text:0000000000519CB5 mov     [rcx+144h], eax
	g_HookManager.WriteMemoryDWORD(0x519CB7, ((0x144 - 0x128) + StartAddr));
	//.text:0000000000519CC2 mov     dword ptr [rax+148h], -1
	g_HookManager.WriteMemoryDWORD(0x519CC4, ((0x148 - 0x128) + StartAddr));
	//.text:0000000000519CDA mov     [rcx+160h], eax
	g_HookManager.WriteMemoryDWORD(0x519CDC, ((0x160 - 0x128) + StartAddr));
	//.text:0000000000519CEC cmp     dword ptr [rdx+138h], -1
	g_HookManager.WriteMemoryDWORD(0x519CEE, ((0x138 - 0x128) + StartAddr));
	//.text:0000000000519CFC mov     [rdx+138h], ecx
	g_HookManager.WriteMemoryDWORD(0x519CFE, ((0x138 - 0x128) + StartAddr));
	//.text:0000000000519D0C cmp     dword ptr [rdx+13Ch], -1
	g_HookManager.WriteMemoryDWORD(0x519D0E, ((0x13C - 0x128) + StartAddr));
	//.text:0000000000519D1C mov     [rdx+13Ch], ecx
	g_HookManager.WriteMemoryDWORD(0x519D1E, ((0x13C - 0x128) + StartAddr));
	//.text:0000000000519D51 mov     [rcx+138h], eax
	g_HookManager.WriteMemoryDWORD(0x519D53, ((0x138 - 0x128) + StartAddr));
	//.text:0000000000519D63 cmp     dword ptr [rdx+12Ch], -1
	g_HookManager.WriteMemoryDWORD(0x519D65, ((0x12C - 0x128) + StartAddr));
	//.text:0000000000519D73 mov     [rdx+12Ch], ecx
	g_HookManager.WriteMemoryDWORD(0x519D75, ((0x12C - 0x128) + StartAddr));
	//.text:0000000000519D80 cmp     dword ptr [rdx+130h], -1
	g_HookManager.WriteMemoryDWORD(0x519D82, ((0x130 - 0x128) + StartAddr));
	//.text:0000000000519D90 mov     [rdx+130h], ecx
	g_HookManager.WriteMemoryDWORD(0x519D92, ((0x130 - 0x128) + StartAddr));
	//.text:0000000000519DC2 mov     [rcx+12Ch], eax
	g_HookManager.WriteMemoryDWORD(0x519DC4, ((0x12C - 0x128) + StartAddr));
	//.text:0000000000519DDF mov     [rcx+r12*4+128h], eax
	g_HookManager.WriteMemoryDWORD(0x519DE3, StartAddr);
	
//CCreature::UnequipItem()
	//.text:000000000051A033 cmp     esi, 12h
	g_HookManager.WriteMemoryBYTE(0x51A035, Size);
	//.text:000000000051A116 cmp     dword ptr [rax+160h], -1
	g_HookManager.WriteMemoryDWORD(0x51A118, ((0x160 - 0x128) + StartAddr));
	//.text:000000000051A11F mov     dword ptr [rax+160h], -1
	g_HookManager.WriteMemoryDWORD(0x51A121, ((0x160 - 0x128) + StartAddr));
	//.text:000000000051A130 mov     dword ptr [rax+144h], -1
	g_HookManager.WriteMemoryDWORD(0x51A132, ((0x144 - 0x128) + StartAddr));
	//.text:000000000051A141 mov     dword ptr [rax+148h], -1
	g_HookManager.WriteMemoryDWORD(0x51A143, ((0x148 - 0x128) + StartAddr));
	//.text:000000000051A159 cmp     dword ptr [rax+164h], -1
	g_HookManager.WriteMemoryDWORD(0x51A15B, ((0x164 - 0x128) + StartAddr));
	//.text:000000000051A162 mov     dword ptr [rax+164h], -1
	g_HookManager.WriteMemoryDWORD(0x51A164, ((0x164 - 0x128) + StartAddr));
	//.text:000000000051A173 mov     dword ptr [rax+150h], -1
	g_HookManager.WriteMemoryDWORD(0x51A175, ((0x150 - 0x128) + StartAddr));
	//.text:000000000051A184 mov     dword ptr [rax+154h], -1
	g_HookManager.WriteMemoryDWORD(0x51A186, ((0x154 - 0x128) + StartAddr));
	//.text:000000000051A197 mov     dword ptr [rax+rsi*4+128h], -1
	g_HookManager.WriteMemoryDWORD(0x51A19A, StartAddr);
	
//CCreature::UnequipItem() - 2
	//.text:000000000051A43B mov     esi, 128h
	g_HookManager.WriteMemoryDWORD(0x51A43C, StartAddr);
	//.text:000000000051A47F cmp     edi, 12h
	g_HookManager.WriteMemoryBYTE(0x51A481, Size);

//void g_DB.SendSaveCharacterInfo(User* pUser) const
	//.text:000000000053A541 mov     edi, 128h
	g_HookManager.WriteMemoryDWORD(0x53A542, StartAddr);
	//.text:000000000053A5DE cmp     rbx, 12h
	//Additional Slots will be added in PlayerAction::SendSaveCharacterInfo
	//.text:000000000053A5EF mov     r8d, [rax+164h]
	g_HookManager.WriteMemoryDWORD(0x53A5F2, ((0x164 - 0x128) + StartAddr));
	//.text:000000000053A643 mov     r8d, [rax+16Ch]
	g_HookManager.WriteMemoryDWORD(0x53A646, ((0x16C - 0x128) + StartAddr));
	
//static bool LoadItemsPacket(CDBSocket *socket, const unsigned char *packet)
	//.text:0000000000571AE1 mov     ecx, 128h
	g_HookManager.WriteMemoryDWORD(0x571AE2, StartAddr);
	//.text:0000000000571B02 cmp     rcx, 170h
	g_HookManager.WriteMemoryDWORD(0x571B05, EndAddr);
	//.text:0000000000571DDB mov     [rcx+rdx*4+128h], eax
	g_HookManager.WriteMemoryDWORD(0x571DDE, StartAddr);
	//.text:0000000000571E0B mov     ecx, [rax+150h]
	g_HookManager.WriteMemoryDWORD(0x571E0D, ((0x150 - 0x128) + StartAddr));
	//.text:0000000000571E16 cmp     ecx, [rax+154h]
	g_HookManager.WriteMemoryDWORD(0x571E18, ((0x154 - 0x128) + StartAddr));
	//.text:0000000000571E1E cmp     ecx, [rax+14Ch]
	g_HookManager.WriteMemoryDWORD(0x571E20, ((0x14C - 0x128) + StartAddr));
	//.text:0000000000571E26 mov     dword ptr [rax+154h], -1
	g_HookManager.WriteMemoryDWORD(0x571E28, ((0x154 - 0x128) + StartAddr));
	//.text:0000000000571E37 mov     dword ptr [rax+14Ch], -1
	g_HookManager.WriteMemoryDWORD(0x571E39, ((0x14C - 0x128) + StartAddr));
	//.text:0000000000571E48 mov     dword ptr [rax+158h], -1
	g_HookManager.WriteMemoryDWORD(0x571E4A, ((0x158 - 0x128) + StartAddr));
	//.text:0000000000571E59 mov     eax, [rcx+150h]
	g_HookManager.WriteMemoryDWORD(0x571E5B, ((0x150 - 0x128) + StartAddr));
	//.text:0000000000571E5F mov     [rcx+16Ch], eax
	g_HookManager.WriteMemoryDWORD(0x571E61, ((0x16C - 0x128) + StartAddr));
	//.text:0000000000571E67 mov     dword ptr [rax+154h], -1
	g_HookManager.WriteMemoryDWORD(0x571E69, ((0x154 - 0x128) + StartAddr));
	//.text:0000000000571E78 mov     eax, [rcx+150h]
	g_HookManager.WriteMemoryDWORD(0x571E7A, ((0x150 - 0x128) + StartAddr));
	//.text:0000000000571E7E mov     [rcx+164h], eax
	g_HookManager.WriteMemoryDWORD(0x571E80, ((0x164 - 0x128) + StartAddr));
	//.text:0000000000571EA9 mov     r8d, [rax+160h]
	g_HookManager.WriteMemoryDWORD(0x571EAC, ((0x160 - 0x128) + StartAddr));
	//.text:0000000000571EF2 mov     [rcx+148h], eax
	g_HookManager.WriteMemoryDWORD(0x571EF4, ((0x148 - 0x128) + StartAddr));
	//.text:0000000000571F06 mov     dword ptr [rax+148h], -1
	g_HookManager.WriteMemoryDWORD(0x571F08, ((0x148 - 0x128) + StartAddr));

//static bool ReplyLoadPetItems(CDBSocket *socket, const unsigned char *packet)
	//.text:0000000000572459 mov     edx, 128h
	g_HookManager.WriteMemoryDWORD(0x57245A, StartAddr);
	//text:0000000000572473 cmp     rdx, 170h
	g_HookManager.WriteMemoryDWORD(0x572476, EndAddr);
	
//bool CMultiSellList::CheckSendable(CMultiSellInfo * pMultiSellInfo)
	//.text:00000000006920A9 add     rax, 128h
	g_HookManager.WriteMemoryDWORD(0x6920AB, StartAddr);
	//.text:00000000006920BB cmp     rbx, 12h
	g_HookManager.WriteMemoryBYTE(0x6920BE, Size);

//CNPC::EquipItem()
	//.text:00000000006BC1D7 cmp     dword ptr [rax+rcx*4+128h], 0
	g_HookManager.WriteMemoryDWORD(0x6BC1DA, StartAddr);
	
//CNPC::UnequipItem(SlotType)
	//.text:00000000006C2BA5 mov     r8d, [rax+rbx*4+128h]
	g_HookManager.WriteMemoryDWORD(0x6C2BA9, StartAddr);

//bool NpcShowBuyPagePacket(NpcSocket* pSocket, const unsigned char *pPacket)
	//.text:00000000006E5274 mov     ebx, 128h
	g_HookManager.WriteMemoryDWORD(0x6E5275, StartAddr);
	//.text:00000000006E52D2 cmp     rbx, 170h
	g_HookManager.WriteMemoryDWORD(0x6E52D5, EndAddr);

//void COlympiadUser::UnequipForbiddenWeapon()
	//.text:0000000000706E3C mov     esi, 128h
	g_HookManager.WriteMemoryDWORD(0x706E3D, StartAddr);
	//.text:0000000000706F1F cmp     rsi, 170h
	g_HookManager.WriteMemoryDWORD(0x706F22, EndAddr);

//void CPet::SendPetItemList(bool bIsUpdating)
	//.text:000000000074EBD1 mov     esi, 128h
	g_HookManager.WriteMemoryDWORD(0x74EBD2, StartAddr);
	//.text:000000000074EE35 cmp     eax, 12h
	g_HookManager.WriteMemoryBYTE(0x74EE37, Size);
	//.text:000000000074F9D6 cmp     eax, 12h
	g_HookManager.WriteMemoryBYTE(0x74F9D8, Size);

	//REPLACED OFFSET SLOT TABLE
	//.text:000000000074ED23 lea     rsi, SlotOffsetTable4End
	g_HookManager.WriteMemoryDWORD(0x74ED26, ((DWORD)SlotOffsetTableEnd - 0x74ED2A));
	//.text:000000000074EE0F lea     rax, SlotOffsetTable4
	g_HookManager.WriteMemoryDWORD(0x74EE12, ((DWORD)SlotOffsetTable - 0x74EE16));
	//.text:000000000074F73E lea     rsi, SlotOffsetTable4End
	g_HookManager.WriteMemoryDWORD(0x74F741, ((DWORD)SlotOffsetTableEnd - 0x74F745));
	//.text:000000000074F757 lea     rsi, SlotOffsetTable4End
	g_HookManager.WriteMemoryDWORD(0x74F75A, ((DWORD)SlotOffsetTableEnd - 0x74F75E));
	//.text:000000000074F8EB lea     rbx, SlotOffsetTable4End
	g_HookManager.WriteMemoryDWORD(0x74F8EE, ((DWORD)SlotOffsetTableEnd - 0x74F8F2));
	//.text:000000000074F9AB lea     rax, SlotOffsetTable4
	g_HookManager.WriteMemoryDWORD(0x74F9AE, ((DWORD)SlotOffsetTable - 0x74F9B2));
	//.text:000000000074FE68 lea     rbx, SlotOffsetTable4End
	g_HookManager.WriteMemoryDWORD(0x74FE6B, ((DWORD)SlotOffsetTableEnd - 0x74FE6F));
	//.text:000000000074FE76 lea     rbx, SlotOffsetTable4End
	g_HookManager.WriteMemoryDWORD(0x74FE79, ((DWORD)SlotOffsetTableEnd - 0x74FE7D));


//CSkillEffect_p_set_collected::Pump
	//.text:00000000007C4328 mov     edx, [rax+rsi*4+128h]
	g_HookManager.WriteMemoryDWORD(0x7C432B, StartAddr);

//bool CTrade::CheckTradable(User *pUser, CItem * pItem)
	//.text:00000000007F2EE9 add     rax, 128h
	g_HookManager.WriteMemoryDWORD(0x7F2EEB, StartAddr);
	//.text:00000000007F2EFB cmp     rcx, 12h
	g_HookManager.WriteMemoryBYTE(0x7F2EFE, Size);

//User::UnequipItem(SlotType)
	//.text:0000000000819A44 mov     r8d, [rax+148h]
	g_HookManager.WriteMemoryDWORD(0x819A45, ((0x148 - 0x128) + StartAddr));
	//.text:0000000000819AEF mov     r8d, [rax+rdi*4+128h]
	g_HookManager.WriteMemoryDWORD(0x819AF3, StartAddr);

//User::SendItemList()
	//.text:0000000000819EFE mov     esi, 128h
	g_HookManager.WriteMemoryDWORD(0x819EFF, StartAddr);
	//REPLACED OFFSET SLOT TABLE
	//.text:000000000081A04E lea     rbp, SlotOffsetTableEnd
	g_HookManager.WriteMemoryDWORD(0x81A051, ((DWORD)SlotOffsetTableEnd - 0x81A055));
	//.text:000000000081A154 lea     rax, SlotOffsetTable
	g_HookManager.WriteMemoryDWORD(0x81A157, ((DWORD)SlotOffsetTableStart - 0x81A15B));
	//.text:000000000081A92C lea     rbp, SlotOffsetTableEnd
	g_HookManager.WriteMemoryDWORD(0x81A92F, ((DWORD)SlotOffsetTableEnd - 0x81A933));
	//.text:000000000081A93D lea     rbp, SlotOffsetTableEnd
	g_HookManager.WriteMemoryDWORD(0x81A940, ((DWORD)SlotOffsetTableEnd - 0x81A944));
	//.text:000000000081AA84 lea     rbx, SlotOffsetTableEnd
	g_HookManager.WriteMemoryDWORD(0x81AA87, ((DWORD)SlotOffsetTableEnd - 0x81AA8B));
	//.text:000000000081AB3F lea     rax, SlotOffsetTable
	g_HookManager.WriteMemoryDWORD(0x81AB42, ((DWORD)SlotOffsetTableStart - 0x81AB46));
	//.text:000000000081B033 lea     rbx, SlotOffsetTableEnd
	g_HookManager.WriteMemoryDWORD(0x81B036, ((DWORD)SlotOffsetTableEnd - 0x81B03A));
	//.text:000000000081B044 lea     rbx, SlotOffsetTableEnd
	g_HookManager.WriteMemoryDWORD(0x81B047, ((DWORD)SlotOffsetTableEnd - 0x81B04B));


//User::OnDieDropItem(bool bDieOnBattleField)
	//.text:000000000081C85C mov     edi, 128h
	g_HookManager.WriteMemoryDWORD(0x81C85D, StartAddr);
	//.text:000000000081C8C2 cmp     rdi, 170h
	g_HookManager.WriteMemoryDWORD(0x81C8C5, EndAddr);
	//.text:000000000081C9CE mov     edi, 128h
	g_HookManager.WriteMemoryDWORD(0x81C9CF, StartAddr);
	//.text:000000000081CA32 cmp     rdi, 170h
	g_HookManager.WriteMemoryDWORD(0x81CA35, EndAddr);
	//.text:000000000081CDA3 mov     edi, 128h
	g_HookManager.WriteMemoryDWORD(0x81CDA4, StartAddr);
	//.text:000000000081CDC8 cmp     rdi, 170h
	g_HookManager.WriteMemoryDWORD(0x81CDCB, EndAddr);
	//.text:000000000081D5B1 mov     edi, 128h
	g_HookManager.WriteMemoryDWORD(0x81D5B2, StartAddr);
	//.text:000000000081D5D8 cmp     rdi, 170h
	g_HookManager.WriteMemoryDWORD(0x81D5DB, EndAddr);

//void User::SendPackageSendableList(int nTargetUserDBID)
	//REPLACED OFFSET SLOT TABLE
	//.text:0000000000821149 lea     rbx, SlotOffsetTableEnd
	g_HookManager.WriteMemoryDWORD(0x82114C, ((DWORD)SlotOffsetTableEnd - 0x821150));
	//.text:0000000000821231 lea     rax, SlotOffsetTable
	g_HookManager.WriteMemoryDWORD(0x821234, ((DWORD)SlotOffsetTableStart - 0x821238));
	//.text:0000000000821423 lea     rbx, SlotOffsetTableEnd
	g_HookManager.WriteMemoryDWORD(0x821426, ((DWORD)SlotOffsetTableEnd - 0x82142A));

	//.text:00000000008212B6 add     rax, 128h
	g_HookManager.WriteMemoryDWORD(0x8212B8, StartAddr);
	//.text:00000000008212CB cmp     rcx, 12h
	g_HookManager.WriteMemoryBYTE(0x8212CE, Size);

//User::EquipItem()
	//REPLACED OFFSET SLOT TABLE
	//.text:00000000008259E4 lea     rax, SlotOffsetTable
	g_HookManager.WriteMemoryDWORD(0x8259E7, ((DWORD)SlotOffsetTableStart - 0x8259EB));
	//.text:00000000008259EB lea     r8, SlotOffsetTableEnd
	g_HookManager.WriteMemoryDWORD(0x8259EE, ((DWORD)SlotOffsetTableEnd - 0x8259F2));
	

	//.text:00000000008259CF cmp     ebx, 12h
	g_HookManager.WriteMemoryBYTE(0x8259D1, Size);
	//.text:0000000000825A0B mov     r8d, [rdx+148h]
	g_HookManager.WriteMemoryDWORD(0x825A0E, ((0x148 - 0x128) + StartAddr));
	//.text:0000000000825A42 mov     edx, [rax+148h]
	g_HookManager.WriteMemoryDWORD(0x825A44, ((0x148 - 0x128) + StartAddr));
	//.text:0000000000825B29 cmp     dword ptr [rax+138h], -1
	g_HookManager.WriteMemoryDWORD(0x825B2B, ((0x138 - 0x128) + StartAddr));
	//.text:0000000000825B42 cmp     dword ptr [rax+13Ch], -1
	g_HookManager.WriteMemoryDWORD(0x825B44, ((0x13C - 0x128) + StartAddr));
	//.text:0000000000825B5F cmp     dword ptr [rax+12Ch], -1
	g_HookManager.WriteMemoryDWORD(0x825B61, ((0x12C - 0x128) + StartAddr));
	//.text:0000000000825B78 cmp     dword ptr [rax+130h], -1
	g_HookManager.WriteMemoryDWORD(0x825B7A, ((0x130 - 0x128) + StartAddr));
	//.text:0000000000825B9E mov     r8d, [rax+r12*4+128h]
	g_HookManager.WriteMemoryDWORD(0x825BA2, StartAddr);
	//.text:0000000000825BCF mov     edx, [rax+r12*4+128h]
	g_HookManager.WriteMemoryDWORD(0x825BD3, StartAddr);
	//.text:0000000000825C4D mov     r8d, [rax+16Ch]
	g_HookManager.WriteMemoryDWORD(0x825C50, ((0x16C - 0x128) + StartAddr));
	//.text:0000000000825C7D mov     edx, [rax+16Ch]
	g_HookManager.WriteMemoryDWORD(0x825C7F, ((0x16C - 0x128) + StartAddr));
	//.text:0000000000825CF6 mov     edi, 140h
	g_HookManager.WriteMemoryDWORD(0x825CF7, ((0x140 - 0x128) + StartAddr));
	//.text:0000000000825DA6 cmp     rdi, 164h
	g_HookManager.WriteMemoryDWORD(0x825DA9, ((0x164 - 0x128) + StartAddr));
	//.text:0000000000825DCF mov     r8d, [rax+144h]
	g_HookManager.WriteMemoryDWORD(0x825DD2, ((0x144 - 0x128) + StartAddr));
	//.text:0000000000825DFF mov     edx, [rax+144h]
	g_HookManager.WriteMemoryDWORD(0x825E01, ((0x144 - 0x128) + StartAddr));
	//.text:0000000000825E6D mov     r8d, [rax+148h]
	g_HookManager.WriteMemoryDWORD(0x825E70, ((0x148 - 0x128) + StartAddr));
	//.text:0000000000825E9D mov     edx, [rax+148h]
	g_HookManager.WriteMemoryDWORD(0x825E9F, ((0x148 - 0x128) + StartAddr));
	//.text:0000000000825F27 mov     r8d, [rax+160h]
	g_HookManager.WriteMemoryDWORD(0x825F2A, ((0x160 - 0x128) + StartAddr));
	//.text:0000000000825F57 mov     edx, [rax+160h]
	g_HookManager.WriteMemoryDWORD(0x825F59, ((0x160 - 0x128) + StartAddr));
	//.text:0000000000825FD7 mov     r8d, [rax+150h]
	g_HookManager.WriteMemoryDWORD(0x825FDA, ((0x150 - 0x128) + StartAddr));
	//.text:0000000000826007 mov     edx, [rax+150h]
	g_HookManager.WriteMemoryDWORD(0x826009, ((0x150 - 0x128) + StartAddr));
	//.text:0000000000826075 mov     r8d, [rax+154h]
	g_HookManager.WriteMemoryDWORD(0x826078, ((0x154 - 0x128) + StartAddr));
	//.text:00000000008260A5 mov     edx, [rax+154h]
	g_HookManager.WriteMemoryDWORD(0x8260A7, ((0x154 - 0x128) + StartAddr));
	//.text:000000000082612F mov     r8d, [rax+164h]
	g_HookManager.WriteMemoryDWORD(0x826132, ((0x164 - 0x128) + StartAddr));
	//.text:000000000082615F mov     edx, [rax+164h]
	g_HookManager.WriteMemoryDWORD(0x826161, ((0x164 - 0x128) + StartAddr));
	//.text:000000000082622D cmp     dword ptr [rax+144h], -1
	g_HookManager.WriteMemoryDWORD(0x82622F, ((0x144 - 0x128) + StartAddr));
	//.text:0000000000826448 mov     [rdx+148h], ecx
	g_HookManager.WriteMemoryDWORD(0x82644A, ((0x148 - 0x128) + StartAddr));
	//.text:0000000000826478 mov     r8d, [rax+148h]
	g_HookManager.WriteMemoryDWORD(0x82647B, ((0x148 - 0x128) + StartAddr));
	//.text:00000000008264B2 mov     dword ptr [rax+148h], -1
	g_HookManager.WriteMemoryDWORD(0x8264B4, ((0x148 - 0x128) + StartAddr));
	//.text:000000000082651A mov     r8d, [rax+148h]
	g_HookManager.WriteMemoryDWORD(0x82651D, ((0x148 - 0x128) + StartAddr));
	//.text:000000000082654E mov     edx, [rax+148h]
	g_HookManager.WriteMemoryDWORD(0x826550, ((0x148 - 0x128) + StartAddr));
	//.text:00000000008265BD mov     dword ptr [rax+148h], -1
	g_HookManager.WriteMemoryDWORD(0x8265BF, ((0x148 - 0x128) + StartAddr));
	
//User::UnequipItem(CItem*)
	//.text:0000000000826BC4 add     rax, 128h
	g_HookManager.WriteMemoryDWORD(0x826BC6, StartAddr);
	//.text:0000000000826BDB cmp     rcx, 12h
	g_HookManager.WriteMemoryBYTE(0x826BDE, Size);
	//.text:0000000000826C00 mov     eax, 12h
	g_HookManager.WriteMemoryBYTE(0x826C01, Size);
	//.text:0000000000826E1B mov     dword ptr [rcx+148h], -1
	g_HookManager.WriteMemoryDWORD(0x826E1D, ((0x148 - 0x128) + StartAddr));
	//.text:0000000000826E57 mov     r8d, [rax+148h]
	g_HookManager.WriteMemoryDWORD(0x826E5A, ((0x148 - 0x128) + StartAddr));
	//.text:0000000000826E85 mov     edx, [rax+148h]
	g_HookManager.WriteMemoryDWORD(0x826E87, ((0x148 - 0x128) + StartAddr));
	//.text:0000000000826EEA mov     dword ptr [rax+148h], 0FFFFFFFFh
	g_HookManager.WriteMemoryDWORD(0x826EEC, ((0x148 - 0x128) + StartAddr));
	
//void User::SendPrivateStoreManageList(CUserSocket * pTargetSocket)
	//.text:000000000082971F mov     esi, 128h
	g_HookManager.WriteMemoryDWORD(0x829720, StartAddr);

//void User::PrivateStoreListReq(int nBuyerId)
	//.text:000000000082EBE3 mov     esi, 128h
	g_HookManager.WriteMemoryDWORD(0x82EBE4, StartAddr);
	
//void User::PrivateStoreBuyListReq(int nBuyerId)
	//.text:000000000082FC0F mov     ebp, 128h
	g_HookManager.WriteMemoryDWORD(0x82FC10, StartAddr);
	
//void User::PrivateStoreBuyBuy(const unsigned char* packet, User * pBuyer)
	//.text:000000000083E459 mov     esi, 128h
	g_HookManager.WriteMemoryDWORD(0x83E45A, StartAddr);

//void User::SendUserInfo(bool byGM)
	//.text:0000000000856225 mov     esi, 128h
	g_HookManager.WriteMemoryDWORD(0x856226, StartAddr);

//void User::SendCharInfo
	//.text:0000000000858506 lea     rdi, [rdx+128h]
	g_HookManager.WriteMemoryDWORD(0x858509, StartAddr);

//SellPacket
	//.text:000000000087A73E add     rax, 128h 
	g_HookManager.WriteMemoryDWORD(0x87A740, StartAddr);
	//.text:000000000087A74F cmp     rcx, 12h
	g_HookManager.WriteMemoryBYTE(0x87A752, Size);

//void CWareHouseDeposit::RequestEx()
	//.text:00000000008A5C7E mov     edi, 128h
	g_HookManager.WriteMemoryDWORD(0x8A5C7F, StartAddr);
	//.text:00000000008A5CE2 cmp     rdi, 170h
	g_HookManager.WriteMemoryDWORD(0x8A5CE5, EndAddr);
	//.text:00000000008A5D64 cmp     [rcx+rdi*4+128h], eax
	g_HookManager.WriteMemoryDWORD(0x8A5D67, StartAddr);

//void CWareHouseDeposit::Request()
	//.text:00000000008A74BA mov     edi, 128h
	g_HookManager.WriteMemoryDWORD(0x8A74BB, StartAddr);
	//.text:00000000008A751F cmp     rdi, 170h
	g_HookManager.WriteMemoryDWORD(0x8A7512, EndAddr);
	//.text:00000000008A75A4 cmp     [rcx+rdi*4+128h], eax
	g_HookManager.WriteMemoryDWORD(0x8A75A7, StartAddr);

//void CWareHouseDeposit::SendList(WareHouseClass whType)
	//REPLACED OFFSET TABLE
	//.text:00000000008A9DD4 lea     rdi, ItemSlotOffsetTable2End
	g_HookManager.WriteMemoryDWORD(0x8A9DD7, ((DWORD)SlotOffsetTableEnd - 0x8A9DDB));
	//.text:00000000008A9EB3 lea     rax, ItemSlotOffsetTable2
	g_HookManager.WriteMemoryDWORD(0x8A9EB6, ((DWORD)SlotOffsetTableStart - 0x8A9EBA));
	//.text:00000000008AA217 lea     rdi, ItemSlotOffsetTable2End
	g_HookManager.WriteMemoryDWORD(0x8AA21A, ((DWORD)SlotOffsetTableEnd - 0x8AA21E));
	//.text:00000000008AA220 lea     rdi, ItemSlotOffsetTable2End
	g_HookManager.WriteMemoryDWORD(0x8AA223, ((DWORD)SlotOffsetTableEnd - 0x8AA227));

	//.text:00000000008A9F5F mov     ebx, 128h
	g_HookManager.WriteMemoryDWORD(0x8A9F60, StartAddr);
	//.text:00000000008A9FC2 cmp     rbx, 170h
	g_HookManager.WriteMemoryDWORD(0x8A9FC5, EndAddr);

//void CWareHouseDeposit::SendListEx(WareHouseClass whType)
	//REPLACED OFFSET TABLE
	//.text:00000000008AA9EF lea     rdi, ItemSlotOffsetTable2End
	g_HookManager.WriteMemoryDWORD(0x8AA9F2, ((DWORD)SlotOffsetTableEnd - 0x8AA9F6));
	//.text:00000000008AAACA lea     rax, ItemSlotOffsetTable2
	g_HookManager.WriteMemoryDWORD(0x8AAACD, ((DWORD)SlotOffsetTableStart - 0x8AAAD1));
	//.text:00000000008AADDD lea     rdi, ItemSlotOffsetTable2End
	g_HookManager.WriteMemoryDWORD(0x8AADD0, ((DWORD)SlotOffsetTableEnd - 0x8AADE4));
	//

	//.text:00000000008AABA2 mov     ebx, 128h
	g_HookManager.WriteMemoryDWORD(0x8AABA3, StartAddr);
	//.text:00000000008AAC02 cmp     rbx, 170h
	g_HookManager.WriteMemoryDWORD(0x8AAC05, EndAddr);

//User::DropItem()
	//REPLACED OFFSET TABLE
	//.text:000000000080043A lea     rax, SlotOffsetTable
	g_HookManager.WriteMemoryDWORD(0x80043D, ((DWORD)SlotOffsetTableStart - 0x800441));
	//.text:0000000000800441 lea     r9, SlotOffsetTableEnd
	g_HookManager.WriteMemoryDWORD(0x800444, ((DWORD)SlotOffsetTableEnd - 0x800448));

//void User::ShowPreview(int nTotalCount, int arItemList[200])
	//.text:000000000080A600 lea     r10, SlotOffsetTable
	g_HookManager.WriteMemoryDWORD(0x80A603, ((DWORD)SlotOffsetTableStart - 0x80A607));
	//.text:000000000080A60A lea     r11, SlotOffsetTableEnd
	g_HookManager.WriteMemoryDWORD(0x80A60D, ((DWORD)SlotOffsetTableEnd - 0x80A611));
	//.text:000000000080A62B lea     r10, SlotOffsetTable
	g_HookManager.WriteMemoryDWORD(0x80A62E, ((DWORD)SlotOffsetTableStart - 0x80A632));
	//.text:000000000080A634 lea     r10, SlotOffsetTable
	g_HookManager.WriteMemoryDWORD(0x80A637, ((DWORD)SlotOffsetTableStart - 0x80A63B));
	//.text:000000000080A69B cmp     rdx, 12h
	g_HookManager.WriteMemoryBYTE(0x80A69E, Size);
	
}
*/

void CSharedFactory::CheckIndexesForCreature()
{
	CCreatureSharedFactory *g_SD = (CCreatureSharedFactory*)0xE4120A0;
	
	g_Log.Add(CLog::Blue, "var1[%d] var2[%d]", g_SD->firstIndex, g_SD->lastIndex);
	CreatureSharedData* pSD = (CreatureSharedData*)g_SD->lpCreatureData;
	for(int n=0;n<70000;n++)
	{
		g_Log.Add(CLog::Blue, "[%d] index[%d] index2[%d] addr[%p]", n, pSD[n].nSharedIndex, pSD[n].nNextIndex, pSD[n]);
	}
}

CreatureSharedData* CCreatureSharedFactory::Allocate(CCreature *pCreature)
{
	if(pCreature)
	{
		_lock.Lock(__FILEW__, __LINE__);
		int freeIndex = firstIndex;
		
		if(freeIndex == -1)
		{
			g_Log.Add(CLog::Error, "[%s] Shared Memory runout!", __FUNCTION__);
			CRASH_MACRO;
			
		}

		if(freeIndex < 0 || freeIndex >= (*lpCreatureDataCount))
		{
			g_Log.Add(CLog::Error, "[%s] Shared Memory free index invalid [%d]", __FUNCTION__, freeIndex);
			CRASH_MACRO;
		}

		CreatureSharedData* pSD = &lpCreatureData[freeIndex];
		firstIndex = pSD->nNextIndex;
		if(firstIndex < 0 || firstIndex >= (*lpCreatureDataCount))
		{
			g_Log.Add(CLog::Error, "[%s] Shared Memory next index invalid [%d]", __FUNCTION__, firstIndex);
			CRASH_MACRO;
		}

		memset(pSD, 0, sizeof(CreatureSharedData));

		CreatureSharedData* pNextSD = &lpCreatureData[firstIndex];
		pNextSD->nSharedIndex = -1;
		lpCreatureIndex[freeIndex] = pCreature->nObjectID;
		pSD->nSharedIndex = -1;
		pSD->nNextIndex = -1;
		pSD->nServerIndex = freeIndex;

		int lockIndex = freeIndex & 8191;
		if(lockIndex >= 0)
		{
			CIOCriticalSection* lpLocks = (*(CIOCriticalSection**)((INT64)lpCenteredLock + (INT64)(((INT64)lockIndex) * 8)));
			pCreature->lpSharedDataLock = lpLocks;
			addedCount++;
			remainCount--;
		}else
		{
			g_Log.Add(CLog::Error, "[%s] Invalid Lock Index[%d]!", __FUNCTION__, lockIndex);
			CRASH_MACRO;
		}		
		_lock.Unlock();

		return pSD;
	}

	return NULL;
}

BOOL CCreatureSharedFactory::Free(int serverIndex)
{
	_lock.Lock(__FILEW__, __LINE__);
	if(serverIndex > -1)
	{
		lpCreatureIndex[serverIndex] = -1;
		CreatureSharedData* pCurrentSD = &lpCreatureData[serverIndex];
		CreatureSharedData* pLastSD = &lpCreatureData[lastIndex];
		pCurrentSD->nNextIndex = pLastSD->nNextIndex;
		pLastSD->nNextIndex = serverIndex;
		pLastSD->nSharedIndex = lastIndex;
		lastIndex = serverIndex;
		removedCount++;
		remainCount++;
	}else
		g_Log.Add(CLog::Error, "[%s] Invalid index[%d]", __FUNCTION__, serverIndex);
	_lock.Unlock();

	return TRUE;
}