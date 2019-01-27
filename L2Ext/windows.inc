;   This file is part of L2Extend.

;   L2Extend is free software: you can redistribute it and/or modify
;   it under the terms of the GNU General Public License as published by
;   the Free Software Foundation, either version 3 of the License, or
;   (at your option) any later version.

;   L2Extend is distributed in the hope that it will be useful,
;   but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;   GNU General Public License for more details.

;   You should have received a copy of the GNU General Public License
;   along with L2Extend.  If not, see <http://www.gnu.org/licenses/>.

;	You May Not Make Commercial Use of any of our content, and any kind
;	of newer implementations will be need to be shared and commited.

;==============================================================================
;windows³£Á¿¶¨Òå
;==============================================================================
;For kernel32 functions
;------------------------------------------------
;VitureProtect
;------------------------------------------------
; ÄÄ´ Protection and other valuez ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; ezasm's sucked.
;

SECTION_QUERY                   EQU     00000001h
SECTION_MAP_WRITE               EQU     00000002h
SECTION_MAP_READ                EQU     00000004h
SECTION_MAP_EXECUTE             EQU     00000008h
SECTION_EXTEND_SIZE             EQU     00000010h

SECTION_ALL_ACCESS              EQU     STANDARD_RIGHTS_REQUIRED OR \
                                        SECTION_QUERY            OR \
                                        SECTION_MAP_WRITE        OR \
                                        SECTION_MAP_READ         OR \
                                        SECTION_MAP_EXECUTE      OR \
                                        SECTION_EXTEND_SIZE

FILE_MAP_COPY                   EQU     SECTION_QUERY
FILE_MAP_WRITE                  EQU     SECTION_MAP_WRITE
FILE_MAP_READ                   EQU     SECTION_MAP_READ
FILE_MAP_ALL_ACCESS             EQU     SECTION_ALL_ACCESS

PAGE_NOACCESS                   EQU     00000001h
PAGE_READONLY                   EQU     00000002h
PAGE_READWRITE                  EQU     00000004h
PAGE_WRITECOPY                  EQU     00000008h
PAGE_EXECUTE                    EQU     00000010h
PAGE_EXECUTE_READ               EQU     00000020h
PAGE_EXECUTE_READWRITE          EQU     00000040h
PAGE_EXECUTE_WRITECOPY          EQU     00000080h
PAGE_GUARD                      EQU     00000100h
PAGE_NOCACHE                    EQU     00000200h
MEM_COMMIT                      EQU     00001000h
MEM_RESERVE                     EQU     00002000h
MEM_DECOMMIT                    EQU     00004000h
MEM_RELEASE                     EQU     00008000h
MEM_FREE                        EQU     00010000h
MEM_PRIVATE                     EQU     00020000h
MEM_MAPPED                      EQU     00040000h
MEM_TOP_DOWN                    EQU     00100000h
SEC_FILE                        EQU     00800000h
SEC_IMAGE                       EQU     01000000h
SEC_RESERVE                     EQU     04000000h
SEC_COMMIT                      EQU     08000000h
SEC_NOCACHE                     EQU     10000000h
MEM_IMAGE                       EQU     SEC_IMAGE

;GlobalAlloc
;------------------------------------------------
GMEM_FIXED                equ 0000h
GMEM_MOVEABLE             equ 0002h
GMEM_ZEROINIT             equ 0040h
GHND                      equ 0042h
GPTR                      equ 0040h

