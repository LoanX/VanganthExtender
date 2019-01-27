Public ChargeFix, p_block_buff_fix

GetTickCount PROTO
GetHealEffect PROTO :QWORD, :QWORD

.data
CancelReturn QWORD 005012E4h 
pJmpBlockBuffContinue QWORD 00505411h
pJmpBlockBuff QWORD 005053F2h
Jmp_NotReflect QWORD 7992B6h
Jmp_NormalExecJNZ QWORD 799242h
Jmp_NormalExec QWORD 79920Eh
Jmp_i_heal_exit QWORD 7A8213h
Jmp_geo_check_target_exit QWORD 6A2D74h

.code

i_heal_hook proc
;.text:00000000007A820B mulsd   xmm6, qword ptr [rdi+978h]
	mov rdx, rdi ;target
	mov rcx, rsi ;caster
	call GetHealEffect
	mulsd   xmm6, xmm0
	jmp Jmp_i_heal_exit
i_heal_hook endp

p_reflect_skill proc
	mov eax, 1h
	cmp [rdi+0E0h], eax	;Check if it's debuff
	jnz NotReflect
	cmp [rdi+14h], r15d
	jnz NormalExecJNZ
	jmp Jmp_NormalExec
	
	NotReflect:
		jmp Jmp_NotReflect
		
	NormalExecJNZ:
		jmp Jmp_NormalExecJNZ
		
p_reflect_skill endp

p_block_buff_fix proc
	cmp [rsi+0A7Bh], r15b	;Check if skill is on
	jz Continue
	cmp [r12+0E0h], r15d	;Check if buff
	jnz Continue
	cmp rsi, rbp			;Check if speller = target	
	jz Continue
	test rbp, rbp
	jz Continue
	jmp pJmpBlockBuff		;Block the buff
	
	Continue:
		jmp pJmpBlockBuffContinue
p_block_buff_fix endp

ChargeFix proc
;SkillInfo = r12+68h
;SkillInfo+0h - wszSkillName
;SkillInfo+8h - skillID
;RBP - pUser
;overwrited code
;.text:00000000006B8BB6 xor     esi, esi ;Clear ESI
;.text:00000000006B8BB8 mov     [rbp+0BACh], esi ;ZeroCharges
;.text:00000000006B8BBE mov     rax, [rbp+0] ;ExitPoint
	mov rax, [r12+68h] ;SkillInfo
	mov eax, dword ptr [rax+08h] ;SkillID
	cmp eax, 105h ;TripleSonicSlash
	je FourChargesRequied
	cmp eax, 5h ;DoubleSonicSlash
	je ThreeChargesRequied
	cmp eax, 6h ;SonicBlaster
	je TwoChargesRequied
	cmp eax, 11Ch ;HurricaneAssault
	je TwoChargesRequied
	cmp eax, 36h ;ForceBlaster
	je OneChargeRequied
	jmp Exit_Point
		
	FiveChargesRequied:
		mov eax, dword ptr [rbp+0BACh] ;Get current charges
		sub eax, 5h ;Decrease current charges
		mov [rbp+0BACh], eax ;Set new charges
		jmp Exit_Point
		
	FourChargesRequied:
		mov eax, dword ptr [rbp+0BACh] ;Get current charges
		sub eax, 4h ;Decrease current charges
		mov [rbp+0BACh], eax ;Set new charges
		jmp Exit_Point
	ThreeChargesRequied:
		mov eax, dword ptr [rbp+0BACh] ;Get current charges
		sub eax, 3h ;Decrease current charges
		mov [rbp+0BACh], eax ;Set new charges
		jmp Exit_Point
	TwoChargesRequied:
		mov eax, dword ptr [rbp+0BACh] ;Get current charges
		sub eax, 2h ;Decrease current charges
		mov [rbp+0BACh], eax ;Set new charges
		jmp Exit_Point
	OneChargeRequied:
		mov eax, dword ptr [rbp+0BACh] ;Get current charges
		sub eax, 1h ;Decrease current charges
		mov [rbp+0BACh], eax ;Set new charges
		jmp Exit_Point
	Exit_Point:
		test eax, eax
		jz Exit_Fix
		call GetTickCount
		mov [rbp+0BB0h], eax ; Set new timer for charges
		jmp Exit_Fix
	Exit_Fix:
		mov rax, 006B8BBEh
		jmp rax
		
ChargeFix endp

;.text:00000000004F8001 mov     rax, [r14+588h]
;.text:00000000004F8008 cmp     byte ptr [rax+64h], 0           ; nIsPC
;.text:00000000004F800C jnz     loc_4F80F3
;.text:00000000004F8012 cmp     byte ptr [rbx+7D0h], 0
;.text:00000000004F8019 jz      loc_4F80EA
;.text:00000000004F801F cmp     byte ptr [rax+7D0h], 0
;.text:00000000004F8026 jz      loc_4F80E1
;.text:00000000004F802C cmp     edi, esi
;.text:00000000004F802E jl      loc_4F80D8
;.text:00000000004F8034 cmp     byte ptr [rbx+64h], 0


;CCreature__LookNeighborByNPC
;r14 - npc - this
;r13 - creature - target
LookNeighborByNPCFix proc
	mov r12, [r14+588h]
	mov r12d, dword ptr[r12+8C4h]
	cmp r12d, 1	;is epic npc
	jz AddCreature
	cmp byte ptr[rbx+7D0h], 0
	jz SkipCreature
	cmp byte ptr[rax+7D0h], 0
	jz SkipCreature
	cmp edi, esi
	jl SkipCreature
	jmp AddCreature
	
	AddCreature:
		mov r12, 04F8034h
		jmp r12
	
	SkipCreature:
		mov r12, 04F8105h
		jmp r12
	 
LookNeighborByNPCFix endp

;.text:00000000004F7283 mov     rdx, [r14+588h]
;.text:00000000004F728A cmp     byte ptr [rdx+7D0h], 0	;<- jmp + 4 nops
;.text:00000000004F7291 jz      short loc_4F72A8
;.text:00000000004F7293 mov     rax, [r13+588h]
;.text:00000000004F729A cmp     byte ptr [rax+7D0h], 0
;.text:00000000004F72A1 jz      short loc_4F72A8
;.text:00000000004F72A3 mov     r8b, 1
LookNeighborFix1 proc
	mov ecx, dword ptr[rdx+8C4h]	; is epic npc
	cmp ecx, 1	;is epic npc
	jz AddCreature
	mov ecx, dword ptr[rdx+64h]		; is pc
	cmp ecx, 1
	jz PCLook
	cmp byte ptr[rdx+7D0h], 0	;normal execution
	jz SkipCreature
	mov rax, [r13+588h]
	cmp byte ptr[rax+7D0h], 0
	jz SkipCreature
	jmp AddCreature
	
	PCLook:
		mov rax, [r13+588h]
		mov ecx, dword ptr[rax+8C4h]	;is epic npc
		cmp ecx, 1
		jz AddCreature
		cmp byte ptr[rdx+7D0h], 0	;normal execution
		jz SkipCreature
		mov rax, [r13+588h]
		cmp byte ptr[rax+7D0h], 0
		jz SkipCreature
		jmp AddCreature
	
	SkipCreature:
		mov rcx, 4F72A8h
		jmp rcx
		
	AddCreature:
		mov rcx, 4F72A3h
		jmp rcx	
	
LookNeighborFix1 endp


end