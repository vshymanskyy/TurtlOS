#---------------------------------------------------------
# CONFIG
#---------------------------------------------------------
Arch		?= x86
Build		?= Debug

#GenMap		?= true
#CppGenAsm	?= true
#Strip		?= true
#BinOutput	?= true
#ObjDump	?= true

#---------------------------------------------------------
# BINARIES
#---------------------------------------------------------
ECHO		 = @echo
OBJDUMP		 = objdump
SLEEP		 = sleep
CP			 = cp
RM			 = rm -f
RMDIR		 = rm -rf
MKDIR		 = mkdir -p

DEVENV		 = build-essential \
			gcc-arm-linux-gnueabi \
			g++-arm-linux-gnueabi \
			genisoimage \
			uboot-mkimage \
			g++ \
			nasm \
			qemu \
			qemu-kvm-extras \
			vim-gtk \
			cscope \
			exuberant-ctags \
			meld \
			git

ifeq ($(Arch),x86)
	AS			 = nasm -f elf32
	CC			 = gcc -m32
	LD			 = ld -m elf_i386 -L/usr/lib/gcc/x86_64-linux-gnu/4.6/32/
	EM			 = qemu-system-i386 -sdl -debugcon stdio -m 64 -smp 4 -boot d
	GDB			 = gdb
	DEFINES		+= -D __x86
	
	BUILDPATHS	 = lib/hal/common   \
			   lib/hal/common-x86   \
			   lib/hal/$(Arch)  \
			   lib/crt/gcc-common  \
			   lib/crt/gcc-$(Arch)  \
			   lib/std          \
			   lib/libc         \
			   kernel
			   
	CCFLAGS		 += -I include \
				-I lib/hal/common-x86  \
				-I lib/hal/$(Arch)
	
else ifeq ($(Arch),x86-64)
	AS			 = nasm -f elf64
	CC			 = gcc -m64
	LD			 = ld -m elf_x86_64 -L/usr/lib/gcc/x86_64-linux-gnu/4.6/
	EM			 = qemu-system-x86_64 -sdl -debugcon stdio -m 64 -smp 4 -boot d
	GDB			 = gdb
	DEFINES		+= -D __x86_64
	
	BUILDPATHS	 = lib/hal/common   \
			   lib/hal/common-x86   \
			   lib/hal/$(Arch)  \
			   lib/crt/gcc-common  \
			   lib/std          \
			   lib/libc         \
			   kernel
			   
	CCFLAGS		 += -I include \
				-I lib/hal/common-x86  \
				-I lib/hal/$(Arch)

else ifeq ($(Arch),arm)
	TCHAIN		 = arm-linux-gnueabihf-
	AS			 = $(TCHAIN)as -mcpu=cortex-a9
	CC			 = $(TCHAIN)gcc -mcpu=cortex-a9
	LD			 = $(TCHAIN)ld -L/usr/lib/gcc/arm-linux-gnueabihf/4.8/
	OBJCOPY		 = $(TCHAIN)objcopy
	EM			 = qemu-system-arm -cpu arm1176 -m 256 -M raspi -no-reboot -serial stdio -show-cursor
	GDB			 = gdb
	DEFINES		+= -D __arm
	
	BUILDPATHS	 = lib/hal/common  \
			   lib/hal/$(Arch)  \
			   lib/crt/gcc-common  \
			   lib/crt/gcc-$(Arch)  \
			   lib/libc         \
			   lib/std
			   
	CCFLAGS		 += -I include \
			-I lib/hal/$(Arch) \
			-I lib/hal/mach-versatilepb

else
	$(error Target platform '$(Arch)' not supported)
endif

#---------------------------------------------------------
# PATHS
#---------------------------------------------------------
INCLUDE		 = include
BUILD		 = build
DOCS		 = $(BUILD)/docs
DEST		 = $(BUILD)/$(Build)/$(Arch)
TEMP		 = $(BUILD)/temp/$(Build)/$(Arch)
TEMP_ISO	 = $(TEMP).iso

#---------------------------------------------------------
# FILES
#---------------------------------------------------------
GLOBALINC	 = global.h
LINKSCRIPT	 = lib/hal/$(Arch)/linker.ld
TARBALL		 = TurtlOS.tar.bz2

AUXFILES	 = $(LINKSCRIPT) \
			   Makefile      \
			   Readme.txt    \
			   doxyfile      \
			   grub.cfg		 \
			   .project		 \
			   .cproject

