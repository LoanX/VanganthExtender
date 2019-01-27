
GetTickCount PROTO

.data

.code

ReallocateIOBuffer1 proc
;.text:0000000000620B62 xor     ecx, ecx
;.text:0000000000620B64 lea     rax, [rbx+10h]
;.text:0000000000620B68 mov     [rsp+68h+var_38], rcx
	xor ecx, ecx
	lea rax, [rbx+2048h]
	mov r8, 620B68h
	jmp r8
ReallocateIOBuffer1 endp

ReallocateIOBuffer2 proc
;.text:00000000007D592A lea     rax, [rbp+10h]	;hook here
;.text:00000000007D592E mov     [rsp+58h], rax
;.text:00000000007D5933 lea     rax, [rdi+70h]	;jump back here

	lea rax, [rbp+2048h]
	mov [rsp+58h], rax
	mov rax, 7D5933h
	jmp rax
ReallocateIOBuffer2 endp

ReallocateIOBuffer3 proc
;.text:0000000000622299 lea     rax, [rcx+10h]	;hook here
;.text:000000000062229D mov     r8d, 3BBh
;.text:00000000006222A3 mov     [rsp+468h+Buffers.buf], rax
;.text:00000000006222A8 mov     rax, [rbx]	;jump back here
	lea rax, [rcx+2048h]
	mov r8d, 3BBh
	mov [rsp+50h], rax
	mov rax, 6222A8h
	jmp rax
ReallocateIOBuffer3 endp

ReallocateIOBuffer4 proc
;.text:00000000004BB8D6 add     ebx, 2			;hook here
;.text:00000000004BB8EC mov     [rdi+10h], bl	
;.text:00000000004BB8EF mov     ecx, ebx
;.text:00000000004BB8F1 sar     ecx, 8
;.text:00000000004BB8F4 mov     [rdi+11h], cl
;.text:00000000004BB8F7 movsxd  rax, ebx
;.text:00000000004BB8FA mov     [rax+rdi+0Eh], bl
;.text:00000000004BB8FE mov     [rax+rdi+0Fh], cl
;.text:00000000004BB902 jmp     short loc_4BB90F
	
	add ebx, 2
	mov [rdi+2048h], bl
	mov ecx, ebx
	sar ecx, 8
	mov [rdi+2049h], cl
	movsxd rax, ebx
	mov [rax+rdi+2046h], bl
	mov [rax+rdi+2047h], cl
	mov rcx, 4BB90Fh
	jmp rcx
ReallocateIOBuffer4 endp

ReallocateIOBuffer5 proc
;.text:000000000054A930 movzx   esi, byte ptr [rbx+12h]
;.text:000000000054A934 cmp     sil, 7Dh                        ; RequestPing
;.text:000000000054A938 jz      short loc_54A946
;.text:000000000054A93A cmp     sil, 7Eh                        ; RequestPing2
;.text:000000000054A93E jz      short loc_54A946
;.text:000000000054A940 cmp     sil, 7Fh                        ; RequestPing3
;.text:000000000054A944 jnz     short loc_54A975
;.text:000000000054A946
;.text:000000000054A946 loc_54A946:                             ; CODE XREF: CDBSocket__SendV+128j
;.text:000000000054A946                                         ; CDBSocket__SendV+12Ej
;.text:000000000054A946 call    cs:GetTickCount
;.text:000000000054A94C mov     [rbx+2038h], eax
;.text:000000000054A952 mov     dword ptr [rbx+203Ch], 1
;.text:000000000054A95C movzx   eax, sil
;.text:000000000054A960 mov     [rbx+2040h], eax
;.text:000000000054A966 cmp     sil, 7Fh
;.text:000000000054A96A jnz     short loc_54A975
;.text:000000000054A96C mov     eax, [rbx+17h]
;.text:000000000054A96F mov     [rbx+2044h], eax
;.text:000000000054A975
;.text:000000000054A975 loc_54A975:                             ; CODE XREF: CDBSocket__SendV+134j
;.text:000000000054A975                                         ; CDBSocket__SendV+15Aj
;.text:000000000054A975 add     edi, 2
;.text:000000000054A978 mov     [rbx+10h], dil
;.text:000000000054A97C mov     ecx, edi
;.text:000000000054A97E sar     ecx, 8
;.text:000000000054A981 mov     [rbx+11h], cl
;.text:000000000054A984 movsxd  rax, edi
;.text:000000000054A987 mov     [rax+rbx+0Eh], dil
;.text:000000000054A98C mov     [rax+rbx+0Fh], cl
;.text:000000000054A990 jmp     short loc_54A99E

	movzx esi, byte ptr[rbx+204Ah]
	cmp sil, 7Dh
	jz RequestPing
	cmp sil, 7Eh
	jz RequestPing
	cmp sil, 7Fh
	jnz FillSizeOffset
	
	RequestPing:
	call GetTickCount
	mov [rbx+2038h], eax
	mov dword ptr[rbx+203Ch], 1
	movzx eax, sil
	mov [rbx+2040h], eax
	cmp sil, 7Fh
	jnz FillSizeOffset
	mov eax, [rbx+204Fh]
	mov [rbx+2044h], eax
	
	FillSizeOffset:
	add edi, 2
	mov [rbx+2048h], dil
	mov ecx, edi
	sar ecx, 8
	mov [rbx+2049h], cl
	movsxd rax, edi
	mov [rax+rbx+2046h], dil
	mov [rax+rbx+2047h], cl
	mov rcx, 54A99Eh
	jmp rcx
	