;------------------------------------------------
;For user32 functions
;------------------------------------------------
;Menu Flag
;------------------------------------------------
MF_INSERT                 equ 0h
MF_CHANGE                 equ 80h
MF_APPEND                 equ 100h
MF_DELETE                 equ 200h
MF_REMOVE                 equ 1000h
MF_BYCOMMAND              equ 0h
MF_BYPOSITION             equ 400h
MF_SEPARATOR              equ 800h
MF_ENABLED                equ 0h
MF_GRAYED                 equ 1h
MF_DISABLED               equ 2h
MF_UNCHECKED              equ 0h
MF_CHECKED                equ 8h
MF_USECHECKBITMAPS        equ 200h
MF_STRING                 equ 0h
MF_BITMAP                 equ 4h
MF_OWNERDRAW              equ 100h
MF_POPUP                  equ 10h
MF_MENUBARBREAK           equ 20h
MF_MENUBREAK              equ 40h
MF_UNHILITE               equ 0h
MF_HILITE                 equ 80h
MF_SYSMENU                equ 2000h
MF_HELP                   equ 4000h
MF_MOUSESELECT            equ 8000h
MF_END                    equ 80h
MF_DEFAULT                equ 1000h
MF_RIGHTJUSTIFY           equ 4000h

; ÄÄ´ Message Box suport valuez ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

MB_OK                           EQU     00000000h
MB_OKCANCEL                     EQU     00000001h
MB_ABORTRETRYIGNORE             EQU     00000002h
MB_YESNOCANCEL                  EQU     00000003h
MB_YESNO                        EQU     00000004h
MB_RETRYCANCEL                  EQU     00000005h
MB_TYPEMASK                     EQU     0000000Fh

MB_ICONHAND                     EQU     00000010h
MB_ICONQUESTION                 EQU     00000020h
MB_ICONEXCLAMATION              EQU     00000030h
MB_ICONASTERISK                 EQU     00000040h
MB_ICONMASK                     EQU     000000F0h

MB_ICONINFORMATION              EQU     MB_ICONASTERISK
MB_ICONSTOP                     EQU     MB_ICONHAND

MB_DEFBUTTON1                   EQU     00000000h
MB_DEFBUTTON2                   EQU     00000100h
MB_DEFBUTTON3                   EQU     00000200h
MB_DEFMASK                      EQU     00000F00h

MB_APPLMODAL                    EQU     00000000h
MB_SYSTEMMODAL                  EQU     00001000h
MB_TASKMODAL                    EQU     00002000h

MB_NOFOCUS                      EQU     00008000h

; ÄÄ´ Some global constantz ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

NULL                            EQU     0
FALSE                           EQU     0
TRUE                            EQU     1
MAX_PATH                        EQU     260
INVALID_HANDLE_VALUE            EQU     -1
STANDARD_RIGHTS_REQUIRED        EQU     000F0000h

; ÄÄ´ Desired access valuez ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

GENERIC_READ                    EQU     80000000h
GENERIC_WRITE                   EQU     40000000h

; ÄÄ´ Share mode valuez ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

FILE_SHARE_READ                 EQU     00000001h
FILE_SHARE_WRITE                EQU     00000002h

; ÄÄ´ Creation disposition valuez ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

CREATE_NEW                      EQU     1
CREATE_ALWAYS                   EQU     2
OPEN_EXISTING                   EQU     3
OPEN_ALWAYS                     EQU     4
TRUNCATE_EXISTING               EQU     5

; ÄÄ´ File attributez and flag valuez ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

FILE_ATTRIBUTE_READONLY         EQU     00000001h
FILE_ATTRIBUTE_HIDDEN           EQU     00000002h
FILE_ATTRIBUTE_SYSTEM           EQU     00000004h
FILE_ATTRIBUTE_DIRECTORY        EQU     00000010h
FILE_ATTRIBUTE_ARCHIVE          EQU     00000020h
FILE_ATTRIBUTE_NORMAL           EQU     00000080h
FILE_ATTRIBUTE_TEMPORARY        EQU     00000100h
FILE_ATTRIBUTE_ATOMIC_WRITE     EQU     00000200h
FILE_ATTRIBUTE_XACTION_WRITE    EQU     00000400h
FILE_ATTRIBUTE_COMPRESSED       EQU     00000800h
FILE_ATTRIBUTE_HAS_EMBEDDING    EQU     00001000h

