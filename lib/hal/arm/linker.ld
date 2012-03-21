ENTRY(_Reset)
SECTIONS
{
	__kernelStart = 0x10000;
	. = 0x10000;
	.startup . :
	{
		build/temp/Debug/arm/lib/hal/arm/boot/loader.b(.text)
	}
	.text :
	{
		__textSectStart = .;
		;__loaderStart = .;
		;*(.loader)
		;__loaderEnd = .;
		__textStart = .;
		*(.text)
		__textEnd = .;
		__rodataStart = .;
		*(.rodata)
		__rodataEnd = .;
		__textSectEnd = .; 
	}
	.data :
	{
		__dataSectStart = .;
		__ctorsStart = .; 
		*(.ctor*)
		__ctorsEnd = .;
		__dtorsStart = .;
		*(.dtor*)
		__dtorsEnd = .;
		__ehframeStart = .;
		*(.eh_frame)
		QUAD(0)
		__ehframeEnd = .;
		*(.data)
		__dataSectEnd = .;
	}
	.bss :
	{
		__bssSectStart = .;
		__bssStart = .;
		*(.bss)
		__bssEnd = .;
		__commonStart = .;
		*(COMMON)
		__commonEnd = .;
		__bssSectEnd = .;
	}
	. = . + 0x1000; /* 4kB of stack memory */
	stack_top = .;
	__kernelEnd = .;
}
