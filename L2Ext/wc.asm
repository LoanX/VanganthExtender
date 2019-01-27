
GetCollisionHashLink PROTO
FreeCollisionHashLink PROTO :QWORD

.data

.code
;CWorldCollisionHash::EnterCreature
;.text:00000000008C510B mov     rcx, cs:19AD01E0h
;.text:00000000008C5112 mov     cs:19AD01E8h, ebp
;.text:00000000008C5118 lock dec dword ptr cs:19AD01F4h
;.text:00000000008C511F mov     [rcx], r14			;hook
;.text:00000000008C5122 mov     rax, [r13+0]		; + 1 nop
;.text:00000000008C5126 mov     [rcx+8], rax		; + 4 nops
;.text:00000000008C512A mov     [r13+0], rcx		; + 4 nops
;.text:00000000008C512E mov     ebx, [rsp+128h+var_F8]	; jump back

CWorldCollisionHash_EnterCreature1 proc
	call GetCollisionHashLink
	mov rcx, rax
	mov [rcx], r14
	mov rax, [r13]
	mov [rcx+8], rax
	mov [r13], rcx
	mov ebx, 8C512Eh
	jmp rbx
CWorldCollisionHash_EnterCreature1 endp

;.text:00000000008C5585 mov     cs:19AD01ECh, eax
;.text:00000000008C558B mov     rcx, cs:19AD01E0h
;.text:00000000008C5592 mov     cs:19AD01E8h, ebp
;.text:00000000008C5598 lock dec dword ptr cs:19AD01F4h
;.text:00000000008C559F mov     [rcx], r14		;hook
;.text:00000000008C55A2 mov     rax, [r13+0]	; + 1 nop
;.text:00000000008C55A6 mov     [rcx+8], rax	; + 4 nops
;.text:00000000008C55AA mov     [r13+0], rcx	; + 4 nops
;.text:00000000008C55AE mov     ebx, [rsp+128h+var_F8]	;jump back

CWorldCollisionHash_EnterCreature2 proc
	call GetCollisionHashLink
	mov rcx, rax
	mov [rcx], r14
	mov rax, [r13]
	mov [rcx+8], rax
	mov [r13], rcx
	mov ebx, 8C55AEh
	jmp rbx
CWorldCollisionHash_EnterCreature2 endp

;CWorldCollisionHash::RemoveCreature
;.text:00000000008C367C mov     cs:19AD01F0h, eax
;.text:00000000008C3682 mov     cs:19AD01E0h, rdi	;hook + 2 nops
;.text:00000000008C3689 mov     dword ptr cs:19AD01E8h, 0
;.text:00000000008C3693 lock inc dword ptr cs:19AD01F4h

CWorldCollisionHash_RemoveCreature1 proc
	mov rcx, rdi
	call FreeCollisionHashLink
	mov rdi, 8C3689h
	jmp rdi	
CWorldCollisionHash_RemoveCreature1 endp

;CWorldCollisionHash::MoveCreature
;.text:00000000008C73CD mov     cs:19AD01F0h, eax
;.text:00000000008C73D3 mov     cs:19AD01E0h, rdi	;hook + 2 nops
;.text:00000000008C73DA mov     dword ptr cs:19AD01E8h, 0
;.text:00000000008C73E4 lock inc dword ptr cs:19AD01F4h
;.text:00000000008C73EB movzx   ebx, r14b

CWorldCollisionHash_MoveCreature1 proc
	mov rcx, rdi
	call FreeCollisionHashLink
	mov rbx, 8C73DAh
	jmp rbx
CWorldCollisionHash_MoveCreature1 endp

end