FILE_FLAG_POSIX_SEMANTICS       EQU     01000000h
FILE_FLAG_BACKUP_SEMANTICS      EQU     02000000h
FILE_FLAG_DELETE_ON_CLOSE       EQU     04000000h
FILE_FLAG_SEQUENTIAL_SCAN       EQU     08000000h
FILE_FLAG_RANDOM_ACCESS         EQU     10000000h
FILE_FLAG_NO_BUFFERING          EQU     20000000h
FILE_FLAG_OVERLAPPED            EQU     40000000h
FILE_FLAG_WRITE_THROUGH         EQU     80000000h

; ÄÄ´ Protection and other valuez ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

SECTION_QUERY                   EQU     00000001h
SECTION_MAP_WRITE               EQU     00000002h
SECTION_MAP_READ                EQU     00000004h
SECTION_MAP_EXECUTE             EQU     00000008h
SECTION_EXTEND_SIZE             EQU     00000010h

SECTION_ALL_ACCESS              EQU     STANDARD_RIGHTS_REQUIRED OR \
                                        SECTION_QUERY            OR \
                                        SECTION_MAP_WRITE        OR \
                                        SECTION_MAP_READ         OR \
                                        SECTION_MAP_EXECUTE      OR \
                                        SECTION_EXTEND_SIZE

FILE_MAP_COPY                   EQU     SECTION_QUERY
FILE_MAP_WRITE                  EQU     SECTION_MAP_WRITE
FILE_MAP_READ                   EQU     SECTION_MAP_READ
FILE_MAP_ALL_ACCESS             EQU     SECTION_ALL_ACCESS

; ÄÄ´ Code Page valuez ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

CP_ACP                          EQU     0       ; ANSI code page
CP_OEMCP                        EQU     1       ; OEM  code page
CP_MACCP                        EQU     2       ; MAC  code page

; ÄÄ´ LoadLibrary dwFlagz ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

DONT_RESOLVE_DLL_REFERENCES		EQU	00000001h
LOAD_IGNORE_CODE_AUTHZ_LEVEL		EQU	00000010h
LOAD_LIBRARY_AS_DATAFILE		EQU	00000002h
LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE	EQU	00000040h
LOAD_LIBRARY_AS_IMAGE_RESOURCE		EQU	00000020h
LOAD_WITH_ALTERED_SEARCH_PATH		EQU	00000008h

; ÄÄ´ Some general Win32 related structurez ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

FILETIME                        STRUC
        FT_dwLowDateTime        DD ?
        FT_dwHighDateTime       DD ?
FILETIME                        ENDS

WIN32_FIND_DATA                 STRUC
        WFD_dwFileAttributes    DD ?
        WFD_ftCreationTime      FILETIME <>
        WFD_ftLastAccessTime    FILETIME <>
        WFD_ftLastWriteTime     FILETIME <>
        WFD_nFileSizeHigh       DD ?
        WFD_nFileSizeLow        DD ?
        WFD_dwReserved0         DD ?
        WFD_dwReserved1         DD ?
        WFD_szFileName          DB MAX_PATH DUP (?)
        WFD_szAlternateFileName DB 13 DUP (?)
                                DB 3 DUP (?)    ; dword padding
WIN32_FIND_DATA                 ENDS

SIZEOF_WIN32_FIND_DATA  EQU     SIZE WIN32_FIND_DATA

; ÄÄ´ SEH related constantz and return valuez ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

EXCEPTION_EXECUTE_HANDLER       	EQU     1
EXCEPTION_CONTINUE_SEARCH       	EQU     0
EXCEPTION_CONTINUE_EXECUTION    	EQU     -1

