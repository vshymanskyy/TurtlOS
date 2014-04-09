ENTRY (loader)
SECTIONS
{
	__kernelStart = 0x100000;
	.text __kernelStart : AT(__kernelStart)
	{
		__textSectStart = .;
		__loaderStart = .;
		*(.loader)
		__loaderEnd = .;
		__textStart = .;
		*(.text)
		__textEnd = .;
		__rodataStart = .;
		*(.rodata)
		__rodataEnd = .;
		__textSectEnd = .;
	}
	.data : AT(__kernelStart + (__dataSectStart - __textSectStart))
	{
		__dataSectStart = .;
		__ctorsStart = .; 
		*(.ctor*)
		__ctorsEnd = .;
		__dtorsStart = .;
		*(.dtor*)
		__dtorsEnd = .;
		
      __ctorsStart = .;
      LONG((__ctorsEnd - __ctorsStart) / 4 - 2)
      *(.ctors)
      LONG(0)
      __ctorsEnd = .;
      __dtorsStart = .;
      LONG((__dtorsEnd - __dtorsStart) / 4 - 2)
      *(.dtors)
      LONG(0)
      __dtorsEnd = .;
      
		__ehframeStart = .;
		*(.eh_frame)
		QUAD(0)
		__ehframeEnd = .;
		*(.data)
		__dataSectEnd = .;
	}
	.bss : AT(__kernelStart + (__bssStart - __textSectStart))
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
	__kernelEnd = .;
}
