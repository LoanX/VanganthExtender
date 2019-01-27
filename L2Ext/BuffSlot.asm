Public CheckBuffAmount
GetMaxAbnormals PROTO :QWORD, :QWORD
.data
pJmpExit QWORD 00505805h
pJmpLower QWORD 00505C3Bh
.code
;.text:00000000005057FB cmp     r15d, 14h
;.text:00000000005057FF jl      loc_505C3B
;.text:0000000000505805 xor     r13d, r13d
CheckBuffAmount proc
	push rcx
	push rdx
	mov rdx, [rsp+2180h+10h] ;CSkillInfo +10h becouse of 2x push
	mov rcx, rsi ; rsi - pUser
	call GetMaxAbnormals
	pop rdx
	pop rcx
	cmp r15d, eax
	jl JumpLower
	jmp pJmpExit
	
	JumpLower:
		jmp pJmpLower
CheckBuffAmount endp
end