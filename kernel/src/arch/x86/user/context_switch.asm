; File:       arch/x86/user/context_switch.asm
; Author:     vodozhaba
; Created on: Feb 09, 2018
; Purpose:    Low-level code to be run between kernel and user space.

section .text
Use32
    global X86UserlandJump
    X86UserlandJump:
        push ebp
        mov ebp, esp
        mov eax, [ebp+8]
        push eax
        extern X86SwitchPageDirectory
        call X86SwitchPageDirectory
        lea esp, [ebp+12]
        pop eax
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov al, 0x20
        out 0x20, al
        popad
        add esp, 8
        iret