GENFILES	 = $(TARBALL) \
			   cscope.*   \
			   tags

TARPATHS	 = config      \
			   include     \
			   kernel      \
			   lib         \
			   tools       \
			   .settings

MKINITRD	 = $(BUILD)/mkinitrd
BIN2INL		 = $(BUILD)/bin2inl
KERNEL		 = $(DEST)/kernel-elf
INITRD		 = $(DEST)/initrd.img
WAKER		 = $(TEMP)/waker.out
GDBCONF		 = $(DEST)/gdb.cfg
DDDCONF		 = $(DEST)/ddd.cfg
GRUBCONF	 = $(DEST)/grub.cfg
IMAGE		 = $(DEST)/image.iso

ASMFILES	 = $(shell find $(BUILDPATHS) ! -path "*.svn*" -a -name "*.s")
CFILES	     = $(shell find $(BUILDPATHS) ! -path "*.svn*" -a -name "*.c")
CPPFILES	 = $(shell find $(BUILDPATHS) ! -path "*.svn*" -a -name "*.cpp")
HDRFILES	 = $(shell find $(BUILDPATHS) ! -path "*.svn*" -a -name "*.h")
INLFILES	 = $(shell find $(BUILDPATHS) ! -path "*.svn*" -a -name "*.inl")

OUTFILES	 = $(patsubst %.s, $(TEMP)/%.b, $(ASMFILES))
OBJFILES	 = $(patsubst %.cpp, $(TEMP)/%.obj, $(CPPFILES)) $(patsubst %.c, $(TEMP)/%.o, $(CFILES))
DEPFILES	 = $(patsubst %.cpp, $(TEMP)/%.dep, $(CPPFILES)) $(patsubst %.c, $(TEMP)/%.d, $(CFILES))

LINKFILES	 = $(OUTFILES) $(OBJFILES)
#-lsupc++ -lgcc

TARFILES	 = $(shell find $(TARPATHS) ! -path "*.svn*" -a -name "*.*") $(AUXFILES)

#---------------------------------------------------------
# General FLAGS
#---------------------------------------------------------
CCFLAGS		 += -std=c99 \
			   -nostartfiles   \
			   -nodefaultlibs  \
			   -ffreestanding  \
			   -fno-keep-inline-functions \
			   -fno-builtin    \
			   -MMD -MP -MF $(@:.obj=.dep)
			   
CPPFLAGS	 += -std=c++11 \
			   -fno-exceptions \
			   -fno-rtti

LDFLAGS		 += -nodefaultlibs
#ASFLAGS		 += -w+gnu-elf-extensions

#----- Build type ----------------------------------------
ifeq ($(Build),Debug)
	CCFLAGS		+= -pedantic \
				   -W \
				   -Wall \
				   -Wvla \
				   -Wextra \
				   -Wshadow \
				   -Winline \
				   -Wconversion \
				   -Wcast-align \
				   -Wno-long-long \
				   -Wwrite-strings \
				   -Wpointer-arith \
				   -Wredundant-decls \
				   -Wmissing-declarations \
				   -Wunsafe-loop-optimizations \
				   -Wno-variadic-macros \
				   -g3 -fstack-protector-all

	CPPFLAGS	+= -Woverloaded-virtual
	DEFINES		+= -D DEBUG
else ifeq ($(Build),Release)
	CCFLAGS		+= -O4 -fno-stack-protector
	LDFLAGS		+= -O4 -x -X
	DEFINES		+= -D RELEASE
ifeq ($(Arch),x86)
	CCFLAGS		+= -mregparm=3
endif

else
$(error Build type '$(Build)' not supported)
endif

#ifdef CppGenAsm
#CCFLAGS		+= -Wa,-a,-ad,-aln=$(@:.obj=.lst) -masm=intel
#endif

ifdef Strip
LDFLAGS		+= -s
endif

ifdef BinOutput
LDFLAGS		+= --oformat binary
endif

ifdef GenMap
LDFLAGS		+= -M -Map $(@:.bin=.map)
endif

#---------------------------------------------------------
# RULES
#---------------------------------------------------------
.PHONY: all clean tar todo kernel image run run_debug run_ddd cscope ctags docs devenv

image: $(IMAGE)

