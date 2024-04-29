/*******************************************************************************
 * @file       linker.ld
 * @license    This file is part of the ImagineOn Coldwave eOS software package
 *             licensed under the ImagineOn software-licensing terms available
 *             under https://www.imagineon.de/de/info/licensing-terms
 * @copyright  Copyright (c) 2023. ImagineOn GmbH. www.imagineon.de.
 ******************************************************************************/
/*------------------------------------------------------------------------------
 *      Linker script for Coldwave OS
 *-----------------------------------------------------------------------------*/

MEMORY
{
  flash (rx)				: ORIGIN = 0x8008000, LENGTH = 0xb8000
  ram   (rwx)				: ORIGIN = 0x20000000, LENGTH = 0x00040000
}

OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
SEARCH_DIR(.)

STACK_SIZE            = DEFINED(STACK_SIZE) ? STACK_SIZE : 0x400;
HEAP_SIZE             = __HeapEnd - __HeapBase;
SHARED_RAM_SIZE       = 0x1400;
REBOOT_SAFE_SIZE      = 0x100;
FS_START_ADDRESS      = 0x8178000;
END_OF_IMAGE          = cw_end_of_image;

SECTIONS
{

   /* Cortex-M requires 64bit Stack alignment, therefore here we subtract 8 from the top */
     __StackTop   = ORIGIN(ram) + LENGTH(ram) - 8 ;
     __StackLimit = __StackTop - STACK_SIZE;
     __RebootSafe  = ORIGIN(ram) + LENGTH(ram) - REBOOT_SAFE_SIZE -  SHARED_RAM_SIZE - STACK_SIZE - 8;
     __HeapEnd     = __RebootSafe ;

    .cw_boot_image_header : {  KEEP(*(.cw_boot_image_header .cw_boot_image_header.*)) } > flash

    .vectors :
    {
        . = ALIGN(0x200);
        _sfixed = .;
        KEEP(*(.vectors .vectors.*))

    }

    .layout :
    {
        _slayout = . ;
        LONG(ORIGIN(ram))
        LONG(ORIGIN(flash))
        LONG(ORIGIN(ram) + LENGTH(ram))
        LONG(ORIGIN(flash) + LENGTH(flash))
        LONG(END_OF_IMAGE)
        KEEP(*(.cw_init_fun .cw_init_fun.*))
        KEEP(*(.cw_version_info .cw_version_info.*))
        KEEP(*(.cw_init_stack_size .cw_init_stack_size.*))
    } > flash

    .text   :
    {
        . = ALIGN(8);

        *(.text .text.* .gnu.linkonce.t.*)
        *(.glue_7t) *(.glue_7)
        *(.rodata .rodata* .gnu.linkonce.r.*)
        *(.ARM.extab* .gnu.linkonce.armextab.*)

        /* Support C constructors, and C destructors in both user code
           and the C library. This also provides support for C++ code. */
        . = ALIGN(4);
        KEEP(*(.init))
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;

        . = ALIGN(4);
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;

        . = ALIGN(0x4);
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*crtend.o(.ctors))

        . = ALIGN(4);
        KEEP(*(.fini))

        . = ALIGN(4);
        __fini_array_start = .;
        KEEP (*(.fini_array))
        KEEP (*(SORT(.fini_array.*)))
        __fini_array_end = .;

        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*crtend.o(.dtors))

        . = ALIGN(4);
        _efixed = .;            /* End of text section */
    } > flash

    .cw_sysconfig :
    {
         . = ALIGN(4);
         cw_sysconfig_start = .;
         KEEP (*(.cw_bl_sysconfig))
         KEEP (*(.cw_sysconfig))
         cw_sysconfig_end = .;
         . = ALIGN(4);
         cw_platform_init_start = .;
         KEEP (*(.cw_platform_init .cw_platform_init.*))
         cw_platform_init_end = .;
         . = ALIGN(4);
         cw_driver_init_start = .;
         KEEP (*(.cw_bl_driver_init .cw_bl_driver_init.*))
         KEEP (*(.cw_driver_init .cw_driver_init.*))
         cw_driver_init_end = .;
         . = ALIGN(4);
         KEEP (*(.cw_sysconfig_params .cw_sysprops))
     } > flash

    /* .ARM.exidx is sorted, so has to go in its own output section.  */
    PROVIDE_HIDDEN (__exidx_start = .);
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    }  > flash
    PROVIDE_HIDDEN (__exidx_end = .);

    . = ALIGN(4);

    _etext = LOADADDR(.relocate);


    .end_of_image :
       {
            . = ALIGN(4);
            cw_end_of_image = .;
            LONG(cw_end_of_image - .)
       } > flash

    .cw_fs_start_address FS_START_ADDRESS :
    {
      cw_fs_start_address = .;
    } > flash

    .cw_shared_memory  (NOLOAD) :
    {
        . = ALIGN(4);
        cw_shared_memory_start = .;
        *(.cw_shared_memory)
        cw_shared_memory_end = .;
        . += SHARED_RAM_SIZE - (cw_shared_memory_end - cw_shared_memory_start);
    } > ram

    .relocate :
    {
        . = ALIGN(4);
        _srelocate = .;
        *(.ramfunc .ramfunc.*);
        *(.data .data.*);
        . = ALIGN(4);
        _erelocate = .;
    } > ram AT > flash


    .bss (NOLOAD) :
     {
        . = ALIGN(4);
        __bss_start__ = . ;
        _sbss = . ;
        _szero = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = . ;
        _ezero = .;
        __bss_end__ = . ;
     } > ram

     .isr_table :
     {
       . = ALIGN(4);
         cw_isr_table_start = . ;
         cw_isr_table = . ;
         . += (2 * SIZEOF(.vectors));
         cw_isr_table_end = . ;
     } > ram
     
    .heap (NOLOAD) :
    {
        . = ALIGN(8);
        __HeapBase = .;
        _end = . ;
        end = . ;
        __end__ = .;
        __HeapBaseCheck = .;
        . += HEAP_SIZE;
        __HeapLimit = .;
    } > ram
    
    .cw_reboot_safe __RebootSafe (NOLOAD) :
    {
        . = ALIGN(4);
         cw_reboot_safe_start = .;
        *(.cw_reboot_safe)
        cw_reboot_safe_end = .;
        . += REBOOT_SAFE_SIZE - (cw_reboot_safe_end  - cw_reboot_safe_start);
    } > ram

    .stack __StackLimit :
    {
       . = ALIGN(8);
       . += STACK_SIZE;
       __stack = .;
       _estack = .;
    } > ram
}


