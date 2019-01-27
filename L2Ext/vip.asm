

.data
	GetDropRateWrapper PROTO
	GetSpoilRateWrapper PROTO :QWORD
	GetCraftRateWrapper PROTO :QWORD, :QWORD
	GetUserLimitWrapper PROTO :DWORD
.code

UserLimitHook proc
	sub rsp, 208h
	movdqa  [rsp+208h - 18h], xmm0
	movdqa  [rsp+208h - 28h], xmm1
	movdqa  [rsp+208h - 38h], xmm2
	movdqa  [rsp+208h - 48h], xmm3
	movdqa  [rsp+208h - 58h], xmm4
	movdqa  [rsp+208h - 68h], xmm5
	mov [rsp+208h - 78h], r11
	mov [rsp+208h - 80h], r10
	mov [rsp+208h - 88h], r9
	mov [rsp+208h - 90h], r8
	mov [rsp+208h - 98h], rdx
	mov [rsp+208h - 0A0h], rcx
	mov ecx, [rsp+208h + 6Ch]	;accountId
	call GetUserLimitWrapper
	mov r11, [rsp+208h - 78h]
	mov r10, [rsp+208h - 80h]
	mov r9, [rsp+208h - 88h]
	mov r8, [rsp+208h - 90h]
	mov rdx, [rsp+208h - 98h]
	mov rcx, [rsp+208h - 0A0h]
	movdqa  xmm5, [rsp + 208h - 68h]
	movdqa  xmm4, [rsp + 208h - 58h]
	movdqa  xmm3, [rsp + 208h - 48h]
	movdqa  xmm2, [rsp + 208h - 38h]
	movdqa  xmm1, [rsp + 208h - 28h]
	movdqa  xmm0, [rsp + 208h - 18h]
	add rsp, 208h
	ret
UserLimitHook endp

CraftRateHook1 proc
	sub rsp, 208h
	movdqa  [rsp+208h - 28h], xmm1
	movdqa  [rsp+208h - 38h], xmm2
	movdqa  [rsp+208h - 48h], xmm3
	movdqa  [rsp+208h - 58h], xmm4
	movdqa  [rsp+208h - 68h], xmm5
	mov [rsp+208h - 78h], r11
	mov [rsp+208h - 80h], r10
	mov [rsp+208h - 88h], r9
	mov [rsp+208h - 90h], r8
	mov [rsp+208h - 98h], rdx
	mov [rsp+208h - 0A0h], rcx
	mov rdx, r13
	mov rcx, [rsp+208h + 2120h]	;pUser
	call GetCraftRateWrapper
	mov r11, [rsp+208h - 78h]
	mov r10, [rsp+208h - 80h]
	mov r9, [rsp+208h - 88h]
	mov r8, [rsp+208h - 90h]
	mov rdx, [rsp+208h - 98h]
	mov rcx, [rsp+208h - 0A0h]
	movdqa  xmm5, [rsp + 208h - 68h]
	movdqa  xmm4, [rsp + 208h - 58h]
	movdqa  xmm3, [rsp + 208h - 48h]
	movdqa  xmm2, [rsp + 208h - 38h]
	movdqa  xmm1, [rsp + 208h - 28h]
	add rsp, 208h
	ret
CraftRateHook1 endp

CraftRateHook2 proc
	sub rsp, 208h
	movdqa  [rsp+208h - 28h], xmm1
	movdqa  [rsp+208h - 38h], xmm2
	movdqa  [rsp+208h - 48h], xmm3
	movdqa  [rsp+208h - 58h], xmm4
	movdqa  [rsp+208h - 68h], xmm5
	mov [rsp+208h - 78h], r11
	mov [rsp+208h - 80h], r10
	mov [rsp+208h - 88h], r9
	mov [rsp+208h - 90h], r8
	mov [rsp+208h - 98h], rdx
	mov [rsp+208h - 0A0h], rcx
	mov rdx, r13
	mov rcx, [rsp+208h + 2150h]	;pUser
	call GetCraftRateWrapper
	mov r11, [rsp+208h - 78h]
	mov r10, [rsp+208h - 80h]
	mov r9, [rsp+208h - 88h]
	mov r8, [rsp+208h - 90h]
	mov rdx, [rsp+208h - 98h]
	mov rcx, [rsp+208h - 0A0h]
	movdqa  xmm5, [rsp + 208h - 68h]
	movdqa  xmm4, [rsp + 208h - 58h]
	movdqa  xmm3, [rsp + 208h - 48h]
	movdqa  xmm2, [rsp + 208h - 38h]
	movdqa  xmm1, [rsp + 208h - 28h]
	add rsp, 208h
	ret
CraftRateHook2 endp

SpoilRateHook proc
	mov rbx, [rsp+110h]	;pSkillUser
	sub rsp, 208h
	movdqa  [rsp+208h - 18h], xmm0
	movdqa  [rsp+208h - 38h], xmm2
	movdqa  [rsp+208h - 48h], xmm3
	movdqa  [rsp+208h - 58h], xmm4
	movdqa  [rsp+208h - 68h], xmm5
	mov [rsp+208h - 78h], r11
	mov [rsp+208h - 80h], r10
	mov [rsp+208h - 88h], r9
	mov [rsp+208h - 90h], r8
	mov [rsp+208h - 98h], rdx
	mov [rsp+208h - 0A0h], rcx
	mov rcx, rbx
	call GetSpoilRateWrapper
	movsd xmm1, xmm0
	mov r11, [rsp+208h - 78h]
	mov r10, [rsp+208h - 80h]
	mov r9, [rsp+208h - 88h]
	mov r8, [rsp+208h - 90h]
	mov rdx, [rsp+208h - 98h]
	mov rcx, [rsp+208h - 0A0h]
	movdqa  xmm5, [rsp + 208h - 68h]
	movdqa  xmm4, [rsp + 208h - 58h]
	movdqa  xmm3, [rsp + 208h - 48h]
	movdqa  xmm2, [rsp + 208h - 38h]
	movdqa  xmm0, [rsp + 208h - 18h]
	add rsp, 208h
	ret
SpoilRateHook endp

DropRateHook proc
	sub rsp, 208h
	movdqa  [rsp+208h - 18h], xmm0
	movdqa  [rsp+208h - 28h], xmm1
	movdqa  [rsp+208h - 38h], xmm2
	movdqa  [rsp+208h - 48h], xmm3
	movdqa  [rsp+208h - 58h], xmm4
	movdqa  [rsp+208h - 68h], xmm5
	mov [rsp+208h - 78h], r11
	mov [rsp+208h - 80h], r10
	mov [rsp+208h - 88h], r9
	mov [rsp+208h - 90h], r8
	mov [rsp+208h - 98h], rdx
	mov [rsp+208h - 0A0h], rcx
	call GetDropRateWrapper
	mulsd xmm7, xmm0
	mov r11, [rsp+208h - 78h]
	mov r10, [rsp+208h - 80h]
	mov r9, [rsp+208h - 88h]
	mov r8, [rsp+208h - 90h]
	mov rdx, [rsp+208h - 98h]
	mov rcx, [rsp+208h - 0A0h]
	movdqa  xmm5, [rsp + 208h - 68h]
	movdqa  xmm4, [rsp + 208h - 58h]
	movdqa  xmm3, [rsp + 208h - 48h]
	movdqa  xmm2, [rsp + 208h - 38h]
	movdqa  xmm1, [rsp + 208h - 28h]
	movdqa  xmm0, [rsp + 208h - 18h]
	add rsp, 208h
	ret
DropRateHook endp

end