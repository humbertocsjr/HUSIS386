#include <HUSIS.h>

void * _IRQ_Rotinas[16] = 
{
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0
};

void IRQ_Processa(Regs_t * regs)
{
    void (*processador)(Regs_t *regs) = _IRQ_Rotinas[regs->Interrupcao-32];

    if(MSG_EXIBE_CHAMADA_IRQ & (regs->Interrupcao > 32))Mensagem("IRQ", "Processada IRQ{0:u}", regs->Interrupcao-32);
    if(processador != 0) processador(regs);

    if(regs->Interrupcao >= 40) ES_EscrevaByte(0xa0, 0x20);
    ES_EscrevaByte(0x20, 0x20);
}

Status_t IRQ_Registra(Pos_t irq, void (*processador)(Regs_t *regs))
{
    if(irq > 15) return STATUS_POSICAO_INVALIDA;
    if(MSG_EXIBE_REGISTRO_IRQ) Mensagem("IRQ", "Registrando IRQ{0:u}", irq);
    _IRQ_Rotinas[irq] = (void *)processador;
    return STATUS_OK;
}

Status_t IRQ_Desregistra(Pos_t irq)
{
    if(irq > 15) return STATUS_POSICAO_INVALIDA;
    if(MSG_EXIBE_REGISTRO_IRQ) Mensagem("IRQ", "Desregistrando IRQ{0:u}", irq);
    _IRQ_Rotinas[irq] = 0;
    return STATUS_OK;
}

void IRQ()
{
    extern void IRQ00(); IDT_Config(32, (UInt_t)&IRQ00, 0x8, 0x8e);
    extern void IRQ01(); IDT_Config(33, (UInt_t)&IRQ01, 0x8, 0x8e);
    extern void IRQ02(); IDT_Config(34, (UInt_t)&IRQ02, 0x8, 0x8e);
    extern void IRQ03(); IDT_Config(35, (UInt_t)&IRQ03, 0x8, 0x8e);
    extern void IRQ04(); IDT_Config(36, (UInt_t)&IRQ04, 0x8, 0x8e);
    extern void IRQ05(); IDT_Config(37, (UInt_t)&IRQ05, 0x8, 0x8e);
    extern void IRQ06(); IDT_Config(38, (UInt_t)&IRQ06, 0x8, 0x8e);
    extern void IRQ07(); IDT_Config(39, (UInt_t)&IRQ07, 0x8, 0x8e);
    extern void IRQ08(); IDT_Config(40, (UInt_t)&IRQ08, 0x8, 0x8e);
    extern void IRQ09(); IDT_Config(41, (UInt_t)&IRQ09, 0x8, 0x8e);
    extern void IRQ10(); IDT_Config(42, (UInt_t)&IRQ10, 0x8, 0x8e);
    extern void IRQ11(); IDT_Config(43, (UInt_t)&IRQ11, 0x8, 0x8e);
    extern void IRQ12(); IDT_Config(44, (UInt_t)&IRQ12, 0x8, 0x8e);
    extern void IRQ13(); IDT_Config(45, (UInt_t)&IRQ13, 0x8, 0x8e);
    extern void IRQ14(); IDT_Config(46, (UInt_t)&IRQ14, 0x8, 0x8e);
    extern void IRQ15(); IDT_Config(47, (UInt_t)&IRQ15, 0x8, 0x8e);

    ES_EscrevaByte(0x20, 0x11);
    ES_EscrevaByte(0xa0, 0x11);
    ES_EscrevaByte(0x21, 0x20);
    ES_EscrevaByte(0xa1, 0x28);
    ES_EscrevaByte(0x21, 0x04);
    ES_EscrevaByte(0xa1, 0x02);
    ES_EscrevaByte(0x21, 0x01);
    ES_EscrevaByte(0xa1, 0x01);
    ES_EscrevaByte(0x21, 0x0);
    ES_EscrevaByte(0xa1, 0x0);
    ES_EscrevaByte(0xa0, 0x20);
    ES_EscrevaByte(0x20, 0x20);
}