ReallocateIOBuffer5 endp

ReallocateIOBuffer6 proc
;.text:0000000000622671 lea     rax, [rbx+10h]
;.text:0000000000622675 mov     [rsp+50h], rax
;.text:000000000062267A lea     rax, [rdi+70h]
	lea rax, [rbx+2048h]
	mov [rsp+50h], rax
	mov rax, 62267Ah
	jmp rax
ReallocateIOBuffer6 endp

;.text:000000000062087E lea     rcx, [rax+r11+10h]
;.text:0000000000620883 mov     [rsp+88h+Buffers.buf], rcx

ReallocateIOBuffer7 proc
	lea rcx, [rax+r11+2048h]
	mov r8, 620883h
	jmp r8
ReallocateIOBuffer7 endp

;.text:00000000004BBABF movzx   ecx, byte ptr [rax+r12+11h]
;.text:00000000004BBAC5 shl     ecx, 8
;.text:00000000004BBAC8 movzx   eax, byte ptr [rax+r12+10h]
;.text:00000000004BBACE lea     edx, [rcx+rax-2]


ReallocateIOBuffer8 proc
	movzx ecx, byte ptr[rax+r12+2049h]
	shl ecx, 8
	movzx eax, byte ptr [rax+r12+2048h]
	mov rdx, 4BBACEh
	jmp rdx
ReallocateIOBuffer8 endp

;.text:00000000004BBB24 cmp     byte ptr [rax+r12+10h], 18h
;.text:00000000004BBB2A jnb     loc_4BBCAD
;.text:00000000004BBB30
;.text:00000000004BBB30 loc_4BBB30:                             ; CODE XREF: CCommSocket__OnRead+16Fj
;.text:00000000004BBB30 lea     r9, aCpacket_0 


ReallocateIOBuffer9 proc
	cmp byte ptr [rax+r12+2048h], 18h
	jnb NotPacket
	mov r9, 4BBB30h
	jmp r9
	
	NotPacket:
		mov r9, 4BBCADh
		jmp r9
		
ReallocateIOBuffer9 endp

;.text:00000000004BBBC1 movzx   edx, byte ptr [rax+r12+10h]
;.text:00000000004BBBC7 mov     rax, [rbx+0B8h]
ReallocateIOBuffer10 proc
	movzx edx, byte ptr[rax+r12+2048h]
	mov rax, 4BBBC7h
	jmp rax
ReallocateIOBuffer10 endp

;.text:00000000004BBCB0 movzx   r9d, byte ptr [rax+r12+10h]
;.text:00000000004BBCB6 lea     r8, aCommsocketUnkn             ; "[commsocket]unknown protocol %d

ReallocateIOBuffer11 proc
	movzx r9d, byte ptr[rax+r12+2048h]
	mov r8, 4BBCB6h
	jmp r8
ReallocateIOBuffer11 endp

;.text:000000000054D8EF movzx   ecx, byte ptr [rax+r12+11h]
;.text:000000000054D8F5 shl     ecx, 8
;.text:000000000054D8F8 movzx   eax, byte ptr [rax+r12+10h]
;.text:000000000054D8FE lea     edx, [rcx+rax-2]

ReallocateIOBuffer12 proc
	movzx ecx, byte ptr[rax+r12+2049h]
	shl ecx, 8
	movzx eax, byte ptr[rax+r12+2048h]
	mov rdx, 54D8FEh
	jmp rdx	
ReallocateIOBuffer12 endp

;.text:000000000054D957 cmp     byte ptr [rax+r12+10h], 0Dh
;.text:000000000054D95D jnb     loc_54DAFE
;.text:000000000054D963
;.text:000000000054D963 loc_54D963:                             ; CODE XREF: CDBSocket__OnRead+172j
;.text:000000000054D963 cmp     rcx, cs:off_BF0338
ReallocateIOBuffer13 proc
	cmp byte ptr[rax+r12+2048h], 0Dh
	jnb NotBelow
	mov r9, 54D963h
	jmp r9
	
	NotBelow:
		mov r9, 54DAFEh
		jmp r9
	
ReallocateIOBuffer13 endp

;.text:000000000054D96F cmp     byte ptr [rax+r12+10h], 0D9h
;.text:000000000054D975 jnb     loc_54DB2B
;.text:000000000054D97B
;.text:000000000054D97B loc_54D97B:                             ; CODE XREF: CDBSocket__OnRead+18Aj
;.text:000000000054D97B lea     r9, aCpacket_0 
ReallocateIOBuffer14 proc
	cmp byte ptr[rax+r12+2048h], 0D9h
	jnb NotBelow
	mov r9, 54D97Bh
	jmp r9
	
	NotBelow:
		mov r9, 54DB2Bh
		jmp r9
	
ReallocateIOBuffer14 endp

;.text:000000000054DA0C movzx   edx, byte ptr [rax+r12+10h]
;.text:000000000054DA12 mov     rax, [rdi+0B8h]
ReallocateIOBuffer15 proc
	movzx edx, byte ptr [rax+r12+2048h]
	mov rax, 54DA12h
	jmp rax
