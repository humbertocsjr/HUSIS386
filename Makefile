all:
	@make -C Codigo all
	@cp -f Codigo/Nucleo/Temp/HUSIS ISO/HUSIS
	@mkdir -p ISO/boot/grub
	@cp -f grub.cfg ISO/boot/grub/grub.cfg

iso: all
	@echo " -= Gerando imagem ISO =-"
	@grub-mkrescue -o husis.iso ISO >/dev/null 2>/dev/null

disquete: all
	@echo " -= Gerando imagem de Disquete =-"
	@mdel -i husis.img ::/boot/menu.cfg
	@mcopy -i husis.img grub.lst ::/boot/menu.cfg
	@mdel -i husis.img ::/husis
	@mcopy -i husis.img ISO/HUSIS ::/husis
	

qemu: iso disquete
	@echo " -= Executando Emulador =-"
	@qemu-system-i386 -boot a -m 16 -cdrom husis.iso -drive format=raw,file=husis.img,if=floppy

mac-requisitos:
	brew install nasm mtool
	brew tap nativeos/i386-elf-toolchain 
	brew install i386-elf-binutils i386-elf-gcc
	brew install nativeos/i386-elf-toolchain/i386-elf-grub
	brew install xorriso