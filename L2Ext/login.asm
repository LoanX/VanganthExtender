
.code


;.text:000000000053E6C5 mov     r12d, dword ptr [rsp+0F8h+var_C0]
;.text:000000000053E6CA jmp     loc_53E808 ; <--- hook here
LoginFix proc
	mov rcx, 1110C98h
	mov rax, 781DE0h
	call rax
	mov rax, 53E958h
	jmp rax	
LoginFix endp

end