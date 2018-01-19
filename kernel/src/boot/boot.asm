; File: boot/boot.asm
; Author: vodozhaba
; Created on: Aug 20, 2016
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

section .data
align 0x1000
BootPageDirectory:
    times 1024 dd 0x00000000
section .bss
align 16
    stack_bottom:
	    resb 16384 ; 16K of stack seem enough
    stack_top:

section .text
Use32
    global _start
    _start:
    extern _KERNEL_STATIC_MEM_START
    extern _KERNEL_STATIC_MEM_END
    mov eax, _KERNEL_STATIC_MEM_END
    mov edx, _KERNEL_STATIC_MEM_START
    and edx, 0xFFFC0000 ; 4MiB alignment
    sub eax, edx
    xor edx, edx
    mov ecx, 0x400000
    div ecx
    test edx, edx
    jz .gen_pd
    inc eax 
    .gen_pd:
    xor ecx, ecx
    mov edx, 0x00000083
    mov esi, BootPageDirectory
    sub esi, _KERNEL_STATIC_MEM_START
    mov edi, _KERNEL_STATIC_MEM_START
    shr edi, 20
    add edi, esi
    mov cr3, esi
    .gen_pde:
    mov [esi], edx
    mov [edi], edx
    add esi, 0x00000004
    add edi, 0x00000004
    add edx, 0x00400000
    inc ecx
    cmp ecx, eax
    jb .gen_pde
    mov eax, cr4
    or eax, 0x00000010
    mov cr4, eax
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    lea eax, [.higherhalf]
    jmp eax
.higherhalf:
    mov esp, stack_top ; Stack grows in a backwards direction
    push ebx
    extern main
    call main
    add esp, 4
    cli ; If main returns, we will halt the computer
.halt:
    hlt
    jmp .halt