ReallocateIOBuffer15 endp

;.text:000000000054DB01 movzx   r9d, byte ptr [rax+r12+10h]
;.text:000000000054DB07 lea     r8, aAuthUnknownPro             ; "[auth]unknown protocol %d"
ReallocateIOBuffer16 proc
	movzx r9d, byte ptr[rax+r12+2048h]
	mov r8, 54DB07h
	jmp r8
ReallocateIOBuffer16 endp

;.text:000000000054DB2E movzx   r9d, byte ptr [rax+r12+10h]
;.text:000000000054DB34 lea     r8, aDbsocketUnknow             ; "[dbsocket]unknown protocol %d"
ReallocateIOBuffer17 proc
	movzx r9d, byte ptr[rax+r12+2048h]
	mov r8, 54DB34h
	jmp r8
ReallocateIOBuffer17 endp

;.text:000000000062131C lea     rdx, [r12+rax+11h]
;.text:0000000000621321 mov     rcx, [rsi+18h]
ReallocateIOBuffer18 proc
	lea rdx, [r12+rax+2049h]
	mov rcx, 621321h
	jmp rcx
ReallocateIOBuffer18 endp

;.text:000000000064EA6A movzx   ecx, byte ptr [rax+r12+11h]
;.text:000000000064EA70 shl     ecx, 8
;.text:000000000064EA73 movzx   eax, byte ptr [rax+r12+10h]
;.text:000000000064EA79 lea     edx, [rcx+rax-2]
ReallocateIOBuffer19 proc
	movzx ecx, byte ptr[rax+r12+2049h]
	shl ecx, 8
	movzx eax, byte ptr[rax+r12+2048h]
	mov rdx, 64EA79h
	jmp rdx
ReallocateIOBuffer19 endp

;.text:000000000064EAC6 cmp     byte ptr [r13+r12+10h], 3
;.text:000000000064EACC jnb     loc_64EC56
;.text:000000000064EAD2 lea     r9, aCpacket_0                  ; "CPacket"
ReallocateIOBuffer20 proc
	cmp byte ptr[r13+r12+2048h], 3
	jnb NotBelow
	mov r9, 64EAD2h
	jmp r9
	NotBelow:
		mov r9, 64EC56h
		jmp r9
ReallocateIOBuffer20 endp

;.text:000000000064EB52 movzx   edx, byte ptr [r13+r12+10h]
;.text:000000000064EB58 mov     rax, [rbx+0B8h]
ReallocateIOBuffer21 proc
	movzx edx, byte ptr[r13+r12+2048h]
	mov rax, 64EB58h
	jmp rax
ReallocateIOBuffer21 endp

;.text:000000000064EC59 movzx   r9d, byte ptr [rax+r12+10h]
;.text:000000000064EC5F lea     r8, aLogsocketUnkno             ; "[Logsocket]unknown protocol %d"
ReallocateIOBuffer22 proc
	movzx r9d, byte ptr[rax+r12+2048h]
	mov r8, 64EC5Fh
	jmp r8
ReallocateIOBuffer22 endp

;.text:00000000006211D4 lea     rdx, [rdx+rax+11h]
;.text:00000000006211D9 mov     rax, [rbx+28h]
ReallocateIOBuffer23 proc
	lea rdx, [rdx+rax+2049h]
	mov rax, 6211D9h
	jmp rax
ReallocateIOBuffer23 endp

;.text:00000000006D479A movzx   ecx, byte ptr [rax+r12+11h]
;.text:00000000006D47A0 shl     ecx, 8
;.text:00000000006D47A3 movzx   eax, byte ptr [rax+r12+10h]
;.text:00000000006D47A9 lea     edx, [rcx+rax-2]
ReallocateIOBuffer24 proc
	movzx ecx, byte ptr[rax+r12+2049h]
	shl ecx, 8
	movzx eax, byte ptr[rax+r12+2048h]
	mov rdx, 6D47A9h
	jmp rdx
ReallocateIOBuffer24 endp

;.text:00000000006D47FC cmp     byte ptr [r13+r12+10h], 7Ch
;.text:00000000006D4802 jnb     loc_6D4A42
;.text:00000000006D4808 lea     r9, aCpacket_0                  ; "CPacket"
ReallocateIOBuffer25 proc
	cmp byte ptr[r13+r12+2048h], 7Ch
	jnb NotBelow
	mov r9, 6D4808h
	jmp r9
	NotBelow:
		mov r9, 6D4A42h
		jmp r9
ReallocateIOBuffer25 endp

;.text:00000000006D4887 movzx   ecx, byte ptr [r13+r12+10h]
;.text:00000000006D488D mov     rax, [rbx+100h]
;.text:00000000006D4894 mov     rcx, [rax+rcx*8]
;.text:00000000006D4898 mov     [rdi+28h], rcx
;.text:00000000006D489C mov     r15d, [rbx+110h]
;.text:00000000006D48A3 movzx   r13d, byte ptr [r13+r12+10h]
;.text:00000000006D48A9 mov     eax, cs:83EF150h
ReallocateIOBuffer26 proc
	movzx ecx, byte ptr[r13+r12+2048h]
	mov rax, [rbx+100h]
	mov rcx, [rax+rcx*8]
	mov [rdi+28h], rcx
	mov r15d, [rbx+110h]
	movzx r13d, byte ptr[r13+r12+2048h]
	mov rax, 6D48A9h
	jmp rax
