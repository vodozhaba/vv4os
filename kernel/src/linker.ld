ENTRY(_start)
SECTIONS
{
    _KERNEL_STATIC_MEM_START = 0xC0000000;
    . = _KERNEL_STATIC_MEM_START + 0x100000;
    .text BLOCK(4K) : AT(ADDR(.text) - _KERNEL_STATIC_MEM_START) ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }
    .rdata BLOCK(4K) : AT(ADDR(.rdata) - _KERNEL_STATIC_MEM_START) ALIGN(4K)
    {
        *(.rdata)
    }
    .data BLOCK(4K) : AT(ADDR(.data) - _KERNEL_STATIC_MEM_START) ALIGN(4K)
    {
        *(.data)
    }
    .bss BLOCK(16K) : AT(ADDR(.bss) - _KERNEL_STATIC_MEM_START) ALIGN(16K)
    {
        *(COMMON)
        *(.bss)
    }
    _KERNEL_STATIC_MEM_END = .;
}
