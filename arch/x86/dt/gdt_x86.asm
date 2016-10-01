; File:       /vv4os/arch/x86/dt/gdt.asm
; Author:     velikiyv4
; Created on: Oct 1, 2016
; Purpose:    Allows working with x86 Global Descriptor Table.

 section .text
 Use32
 	global SetGdtr
 	SetGdtr:
 		push ebp
 		mov ebp, esp
 		mov eax, [ebp+4]
 		lgdt [eax]
 		leave
 		ret
