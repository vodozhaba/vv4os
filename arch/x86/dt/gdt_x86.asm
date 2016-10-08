; File:       /vv4os/arch/x86/dt/gdt_x86.asm
; Author:     velikiyv4
; Created on: Oct 1, 2016
; Purpose:    Allows working with x86 Global Descriptor Table.

 section .text
 Use32
 	global SetGdtr
 	SetGdtr:
 		push ebp
 		mov ebp, esp
 		mov eax, [ebp+8]
 		lgdt [eax]
 		mov eax, 0x10
		mov cs, eax
		mov eax, 0x18
		mov ds, eax
		mov es, eax
		mov fs, eax
		mov gs, eax
 		leave
 		ret
