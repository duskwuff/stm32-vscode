/* Special symbols and aliases */
_sidata = LOADADDR(.data);  /* used by startup */
__bss_start__ = _sbss;      /* required by crt0 */
__bss_end__ = _ebss;        /* required by crt0 */
end = _end;                 /* required by rdimon */
__end__ = _end;             /* ditto */
__stack = ORIGIN(STACK_SPACE) + LENGTH(STACK_SPACE);

/* Define output sections */
SECTIONS
{
    /* Vector table -- must be at the top of memory */
    .isr_vector : {
        . = ALIGN(8);
        KEEP(*(.isr_vector))
        . = ALIGN(8);
    } >VECTOR_SPACE

    .text : {
        . = ALIGN(8);
        *(.text)
        *(.text*)
        *(.glue_7)
        *(.glue_7t)
        *(.eh_frame)
        KEEP (*(.init))
        KEEP (*(.fini))
        _etext = .;
        . = ALIGN(8);
    } >CODE_SPACE

    .rodata : {
        . = ALIGN(8);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(8);
    } >RODATA_SPACE

    .ARM.extab : {
        . = ALIGN(8);
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(8);
    } >CODE_SPACE

    .ARM : {
        . = ALIGN(8);
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
        . = ALIGN(8);
    } >CODE_SPACE

    .preinit_array : {
        . = ALIGN(8);
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP (*(.preinit_array*))
        PROVIDE_HIDDEN (__preinit_array_end = .);
        . = ALIGN(8);
    } >CODE_SPACE

    .init_array : {
        . = ALIGN(8);
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array*))
        PROVIDE_HIDDEN (__init_array_end = .);
        . = ALIGN(8);
    } >CODE_SPACE

    .fini_array : {
        . = ALIGN(8);
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT(.fini_array.*)))
        KEEP (*(.fini_array*))
        PROVIDE_HIDDEN (__fini_array_end = .);
        . = ALIGN(8);
    } >CODE_SPACE

    .data : {
        . = ALIGN(8);
        _sdata = .;
        *(.data)
        *(.data*)
        _edata = .;
        . = ALIGN(8);
    } >DATA_SPACE AT>RODATA_SPACE

    .bss : {
        . = ALIGN(8);
        _sbss = .;
        *(.bss)
        *(.bss*)
        *(COMMON)
        _ebss = .;
        _end = .;
        . = ALIGN(8);
    } >DATA_SPACE

    /* Remove information from the standard libraries */
    /DISCARD/ : {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }

    .ARM.attributes 0 : {
        *(.ARM.attributes)
    }
}
