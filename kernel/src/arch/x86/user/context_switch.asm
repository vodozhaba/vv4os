; File:       arch/x86/user/context_switch.asm
; Author:     vodozhaba
; Created on: Feb 09, 2018
; Purpose:    Low-level code to be run between kernel and user space.

section .text
Use32
    global UserlandJump
    UserlandJump:
        push ebp
        mov ebp, esp
        mov eax, [ebp+12]
        push eax
        extern X86SwitchPageDirectory
        call X86SwitchPageDirectory
        mov ax, 0x23
        mov ds, ax
        mov es, ax
        mov gs, ax
        mov fs, ax
        mov eax, esp
        push 0x23
        push eax
        pushf
        push 0x1B
        mov eax, [ebp+8]
        push eax
        iret