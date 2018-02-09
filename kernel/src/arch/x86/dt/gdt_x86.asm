; File:       arch/x86/dt/gdt_x86.asm
; Author:     vodozhaba
; Created on: Oct 1, 2016
; Purpose:    Allows working with x86 Global Descriptor Table.

 section .text
 Use32
     global X86SetGdtr
     X86SetGdtr:
 	    push ebp
 	    mov ebp, esp
 	    mov eax, [ebp+8]
 	    lgdt [eax]
 	    jmp 0x08:.flush
 	    .flush:
	    mov ax, 0x10
	    mov ds, ax
	    mov es, ax
	    mov fs, ax
	    mov gs, ax
	    mov ss, ax
		mov ax, 0x28
		ltr ax
 	    leave
 	    ret
