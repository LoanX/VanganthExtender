; ****************************************************************************
; Module: VirtualizerSDKCustomVmMacros.asm
; Description: Another way to link with the SecureEngine SDK via an ASM module
;
; Author/s: Oreans Technologies 
; (c) 2014 Oreans Technologies
;
; --- File generated automatically from Oreans VM Generator (19/8/2014) ---
; ****************************************************************************


IFDEF RAX

ELSE

.586
.model flat,stdcall
option casemap:none

ENDIF


; ****************************************************************************
;                                 Constants
; ****************************************************************************

.CONST


; ****************************************************************************
;                               Data Segment
; ****************************************************************************

.DATA


; ****************************************************************************
;                               Code Segment
; ****************************************************************************

.CODE

IFDEF RAX

; ****************************************************************************
; VIRTUALIZER_TIGER_WHITE definition
; ****************************************************************************

VIRTUALIZER_TIGER_WHITE_START_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 103
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_TIGER_WHITE_START_ASM64 ENDP

VIRTUALIZER_TIGER_WHITE_END_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 503
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_TIGER_WHITE_END_ASM64 ENDP


; ****************************************************************************
; VIRTUALIZER_TIGER_RED definition
; ****************************************************************************

VIRTUALIZER_TIGER_RED_START_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 104
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_TIGER_RED_START_ASM64 ENDP

VIRTUALIZER_TIGER_RED_END_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 504
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_TIGER_RED_END_ASM64 ENDP


; ****************************************************************************
; VIRTUALIZER_TIGER_BLACK definition
; ****************************************************************************

VIRTUALIZER_TIGER_BLACK_START_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 105
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_TIGER_BLACK_START_ASM64 ENDP

VIRTUALIZER_TIGER_BLACK_END_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 505
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_TIGER_BLACK_END_ASM64 ENDP


; ****************************************************************************
; VIRTUALIZER_FISH_WHITE definition
; ****************************************************************************

VIRTUALIZER_FISH_WHITE_START_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 107
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_FISH_WHITE_START_ASM64 ENDP

VIRTUALIZER_FISH_WHITE_END_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 507
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_FISH_WHITE_END_ASM64 ENDP


; ****************************************************************************
; VIRTUALIZER_FISH_RED definition
; ****************************************************************************

VIRTUALIZER_FISH_RED_START_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 109
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_FISH_RED_START_ASM64 ENDP

VIRTUALIZER_FISH_RED_END_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 509
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_FISH_RED_END_ASM64 ENDP


; ****************************************************************************
; VIRTUALIZER_FISH_BLACK definition
; ****************************************************************************

VIRTUALIZER_FISH_BLACK_START_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 111
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_FISH_BLACK_START_ASM64 ENDP

VIRTUALIZER_FISH_BLACK_END_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 511
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_FISH_BLACK_END_ASM64 ENDP


; ****************************************************************************
; VIRTUALIZER_PUMA_WHITE definition
; ****************************************************************************

VIRTUALIZER_PUMA_WHITE_START_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 113
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_PUMA_WHITE_START_ASM64 ENDP

VIRTUALIZER_PUMA_WHITE_END_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 513
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_PUMA_WHITE_END_ASM64 ENDP


; ****************************************************************************
; VIRTUALIZER_PUMA_RED definition
; ****************************************************************************

VIRTUALIZER_PUMA_RED_START_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 115
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_PUMA_RED_START_ASM64 ENDP

VIRTUALIZER_PUMA_RED_END_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 515
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_PUMA_RED_END_ASM64 ENDP


; ****************************************************************************
; VIRTUALIZER_PUMA_BLACK definition
; ****************************************************************************

VIRTUALIZER_PUMA_BLACK_START_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 117
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_PUMA_BLACK_START_ASM64 ENDP

VIRTUALIZER_PUMA_BLACK_END_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 517
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_PUMA_BLACK_END_ASM64 ENDP


; ****************************************************************************
; VIRTUALIZER_SHARK_WHITE definition
; ****************************************************************************

VIRTUALIZER_SHARK_WHITE_START_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 119
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_SHARK_WHITE_START_ASM64 ENDP

VIRTUALIZER_SHARK_WHITE_END_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 519
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_SHARK_WHITE_END_ASM64 ENDP


; ****************************************************************************
; VIRTUALIZER_SHARK_RED definition
; ****************************************************************************

VIRTUALIZER_SHARK_RED_START_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 121
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_SHARK_RED_START_ASM64 ENDP

VIRTUALIZER_SHARK_RED_END_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 521
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_SHARK_RED_END_ASM64 ENDP


; ****************************************************************************
; VIRTUALIZER_SHARK_BLACK definition
; ****************************************************************************

VIRTUALIZER_SHARK_BLACK_START_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 123
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_SHARK_BLACK_START_ASM64 ENDP

VIRTUALIZER_SHARK_BLACK_END_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 523
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_SHARK_BLACK_END_ASM64 ENDP


; ****************************************************************************
; VIRTUALIZER_MUTATE_ONLY definition
; ****************************************************************************

VIRTUALIZER_MUTATE_ONLY_START_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 16
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_MUTATE_ONLY_START_ASM64 ENDP