ReallocateIOBuffer26 endp

;.text:00000000006D4A45 movzx   r9d, byte ptr [rax+r12+10h]
;.text:00000000006D4A4B lea     r8, aNpcsocketUnkno             ; "npcsocket: unknown protocol %d"
ReallocateIOBuffer27 proc
	movzx r9d, byte ptr[rax+r12+2048h]
	mov r8, 6D4A4Bh
	jmp r8
ReallocateIOBuffer27 endp

;.text:000000000062155C lea     rdx, [r12+rax+11h]
;.text:0000000000621561 mov     rcx, [rsi+18h]
ReallocateIOBuffer28 proc
	lea rdx, [r12+rax+2049h]
	mov rcx, 621561h
	jmp rcx
ReallocateIOBuffer28 endp

;.text:000000000075DE0A movzx   ecx, byte ptr [rax+r12+11h]
;.text:000000000075DE10 shl     ecx, 8
;.text:000000000075DE13 movzx   eax, byte ptr [rax+r12+10h]
;.text:000000000075DE19 lea     edx, [rcx+rax+1]
ReallocateIOBuffer29 proc
	movzx ecx, byte ptr[rax+r12+2049h]
	shl ecx, 8
	movzx eax, byte ptr[rax+r12+2048h]
	mov rdx, 75DE19h
	jmp rdx
ReallocateIOBuffer29 endp

;.text:000000000075DE66 cmp     byte ptr [r13+r12+10h], 0ACh
;.text:000000000075DE6C jnb     loc_75DFFA
;.text:000000000075DE72 lea     r9, aCpacket_0                  ; "CPacket"
ReallocateIOBuffer30 proc
	cmp byte ptr[r13+r12+2048h], 0ACh
	jnb NotBelow
	mov r9, 75DE72h
	jmp r9
	NotBelow:
		mov r9, 75DFFAh
		jmp r9
ReallocateIOBuffer30 endp

;.text:000000000075DEF2 movzx   edx, byte ptr [r13+r12+10h]
;.text:000000000075DEF8 mov     rax, [rbx+0B8h]
ReallocateIOBuffer31 proc
	movzx edx, byte ptr[r13+r12+2048h]
	mov rax, 75DEF8h
	jmp rax
ReallocateIOBuffer31 endp

;.text:000000000075DFFD movzx   r9d, byte ptr [rax+r12+10h]
;.text:000000000075E003 lea     r8, aPetitionSocket             ; "[petition socket]unknown protocol %d"
ReallocateIOBuffer32 proc
	movzx r9d, byte ptr[rax+r12+2048h]
	mov r8, 75E003h
	jmp r8
ReallocateIOBuffer32 endp

;.text:000000000087700D mov     r15d, [rax+2010h]
;.text:0000000000877014 lea     r13, [rax+10h]
;.text:0000000000877018 cmp     dword ptr [rsi+10Ch], 2
ReallocateIOBuffer33 proc
	mov r15d, [rax+2010h]
	lea r13, [rax+2048h]
	mov rcx, 877018h
	jmp rcx
ReallocateIOBuffer33 endp

;.text:00000000007D5E0F lea     rcx, [rax+12h]
;.text:00000000007D5E13 mov     r9, rsi
;.text:00000000007D5E16 mov     r8, rbp
ReallocateIOBuffer34 proc
	lea rcx, [rax+204Ah]
	mov r9, rsi
	mov r8, 7D5E16h
	jmp r8
ReallocateIOBuffer34 endp

;.text:00000000007D5E5B mov     [rbx+10h], r11b
;.text:00000000007D5E5F mov     eax, r11d
;.text:00000000007D5E62 sar     eax, 8
;.text:00000000007D5E65 mov     [rbx+11h], al
;.text:00000000007D5E68 mov     [rbx+2010h], r11d
;.text:00000000007D5E6F lea     rcx, [rdi+28h]
ReallocateIOBuffer35 proc
	mov [rbx+2048h], r11b
	mov eax, r11d
	sar eax, 8
	mov [rbx+2049h], al
	mov [rbx+2010h], r11d
	mov rcx, 7D5E6Fh
	jmp rcx
ReallocateIOBuffer35 endp

;.text:0000000000620B64 lea     rax, [rbx+10h]
;.text:0000000000620B68 mov     [rsp+30h], rcx
;.text:0000000000620B6D mov     [rsp+50h], rax
;.text:0000000000620B72 lea     rax, [rdi+70h]

ReallocateIOBuffer36 proc
	lea rax, [rbx+2048h]
	mov [rsp+30h], rcx
	mov [rsp+50h], rax
	mov rax, 620B72h
	jmp rax
ReallocateIOBuffer36 endp

;.text:0000000000403F1E lea     rcx, [rax+12h]
;.text:0000000000403F22 mov     r9, rdi
;.text:0000000000403F25 mov     r8, rbx
ReallocateIOBuffer37 proc
	lea rcx, [rax+204Ah]
	mov r9, rdi
	mov r8, 403F25h
	jmp r8
ReallocateIOBuffer37 endp

