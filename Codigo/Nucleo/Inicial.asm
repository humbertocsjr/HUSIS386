[bits 32]
cpu 386
global start

start:
    mov esp, HusisPilha
    jmp PreInicial

align 4
MultibootCabecalho:
    ; Multiboot macros to make a few lines later more readable
    MULTIBOOT_PAGE_ALIGN	equ 1<<0
    MULTIBOOT_MEMORY_INFO	equ 1<<1
    MULTIBOOT_AOUT_KLUDGE	equ 1<<16
    MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
    MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
    MULTIBOOT_CHECKSUM	equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
    EXTERN code, bss, end

    ; This is the GRUB Multiboot header. A boot signature
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM

    ; AOUT kludge - must be physical addresses. Make a note of these:
    ; The linker script fills in the data for these ones!
    dd MultibootCabecalho
    dd code
    dd bss
    dd end
    dd start

PtrMultiboot: dd 0

PreInicial:
    mov [PtrMultiboot], ebx
    extern Inicio
    call Inicio
    .loop:
        hlt
        jmp .loop

global _Multiboot_Leia
_Multiboot_Leia:
    mov eax, [PtrMultiboot]
    ret

global _Mem_LeiaNucleoCodigo
_Mem_LeiaNucleoCodigo:
    mov eax, code
    ret

global _Mem_LeiaNucleoBSS
_Mem_LeiaNucleoBSS:
    mov eax, bss
    ret

global _Mem_LeiaNucleoFim
_Mem_LeiaNucleoFim:
    mov eax, end
    ret


global _Mem_LeiaCR0
_Mem_LeiaCR0:
    mov eax, cr0
    ret

global _Mem_EscrevaCR0:
_Mem_EscrevaCR0:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr0,  eax
    pop ebp
    ret

global _Mem_LeiaCR3
_Mem_LeiaCR3:
    mov eax, cr3
    ret

global _Mem_EscrevaCR3:
_Mem_EscrevaCR3:
    push ebp
    mov ebp, esp
    mov eax, [ebp+8]
    mov cr3,  eax
    pop ebp
    ret

global GDT_Atualiza
GDT_Atualiza:
    extern _GDT_Ponteiro
    lgdt [_GDT_Ponteiro]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.continua
    .continua:
    ret

global IDT_Atualiza
IDT_Atualiza:
    extern _IDT_Ponteiro
    lidt [_IDT_Ponteiro]
    ret

extern ISR_ProcessaErro

global ISR00
ISR00:
    cli
    push byte 0
    push byte 00
    jmp _isr

global ISR01
ISR01:
    cli
    push byte 0
    push byte 01
    jmp _isr

global ISR02
ISR02:
    cli
    push byte 0
    push byte 02
    jmp _isr

global ISR03
ISR03:
    cli
    push byte 0
    push byte 03
    jmp _isr

global ISR04
ISR04:
    cli
    push byte 0
    push byte 04
    jmp _isr

global ISR05
ISR05:
    cli
    push byte 0
    push byte 05
    jmp _isr

global ISR06
ISR06:
    cli
    push byte 0
    push byte 06
    jmp _isr

global ISR07
ISR07:
    cli
    push byte 0
    push byte 07
    jmp _isr

global ISR08
ISR08:
    cli
    push byte 08
    jmp _isr

global ISR09
ISR09:
    cli
    push byte 0
    push byte 09
    jmp _isr

global ISR10
ISR10:
    cli
    push byte 10
    jmp _isr

global ISR11
ISR11:
    cli
    push byte 11
    jmp _isr

global ISR12
ISR12:
    cli
    push byte 12
    jmp _isr

global ISR13
ISR13:
    cli
    push byte 13
    jmp _isr

global ISR14
ISR14:
    cli
    push byte 14
    jmp _isr

global ISR15
ISR15:
    cli
    push byte 0
    push byte 15
    jmp _isr

global ISR16
ISR16:
    cli
    push byte 0
    push byte 16
    jmp _isr

global ISR17
ISR17:
    cli
    push byte 17
    jmp _isr

global ISR18
ISR18:
    cli
    push byte 0
    push byte 18
    jmp _isr

global ISR19
ISR19:
    cli
    push byte 0
    push byte 19
    jmp _isr

global ISR20
ISR20:
    cli
    push byte 0
    push byte 20
    jmp _isr

global ISR21
ISR21:
    cli
    push byte 21
    jmp _isr

global ISR22
ISR22:
    cli
    push byte 0
    push byte 22
    jmp _isr

global ISR23
ISR23:
    cli
    push byte 0
    push byte 23
    jmp _isr

global ISR24
ISR24:
    cli
    push byte 0
    push byte 24
    jmp _isr

global ISR25
ISR25:
    cli
    push byte 0
    push byte 25
    jmp _isr

global ISR26
ISR26:
    cli
    push byte 0
    push byte 26
    jmp _isr

global ISR27
ISR27:
    cli
    push byte 0
    push byte 27
    jmp _isr

global ISR28
ISR28:
    cli
    push byte 0
    push byte 28
    jmp _isr

global ISR29
ISR29:
    cli
    push byte 0
    push byte 29
    jmp _isr

global ISR30
ISR30:
    cli
    push byte 0
    push byte 30
    jmp _isr

global ISR31
ISR31:
    cli
    push byte 0
    push byte 31
    jmp _isr


_isr:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, ISR_ProcessaErro
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret           


extern IRQ_Processa

global IRQ00
IRQ00:
    cli
    push byte 0
    push byte 32
    jmp _irq

global IRQ01
IRQ01:
    cli
    push byte 0
    push byte 33
    jmp _irq

global IRQ02
IRQ02:
    cli
    push byte 0
    push byte 34
    jmp _irq

global IRQ03
IRQ03:
    cli
    push byte 0
    push byte 35
    jmp _irq

global IRQ04
IRQ04:
    cli
    push byte 0
    push byte 36
    jmp _irq

global IRQ05
IRQ05:
    cli
    push byte 0
    push byte 37
    jmp _irq

global IRQ06
IRQ06:
    cli
    push byte 0
    push byte 38
    jmp _irq

global IRQ07
IRQ07:
    cli
    push byte 0
    push byte 39
    jmp _irq

global IRQ08
IRQ08:
    cli
    push byte 0
    push byte 40
    jmp _irq

global IRQ09
IRQ09:
    cli
    push byte 0
    push byte 41
    jmp _irq

global IRQ10
IRQ10:
    cli
    push byte 0
    push byte 42
    jmp _irq

global IRQ11
IRQ11:
    cli
    push byte 0
    push byte 43
    jmp _irq

global IRQ12
IRQ12:
    cli
    push byte 0
    push byte 44
    jmp _irq

global IRQ13
IRQ13:
    cli
    push byte 0
    push byte 45
    jmp _irq

global IRQ14
IRQ14:
    cli
    push byte 0
    push byte 46
    jmp _irq

global IRQ15
IRQ15:
    cli
    push byte 0
    push byte 47
    jmp _irq

_irq:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, IRQ_Processa
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret      

section .bss
    resb 8192
HusisPilha:
