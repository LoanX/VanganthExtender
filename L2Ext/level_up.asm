
.DATA
	RETURNADDR1								QWORD	7FF55Ah
	RETURNADDR2								QWORD	7499BEh
		
.CODE
	
	; CPet::SetExp Fix
	LevelUpSystemFixHook2	PROC
	
		; Checking Pet NPC Class ID
		mov		rax, [rdi+588h]					; Shared Data
		mov		eax, dword ptr[rax+788h]		; NPC_CLASS_ID
	
		cmp		eax, 1012564					; NPC_CLASS_ID (SINEATER)
		jne		NOT_SIN_EATER			
		
		SIN_EATER:
		
			mov		rax, [rdi+588h]				; Shared Data
			mov		eax, dword ptr [rax+444h]	; NPC LEVEL
			
			cmp		eax, 50h					; Checking If The Current Level is 80
			jl		LOWER_81					; If Lower do Not Apply The Rule
			
			CURRENT_81:
			
				cmp     ebx, 46182894
				jl      LeaveFix
		
				test    ecx, ecx
				js      SetMaxAllowedExp1
			
				cmp     ecx, r9d
				jge     SetMaxAllowedExp1
			
				mov     rdx, [rbp+rcx*8+0DDF98A0h]
			
				SetMaxAllowedExp1:
					
					mov     ebx, 46182894
					dec     ebx
					jmp		LeaveFix				
		
			LOWER_81:
			
				cmp     ebx, [r8+3CC4h]
				jl      LeaveFix
		
				test    ecx, ecx
				js      SetMaxAllowedExp2
			
				cmp     ecx, r9d
				jge     SetMaxAllowedExp2
			
				mov     rdx, [rbp+rcx*8+0DDF98A0h]
			
				SetMaxAllowedExp2:
					
					mov     ebx, [rdx+3CC4h]
					dec     ebx
					jmp		LeaveFix		
	
		NOT_SIN_EATER:
		
			cmp     ebx, [r8+3CC4h]
			jl      LeaveFix
	
			test    ecx, ecx
			js      SetMaxAllowedExp3
		
			cmp     ecx, r9d
			jge     SetMaxAllowedExp3
		
			mov     rdx, [rbp+rcx*8+0DDF98A0h]
		
			SetMaxAllowedExp3:
				
				mov     ebx, [rdx+3CC4h]
				dec     ebx
		
		LeaveFix:
			jmp	RETURNADDR2
	
	LevelUpSystemFixHook2	ENDP
;.text:0000000000856B6A mov     eax, [rsp+450h] <-- Here we hook (5+6nops);
;.text:0000000000856B71 mov     [rsp+68h], eax
;.text:0000000000856B75 mov     eax, [rsp+788h+var_330]
	
UserInfoExpFix1 proc
	xor rax, rax
	mov eax, [rsp+450h] ;exp
	cmp eax, 5A143D3Ah
	ja RecalculateExp
	jmp Exit
	RecalculateExp:
	sub eax, 5A143D3Ah
	imul rax, rax, 40h
	add rax, 5A143D3Ah
	jmp Exit
	Exit:
	mov [rsp+68h], rax
	mov rax, 856B75h
	jmp rax
	
UserInfoExpFix1 endp
;.text:000000000085760D mov     eax, [rsp+450h] ;<-hook (5+6nops)
;.text:0000000000857614 mov     [rsp+68h], eax
;.text:0000000000857618 mov     eax, [rsp+788h+var_330]
UserInfoExpFix2 proc
	xor rax, rax
	mov eax, [rsp+450h] ;exp
	cmp eax, 5A143D3Ah
	ja RecalculateExp
	jmp Exit
	RecalculateExp:
	sub eax, 5A143D3Ah
	imul rax, rax, 40h
	add rax, 5A143D3Ah
	jmp Exit
	Exit:
	mov [rsp+68h], rax
	mov rax, 857618h
	jmp rax
	
UserInfoExpFix2 endp
END