all:
	$(MAKE) image Arch=x86 Build=Debug
	$(MAKE) image Arch=x86 Build=Release
	$(MAKE) image Arch=x86-64 Build=Debug
	$(MAKE) image Arch=x86-64 Build=Release
	$(MAKE) kernel Arch=arm Build=Debug
	$(MAKE) kernel Arch=arm Build=Release
	
tools: $(BIN2INL) $(MKINITRD)

clean:
	$(ECHO) Cleaning
	$(RMDIR) $(BUILD)
	$(RM) $(GENFILES)

#----- Linking -------------------------------------------
$(KERNEL): $(LINKFILES) $(LINKSCRIPT)
	$(ECHO) Linking to $@
	$(MKDIR) $(shell dirname $@)
	$(LD) $(LDFLAGS) -T $(LINKSCRIPT) -o $@ $(LINKFILES)
	-chmod a+x $@
ifdef ObjDump
	$(OBJDUMP) -dlxw -M intel $@ > $@.dump
endif

kernel: $(KERNEL)

#----- Compiling -----------------------------------------
$(TEMP)/%.obj: %.cpp $(INCLUDE)/$(GLOBALINC)
	$(ECHO) Compiling $<
	$(MKDIR) $(shell dirname $@)
	$(CC) $(CCFLAGS) $(CPPFLAGS) $(DEFINES) -I $(INCLUDE) -include $(GLOBALINC) -o $@ -c $<
ifdef CppGenAsm
	$(CC) $(CCFLAGS) $(CPPFLAGS) $(DEFINES) -I $(INCLUDE) -include $(GLOBALINC) -masm=intel -Wa,-adhl=$(@:.obj=.lst) -o $(@:.obj=.lst2) -S $<
endif

$(TEMP)/%.o: %.c $(INCLUDE)/$(GLOBALINC)
	$(ECHO) Compiling $<
	$(MKDIR) $(shell dirname $@)
	$(CC) $(CCFLAGS) $(CFLAGS) $(DEFINES) -I $(INCLUDE) -include $(GLOBALINC) -o $@ -c $<
ifdef CppGenAsm
	$(CC) $(CCFLAGS) $(CFLAGS) $(DEFINES) -I $(INCLUDE) -include $(GLOBALINC) -masm=intel -Wa,-adhl=$(@:.o=.l) -o $(@:.o=.l2) -S $<
endif

$(TEMP)/%.b: %.s
	$(ECHO) Assembling $<
	$(MKDIR) $(shell dirname $@)
ifeq ($(Arch),x86)
	$(AS) $(ASFLAGS) -i $(shell dirname $<)/ -o $@ $<
else ifeq ($(Arch),x86-64)
	$(AS) $(ASFLAGS) -i $(shell dirname $<)/ -o $@ $<
else ifeq ($(Arch),arm)
	$(AS) $(ASFLAGS) -g $< -o $@
endif

$(WAKER): lib/hal/$(Arch)/boot/waker.asm
	$(ECHO) Assembling $<
	nasm -i $(shell dirname $<)/ -o $@ $<

#----- Additional dependencies ---------------------------
$(LINKFILES): Makefile
-include $(DEPFILES)

#----- Tools ---------------------------------------------
$(BIN2INL): tools/bin2inl.cpp
	$(MKDIR) $(shell dirname $@)
	$(ECHO) Building bin2inl
	@g++ $< -O4 -s -o $@

$(MKINITRD): tools/mkinitrd.cpp
	$(MKDIR) $(shell dirname $@)
	$(ECHO) Building mkinitrd
	@g++ $< -O4 -s -o $@

#----- Configuration files -------------------------------
$(GDBCONF):
	$(MKDIR) $(shell dirname $@)
	$(ECHO) Preparing $@
	$(ECHO) "set disassembly-flavor intel"    > $@
	$(ECHO) "set remotetimeout 5"            >> $@
	$(ECHO) "file $(KERNEL)"                 >> $@
ifeq ($(Arch),x86)
	$(ECHO) "set archi i386:intel"           >> $@
else ifeq ($(Arch),x86-64)
	$(ECHO) "set archi i386:x86-64:intel"    >> $@
endif
	$(ECHO) "b assert_failed"                >> $@
	$(ECHO) "b entry"                        >> $@

$(DDDCONF):
	$(MKDIR) $(shell dirname $@)
	$(ECHO) Preparing $@
	$(ECHO) "set disassembly-flavor intel"    > $@
	$(ECHO) "set remotetimeout 5"            >> $@