VIRTUALIZER_MUTATE_ONLY_END_ASM64 PROC

    push    rax
    push    rbx
    push    rcx

    mov     eax, 'CV'
    mov     ebx, 17
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     rcx
    pop     rbx
    pop     rax
    ret

VIRTUALIZER_MUTATE_ONLY_END_ASM64 ENDP

ELSE

; ****************************************************************************
; VIRTUALIZER_TIGER_WHITE definition
; ****************************************************************************

VIRTUALIZER_TIGER_WHITE_START_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 100
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_TIGER_WHITE_START_ASM32 ENDP

VIRTUALIZER_TIGER_WHITE_END_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 500
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_TIGER_WHITE_END_ASM32 ENDP


; ****************************************************************************
; VIRTUALIZER_TIGER_RED definition
; ****************************************************************************

VIRTUALIZER_TIGER_RED_START_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 101
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_TIGER_RED_START_ASM32 ENDP

VIRTUALIZER_TIGER_RED_END_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 501
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_TIGER_RED_END_ASM32 ENDP


; ****************************************************************************
; VIRTUALIZER_TIGER_BLACK definition
; ****************************************************************************

VIRTUALIZER_TIGER_BLACK_START_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 102
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_TIGER_BLACK_START_ASM32 ENDP

VIRTUALIZER_TIGER_BLACK_END_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 502
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_TIGER_BLACK_END_ASM32 ENDP


; ****************************************************************************
; VIRTUALIZER_FISH_WHITE definition
; ****************************************************************************

VIRTUALIZER_FISH_WHITE_START_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 106
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_FISH_WHITE_START_ASM32 ENDP

VIRTUALIZER_FISH_WHITE_END_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 506
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_FISH_WHITE_END_ASM32 ENDP


; ****************************************************************************
; VIRTUALIZER_FISH_RED definition
; ****************************************************************************

VIRTUALIZER_FISH_RED_START_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 108
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_FISH_RED_START_ASM32 ENDP

VIRTUALIZER_FISH_RED_END_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 508
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_FISH_RED_END_ASM32 ENDP


; ****************************************************************************
; VIRTUALIZER_FISH_BLACK definition
; ****************************************************************************

VIRTUALIZER_FISH_BLACK_START_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 110
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_FISH_BLACK_START_ASM32 ENDP

VIRTUALIZER_FISH_BLACK_END_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 510
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_FISH_BLACK_END_ASM32 ENDP


; ****************************************************************************
; VIRTUALIZER_PUMA_WHITE definition
; ****************************************************************************

VIRTUALIZER_PUMA_WHITE_START_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 112
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_PUMA_WHITE_START_ASM32 ENDP

VIRTUALIZER_PUMA_WHITE_END_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 512
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_PUMA_WHITE_END_ASM32 ENDP


; ****************************************************************************
; VIRTUALIZER_PUMA_RED definition
; ****************************************************************************

VIRTUALIZER_PUMA_RED_START_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 114
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_PUMA_RED_START_ASM32 ENDP

VIRTUALIZER_PUMA_RED_END_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 514
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_PUMA_RED_END_ASM32 ENDP


; ****************************************************************************
; VIRTUALIZER_PUMA_BLACK definition
; ****************************************************************************

VIRTUALIZER_PUMA_BLACK_START_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 116
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_PUMA_BLACK_START_ASM32 ENDP

VIRTUALIZER_PUMA_BLACK_END_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 516
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_PUMA_BLACK_END_ASM32 ENDP


; ****************************************************************************
; VIRTUALIZER_SHARK_WHITE definition
; ****************************************************************************

VIRTUALIZER_SHARK_WHITE_START_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 118
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_SHARK_WHITE_START_ASM32 ENDP

VIRTUALIZER_SHARK_WHITE_END_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 518
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_SHARK_WHITE_END_ASM32 ENDP


; ****************************************************************************
; VIRTUALIZER_SHARK_RED definition
; ****************************************************************************

VIRTUALIZER_SHARK_RED_START_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 120
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_SHARK_RED_START_ASM32 ENDP

VIRTUALIZER_SHARK_RED_END_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 520
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_SHARK_RED_END_ASM32 ENDP


; ****************************************************************************
; VIRTUALIZER_SHARK_BLACK definition
; ****************************************************************************

VIRTUALIZER_SHARK_BLACK_START_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 122
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_SHARK_BLACK_START_ASM32 ENDP

VIRTUALIZER_SHARK_BLACK_END_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 522
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_SHARK_BLACK_END_ASM32 ENDP


; ****************************************************************************
; VIRTUALIZER_MUTATE_ONLY definition
; ****************************************************************************

VIRTUALIZER_MUTATE_ONLY_START_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 16
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_MUTATE_ONLY_START_ASM32 ENDP

VIRTUALIZER_MUTATE_ONLY_END_ASM32 PROC

    push    eax
    push    ebx
    push    ecx

    mov     eax, 'CV'
    mov     ebx, 17
    mov     ecx, 'CV'
    add     ebx, eax
    add     ecx, eax

    pop     ecx
    pop     ebx
    pop     eax
    ret

VIRTUALIZER_MUTATE_ONLY_END_ASM32 ENDP

ENDIF

END
