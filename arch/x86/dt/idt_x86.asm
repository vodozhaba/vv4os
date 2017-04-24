; File:       /vv4os/arch/x86/dt/idt_x86.asm
; Author:     vodozhaba
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

 	%macro X86SoftInt 1
 	global X86SoftInt%1h
 	X86SoftInt%1h:
 		cli
 		push dword 0
 		push dword 0x%1
 		jmp X86SoftIntCommon
 	%endmacro

 	%macro X86Irq 2
 	global X86Irq%1
 	X86Irq%1:
 		cli
 		push dword 0
 		push dword 0x%2
 		jmp X86IrqCommon
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

	X86Irq 0,  20
	X86Irq 1,  21
	X86Irq 2,  22
	X86Irq 3,  23
	X86Irq 4,  24
	X86Irq 5,  25
	X86Irq 6,  26
	X86Irq 7,  27
	X86Irq 8,  28
	X86Irq 9,  29
	X86Irq 10, 2A
	X86Irq 11, 2B
	X86Irq 12, 2C
	X86Irq 13, 2D
	X86Irq 14, 2E
	X86Irq 15, 2F


 	X86SoftInt 30
 	X86SoftInt 31
 	X86SoftInt 32
 	X86SoftInt 33
 	X86SoftInt 34
 	X86SoftInt 35
 	X86SoftInt 36
 	X86SoftInt 37
 	X86SoftInt 38
 	X86SoftInt 39
 	X86SoftInt 3A
 	X86SoftInt 3B
 	X86SoftInt 3C
 	X86SoftInt 3D
 	X86SoftInt 3E
 	X86SoftInt 3F
 	X86SoftInt 40
 	X86SoftInt 41
 	X86SoftInt 42
 	X86SoftInt 43
 	X86SoftInt 44
 	X86SoftInt 45
 	X86SoftInt 46
 	X86SoftInt 47
 	X86SoftInt 48
 	X86SoftInt 49
 	X86SoftInt 4A
 	X86SoftInt 4B
 	X86SoftInt 4C
 	X86SoftInt 4D
 	X86SoftInt 4E
 	X86SoftInt 4F
 	X86SoftInt 50
 	X86SoftInt 51
 	X86SoftInt 52
 	X86SoftInt 53
 	X86SoftInt 54
 	X86SoftInt 55
 	X86SoftInt 56
 	X86SoftInt 57
 	X86SoftInt 58
 	X86SoftInt 59
 	X86SoftInt 5A
 	X86SoftInt 5B
 	X86SoftInt 5C
 	X86SoftInt 5D
 	X86SoftInt 5E
 	X86SoftInt 5F
 	X86SoftInt 60
 	X86SoftInt 61
 	X86SoftInt 62
 	X86SoftInt 63
 	X86SoftInt 64
 	X86SoftInt 65
 	X86SoftInt 66
 	X86SoftInt 67
 	X86SoftInt 68
 	X86SoftInt 69
 	X86SoftInt 6A
 	X86SoftInt 6B
 	X86SoftInt 6C
 	X86SoftInt 6D
 	X86SoftInt 6E
 	X86SoftInt 6F
 	X86SoftInt 70
 	X86SoftInt 71
 	X86SoftInt 72
 	X86SoftInt 73
 	X86SoftInt 74
 	X86SoftInt 75
 	X86SoftInt 76
 	X86SoftInt 77
 	X86SoftInt 78
 	X86SoftInt 79
 	X86SoftInt 7A
 	X86SoftInt 7B
 	X86SoftInt 7C
 	X86SoftInt 7D
 	X86SoftInt 7E
 	X86SoftInt 7F
 	X86SoftInt 80
 	X86SoftInt 81
 	X86SoftInt 82
 	X86SoftInt 83
 	X86SoftInt 84
 	X86SoftInt 85
 	X86SoftInt 86
 	X86SoftInt 87
 	X86SoftInt 88
 	X86SoftInt 89
 	X86SoftInt 8A
 	X86SoftInt 8B
 	X86SoftInt 8C
 	X86SoftInt 8D
 	X86SoftInt 8E
 	X86SoftInt 8F
 	X86SoftInt 90
 	X86SoftInt 91
 	X86SoftInt 92
 	X86SoftInt 93
 	X86SoftInt 94
 	X86SoftInt 95
 	X86SoftInt 96
 	X86SoftInt 97
 	X86SoftInt 98
 	X86SoftInt 99
 	X86SoftInt 9A
 	X86SoftInt 9B
 	X86SoftInt 9C
 	X86SoftInt 9D
 	X86SoftInt 9E
 	X86SoftInt 9F
 	X86SoftInt A0
 	X86SoftInt A1
 	X86SoftInt A2
 	X86SoftInt A3
 	X86SoftInt A4
 	X86SoftInt A5
 	X86SoftInt A6
 	X86SoftInt A7
 	X86SoftInt A8
 	X86SoftInt A9
 	X86SoftInt AA
 	X86SoftInt AB
 	X86SoftInt AC
 	X86SoftInt AD
 	X86SoftInt AE
 	X86SoftInt AF
 	X86SoftInt B0
 	X86SoftInt B1
 	X86SoftInt B2
 	X86SoftInt B3
 	X86SoftInt B4
 	X86SoftInt B5
 	X86SoftInt B6
 	X86SoftInt B7
 	X86SoftInt B8
 	X86SoftInt B9
 	X86SoftInt BA
 	X86SoftInt BB
 	X86SoftInt BC
 	X86SoftInt BD
 	X86SoftInt BE
 	X86SoftInt BF
 	X86SoftInt C0
 	X86SoftInt C1
 	X86SoftInt C2
 	X86SoftInt C3
 	X86SoftInt C4
 	X86SoftInt C5
 	X86SoftInt C6
 	X86SoftInt C7
 	X86SoftInt C8
 	X86SoftInt C9
 	X86SoftInt CA
 	X86SoftInt CB
 	X86SoftInt CC
 	X86SoftInt CD
 	X86SoftInt CE
 	X86SoftInt CF
 	X86SoftInt D0
 	X86SoftInt D1
 	X86SoftInt D2
 	X86SoftInt D3
 	X86SoftInt D4
 	X86SoftInt D5
 	X86SoftInt D6
 	X86SoftInt D7
 	X86SoftInt D8
 	X86SoftInt D9
 	X86SoftInt DA
 	X86SoftInt DB
 	X86SoftInt DC
 	X86SoftInt DD
 	X86SoftInt DE
 	X86SoftInt DF
 	X86SoftInt E0
 	X86SoftInt E1
 	X86SoftInt E2
 	X86SoftInt E3
 	X86SoftInt E4
 	X86SoftInt E5
 	X86SoftInt E6
 	X86SoftInt E7
 	X86SoftInt E8
 	X86SoftInt E9
 	X86SoftInt EA
 	X86SoftInt EB
 	X86SoftInt EC
 	X86SoftInt ED
 	X86SoftInt EE
 	X86SoftInt EF
 	X86SoftInt F0
 	X86SoftInt F1
 	X86SoftInt F2
 	X86SoftInt F3
 	X86SoftInt F4
 	X86SoftInt F5
 	X86SoftInt F6
 	X86SoftInt F7
 	X86SoftInt F8
 	X86SoftInt F9
 	X86SoftInt FA
 	X86SoftInt FB
 	X86SoftInt FC
 	X86SoftInt FD
 	X86SoftInt FE
 	X86SoftInt FF

	X86IsrCommon:
		pusha
		xor eax, eax
		mov ax, ds
		push eax
		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		extern X86IsrHandler
		call X86IsrHandler
		pop eax
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		popa
		add esp, 8
		iret

	X86SoftIntCommon:
		pusha
		xor eax, eax
		mov ax, ds
		push eax
		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		extern X86SoftIntHandler
		call X86SoftIntHandler
		pop eax
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		popa
		add esp, 8
		iret

		X86IrqCommon:
		pusha
		xor eax, eax
		mov ax, ds
		push eax
		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		extern X86IrqHandler
		call X86IrqHandler
		pop eax
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		popa
		add esp, 8
		sti
		iret