ifeq ($(Arch),x86)
	$(ECHO) "set archi i386:intel"           >> $@
else ifeq ($(Arch),x86-64)
	$(ECHO) "set archi i386:x86-64:intel"    >> $@
endif
	$(ECHO) "symbol-file $(KERNEL)"          >> $@
	$(ECHO) "target remote :1234"            >> $@
	$(ECHO) "b assert_failed"                >> $@
	$(ECHO) "b entry"                        >> $@
	$(ECHO) "c"                              >> $@


#----- Initrd creation -----------------------------------
$(INITRD): $(MKINITRD) $(WAKER) Readme.txt
	$(MKDIR) $(shell dirname $@)
	$(ECHO) Making $@
	./$(MKINITRD) Readme.txt Readme.txt $(WAKER) waker.bin > $@

#----- Floppy creation -----------------------------------
$(IMAGE): $(KERNEL) $(INITRD) $(GDBCONF) grub.cfg
	$(MKDIR) $(shell dirname $@)
	$(ECHO) Creating $@
	$(MKDIR) $(TEMP_ISO)/boot/grub
	$(CP) grub.cfg $(TEMP_ISO)/boot/grub
	$(CP) /boot/grub/unicode.pf2 $(TEMP_ISO)/boot/grub
	$(CP) $(KERNEL) $(TEMP_ISO)/kernel.bin
	$(CP) $(INITRD) $(TEMP_ISO)/initrd.img
	grub-mkimage -p /boot/grub -O i386-pc -o $(TEMP_ISO)/core.img biosdisk normal iso9660 multiboot vbe video_bochs video_cirrus videoinfo gfxterm cpuid gfxmenu halt hexdump lspci lsacpi lsapm pci png jpeg reboot serial terminfo usb_keyboard usb usbserial_common usbserial_ftdi usbtest help
	cat /usr/lib/grub/i386-pc/cdboot.img $(TEMP_ISO)/core.img > $(TEMP_ISO)/boot.img
	genisoimage -quiet -pad -input-charset ascii -R -A "TurtlOS" -no-emul-boot -boot-load-size 4 -boot-info-table -b boot.img -c boot.catalog -hide boot.img -hide boot.catalog -o $(IMAGE) $(TEMP_ISO)

#----- Run -----------------------------------------------
ifeq ($(Arch),x86)
run: $(IMAGE)
	$(EM) $(EMFLAGS) -cdrom $(IMAGE)
else ifeq ($(Arch),x86-64)
run: $(IMAGE)
	$(EM) $(EMFLAGS) -cdrom $(IMAGE)
else ifeq ($(Arch),arm)
run: $(KERNEL)
	$(OBJCOPY) -O binary $(KERNEL) $(DEST)/kernel-bin
	$(EM) $(EMFLAGS) -kernel $(DEST)/kernel-bin
endif


run_debug: $(IMAGE)
	$(EM) $(EMFLAGS) -s -S -cdrom $(IMAGE)

run_ddd: $(IMAGE) $(DDDCONF)
	$(EM) $(EMFLAGS) -s -S -cdrom $(IMAGE) &
	$(SLEEP) 1
	ddd -x $(DDDCONF) --debugger $(GDB)


#---------------------------------------------------------
# Utilities
#---------------------------------------------------------
tar $(TARBALL):
	$(ECHO) Making tarball
	tar -cjf TurtlOS.tar.bz2 $(TARFILES)

ctags tags:
	$(ECHO) Building ctags database
	ctags -R -I --c++-kinds=+p --fields=+iaS --extra=+q .

cscope cscope.out:
	$(ECHO) Building cscope database
	find . -name '*.[acihlyACHILY]' -o -name '*.asm' -o -name '*.inc' -o -name '*.java' -o -name '*.cc' -o -name '*.[ch]pp' -o -name '*.[CH]PP' -o -name '*.py' -o -name '*.pl' -o -name '*.sh' > cscope.files
	cscope -bk

docs:
	$(ECHO) Building documentation
	$(RMDIR) $(DOCS)
	$(MKDIR) $(DOCS)
	doxygen

todo:
	-for file in $(ALLFILES); do fgrep --exclude Makefile --color=auto -H -n -e TODO -e FIXME $$file; done; true

devenv:
	sudo apt-get install $(DEVENV)