EXCEPTION_ACCESS_VIOLATION              EQU     0C0000005h
EXCEPTION_DATATYPE_MISALIGNMENT         EQU     080000002h
EXCEPTION_BREAKPOINT                    EQU     080000003h
EXCEPTION_SINGLE_STEP                   EQU     080000004h
EXCEPTION_ARRAY_BOUNDS_EXCEEDED         EQU     0C000008Ch
EXCEPTION_FLT_DENORMAL_OPERAND          EQU     0C000008Dh
EXCEPTION_FLT_DIVIDE_BY_ZERO            EQU     0C000008Eh
EXCEPTION_FLT_INEXACT_RESULT            EQU     0C000008Fh
EXCEPTION_FLT_INVALID_OPERATION         EQU     0C0000090h
EXCEPTION_FLT_OVERFLOW                  EQU     0C0000091h
EXCEPTION_FLT_STACK_CHECK               EQU     0C0000092h
EXCEPTION_FLT_UNDERFLOW                 EQU     0C0000093h
EXCEPTION_INT_DIVIDE_BY_ZERO            EQU     0C0000094h
EXCEPTION_INT_OVERFLOW                  EQU     0C0000095h
EXCEPTION_PRIV_INSTRUCTION              EQU     0C0000096h
EXCEPTION_IN_PAGE_ERROR                 EQU     0C0000006h
EXCEPTION_ILLEGAL_INSTRUCTION           EQU     0C000001Dh
EXCEPTION_NONCONTINUABLE_EXCEPTION      EQU     0C0000025h
EXCEPTION_STACK_OVERFLOW                EQU     0C00000FDh
EXCEPTION_INVALID_DISPOSITION           EQU     0C0000026h
EXCEPTION_GUARD_PAGE                    EQU     080000001h

;==============================================================================
;windows api functions
;write by ezasm(ezasm@hotmail.com)
;==============================================================================
;kernel32.dll
;------------------------------------------------
DebugBreak		PROTO
VirtualAlloc	PROTO :DWORD,:DWORD,:DWORD,:DWORD
VirtualAllocEx	PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
VirtualFree		PROTO :DWORD,:DWORD,:DWORD
VirtualFreeEx	PROTO :DWORD,:DWORD,:DWORD,:DWORD
VirtualLock		PROTO :DWORD,:DWORD
VirtualProtect	PROTO :DWORD,:DWORD,:DWORD,:DWORD
VirtualProtectEx	PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
VirtualQuery	PROTO :DWORD,:DWORD,:DWORD
VirtualQueryEx	PROTO :DWORD,:DWORD,:DWORD,:DWORD
VirtualUnlock	PROTO :DWORD,:DWORD
GlobalAlloc		PROTO :DWORD,:DWORD
GlobalFree		PROTO :DWORD
RtlZeroMemory	PROTO :DWORD,:DWORD
lstrlenW		PROTO :DWORD
lstrcpyW		PROTO :DWORD,:DWORD
SystemTimeToFileTime	PROTO :DWORD,:DWORD
GetTickCount	PROTO

;user32.dll
;------------------------------------------------
GetMenu			PROTO :DWORD
CreateMenu		PROTO
InsertMenuA		PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD
AppendMenuA		PROTO :DWORD,:DWORD,:DWORD,:DWORD
CreateWindowExW PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD
CreateWindowExA PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD

;ws2_32.dll
;------------------------------------------------
bind			PROTO :DWORD,:DWORD,:DWORD

;==============================================================================
; vc runtime library
;==============================================================================
;vs lib
;------------------------------------------------
realloc			PROTO :DWORD,:DWORD
malloc			PROTO :DWORD
free			PROTO :DWORD
HeapAlloc		PROTO :DWORD,:DWORD,:DWORD
HeapFree		PROTO :DWORD,:DWORD,:DWORD
memset			PROTO :DWORD,:DWORD,:DWORD
memcpy			PROTO :DWORD,:DWORD,:DWORD
memmove			PROTO :DWORD,:DWORD,:DWORD
memcpy_s		PROTO :DWORD,:DWORD,:DWORD,:DWORD
wcscpy_s		PROTO :DWORD,:DWORD,:DWORD
wcsncpy_s		PROTO :DWORD,:DWORD,:DWORD,:DWORD
wcslen			PROTO :DWORD
wcsncpy			PROTO :DWORD,:DWORD,:DWORD
WSASend			PROTO :DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD,:DWORD

