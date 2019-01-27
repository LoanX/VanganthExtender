
CreateIntParam PROTO :QWORD, :DWORD

.data

.code
;text:00000000008EFA2F movsxd  rdx, dword ptr [rcx+34h]        ; jumptable 008EF747 case 14
;.text:00000000008EFA33 mov     rax, [rcx+18h]
;.text:00000000008EFA37 mov     rcx, [rcx+0F0h]
;.text:00000000008EFA3E mov     eax, [rax+rdx*8]	;hook here + 1 nop
;.text:00000000008EFA41 mov     [rcx+28h], eax
;.text:00000000008EFA44 jmp     loc_8F7386

CreateIntParamHook proc
	mov edx, [rax+rdx*8]	;param value, rcx is skill info
	call CreateIntParam
	mov rax, 8EFA44h
	jmp rax 
CreateIntParamHook endp

end