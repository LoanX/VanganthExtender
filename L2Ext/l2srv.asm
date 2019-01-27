include windows.inc

SpoilAmountWrapper PROTO :REAL8, :REAL8, :REAL8, :REAL8
.data
SharedGuard QWORD 00401500h
JmpNormalExit QWORD 00890A0Bh
JmpTimerExit QWORD 008909BFh
JmpChronoWeapon1 QWORD 00517ABCh
JmpSpoilFixExit QWORD 007D0968h
double100Ptr QWORD 4059000000000000h
JumpCombatMode QWORD 008909BFh
JumpNotOfflineShop QWORD 00890A0Bh
JumpOfflineShopHandler QWORD 008909BFh
JumpSpoilFixEx QWORD 007D098Fh
HalfValue REAL4 0.5
.code

;;;;;;;;;;;;;;;;;;;;Spoil fix;;;;;;;;;;;;
;.text:00000000007D0921 movsd   xmm1, xmm7
;.text:00000000007D0925 movsd   xmm0, xmm8
;.text:00000000007D092A call    RandomMinMax
;.text:00000000007D092F movlpd  xmm1, cs:SpoilRate
;.text:00000000007D0937 mulsd   xmm1, qword ptr [r12+10h]
;.text:00000000007D093E xor     ebx, ebx
;.text:00000000007D0940 comisd  xmm1, xmm7		jump below equal 100.0
;.text:00000000007D0944 jbe     short loc_7D0968
;.text:00000000007D0946 cvttsd2si ecx, xmm1				
;.text:00000000007D094A mov     eax, 51EB851Fh
;.text:00000000007D094F imul    ecx
;.text:00000000007D0951 mov     ebx, edx
;.text:00000000007D0953 sar     ebx, 5
;.text:00000000007D0956 mov     eax, ebx
;.text:00000000007D0958 shr     eax, 1Fh
;.text:00000000007D095B add     ebx, eax
;.text:00000000007D095D mov     eax, ebx
;.text:00000000007D095F imul    eax, 64h
;.text:00000000007D0962 sub     ecx, eax	<--- Hook start
;.text:00000000007D0964 cvtsi2sd xmm1, ecx		
;.text:00000000007D0968
;.text:00000000007D0968 loc_7D0968:                             ; CODE XREF: CSkillEffect_i_spoil__Instant+3D4j
;.text:00000000007D0968 comisd  xmm0, xmm1
;.text:00000000007D096C ja      loc_7D0A61
;.text:00000000007D0972 cvtsi2sd xmm1, dword ptr [r12+8]
;.text:00000000007D0979 cvtsi2sd xmm0, dword ptr [r12+4]
;.text:00000000007D0980 call    RandomMinMax
;.text:00000000007D0985 addsd   xmm0, xmm6	<---- second hook
;.text:00000000007D0989 cvttsd2si esi, xmm0
;.text:00000000007D098D add     esi, ebx
;.text:00000000007D098F test    esi, esi
;.text:00000000007D0991 jle     loc_7D0A61

SpoilFix proc
	sub ecx, eax
	movsd   xmm1, xmm7
	jmp JmpSpoilFixExit
SpoilFix endp

SpoilFixEx proc
	movsd xmm3, xmm0	;dice
	movsd xmm2, xmm1	; base_chance
	cvtsi2sd xmm1, dword ptr[r12+8]	;max
	cvtsi2sd xmm0, dword ptr[r12+4]	;min
	call SpoilAmountWrapper
	mov esi, eax
	jmp JumpSpoilFixEx
	
SpoilFixEx endp

OldSpoilFix proc
	sub ecx, eax
	movsd   xmm1, xmm7
	dec ebx
	jmp JmpSpoilFixExit
OldSpoilFix endp

;.text:00000000007D0989 cvttsd2si esi, xmm0
;.text:00000000007D098D add     esi, ebx
;.text:00000000007D098F test    esi, esi
;.text:00000000007D0991 jle     loc_7D0A61
;.text:00000000007D0997 lea     rbp, [r13+1338h]

SpoilFix2 proc
	imul esi, ebx
SpoilFix2 endp


;.text:0000000000517A8D cmp     esi, 5817 <- hook + 1 nop
;.text:0000000000517A93 jz      short loc_517ABC
;.text:0000000000517A95 mov     rax, [rbx]
ChronoWeaponFix1 proc
	cmp esi, 5817
	jz ChronoWeapon
	cmp esi, 7058
	jz ChronoWeapon
	cmp esi, 8350
	jz ChronoWeapon
	jmp NormalExit
	
	ChronoWeapon:
		mov rax, 00517ABCh
		jmp rax
	NormalExit:
		mov rax, 00517A95h
		jmp rax
ChronoWeaponFix1 endp

;.text:00000000008B86A4 cmp     r14d, 5817 <- hook + 2 nop
;.text:00000000008B86AB jz      short loc_8B86B2
;.text:00000000008B86AD mov     [rbx], r12
ChronoWeaponFix2 proc
	cmp r14d, 5817
	jz ChronoWeapon
	cmp r14d, 7058
	jz ChronoWeapon
	cmp r14d, 8350
	jz ChronoWeapon
	jmp NormalExit
	
	ChronoWeapon:
		mov r8, 008B86B2h
		jmp r8
		
	NormalExit:
		mov r8, 008B86ADh
		jmp r8
ChronoWeaponFix2 endp

CheckOfflineShopWhenExit proc
	cmp byte ptr[r11+0CA5h], 0   ;Check for combat mode
	jz OfflineShopCheck
	jmp JumpCombatMode
	
	OfflineShopCheck:
		mov r8, [r11+0588h]
		cmp dword ptr[r8+0864h], 0  ;Check for offline shop
		jz NotOfflineShop
		jmp JumpOfflineShopHandler
	
	NotOfflineShop:
		jmp JumpNotOfflineShop
		
	
CheckOfflineShopWhenExit endp

;.text:000000000068A26E call    CParty__GetMaster
;.text:000000000068A273 mov     rdi, [rax+588h]
;.text:000000000068A27A mov     rcx, [rsp+178h+var_D8]
;68A563 exit
CMultiPartyCommandChannelManager_WithdrawInternal1 proc
	test rax, rax
	jz Exit
	mov rdi, [rax+588h]
	mov rcx, 68A27Ah
	jmp rcx
	
	Exit:
		mov rcx, 68A563h
		jmp rcx
CMultiPartyCommandChannelManager_WithdrawInternal1 endp

;.text:00000000007FF54A jle     loc_7FF5F4
;.text:00000000007FF550 cvtsi2sd xmm6, edi
;.text:00000000007FF554 mov     edx, [rcx+444h]	; <-- hook here + 1 nop
;.text:00000000007FF55A lea     rcx, cs:0E1F3430h
;rbx - pUser, rcx, sharedcreaturedata
CursedWeaponKarmaFix1 proc
	mov edx, [rcx + 840h]	;nIsCursedWeaponOwner
	test edx, edx
	jz NormalExecution
	jmp Exit
	
	Exit:
		mov rcx, 7FF5F4h
		jmp rcx
	
	NormalExecution:
		mov edx, [rcx+444h]
		mov rcx, 7FF55Ah
		jmp rcx
CursedWeaponkarmaFix1 endp


end