; ÄÄ´ macros ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ


XPUSHAD_ALL_NO_RBP MACRO

	sub rsp, 0B0h
	
	mov [rsp+20h], rax
	mov [rsp+28h], rbx
	mov [rsp+30h], rcx
	mov [rsp+38h], rdx
	mov [rsp+40h], r8
	mov [rsp+48h], r9 
	mov [rsp+50h], r10
	mov [rsp+58h], r11
	mov [rsp+60h], r12
	mov [rsp+68h], r13
	mov [rsp+70h], r14
	mov [rsp+78h], r15
	mov [rsp+80h], rsi
	mov [rsp+88h], rdi

ENDM

XPOPAD_ALL_NO_RBP MACRO

	mov rax, [rsp+20h]
	mov rbx, [rsp+28h]
	mov rcx, [rsp+30h]
	mov rdx, [rsp+38h]
	mov r8,  [rsp+40h]
	mov r9,  [rsp+48h]
	mov r10, [rsp+50h]
	mov r11, [rsp+58h]
	mov r12, [rsp+60h]
	mov r13, [rsp+68h]
	mov r14, [rsp+70h]
	mov r15, [rsp+78h]
	mov rsi, [rsp+80h]
	mov rdi, [rsp+88h]
	
	add rsp, 0B0h
	
ENDM

XPUSHAD_ALL_NO_RAX MACRO

    sub		rsp, 0B0h
	mov		[rsp+20h], rbx
	mov		[rsp+28h], rcx
	mov		[rsp+30h], rdx
	mov		[rsp+38h], rdi
	mov		[rsp+40h], rsi
	mov		[rsp+48h], r8
	mov		[rsp+50h], r9
	mov		[rsp+58h], r10
	mov		[rsp+60h], r11
	mov		[rsp+68h], r12
	mov		[rsp+70h], r13
	mov		[rsp+78h], r14
	mov		[rsp+80h], r15
	mov		[rsp+88h], rbp

ENDM

XPOPAD_ALL_NO_RAX MACRO

	mov		rbx, [rsp+20h]
	mov		rcx, [rsp+28h]
	mov		rdx, [rsp+30h]
	mov		rdi, [rsp+38h]
	mov		rsi, [rsp+40h]
	mov		r8,  [rsp+48h]
	mov		r9,  [rsp+50h]
	mov		r10, [rsp+58h]
	mov		r11, [rsp+60h]
	mov		r12, [rsp+68h]
	mov		r13, [rsp+70h]
	mov		r14, [rsp+78h]
	mov		r15, [rsp+80h]
	mov		rbp, [rsp+88h]
	add		rsp, 0B0h
	
ENDM

XPUSHAD_ALL_NO_RBP_NO_RAX MACRO

	sub rsp, 0B0h
	
	mov [rsp+28h], rbx
	mov [rsp+30h], rcx
	mov [rsp+38h], rdx
	mov [rsp+40h], r8
	mov [rsp+48h], r9 
	mov [rsp+50h], r10
	mov [rsp+58h], r11
	mov [rsp+60h], r12
	mov [rsp+68h], r13
	mov [rsp+70h], r14
	mov [rsp+78h], r15
	mov [rsp+80h], rsi
	mov [rsp+88h], rdi

ENDM

XPOPAD_ALL_NO_RBP_NO_RAX MACRO

	mov rbx, [rsp+28h]
	mov rcx, [rsp+30h]
	mov rdx, [rsp+38h]
	mov r8,  [rsp+40h]
	mov r9,  [rsp+48h]
	mov r10, [rsp+50h]
	mov r11, [rsp+58h]
	mov r12, [rsp+60h]
	mov r13, [rsp+68h]
	mov r14, [rsp+70h]
	mov r15, [rsp+78h]
	mov rsi, [rsp+80h]
	mov rdi, [rsp+88h]
	
	add rsp, 0B0h
	
