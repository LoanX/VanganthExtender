
.data
float_3 REAL4 3.0
float_5 REAL4 5.0
.code
;cvtsi2ss xmm8, dword ptr [r13+18h]
;.text:0000000000700C32 divss   xmm8, cs:float_3_0
;.text:0000000000700C3B movaps  xmm0, xmm8
;.text:0000000000700C3F call    ceilf
;pOlympiadField = rbx

PointFix1 proc
	mov eax, dword ptr[rbx+18h]
	cmp eax, 1
	jnz NormalExit
	divss xmm8, float_5
	mov rax, 700C3Bh
	jmp rax
	
	NormalExit:
		divss xmm8, float_3
		mov rax, 700C3Bh
		jmp rax
PointFix1 endp
;.text:0000000000700C4D divss   xmm9, cs:float_3_0
PointFix2 proc
	mov eax, dword ptr[rbx+18h]
	cmp eax, 1
	jnz NormalExit
	divss xmm9, float_5
	mov rax, 700C56h
	jmp rax
	
	NormalExit:
		divss xmm9, float_3
		mov rax, 700C56h
		jmp rax
PointFix2 endp
end