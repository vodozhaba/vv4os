section .bss
    align 0x1000
    global _kernel_page_directory
    _kernel_page_directory: resd 1024
    global _kernel_page_table
    _kernel_page_table: times 1024 resd 1024