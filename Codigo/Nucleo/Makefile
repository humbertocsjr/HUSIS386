ASM=nasm -f elf -o
LD=i386-elf-ld -T Ligacao.ld -o
CC=i386-elf-gcc -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -IIncluir -I../Incluir -c -o
all:
	@echo " -= Compilando Nucleo =-"
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
	@$(CC) Temp/Processo.o Processo.c
	@$(CC) Temp/Multitarefa.o Multitarefa.c
	@$(CC) Temp/ATA.o ATA.c
	@$(CC) Temp/Unidade.o Unidade.c
	@$(CC) Temp/Dispositivo.o Dispositivo.c
	@$(CC) Temp/SisArq.o SisArq.c
	@cp Ligacao.ld Temp/Ligacao.ld
	@cd Temp && $(LD) HUSIS Inicial.o Principal.o ES.o Mem.o Term.o Caract.o Const.o GDT.o IDT.o ISR.o IRQ.o Cronometro.o Teclado.o TecladoEnUsIntl.o Comandos.o Multiboot.o Processo.o Multitarefa.o ATA.o Unidade.o Dispositivo.o SisArq.o