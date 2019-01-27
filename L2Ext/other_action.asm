
OnLoadItem PROTO :QWORD, :QWORD, :QWORD, :QWORD, :QWORD, :QWORD, :QWORD, :QWORD, :QWORD, :QWORD
OnSaveItem PROTO :QWORD, :DWORD, :QWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD, :DWORD
.data
.code

OnLoadItemHook proc
	call OnLoadItem ;Disassemble packet and create item with Augmentation data
	mov rdi, rax
	add r15, 48 ;fix packet position 11x4 
	mov rcx, 571CE1h
	jmp rcx
OnLoadItemHook endp

OnSaveItemHook proc
	mov r8, rax ; pItem
	call OnSaveItem
	mov rcx, 539BB5h
	jmp rcx
OnSaveItemHook endp
end