;.text:0000000000403F50 mov     [rsi+10h], r11b
;.text:0000000000403F54 mov     eax, r11d
;.text:0000000000403F57 sar     eax, 8
;.text:0000000000403F5A mov     [rsi+11h], al
;.text:0000000000403F5D mov     [rsi+2010h], r11d
;.text:0000000000403F64 jmp     loc_404039
ReallocateIOBuffer38 proc
	mov [rsi+2048h], r11b
	mov eax, r11d
	sar eax, 8
	mov [rsi+2049h], al
	mov [rsi+2010h], r11d
	mov rax, 404039h
	jmp rax
ReallocateIOBuffer38 endp

;.text:00000000007D58B1 movzx   ecx, word ptr [rbx+10h]
;.text:00000000007D58B5 mov     [rax+10h], cx
;.text:00000000007D58B9 mov     ecx, [rbx+2010h]
;.text:00000000007D58BF mov     [rax+2010h], ecx
;.text:00000000007D58C5 mov     r9d, [rbx+2010h]
;.text:00000000007D58CC sub     r9d, 2
;.text:00000000007D58D0 lea     rdx, [rbx+12h]
;.text:00000000007D58D4 lea     rcx, [rax+12h]
;.text:00000000007D58D8 lea     r8, [rdi+128h]
ReallocateIOBuffer39 proc
	movzx ecx, word ptr[rbx+2048h]
	mov [rax+2048h], cx
	mov ecx, [rbx+2010h]
	mov [rax+2010h], ecx
	mov r9d, [rbx+2010h]
	sub r9d, 2
	lea rdx, [rbx+204Ah]
	lea rcx, [rax+204Ah]
	mov r8, 7D58D8h
	jmp r8
ReallocateIOBuffer39 endp

;.text:00000000007D592A lea     rax, [rbp+10h]
;.text:00000000007D592E mov     [rsp+58h], rax
;.text:00000000007D5933 lea     rax, [rdi+70h]
ReallocateIOBuffer40 proc
	lea rax, [rbp+2048h]
	mov [rsp+58h], rax
	mov rax, 7D5933h
	jmp rax
ReallocateIOBuffer40 endp

;.text:00000000007D5A4D movzx   eax, word ptr [rbx+10h]
;.text:00000000007D5A51 mov     [rcx+r9+10h], ax
;.text:00000000007D5A57 mov     rcx, [rdi+0A0h]
;.text:00000000007D5A5E mov     r9d, [rbx+2010h]
;.text:00000000007D5A65 sub     r9d, 2
;.text:00000000007D5A69 lea     rdx, [rbx+12h]
;.text:00000000007D5A6D mov     eax, [rcx+2010h]
;.text:00000000007D5A73 lea     rcx, [rax+rcx+12h]
;.text:00000000007D5A78 call    sub_7D4D90
ReallocateIOBuffer41 proc
	movzx eax, word ptr[rbx+2048h]
	mov [rcx+r9+2048h], ax
	mov rcx, [rdi+0A0h]
	mov r9d, [rbx+2010h]
	sub r9d, 2
	lea rdx, [rbx+204Ah]
	mov eax, [rcx+2010h]
	lea rcx, [rax+rcx+204Ah]
	mov rax, 7D5A78h
	jmp rax
ReallocateIOBuffer41 endp

;.text:00000000007D5AB7 movzx   ecx, word ptr [rbx+10h]
;.text:00000000007D5ABB mov     [rax+10h], cx
;.text:00000000007D5ABF mov     ecx, [rbx+2010h]
;.text:00000000007D5AC5 mov     [rax+2010h], ecx
;.text:00000000007D5ACB mov     r9d, [rbx+2010h]
;.text:00000000007D5AD2 sub     r9d, 2
;.text:00000000007D5AD6 lea     rdx, [rbx+12h]
;.text:00000000007D5ADA lea     rcx, [rax+12h]
;.text:00000000007D5ADE lea     r8, [rdi+128h]
ReallocateIOBuffer42 proc
	movzx ecx, word ptr[rbx+2048h]
	mov [rax+2048h], cx
	mov ecx, [rbx+2010h]
	mov [rax+2010h], ecx
	mov r9d, [rbx+2010h]
	sub r9d, 2
	lea rdx, [rbx+204Ah]
	lea rcx, [rax+204Ah]
	mov r8, 7D5ADEh
	jmp r8
ReallocateIOBuffer42 endp

;.text:00000000007D4E24 lea     rdx, [rbx+10h]                  ; void *
;.text:00000000007D4E28 lea     rcx, [rax+10h]                  ; void *
;.text:00000000007D4E2C mov     rdi, rax
ReallocateIOBuffer43 proc
	lea rdx, [rbx+2048h]
	lea rcx, [rax+2048h]
	mov rdi, 7D4E2Ch
	jmp rdi
ReallocateIOBuffer43 endp

;.text:000000000047490A lea     rcx, [rax+12h]
;.text:000000000047490E mov     r9, rdi
;.text:0000000000474911 mov     r8, rbx
ReallocateIOBuffer44 proc
	lea rcx, [rax+204Ah]
	mov r9, rdi
	mov r8, 474911h
	jmp r8
ReallocateIOBuffer44 endp

