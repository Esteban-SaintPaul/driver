ENTRY (start)

SECTIONS{
	. = 0x300000;
	.text :{
		*(.text)
		*(.text.*)
	}

	.data ALIGN (0x1000) : {
		*(.data)
		*(.data.*)
		*(.rodata)
		*(.rodata.*)
	}

	.bss : {
		*(.bss)
		*(.bss.*)
	}
}