ENDM

XPUSHAD_ALL MACRO

    sub		rsp, 0B0h
	mov		[rsp+20h], rbx
	mov		[rsp+28h], rcx
	mov		[rsp+30h], rdx
	mov		[rsp+38h], rdi
	mov		[rsp+40h], rsi
	mov		[rsp+48h], r8
	mov		[rsp+50h], r9
	mov		[rsp+58h], r10
	mov		[rsp+60h], r11
	mov		[rsp+68h], r12
	mov		[rsp+70h], r13
	mov		[rsp+78h], r14
	mov		[rsp+80h], r15
	mov		[rsp+88h], rbp
	mov		[rsp+90h], rax

ENDM

XPOPAD_ALL MACRO

	mov		rbx, [rsp+20h]
	mov		rcx, [rsp+28h]
	mov		rdx, [rsp+30h]
	mov		rdi, [rsp+38h]
	mov		rsi, [rsp+40h]
	mov		r8,  [rsp+48h]
	mov		r9,  [rsp+50h]
	mov		r10, [rsp+58h]
	mov		r11, [rsp+60h]
	mov		r12, [rsp+68h]
	mov		r13, [rsp+70h]
	mov		r14, [rsp+78h]
	mov		r15, [rsp+80h]
	mov		rbp, [rsp+88h]
	mov		rax, [rsp+90h]
	add		rsp, 0B0h
	
ENDM

XPUSHAD_NO_R8 MACRO

    sub		rsp, 0B0h
	mov		[rsp+20h], rbx
	mov		[rsp+28h], rcx
	mov		[rsp+30h], rdx
	mov		[rsp+38h], rdi
	mov		[rsp+40h], rsi
	mov		[rsp+50h], r9
	mov		[rsp+58h], r10
	mov		[rsp+60h], r11
	mov		[rsp+68h], r12
	mov		[rsp+70h], r13
	mov		[rsp+78h], r14
	mov		[rsp+80h], r15
	mov		[rsp+88h], rbp
	mov		[rsp+90h], rax

ENDM

XPOPAD_NO_R8 MACRO

	mov		rbx, [rsp+20h]
	mov		rcx, [rsp+28h]
	mov		rdx, [rsp+30h]
	mov		rdi, [rsp+38h]
	mov		rsi, [rsp+40h]
	mov		r9,  [rsp+50h]
	mov		r10, [rsp+58h]
	mov		r11, [rsp+60h]
	mov		r12, [rsp+68h]
	mov		r13, [rsp+70h]
	mov		r14, [rsp+78h]
	mov		r15, [rsp+80h]
	mov		rbp, [rsp+88h]
	mov		rax, [rsp+90h]
	add		rsp, 0B0h
	
ENDM

;replacement for MyFunctionName proc ... does alignment and .code correctly.
;usage: FUNC_BEGIN MyFunctionName
;
FUNC_BEGIN macro Name

_TEXT$00 segment para 'CODE'

        align   8
        public  Name
Name    proc

endm
        
;usage: FUNC_END MyFunctionName
;ends function / code ends
;        
FUNC_END macro Name

Name    endp
_TEXT$00 ends

endm


; Usage:
;
;       jmp     loc(123456h)
;
; I know it should be better done using LOCAL, but this way it looks
; nicer when disassembled, and each pointer is only defined once ;)

loc MACRO Address
	SaveSeg textequ @CurSeg
	Pointer textequ <ploc_&Address&>
	IFNDEF ploc_&Address&
		.CONST
		Pointer dq Address
		SaveSeg SEGMENT
	ENDIF
	EXITM <[Pointer]>
ENDM
