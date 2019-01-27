Public PrivateStoreBuyCheckAugmentation
.data
.code
PrivateStoreBuyCheckAugmentation proc
;.text:000000000083EB3A cmp     [rdx+70h], ecx
;.text:000000000083EB3D jnz     short loc_83EBA1
; rdi - pItem
	cmp [rdx+70h], ecx
	jnz ExitJmp
	cmp dword ptr[rdi+60h], 0h ; Check if not augmented
	jnz ExitJmp
	mov rax, 83EB3Fh
	jmp rax
	
	ExitJmp:
		mov rdx, 83EBA1h
		jmp rdx
PrivateStoreBuyCheckAugmentation endp
end