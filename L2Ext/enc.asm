malloc PROTO: QWORD
free PROTO: QWORD
memcpy PROTO: QWORD, :QWORD, :QWORD
closesocket PROTO: QWORD
EncryptWrapper PROTO: QWORD, :QWORD
DecryptWrapper PROTO: QWORD, :DWORD

.const
retDecryptCall		dq	87715Fh

.code

;free memory allocated for 16 byte key
CloseIOSocketHook proc
	;.text:000000000061E62B                 call    cs:closesocket
	;.text:000000000061E631                 mov     r11, [rbx]
	mov rax, 974620h
	mov rax, [rax]
	call rax	;closesocket
	mov rax, 0A8EE78h
	cmp [rbx], rax							;if socket is not user socket we skip this
	jne exit
	mov rcx, [rbx+120h]
	test rcx, rcx
	jz exit
	mov rax, 9151A0h
	call rax	;free
	xor rcx, rcx
	mov [rbx+120h], rcx
	mov [rbx+128h], rcx
	
exit:
	mov r11, 61E631h
	jmp r11
	
CloseIOSocketHook endp

InlineEncryptHook proc
	;rsi - CIOBuffer
	;rbp - CSocket
	mov rdx, rsi
	mov rcx, rbp
	call EncryptWrapper
	mov rdx, 7D55F8h
	jmp rdx
InlineEncryptHook endp

InlineDecryptHook proc
	;rsi - CSocket
	mov edx, r14d
	mov rcx, rsi
	call DecryptWrapper
	mov rcx, 87715Fh
	jmp rcx
InlineDecryptHook endp

END