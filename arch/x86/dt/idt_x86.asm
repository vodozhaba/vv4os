; File:       /vv4os/arch/x86/dt/idt_x86.asm
; Author:     velikiyv4
; Created on: Oct 1, 2016
; Purpose:    Allows working with x86 Interrupt Descriptor Table.

section .text
Use32
	global X86SetIdtr
	X86SetIdtr:
		push ebp
		mov ebp, esp
		mov eax, [ebp+8]
		lidt [eax]
		leave
		ret

 	%macro X86IsrNoErr 1
 	global X86Isr%1
 	X86Isr%1:
 		cli
 		push dword 0
 		push dword %1
 		jmp X86IsrCommon
 	%endmacro
 	%macro X86IsrErr 1
 	global X86Isr%1
 	X86Isr%1:
 		cli
 		push dword %1
 		jmp X86IsrCommon
 	%endmacro

 	X86IsrNoErr 0
 	X86IsrNoErr 1
 	X86IsrNoErr 2
 	X86IsrNoErr 3
 	X86IsrNoErr 4
 	X86IsrNoErr 5
 	X86IsrNoErr 6
 	X86IsrNoErr 7
 	X86IsrErr   8
 	X86IsrNoErr 9
 	X86IsrErr   10
 	X86IsrErr   11
 	X86IsrErr   12
 	X86IsrErr   13
 	X86IsrErr   14
 	X86IsrNoErr 15
 	X86IsrNoErr 16
 	X86IsrNoErr 17
 	X86IsrNoErr 18
 	X86IsrNoErr 19
 	X86IsrNoErr 20
 	X86IsrNoErr 21
 	X86IsrNoErr 22
 	X86IsrNoErr 23
 	X86IsrNoErr 24
 	X86IsrNoErr 25
 	X86IsrNoErr 26
 	X86IsrNoErr 27
 	X86IsrNoErr 28
 	X86IsrNoErr 29
 	X86IsrNoErr 30
 	X86IsrNoErr 31

	X86IsrCommon:
		pusha
		xor eax, eax
		mov ax, ds
		push eax
		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov gs, ax
		mov fs, ax
		extern X86IsrHandler
		call X86IsrHandler
		pop eax
		mov ds, ax
		mov es, ax
		mov gs, ax
		mov fs, ax
		popa
		add esp, 8
		iret

