all:
	@make -C Codigo all
	@cp -f Codigo/Nucleo/Temp/HUSIS ISO/HUSIS
	@mkdir -p ISO/boot/grub
	@cp -f grub.cfg ISO/boot/grub/grub.cfg

iso: all
	@grub-mkrescue -o husis.iso ISO >/dev/null 2>/dev/null

disquete: all
	@mdel -i husis.img ::/boot/menu.cfg
	@mcopy -i husis.img grub.lst ::/boot/menu.cfg
	@mdel -i husis.img ::/husis
	@mcopy -i husis.img ISO/HUSIS ::/husis
	

qemu: iso disquete
	@qemu-system-i386 -m 256 -cdrom husis.iso

mac-requisitos:
	brew install nasm mtool
	brew tap nativeos/i386-elf-toolchain 
	brew install i386-elf-binutils i386-elf-gcc
	brew install nativeos/i386-elf-toolchain/i386-elf-grub
	brew install xorriso