;.text:000000000047493E mov     [r12+10h], r11b
;.text:0000000000474943 mov     eax, r11d
;.text:0000000000474946 sar     eax, 8
;.text:0000000000474949 mov     [r12+11h], al
;.text:000000000047494E mov     [r12+2010h], r11d
;.text:0000000000474956 jmp     loc_474A31
;.text:000000000047495B ; ---------------------------------------------------------------------------
ReallocateIOBuffer45 proc
	mov [r12+2048h], r11b
	mov eax, r11d
	sar eax, 8
	mov [r12+2049h], al
	mov [r12+2010h], r11d
	mov rax, 474A31h
	jmp rax
ReallocateIOBuffer45 endp

;.text:000000000047557C lea     rcx, [rax+12h]
;.text:0000000000475580 mov     r9, rdi
;.text:0000000000475583 mov     r8, rbx
ReallocateIOBuffer46 proc
	lea rcx, [rax+204Ah]
	mov r9, rdi
	mov r8, 475583h
	jmp r8
ReallocateIOBuffer46 endp

;.text:00000000004755AF mov     [r13+10h], r11b
;.text:00000000004755B3 mov     eax, r11d
;.text:00000000004755B6 sar     eax, 8
;.text:00000000004755B9 mov     [r13+11h], al
;.text:00000000004755BD mov     [r13+2010h], r11d
;.text:00000000004755C4 jmp     loc_47569F
;.text:00000000004755C9 ; ---------------------------------------------------------------------------
ReallocateIOBuffer47 proc
	mov [r13+2048h], r11b
	mov eax, r11d
	sar eax, 8
	mov [r13+2049h], al
	mov [r13+2010h], r11d
	mov rax, 47569Fh
	jmp rax
ReallocateIOBuffer47 endp

;.text:00000000004B005E lea     rcx, [rax+12h]
;.text:00000000004B0062 mov     r9, rdi
;.text:00000000004B0065 mov     r8, rbx
ReallocateIOBuffer48 proc
	lea rcx, [rax+204Ah]
	mov r9, rdi
	mov r8, 4B0065h
	jmp r8
ReallocateIOBuffer48 endp

;.text:00000000004B0090 mov     [rsi+10h], r11b
;.text:00000000004B0094 mov     eax, r11d
;.text:00000000004B0097 sar     eax, 8
;.text:00000000004B009A mov     [rsi+11h], al
;.text:00000000004B009D mov     [rsi+2010h], r11d
;.text:00000000004B00A4 jmp     loc_4B0179
;.text:00000000004B00A9 ; ---------------------------------------------------------------------------
ReallocateIOBuffer49 proc
	mov [rsi+2048h], r11b
	mov eax, r11d
	sar eax, 8
	mov [rsi+2049h], al
	mov [rsi+2010h], r11d
	mov rax, 4B0179h
	jmp rax
ReallocateIOBuffer49 endp

;.text:00000000004E815A lea     rcx, [rax+12h]
;.text:00000000004E815E mov     r9, rdi
;.text:00000000004E8161 mov     r8, rbx
ReallocateIOBuffer50 proc
	lea rcx, [rax+204Ah]
	mov r9, rdi
	mov r8, 4E8161h
	jmp r8
ReallocateIOBuffer50 endp

;.text:00000000004E818E mov     [r12+10h], r11b
;.text:00000000004E8193 mov     eax, r11d
;.text:00000000004E8196 sar     eax, 8
;.text:00000000004E8199 mov     [r12+11h], al
;.text:00000000004E819E mov     [r12+2010h], r11d
;.text:00000000004E81A6 jmp     loc_4E8281
;.text:00000000004E81AB ; ---------------------------------------------------------------------------
ReallocateIOBuffer51 proc
	mov [r12+2048h], r11b
	mov eax, r11d
	sar eax, 8
	mov [r12+2049h], al
	mov [r12+2010h], r11d
	mov rax, 4E8281h
	jmp rax
ReallocateIOBuffer51 endp

;.text:00000000004E8DED lea     rcx, [rax+12h]
;.text:00000000004E8DF1 mov     r9, rdi
;.text:00000000004E8DF4 mov     r8, rbx
ReallocateIOBuffer52 proc
	lea rcx, [rax+204Ah]
	mov r9, rdi
	mov r8, 4E8DF4h
	jmp r8
ReallocateIOBuffer52 endp

;.text:00000000004E8E1F mov     [rsi+10h], r11b
;.text:00000000004E8E23 mov     eax, r11d
;.text:00000000004E8E26 sar     eax, 8
;.text:00000000004E8E29 mov     [rsi+11h], al
;.text:00000000004E8E2C mov     [rsi+2010h], r11d
;.text:00000000004E8E33 jmp     loc_4E8F08
;.text:00000000004E8E38 ; ---------------------------------------------------------------------------
ReallocateIOBuffer53 proc
	mov [rsi+2048h], r11b
	mov eax, r11d
	sar eax, 8
	mov [rsi+2049h], al
	mov [rsi+2010h], r11d
	mov rax, 4E8F08h
	jmp rax
ReallocateIOBuffer53 endp

;.text:00000000004E982D lea     rcx, [rax+12h]
;.text:00000000004E9831 mov     r9, rdi
;.text:00000000004E9834 mov     r8, rbx
ReallocateIOBuffer54 proc
	lea rcx, [rax+204Ah]
	mov r9, rdi
	mov r8, 4E9834h
	jmp r8
ReallocateIOBuffer54 endp

