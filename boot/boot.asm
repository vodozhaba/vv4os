; File: boot/boot.asm
; Author: velikiyv4
; Created on: 08/20/2016
; Purpose: Sets up stack and calls main().

; Multiboot flags

F_MBALIGN equ 1<<0
F_MEMINFO equ 1<<1
F_OWN_VIDEO_MODE equ 1<<2
F_OWN_ADDRS equ 1<<16

; Multiboot required constants

FLAGS equ F_MBALIGN | F_MEMINFO
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC+FLAGS)

; Multiboot constants which are only used if F_OWN_ADDRS flag is set

HEADER_ADDR equ 0
LOAD_ADDR equ 0
LOAD_END_ADDR equ 0
BSS_END_ADDR equ 0
ENTRY_ADDR equ 0

; Multiboot video mode type

MB_VMODE_LINEAR_GRAPHICS equ 0
MB_VMODE_EGA equ 1

; Multiboot constants which are only used if F_OWN_VIDEO_MODE flag is set

MODE_TYPE equ 0
WIDTH equ 0
HEIGHT equ 0
DEPTH equ 0

; Multiboot header

section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM
	dd HEADER_ADDR
	dd LOAD_ADDR
	dd LOAD_END_ADDR
	dd BSS_END_ADDR
	dd ENTRY_ADDR
	dd MODE_TYPE
	dd WIDTH
	dd HEIGHT
    dd DEPTH

section .bootstrap_stack, nobits
align 4
	stack_bottom:
		resb 16384 ; 16K of stack seem enough
	stack_top:

section .text
Use32
	global _start
	_start:
	mov esp, stack_top ; Stack grows in a backwards direction
	extern main
	call main
	cli ; If _kmain returns, we will halt the computer
.halt:
	hlt
	jmp .halt
