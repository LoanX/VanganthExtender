Public AddMemberFix, DelMemberFix
.const
AddMemberJL QWORD 766380h
AddMemberJMP QWORD 7663BEh
DelMemberJL QWORD 768590h
DelMemberJMP QWORD 7685D4h
.code
AddMemberFix proc
	cmp rcx, 08Ch
	jl JmpL
	jmp AddMemberJMP
	JmpL:
		jmp AddMemberJL
AddMemberFix endp
DelMemberFix proc
	cmp rcx, 08Ch
	jl JmpL
	jmp DelMemberJMP
	JmpL:
		jmp DelMemberJL
DelMemberFix endp
end