;.text:00000000004E985F mov     [rsi+10h], r11b
;.text:00000000004E9863 mov     eax, r11d
;.text:00000000004E9866 sar     eax, 8
;.text:00000000004E9869 mov     [rsi+11h], al
;.text:00000000004E986C mov     [rsi+2010h], r11d
;.text:00000000004E9873 jmp     loc_4E9948
;.text:00000000004E9878 ; ---------------------------------------------------------------------------
ReallocateIOBuffer55 proc
	mov [rsi+2048h], r11b
	mov eax, r11d
	sar eax, 8
	mov [rsi+2049h], al
	mov [rsi+2010h], r11d
	mov rax, 4E9948h
	jmp rax
ReallocateIOBuffer55 endp

;.text:000000000082301C lea     rcx, [rax+12h]
;.text:0000000000823020 mov     r9, rsi
;.text:0000000000823023 mov     r8, rbx
ReallocateIOBuffer56 proc
	lea rcx, [rax+204Ah]
	mov r9, rsi
	mov r8, 823023h
	jmp r8
ReallocateIOBuffer56 endp

;.text:0000000000823050 mov     [r12+10h], r11b
;.text:0000000000823055 mov     eax, r11d
;.text:0000000000823058 sar     eax, 8
;.text:000000000082305B mov     [r12+11h], al
;.text:0000000000823060 mov     [r12+2010h], r11d
;.text:0000000000823068 jmp     short loc_823088
;.text:000000000082306A ; ---------------------------------------------------------------------------
ReallocateIOBuffer57 proc
	mov [r12+2048h], r11b
	mov eax, r11d
	sar eax, 8
	mov [r12+2049h], al
	mov [r12+2010h], r11d
	mov rax, 823088h
	jmp rax
ReallocateIOBuffer57 endp

;.text:0000000000840CF1 lea     rcx, [rax+12h]
;.text:0000000000840CF5 mov     r9, rbp
;.text:0000000000840CF8 mov     r8, rdi
ReallocateIOBuffer58 proc
	lea rcx, [rax+204Ah]
	mov r9, rbp
	mov r8, 840CF8h
	jmp r8
ReallocateIOBuffer58 endp

;.text:0000000000840D23 mov     [rbx+10h], r11b
;.text:0000000000840D27 mov     eax, r11d
;.text:0000000000840D2A sar     eax, 8
;.text:0000000000840D2D mov     [rbx+11h], al
;.text:0000000000840D30 mov     [rbx+2010h], r11d
;.text:0000000000840D37 jmp     short loc_840D55
;.text:0000000000840D39 ; ---------------------------------------------------------------------------
ReallocateIOBuffer59 proc
	mov [rbx+2048h], r11b
	mov eax, r11d
	sar eax, 8
	mov [rbx+2049h], al
	mov [rbx+2010h], r11d
	mov rdx, 840D55h
	jmp rdx
ReallocateIOBuffer59 endp

;.text:0000000000622299 lea     rax, [rcx+10h]
;.text:000000000062229D mov     r8d, 3BBh
;.text:00000000006222A3 mov     [rsp+50h], rax
;.text:00000000006222A8 mov     rax, [rbx]
ReallocateIOBuffer60 proc
	lea rax, [rcx+2048h]
	mov r8d, 3BBh
	mov [rsp+50h], rax
	mov rax, 6222A8h
	jmp rax
ReallocateIOBuffer60 endp

;.text:0000000000622671 lea     rax, [rbx+10h]
;.text:0000000000622675 mov     [rsp+50h], rax
;.text:000000000062267A lea     rax, [rdi+70h]
ReallocateIOBuffer61 proc
	lea rax, [rbx+2048h]
	mov [rsp+50h], rax
	mov rax, 62267Ah
	jmp rax
ReallocateIOBuffer61 endp

;.text:00000000004BB898 lea     rcx, [rax+12h]
;.text:00000000004BB89C mov     r9, rbx
;.text:00000000004BB89F mov     r8, rbp
ReallocateIOBuffer62 proc
	lea rcx, [rax+204Ah]
	mov r9, rbx
	mov r8, 4BB89Fh
	jmp r8
ReallocateIOBuffer62 endp

;.text:00000000004BB8EC mov     [rdi+10h], bl
;.text:00000000004BB8EF mov     ecx, ebx
;.text:00000000004BB8F1 sar     ecx, 8
;.text:00000000004BB8F4 mov     [rdi+11h], cl
;.text:00000000004BB8F7 movsxd  rax, ebx
;.text:00000000004BB8FA mov     [rax+rdi+0Eh], bl
;.text:00000000004BB8FE mov     [rax+rdi+0Fh], cl
;.text:00000000004BB902 jmp     short loc_4BB90F
;.text:00000000004BB904 ; ---------------------------------------------------------------------------
ReallocateIOBuffer63 proc
	mov [rdi+2048h], bl
	mov ecx, ebx
	sar ecx, 8
	mov [rdi+2049h], cl
	movsxd rax, ebx
	mov [rax+rdi+2046h], bl
	mov [rax+rdi+2047h], cl
	mov rcx, 4BB90Fh
	jmp rcx
ReallocateIOBuffer63 endp

