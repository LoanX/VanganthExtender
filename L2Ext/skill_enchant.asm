.DATA
	RETURNADDR1								QWORD	79E0E1h
	RETURNADDR2								QWORD	79F9DAh
	RETURNADDR3								QWORD	79FDDAh	
	
	LOC_79E10C								QWORD	79E10Ch
		
	EXTRN									SkillEnchantSystem_DivideReqWrapper:PROC
	EXTERN									nExpTable:DWORD
	
.CODE
	SkillEnchantSystem_ExpFixHook1	PROC
	
		xor		rdx, rdx								; Clearing Register
		mov		ecx, dword ptr[rsi+18h]					; nRequiredExp
		mov		edx, ebx								; nCurrentExp
		call	SkillEnchantSystem_DivideReqWrapper		; Dividing Experience
		mov		rcx, [rbp+158h]
		mov		rdx, [rcx+588h]
		xor		rcx, rcx
		mov		ecx, dword ptr[rdx+444h] ;nLevel
		lea		rdx, nExpTable							; Getting Experience Table ADDR
		sub     ebx, dword ptr [rdx+rcx*4]				; Experience Table
		
		cmp     eax, ebx
		jle		JumpToOtherLocation
		
		FinishNormally:
			jmp		RETURNADDR1	
		
		JumpToOtherLocation:
			jmp		LOC_79E10C		
	
	SkillEnchantSystem_ExpFixHook1	ENDP
	SkillEnchantSystem_ExpFixHook2	PROC
	
		mov     rcx, [rbp+588h]
		mov		rdx, [rcx+11Ch]							; nCurrentExp
		xor		rcx, rcx
		mov		ecx, dword ptr[rbx+18h]							; nRequiredExp		
		call	SkillEnchantSystem_DivideReqWrapper		; Dividing Experience
		mov     rcx, [rbp+588h]
		sub     [rcx+11Ch], eax
				
		movsxd  rax, dword ptr [rcx+444h]				; nLevel
		lea		rdx, nExpTable							; Getting Experience Table ADDR		
		mov     edx, [rdx+rax*4]
		cmp     [rcx+11Ch], edx							; Comparing Available Experience
		jge     FinishNormally
		
		SetExp:
			mov     [rcx+11Ch], edx		
			jmp		RETURNADDR2	
		
		FinishNormally:
			jmp		RETURNADDR2
		
	SkillEnchantSystem_ExpFixHook2	ENDP
	
	SkillEnchantSystem_ExpFixHook3	PROC
	
		mov     rcx, [rbp+588h]
		mov		rdx, [rcx+11Ch]							; nCurrentExp
		xor rcx, rcx							
		mov		ecx, dword ptr[rbx+18h]					; nRequiredExp		
		call	SkillEnchantSystem_DivideReqWrapper		; Dividing Experience
		mov     rcx, [rbp+588h]
		sub     [rcx+11Ch], eax
				
		movsxd  rax, dword ptr [rcx+444h]				; nLevel
		lea		rdx, nExpTable							; Getting Experience Table ADDR		
		mov     edx, [rdx+rax*4]
		cmp     [rcx+11Ch], edx							; Comparing Available Experience
		jge     FinishNormally
		
		SetExp:
			mov     [rcx+11Ch], edx		
			jmp		RETURNADDR3	
		
		FinishNormally:
			jmp		RETURNADDR3	
		                                                                                                                                                                                                                                                                                         	
	SkillEnchantSystem_ExpFixHook3	ENDP
		
END