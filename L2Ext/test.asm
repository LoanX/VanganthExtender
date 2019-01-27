
GetTickCount PROTO
memset PROTO :QWORD, :DWORD, :DWORD
.data

.code

CParty_RouteAdena proc            ; CODE XREF: sub_AC639C
                mov     rax, rsp
                push    rbp
                push    rsi
                push    rdi
                push    r12
                push    r13
                push    r14
                push    r15
                lea     rbp, [rax-5Fh]
                sub     rsp, 0F0h
                mov     qword ptr [rbp-49h], 0FFFFFFFFFFFFFFFEh
                mov     [rax+8], rbx
                mov     r13, rdx
                mov     rsi, rcx
                mov     r8, 2F76DAB4h	;TlsIndex
                mov		r8, [r8]
                mov     rax, gs:[58h]
                mov     r14d, 9D58h
                mov     rdi, [rax+r8*8]
                mov     [rbp+6Fh], rdi
                movsxd  r8, dword ptr [rdi+r14]
                mov     r12, 400000h	;imagebase
                movsxd  rdx, dword ptr[44C13E0h + r12 + r8*4]
                lea     eax, [rdx+1]
                mov     dword ptr[44C13E0h + r12 + r8*4], eax
                imul    r8, 3E8h
                lea     rbx, [r8+rdx]
                mov     rax, 0DEC9F0h ; "bool __cdecl CParty::RouteAdena(class C"...
                mov     [r12+rbx*8+4481790h], rax
                call    GetTickCount
                mov     dword ptr[44A4A10h + r12 + rbx*4], eax
                xor     ebx, ebx
                test    r13, r13
                jz      loc_842C70
                cmp     dword ptr [r13+1Ch], 57
                jnz     loc_842C70
                mov     rax, [r13+58h]
                mov     r12, [rax+48h]
                mov     rcx, rsi
                mov		rax, 840CB0h
                call    rax		;CParty__CancelTradeAllMember
                mov     [rsp+20h], rbx
                mov     [rsp+28h], rbx
                mov     [rsp+30h], rbx
                lea     edx, [rbx+7]
                lea     rcx, [rsp+20h]
                mov		rax, 46EC38h
                call    rax		;XVector__XVector
                xor     edx, edx
                lea     rcx, [rsp+20h]
                mov		rax, 7A8AD4h
                call    rax
                mov     edi, ebx
                mov     [rsp+40h], rbx
                mov     r14d, ebx
                mov     [rsp+48h], rbx
                mov     [rbp-79h], rbx
                mov     r8d, 623h
                mov     rdx, 0DEB9B8h	;"Party.cpp"
                mov     rcx, [rsi+58h]
                mov		rax, 0ACE6DCh
                call    rax		;CYieldLock__Enter
                mov     r15d, ebx
                cmp     [rsi+0CCh], ebx
                jle     short loc_8429DB
                lea     rbx, [rsi+0ACh]

loc_84299C:
                mov     edx, [rbx]
                mov     rcx, 1600930h
                mov		rax, 041A4D4h
                call    rax		;FindObject
                mov     [rbp+7Fh], rax
                test    rax, rax
                jz      short loc_8429C1
                lea     rdx, [rbp+7Fh]
                lea     rcx, [rsp+40h]
                mov		rax, 046F380h
                call    rax

loc_8429C1:
                inc     r15d
                add     rbx, 4
                cmp     r15d, [rsi+0CCh]
                jl      short loc_84299C
                mov     r14, [rsp+48h]
                mov     rdi, [rsp+40h]

loc_8429DB:
                mov     r8d, 62Eh
                mov     rdx, 0DEB9B8h	 ; "Party.cpp"
                mov     rcx, [rsi+58h]
                mov		rax, 0ACE7E4h
                call    rax		;CYieldLock__Leave
                mov     rbx, rdi
                cmp     rdi, r14
                jz      short loc_842A6B

loc_8429F9:
                mov     rsi, [rbx]
                mov     [rbp+7Fh], rsi
                test    rsi, rsi
                jz      short loc_842A62
                mov     rax, [r13+58h]
                lea     rcx, [rsi+0B20h]
                xor     r9d, r9d
                mov     r8, r12
                mov     edx, [rax+58h]
                mov		rax, 0736388h
                call    rax
                test    al, al
                jz      short loc_842A62
                mov     rax, [r13+58h]
                cmp     dword ptr [rax+58h], 57
                jnz     short loc_842A62
                cvttsd2si r8d, qword ptr[0C934C0h]
                mov     rdx, rsi
                mov     rcx, r13
                mov		rax, 07A73BCh
                call    rax		;sub_7A73BC
                test    al, al
                jz      short loc_842A62
                mov     rax, [rsi]
                mov     rcx, rsi
                call    qword ptr [rax+0C18h]
                test    al, al
                jz      short loc_842A62
                lea     rdx, [rbp+7Fh]
                lea     rcx, [rsp+20h]
                mov		rax, 0046F380h
                call    rax		;sub_46F380

loc_842A62:
                add     rbx, 8
                cmp     rbx, r14
                jnz     short loc_8429F9

loc_842A6B:
                mov     rsi, [rsp+28h]
                mov     rbx, [rsp+20h]
                sub     rsi, rbx
                sar     rsi, 3
                test    esi, esi
                jg      short loc_842AD6
                test    rdi, rdi
                jz      short loc_842A9B
                mov     r8d, 1
                mov     rdx, rdi
                mov     rcx, 3805030h
                mov		rax, 0078B618h
                call    rax		;xstd__deleteObject
                nop