;.text:00000000004BB904 mov     [rdi+10h], bl
;.text:00000000004BB907 mov     eax, ebx
;.text:00000000004BB909 sar     eax, 8
;.text:00000000004BB90C mov     [rdi+11h], al
;.text:00000000004BB90F
ReallocateIOBuffer64 proc
	mov [rdi+2048h], bl
	mov eax, ebx
	sar eax, 8
	mov [rdi+2049h], al
	mov rcx, 4BB90Fh
	jmp rcx
ReallocateIOBuffer64 endp

;.text:000000000054A8E2 mov     edx, 1FFEh
;.text:000000000054A8E7 lea     rcx, [rax+12h]
;.text:000000000054A8EB call    VAssemble
ReallocateIOBuffer65 proc
	mov edx, 3FFEh
	lea rcx, [rax+204Ah]
	mov rax, 54A8EBh
	jmp rax
ReallocateIOBuffer65 endp

;.text:000000000054A930 movzx   esi, byte ptr [rbx+12h]
;.text:000000000054A934 cmp     sil, 7Dh
;.text:000000000054A938 jz      short loc_54A946
;.text:000000000054A93A cmp     sil, 7Eh

ReallocateIOBuffer66 proc
	movzx esi, byte ptr[rbx+204Ah]
	cmp sil, 7Dh
	jz NetPing
	mov rax, 54A93Ah
	jmp rax
	NetPing:
		mov rax, 54A946h
		jmp rax
ReallocateIOBuffer66 endp

;.text:000000000054A978 mov     [rbx+10h], dil
;.text:000000000054A97C mov     ecx, edi
;.text:000000000054A97E sar     ecx, 8
;.text:000000000054A981 mov     [rbx+11h], cl
;.text:000000000054A984 movsxd  rax, edi
;.text:000000000054A987 mov     [rax+rbx+0Eh], dil
;.text:000000000054A98C mov     [rax+rbx+0Fh], cl
;.text:000000000054A990 jmp     short loc_54A99E
;.text:000000000054A992 ; ---------------------------------------------------------------------------
ReallocateIOBuffer67 proc
	mov [rbx+2048h], dil
	mov ecx, edi
	sar ecx, 8
	mov [rbx+2049h], cl
	movsxd rax, edi
	mov [rax+rbx+2046h], dil
	mov [rax+rbx+2047h], cl
	mov rcx, 54A99Eh
	jmp rcx
ReallocateIOBuffer67 endp

;.text:000000000054A992 mov     [rbx+10h], dil
;.text:000000000054A996 mov     eax, edi
;.text:000000000054A998 sar     eax, 8
;.text:000000000054A99B mov     [rbx+11h], al
;.text:000000000054A99E
ReallocateIOBuffer68 proc
	mov [rbx+2048h], dil
	mov eax, edi
	sar eax, 8
	mov [rbx+2049h], al
	mov rcx, 54A99Eh
	jmp rcx
ReallocateIOBuffer68 endp

;.text:000000000064EF9C lea     rcx, [rax+12h]
;.text:000000000064EFA0 mov     r8, [rsp+68h]
;.text:000000000064EFA5 mov     edx, 1FFEh
;.text:000000000064EFAA call    VAssemble
ReallocateIOBuffer69 proc
	lea rcx, [rax+204Ah]
	mov r8, [rsp+68h]
	mov edx, 3FFEh
	mov rax, 64EFAAh
	jmp rax
ReallocateIOBuffer69 endp

;.text:000000000064EFB6 mov     [rbx+10h], al
;.text:000000000064EFB9 mov     ecx, eax
;.text:000000000064EFBB sar     ecx, 8
;.text:000000000064EFBE mov     [rbx+11h], cl
;.text:000000000064EFC1
ReallocateIOBuffer70 proc
	mov [rbx+2048h], al
	mov ecx, eax
	sar ecx, 8
	mov [rbx+2049h], cl
	mov rcx, 64EFC1h
	jmp rcx
ReallocateIOBuffer70 endp

;.text:000000000075E1F8 lea     rcx, [rax+12h]
;.text:000000000075E1FC mov     r8, [rsp+88h]
;.text:000000000075E204 mov     edx, 1FFEh
;.text:000000000075E209 call    VAssemble
ReallocateIOBuffer71 proc
	lea rcx, [rax+204Ah]
	mov r8, [rsp+88h]
	mov edx, 3FFEh
	mov rax, 75E209h
	jmp rax
ReallocateIOBuffer71 endp

;.text:000000000075E243 lea     eax, [rbx-3]
;.text:000000000075E246 mov     [rdi+10h], al
;.text:000000000075E249 lea     eax, [rbx-3]
;.text:000000000075E24C sar     eax, 8
;.text:000000000075E24F mov     [rdi+11h], al
;.text:000000000075E252
ReallocateIOBuffer72 proc
	lea eax, [rbx-3]
	mov [rdi+2048h], al
	lea eax, [rbx-3]
	sar eax, 8
	mov [rdi+2049h], al
	mov rcx, 75E252h
	jmp rcx
ReallocateIOBuffer72 endp

;.text:000000000054A96C mov     eax, [rbx+17h]
;.text:000000000054A96F mov     [rbx+2044h], eax
;.text:000000000054A975
ReallocateIOBuffer73 proc
	mov eax, [rbx+204Fh]
	mov [rbx+2044h], eax
	mov rcx, 54A975h
	jmp rcx
ReallocateIOBuffer73 endp
end