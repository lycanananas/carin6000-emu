/* The Doom image: linked at 0 and relocated by the loader (tools/mkflat.py keeps the R_68K_32
   relocations). text, rodata and data are one block in the file; bss follows in memory. */
OUTPUT_FORMAT("elf32-m68k")
ENTRY(_start)
SECTIONS
{
	. = 0;
	.text : {
		*(.text.start)
		*(.text .text.*)
	}
	.rodata : { *(.rodata .rodata.*) }
	.data : {
		*(.data .data.*) *(.sdata .sdata.*)
		. = ALIGN(4);
		__init_array_start = .; *(.init_array*) __init_array_end = .;
		__fini_array_start = .; *(.fini_array*) __fini_array_end = .;
		__preinit_array_start = .; __preinit_array_end = .;
	}
	. = ALIGN(4);
	.bss : {
		__bss_start = .;
		*(.bss .bss.*) *(.sbss .sbss.*) *(COMMON)
		. = ALIGN(4);
		__bss_end = .;
	}
	_end = .;
	end = .;
	/DISCARD/ : { *(.comment) *(.note*) *(.eh_frame*) *(.init) *(.fini) }
}
