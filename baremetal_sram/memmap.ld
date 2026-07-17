ENTRY(Reset_Handler)

MEMORY
{
    FLASH(rx)  : ORIGIN = 0x10000000, LENGTH = 2048K
    RAM(rwx)   : ORIGIN = 0x20000000, LENGTH = 256K
}

SECTIONS
{
    .boot2 0x10000000 :
    {
        __boot2_start__ = .;
        KEEP(*(.boot2))
        __boot2_end__ = .;
    } > FLASH

    ASSERT(__boot2_end__ - __boot2_start__ == 256,
        "Boot2 must be exactly 256 bytes")

    .reset 0x10000100 :
    {
        KEEP(*(.vectors))
        KEEP(*(.reset))
    } > FLASH

    .text :
    AT(LOADADDR(.reset) + SIZEOF(.reset))
    {
        __text_start__ = .;
        *(.text*)
        *(.rodata*)
        __text_end__ = .;
    } > RAM

    __text_load__ = LOADADDR(.text);

    .data :
    {
        __data_start__ = .;
        *(.data*)
        __data_end__ = .;
    } > RAM

    __data_load__ = LOADADDR(.data);

    .bss (NOLOAD):
    {
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
    } > RAM

    _stack_top = ORIGIN(RAM) + LENGTH(RAM);
}
