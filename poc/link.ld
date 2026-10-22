/* Flat, position independent image: entry first, then code and constants. OS-9 modules are
   read-only, so writable sections must stay empty (checked by the Makefile). */
OUTPUT_FORMAT("elf32-m68k")
ENTRY(_start)
SECTIONS
{
	. = 0;
	.text : {
		*(.text.start)
		*(.text .text.*)
		*(.rodata .rodata.*)
	}
	.data : { *(.data .data.*) *(.sdata .sdata.*) }
	.bss : { *(.bss .bss.*) *(.sbss .sbss.*) *(COMMON) }
	/DISCARD/ : { *(.comment) *(.note*) *(.eh_frame*) }
}
