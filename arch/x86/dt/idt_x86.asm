; File:       /vv4os/arch/x86/dt/idt_x86.asm
; Author:     velikiyv4
; Created on: Oct 1, 2016
; Purpose:    Allows working with x86 Interrupt Descriptor Table.

section .text
Use32
	global SetIdtr
	SetIdtr:
		push ebp
		mov ebp, esp
		mov eax, [ebp+8]
		lidt [eax]
		leave
		ret
