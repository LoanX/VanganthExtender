Public ItemListHook1, ItemListHook2, ItemListHook3, ItemListHook4, ItemListHook5, ItemListHook6, InventoryUpdateHook1, InventoryUpdateHook2, InventoryUpdateHook3, InventoryUpdateHook4
ItemListFix PROTO :QWORD, :DWORD, :QWORD, :WORD, :DWORD, :DWORD, :DWORD, :WORD, :WORD, :WORD, :DWORD, :WORD, :WORD
InventoryUpdateFix PROTO :QWORD, :DWORD, :QWORD, :WORD, :WORD, :DWORD, :DWORD, :DWORD, :WORD, :WORD, :WORD, :DWORD, :WORD, :WORD
.data
	Disassemble QWORD 61F510h
	CObjectDB__CreateItem QWORD 6FAB60h
	JExitLoadCharacterDiss QWORD 574B4Dh
.code

LoadCharacterDiss proc
	call Disassemble
	add rax, 28
	jmp JExitLoadCharacterDiss
LoadCharacterDiss endp

ItemListHook1 proc
mov r8, rbp
call ItemListFix
mov rdi, 81AE36h
jmp rdi
ItemListHook1 endp
ItemListHook2 proc
mov r8, rbp
call ItemListFix
mov rdi, 81AD79h
jmp rdi
ItemListHook2 endp
ItemListHook3 proc
mov r8, rbp
call ItemListFix
mov rdi, 81AEBFh
jmp rdi
ItemListHook3 endp
ItemListHook4 proc
mov r8, rbp
call ItemListFix
mov rdi, 74FB9Fh
jmp rdi
ItemListHook4 endp
ItemListHook5 proc
mov r8, rbp
call ItemListFix
mov rdi, 74FC57h
jmp rdi
ItemListHook5 endp
ItemListHook6 proc
mov r8, rbp
call ItemListFix
mov rdi, 74FCD7h
jmp rdi
ItemListHook6 endp
InventoryUpdateHook1 proc
mov r8, r14
call InventoryUpdateFix
mov rbp, 81A461h
jmp rbp
InventoryUpdateHook1 endp
InventoryUpdateHook2 proc
mov r8, r14
call InventoryUpdateFix
mov rbp, 81A615h
jmp rbp
InventoryUpdateHook2 endp
InventoryUpdateHook3 proc
mov r8, r12
call InventoryUpdateFix
mov rbp, 74F24Bh
jmp rbp
InventoryUpdateHook3 endp
InventoryUpdateHook4 proc
mov r8, r12
call InventoryUpdateFix
mov rbp, 74F3F9h
jmp rbp
InventoryUpdateHook4 endp
end