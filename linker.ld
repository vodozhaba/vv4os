ENTRY(_start)
SECTIONS
{
    . = 48M;
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }
    .rdata BLOCK(4K) : ALIGN(4K)
    {
        *(.rdata)
    }
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
        *(.bootstrap_stack)
    }
}
