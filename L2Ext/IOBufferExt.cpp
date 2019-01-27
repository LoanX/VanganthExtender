#include "stdafx.h"
#include "IOBufferExt.h"

void DumpPacket(PCHAR buff, int len)
{
	stringstream nameStream;
	nameStream << time(0) << ".txt";
	ofstream file(nameStream.str().c_str());
	for(int n=0;n<len;n++)
	{
		file << "0x" << hex << (UINT)(BYTE)buff[n] << " ";
	}
	file.close();
}

int VAssembleHook(PCHAR buff, int len, const char* format, va_list va)
{
	int ret = Utils::VAssemble(buff, len, format, va);
	if(ret >= 8190)
	{
		g_Log.Add(CLog::Error, "[%s] Blocking too big packet to cached [%d] opCode[%d][%d][%d][%d][%d][%d][%d]", __FUNCTION__, ret, buff[0], buff[1], buff[2], buff[3], buff[4], buff[5], buff[6]);
		DumpPacket(buff, len);
		g_Log.StackTrace(CLog::Error);
		return 0;
	}
	return ret;
}

void CIOBufferExt::Init()
{
	g_HookManager.WriteCall(0x54A8EB, VAssembleHook);
	//.text:0000000000624457 imul    rcx, 2048h                      ; size_t
	g_HookManager.WriteMemoryDWORD(0x62445A, sizeof(CIOBuffer));
	//.text:000000000062445E call    malloc
	g_HookManager.WriteCall(0x62445E, CIOBufferExt::Malloc);
	//.text:0000000000624493 imul    rcx, 2048h
	g_HookManager.WriteMemoryDWORD(0x624496, sizeof(CIOBuffer));

//Outgoing packets
//CSocket::SendV
	//.text:00000000007D5E0F lea     rcx, [rax+12h]
	g_HookManager.WriteJump(0x7D5E0F, ReallocateIOBuffer34, 2);
	//.text:00000000007D5E19 mov     edx, 8190
	g_HookManager.WriteMemoryDWORD(0x7D5E1A, 16382);
	//.text:00000000007D5E3D cmp     eax, 2000h
	g_HookManager.WriteMemoryDWORD(0x7D5E3E, 16384);
	//;.text:00000000007D5E5B mov     [rbx+10h], r11b
	g_HookManager.WriteJump(0x7D5E5B, ReallocateIOBuffer35, 15);
//CIOSocket::_Write
	//.text:0000000000620B64 lea     rax, [rbx+10h]
	g_HookManager.WriteJump(0x620B64, ReallocateIOBuffer36, 9);
	//.text:0000000000620C89 cmp     eax, 2000h
	g_HookManager.WriteMemoryDWORD(0x620C8A, 0x4000);
	//.text:0000000000620C93 lea     rcx, [r9+rcx+10h]               ; void *
	//.text:0000000000620C98 lea     rdx, [rbx+10h]                  ; void *
	//.text:0000000000620C9C call    memmove
	g_HookManager.WriteCall(0x620C9C, CIOBufferExt::MemMove);
//ASYNCHRONOUS_SERIALIZE_FAST_IMPL(CSocket, AsyncSendConstBuffer, CIOBuffer*, pBuffer)
	//.text:00000000007D58B1 movzx   ecx, word ptr [rbx+10h]
	g_HookManager.WriteJump(0x7D58B1, ReallocateIOBuffer39, 34);
	//.text:00000000007D592A lea     rax, [rbp+10h]
	g_HookManager.WriteJump(0x7D592A, ReallocateIOBuffer40, 4);
	//.text:00000000007D5A39 cmp     eax, 2000h
	g_HookManager.WriteMemoryDWORD(0x7D5A3A, 0x4000);
	//.text:00000000007D5A4D movzx   eax, word ptr [rbx+10h]
	g_HookManager.WriteJump(0x7D5A4D, ReallocateIOBuffer41, 38);
	//.text:00000000007D5A82 lea     rdx, [rbx+10h]                  ; void *
	//.text:00000000007D5A86 lea     rcx, [rcx+r9+10h]               ; void *
	//.text:00000000007D5A8B call    memmove
	g_HookManager.WriteCall(0x7D5A8B, CIOBufferExt::MemMove);
	//.text:00000000007D5AB7 movzx   ecx, word ptr [rbx+10h]
	g_HookManager.WriteJump(0x7D5AB7, ReallocateIOBuffer42, 34);
//CIOBuffer::Copy
	//.text:00000000007D4E24 lea     rdx, [rbx+10h]                  ; void *
	g_HookManager.WriteJump(0x7D4E24, ReallocateIOBuffer43, 3);
//BroadcastToNeighborDistEx
	//.text:0000000000403F1E lea     rcx, [rax+12h]
	g_HookManager.WriteJump(0x403F1E, ReallocateIOBuffer37, 2);
	//.text:0000000000403F28 mov     edx, 8190
	g_HookManager.WriteMemoryDWORD(0x403F29, 16382);
	//.text:0000000000403F50 mov     [rsi+10h], r11b
	g_HookManager.WriteJump(0x403F50, ReallocateIOBuffer38, 20);
//BroadcastToNeighborDist
	//.text:000000000047490A lea     rcx, [rax+12h]
	g_HookManager.WriteJump(0x47490A, ReallocateIOBuffer44, 2);
	//.text:0000000000474914 mov     edx, 1FFEh
	g_HookManager.WriteMemoryDWORD(0x474915, 16382);
	//.text:000000000047493E mov     [r12+10h], r11b
	g_HookManager.WriteJump(0x47493E, ReallocateIOBuffer45, 24);
//BroadcastToNeighborIndex
	//.text:000000000047557C lea     rcx, [rax+12h]
	g_HookManager.WriteJump(0x47557C, ReallocateIOBuffer46, 2);
	//.text:0000000000475586 mov     edx, 1FFEh
	g_HookManager.WriteMemoryDWORD(0x475587, 16382);
	//.text:00000000004755AF mov     [r13+10h], r11b
	g_HookManager.WriteJump(0x4755AF, ReallocateIOBuffer47, 21);
//BroadcastToNeighbor
	//.text:00000000004B005E lea     rcx, [rax+12h]
	g_HookManager.WriteJump(0x4B005E, ReallocateIOBuffer48, 2);
	//.text:00000000004B0068 mov     edx, 1FFEh
	g_HookManager.WriteMemoryDWORD(0x4B0069, 16382);
	//.text:00000000004B0090 mov     [rsi+10h], r11b
	g_HookManager.WriteJump(0x4B0090, ReallocateIOBuffer49, 20);
//BroadcastToTargettingNeighbor
	//.text:00000000004E815A lea     rcx, [rax+12h]
	g_HookManager.WriteJump(0x4E815A, ReallocateIOBuffer50, 2);
	//.text:00000000004E8164 mov     edx, 1FFEh
	g_HookManager.WriteMemoryDWORD(0x4E8165, 16382);
	//.text:00000000004E818E mov     [r12+10h], r11b
	g_HookManager.WriteJump(0x4E818E, ReallocateIOBuffer51, 24);
//BroadcastToNeighborExceptSelf
	//.text:00000000004E8DED lea     rcx, [rax+12h]
	g_HookManager.WriteJump(0x4E8DED, ReallocateIOBuffer52, 2);
	//.text:00000000004E8DF7 mov     edx, 1FFEh
	g_HookManager.WriteMemoryDWORD(0x4E8DF8, 16382);
	//.text:00000000004E8E1F mov     [rsi+10h], r11b
	g_HookManager.WriteJump(0x4E8E1F, ReallocateIOBuffer53, 20);
//BroadcastToNeighborExceptSelf
	//.text:00000000004E982D lea     rcx, [rax+12h]
	g_HookManager.WriteJump(0x4E982D, ReallocateIOBuffer54, 2);
	//.text:00000000004E9837 mov     edx, 1FFEh
	g_HookManager.WriteMemoryDWORD(0x4E9838, 16382);
	//.text:00000000004E985F mov     [rsi+10h], r11b
	g_HookManager.WriteJump(0x4E985F, ReallocateIOBuffer55, 20);
//BroadCastToAllUser
	//.text:000000000082301C lea     rcx, [rax+12h]
	g_HookManager.WriteJump(0x82301C, ReallocateIOBuffer56, 2);
	//.text:0000000000823026 mov     edx, 1FFEh
	g_HookManager.WriteMemoryDWORD(0x823027, 16382);
	//.text:0000000000823050 mov     [r12+10h], r11b
	g_HookManager.WriteJump(0x823050, ReallocateIOBuffer57, 21);
//BroadcastToAllUserEx
	//.text:0000000000840CF1 lea     rcx, [rax+12h]
	g_HookManager.WriteJump(0x840CF1, ReallocateIOBuffer58, 2);
	//.text:0000000000840CFB mov     edx, 1FFEh
	g_HookManager.WriteMemoryDWORD(0x840CFC, 16382);
	//.text:0000000000840D23 mov     [rbx+10h], r11b
	g_HookManager.WriteJump(0x840D23, ReallocateIOBuffer59, 17);
//CIOSocket::WriteCallback
	//.text:0000000000622299 lea     rax, [rcx+10h]
	g_HookManager.WriteJump(0x622299, ReallocateIOBuffer60, 10);
//CIOSocket::Write
	//.text:0000000000622671 lea     rax, [rbx+10h]
	g_HookManager.WriteJump(0x622671, ReallocateIOBuffer61, 4);
	//.text:00000000006227EA cmp     eax, 2000h
	g_HookManager.WriteMemoryDWORD(0x6227EB, 0x4000);
	//.text:00000000006227F4 lea     rdx, [rbx+10h]                  ; void *
	//.text:00000000006227F8 lea     rcx, [r9+rcx+10h]               ; void *
	//.text:00000000006227FD call    memmove
	g_HookManager.WriteCall(0x6227FD, CIOBufferExt::MemMove);
//CCommSocket::SendV
	//.text:00000000004BB898 lea     rcx, [rax+12h]
	g_HookManager.WriteJump(0x4BB898, ReallocateIOBuffer62, 2);
	//.text:00000000004BB8A2 mov     edx, 1FFEh
	g_HookManager.WriteMemoryDWORD(0x4BB8A3, 16382);
	//.text:00000000004BB8EC mov     [rdi+10h], bl
	g_HookManager.WriteJump(0x4BB8EC, ReallocateIOBuffer63, 19);
	//.text:00000000004BB904 mov     [rdi+10h], bl
	g_HookManager.WriteJump(0x4BB904, ReallocateIOBuffer64, 6);
//CDBSocket::SendV
	//.text:000000000054A8E2 mov     edx, 1FFEh
	g_HookManager.WriteJump(0x54A8E2, ReallocateIOBuffer65, 4);
	//.text:000000000054A930 movzx   esi, byte ptr [rbx+12h]
	g_HookManager.WriteJump(0x54A930, ReallocateIOBuffer66, 5);
	//.text:000000000054A96C mov     eax, [rbx+17h]
	g_HookManager.WriteJump(0x54A96C, ReallocateIOBuffer73, 4);
	//.text:000000000054A978 mov     [rbx+10h], dil
	g_HookManager.WriteJump(0x54A978, ReallocateIOBuffer67, 21);
	//.text:000000000054A992 mov     [rbx+10h], dil
	g_HookManager.WriteJump(0x54A992, ReallocateIOBuffer68, 7);
//CLogSocket::Send
	//.text:000000000064EF9C lea     rcx, [rax+12h]
	g_HookManager.WriteJump(0x64EF9C, ReallocateIOBuffer69, 9);
	//.text:000000000064EFB6 mov     [rbx+10h], al
	g_HookManager.WriteJump(0x64EFB6, ReallocateIOBuffer70, 6);
//CPetitionSocket::Send
	//.text:000000000075E1F8 lea     rcx, [rax+12h]
	g_HookManager.WriteJump(0x75E1F8, ReallocateIOBuffer71, 12);
	//.text:000000000075E243 lea     eax, [rbx-3]
	g_HookManager.WriteJump(0x75E243, ReallocateIOBuffer72, 10);

//Incomming packets IOBuffer
//CIOSocket::Read
	//.text:0000000000620794 call    memmove
	g_HookManager.WriteCall(0x620794, CIOBufferExt::MemMove);
	//.text:0000000000620815 call    memmove
	g_HookManager.WriteCall(0x620815, CIOBufferExt::MemMove);
	//.text:0000000000620867 mov     eax, 2000h
	g_HookManager.WriteMemoryDWORD(0x620868, 0x4000);
	//.text:000000000062087E lea     rcx, [rax+r11+10h]
	g_HookManager.WriteJump(0x62087E, ReallocateIOBuffer7);
//CCommSocket::OnRead
	//;.text:00000000004BBABF movzx   ecx, byte ptr [rax+r12+11h]
	g_HookManager.WriteJump(0x4BBABF, ReallocateIOBuffer8, 10);
	//.text:00000000004BBAE0 cmp     edx, 2000h
	g_HookManager.WriteMemoryDWORD(0x4BBAE2, 0x4000);
	//.text:00000000004BBB24 cmp     byte ptr [rax+r12+10h], 18h
	g_HookManager.WriteJump(0x4BBB24, ReallocateIOBuffer9, 7);
	//;.text:00000000004BBBC1 movzx   edx, byte ptr [rax+r12+10h]
	g_HookManager.WriteJump(0x4BBBC1, ReallocateIOBuffer10, 1);
	//.text:00000000004BBCB0 movzx   r9d, byte ptr [rax+r12+10h]
	g_HookManager.WriteJump(0x4BBCB0, ReallocateIOBuffer11, 1);
//void CDBSocket::OnRead
	//.text:000000000054D8EF movzx   ecx, byte ptr [rax+r12+11h]
	g_HookManager.WriteJump(0x54D8EF, ReallocateIOBuffer12, 10);
	//.text:000000000054D910 cmp     edx, 2000h
	g_HookManager.WriteMemoryDWORD(0x54D912, 0x4000);
	//.text:000000000054D957 cmp     byte ptr [rax+r12+10h], 0Dh
	g_HookManager.WriteJump(0x54D957, ReallocateIOBuffer13, 7);
	//.text:000000000054D96F cmp     byte ptr [rax+r12+10h], 0D9h
	g_HookManager.WriteJump(0x54D96F, ReallocateIOBuffer14, 7);
	//.text:000000000054DA0C movzx   edx, byte ptr [rax+r12+10h]
	g_HookManager.WriteJump(0x54DA0C, ReallocateIOBuffer15, 1);
	//.text:000000000054DB01 movzx   r9d, byte ptr [rax+r12+10h]
	g_HookManager.WriteJump(0x54DB01, ReallocateIOBuffer16, 1);
	//.text:000000000054DB2E movzx   r9d, byte ptr [rax+r12+10h]
	g_HookManager.WriteJump(0x54DB2E, ReallocateIOBuffer17, 1);
//CPacket1::sub_621280
	//.text:000000000062131C lea     rdx, [r12+rax+11h]
	g_HookManager.WriteJump(0x62131C, ReallocateIOBuffer18);
//void CLogSocket::OnRead
	//.text:000000000064EA6A movzx   ecx, byte ptr [rax+r12+11h]
	g_HookManager.WriteJump(0x64EA6A, ReallocateIOBuffer19, 10);
	//.text:000000000064EA8B cmp     edx, 2000h
	g_HookManager.WriteMemoryDWORD(0x64EA8D, 0x4000);
	//text:000000000064EAC6 cmp     byte ptr [r13+r12+10h], 3
	g_HookManager.WriteJump(0x64EAC6, ReallocateIOBuffer20, 7);
	//.text:000000000064EB52 movzx   edx, byte ptr [r13+r12+10h]
	g_HookManager.WriteJump(0x64EB52, ReallocateIOBuffer21, 1);
	//;.text:000000000064EC59 movzx   r9d, byte ptr [rax+r12+10h]
	g_HookManager.WriteJump(0x64EC59, ReallocateIOBuffer22, 1);
//CPacket2::sub_6211C0
	//.text:00000000006211D4 lea     rdx, [rdx+rax+11h]
	g_HookManager.WriteJump(0x6211D4, ReallocateIOBuffer23);
//NpcSocket::OnRead
	//.text:00000000006D479A movzx   ecx, byte ptr [rax+r12+11h]
	g_HookManager.WriteJump(0x6D479A, ReallocateIOBuffer24, 10);
	//.text:00000000006D47BB cmp     edx, 2000h
	g_HookManager.WriteMemoryDWORD(0x6D47BD, 0x4000);
	//.text:00000000006D47FC cmp     byte ptr [r13+r12+10h], 7Ch
	g_HookManager.WriteJump(0x6D47FC, ReallocateIOBuffer25, 7);
	//.text:00000000006D4887 movzx   ecx, byte ptr [r13+r12+10h]
	g_HookManager.WriteJump(0x6D4887, ReallocateIOBuffer26, 29);
	//.text:00000000006D4A45 movzx   r9d, byte ptr [rax+r12+10h]
	g_HookManager.WriteJump(0x6D4A45, ReallocateIOBuffer27, 1);
//CPacket3::sub_6214C0
	//.text:000000000062155C lea     rdx, [r12+rax+11h]
	g_HookManager.WriteJump(0x62155C, ReallocateIOBuffer28);
//void CPetitionSocket::OnRead
	//.text:000000000075DE0A movzx   ecx, byte ptr [rax+r12+11h]
	g_HookManager.WriteJump(0x75DE0A, ReallocateIOBuffer29, 10);
	//.text:000000000075DE2B cmp     edx, 2000h
	g_HookManager.WriteMemoryDWORD(0x75DE2D, 0x4000);
	//.text:000000000075DE66 cmp     byte ptr [r13+r12+10h], 0ACh
	g_HookManager.WriteJump(0x75DE66, ReallocateIOBuffer30, 7);
	//.text:000000000075DEF2 movzx   edx, byte ptr [r13+r12+10h]
	g_HookManager.WriteJump(0x75DEF2, ReallocateIOBuffer31, 1);
	//.text:000000000075DFFD movzx   r9d, byte ptr [rax+r12+10h]
	g_HookManager.WriteJump(0x75DFFD, ReallocateIOBuffer32, 1);
//CUserSocket::OnRead
	//.text:0000000000877014 lea     r13, [rax+10h]
	g_HookManager.WriteJump(0x87700D, ReallocateIOBuffer33, 6);
	//.text:000000000087709A cmp     edx, 2000h
	g_HookManager.WriteMemoryDWORD(0x87709C, 0x4000);
}

void CIOBufferExt::MemMove(UINT64 pDest, UINT64 pSrc, UINT64 size)
{
	pDest += 0x2038;
	pSrc += 0x2038;
	typedef void(*f)(UINT64, UINT64, UINT64);
	f(0x916100L)(pDest, pSrc, size);
}

LPVOID CIOBufferExt::Malloc(UINT64 size)
{
	typedef LPVOID (*f)(UINT64);
	LPVOID pRet = f(0x9164A0L)( size );
	int tries = 15;
	while(pRet == NULL && tries > 0)
	{
		tries--;
		if(pRet == NULL)
			g_Log.Add(CLog::Error, "[%s] Cannot allocate memory for IOBuffer [%d] size. - trying [%d]", __FUNCTION__, size, (15-tries));

		Sleep(50);
		pRet = f(0x9164A0L)( size );
	}

	return pRet;
}
