ASM=nasm -f elf -o
LD=i386-elf-ld -T Ligacao.ld -o
CC=i386-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -IIncluir -I../Incluir -c -o
all:
	@$(ASM) Temp/Inicial.o Inicial.asm
	@$(CC) Temp/Principal.o Principal.c
	@$(CC) Temp/ES.o ES.c
	@$(CC) Temp/Mem.o Mem.c
	@$(CC) Temp/Term.o Term.c
	@$(CC) Temp/Caract.o Caract.c
	@$(CC) Temp/Const.o Const.c
	@$(CC) Temp/GDT.o GDT.c
	@$(CC) Temp/IDT.o IDT.c
	@$(CC) Temp/ISR.o ISR.c
	@$(CC) Temp/IRQ.o IRQ.c
	@$(CC) Temp/Cronometro.o Cronometro.c
	@$(CC) Temp/Teclado.o Teclado.c
	@$(CC) Temp/TecladoEnUsIntl.o Teclados/EnUsIntl.c
	@$(CC) Temp/Comandos.o Comandos.c
	@$(CC) Temp/Multiboot.o Multiboot.c
	@$(LD) Temp/HUSIS Temp/Inicial.o Temp/Principal.o Temp/ES.o Temp/Mem.o Temp/Term.o Temp/Caract.o Temp/Const.o Temp/GDT.o Temp/IDT.o Temp/ISR.o Temp/IRQ.o Temp/Cronometro.o Temp/Teclado.o Temp/TecladoEnUsIntl.o Temp/Comandos.o Temp/Multiboot.o