loc_842A9B:
                test    rbx, rbx
                jz      short loc_842AB6
                mov     r8d, 1
                mov     rdx, rbx
                mov     rcx, 3805030h
                mov		rax, 0078B618h
                call    rax		;xstd__deleteObject
                nop

loc_842AB6:
                mov     rax, [rbp+6Fh]
                mov     ecx, 9D58h
                movsxd  rax, dword ptr [rax+rcx]
                mov     rcx, 400000h	 ; 400000
                dec     dword ptr[44C13E0h + rcx + rax*4]
                jmp     loc_842C7C
; ---------------------------------------------------------------------------

loc_842AD6:
                xor     edx, edx        ; int
                lea     r8d, [rdx+1Ch]  ; size_t
                lea     rcx, [rbp-69h]  ; void *
                call    memset
                movsxd  r14, esi
                mov     rax, r12
                cqo
                idiv    r14
                mov     r15, rax
                mov     rcx, r14
                imul    rcx, rax
                sub     r12, rcx
                test    rax, rax
                jns     short loc_842B3D
                test    rdi, rdi
                jz      short loc_842B1D
                mov     r8d, 1
                mov     rdx, rdi
                mov     rcx, 3805030h
                mov		rax, 0078B618h
                call    rax		;xstd__deleteObject
                nop

loc_842B1D:
                test    rbx, rbx
                jz      short loc_842B38
                mov     r8d, 1
                mov     rdx, rbx
                mov     rcx, 3805030h
                mov		rax, 0078B618h
                call    rax		;xstd__deleteObject
                nop

loc_842B38:
                jmp     loc_842AB6
; ---------------------------------------------------------------------------

loc_842B3D:
                test    r12, r12
                jz      short loc_842B56

loc_842B42:
				mov		rax, 00B3508Ch
                call    rax		;sub_B3508C
                cdq
                idiv    esi
                movsxd  rax, edx
                inc     dword ptr [rbp+rax*4-69h]
                dec     r12
                jnz     short loc_842B42

loc_842B56:
                xor     esi, esi
                test    r14, r14
                jle     loc_842C1B

loc_842B61:
                mov     r12, [rbx+rsi*8]
                test    r12, r12
                jz      short loc_842BD6
                movsxd  rax, dword ptr [rbp+rsi*4-69h]
                add     rax, r15
                test    rax, rax
                jle     short loc_842BD6
                lea     rcx, [rbp-39h]
                mov		rax, 00758F2Ch
                call    rax		;sub_758F2C
                mov     dword ptr [rbp-39h], 39h
                movsxd  rax, dword ptr [rbp+rsi*4-69h]
                add     rax, r15
                mov     [rbp-31h], rax
                mov     rax, [r12]
                lea     rdx, [rbp-39h]
                mov     rcx, r12
                call    qword ptr [rax+950h]
                test    al, al
                jz      short loc_842BE0
                mov     r8, [rbp-31h]
                mov     rdx, r12
                mov     rcx, r13
                mov		rax, 00748A00h
                call    rax		;sub_748A00
                cmp     dword ptr [r13+74h], 0
                jbe     short loc_842BD6
                lea     rcx, [r12+4FB8h]
                mov     r9, [rbp-31h]
                xor     r8d, r8d
                lea     edx, [r8+1]
                mov		rax, 97E0E0h
                call    rax		;sub_97E0E0

loc_842BD6:
                inc     rsi
                cmp     rsi, r14
                jge     short loc_842C1B
                jmp     loc_842B61
; ---------------------------------------------------------------------------

loc_842BE0:
                test    rdi, rdi
                jz      short loc_842BFB
                mov     r8d, 1
                mov     rdx, rdi
                mov     rcx, 3805030h
                mov		rax, 0078B618h
                call    rax		;xstd__deleteObject
                nop

loc_842BFB:
                test    rbx, rbx
                jz      short loc_842C16
                mov     r8d, 1
                mov     rdx, rbx
                mov     rcx, 3805030h
                mov		rax, 0078B618h
                call    rax		;xstd__deleteObject
                nop

loc_842C16:
                jmp     loc_842AB6
; ---------------------------------------------------------------------------

loc_842C1B:
                test    rdi, rdi
                jz      short loc_842C36
                mov     r8d, 1
                mov     rdx, rdi
                mov     rcx, 3805030h
                mov		rax, 0078B618h
                call    rax		;xstd__deleteObject
                nop

loc_842C36:
                test    rbx, rbx
                jz      short loc_842C51
                mov     r8d, 1
                mov     rdx, rbx
                mov     rcx, 3805030h
                mov		rax, 0078B618h
                call    rax		;xstd__deleteObject
                nop

loc_842C51:
                mov     rax, [rbp+6Fh]
                mov     ecx, 9D58h
                movsxd  rax, dword ptr [rax+rcx]
                mov     rcx, 400000h
                dec     dword ptr [44C13E0h + rcx + rax*4]
                mov     al, 1
                jmp     short loc_842C7E
; ---------------------------------------------------------------------------

loc_842C70:                             
                movsxd  rax, dword ptr [rdi+r14]
                dec     dword ptr[44C13E0h + r12 + rax*4]

loc_842C7C:
                xor     al, al

loc_842C7E:
                mov     rbx, [rsp+130h]
                add     rsp, 0F0h
                pop     r15
                pop     r14
                pop     r13
                pop     r12
                pop     rdi
                pop     rsi
                pop     rbp
                ret
CParty_RouteAdena endp

end