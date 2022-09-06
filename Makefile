all:
	@make -C Codigo all
	@cp -f Codigo/Nucleo/Temp/HUSIS ISO/HUSIS
	@mkdir -p ISO/boot/grub
	@cp -f grub.cfg ISO/boot/grub/grub.cfg

iso: all
	@grub-mkrescue -o husis.iso ISO > /dev/null

qemu: iso
	@qemu-system-i386 -m 256 -cdrom husis.iso

mac-requisitos:
	brew install nasm
	brew tap nativeos/i386-elf-toolchain 
	brew install i386-elf-binutils i386-elf-gcc
	brew install nativeos/i386-elf-toolchain/i386-elf-grub
	brew install xorriso