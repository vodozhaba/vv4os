; File: proc_exts/fpu.asm
; Author: velikiyv4
; Created on: Aug 21, 2016
; Purpose: Provides interface for FPU if there is one.

section .data
	status dw 0xFFFF
section .text
Use32
	[GLOBAL HasFpu]
	HasFpu:
		mov eax, cr0
		and eax, 0xFFFFFFF3
		mov cr0, eax
		fninit
		fnstsw [status]
		cmp word [status], 0
		jz .has
			mov eax, 0
			ret
		.has:
			mov eax, 1
			ret
