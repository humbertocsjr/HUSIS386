#include <HUSIS.h>


SByte_t * ISR_Nome[] = 
{
    "Divisao por Zero",
    "Depuracao",
    "Interrupcao nao mascaravel",
    "Ponto de Parada da Depuracao",
    "Estouro de Capacidade",
    "Inacessivel",
    "Comando do Processador Invalido",
    "Sem Coprocessador",
    "Falha Dupla",
    "Coprocessador ultrapassou o segumento",
    "TSS Invalida",
    "Segmento nao existe",
    "Falha na Pilha",
    "Falha Geral de Protecao",
    "Falha na Paginacao",
    "Interrupcao desconhecida",
    "Falha do Coprocessador",
    "Verificacao de Alinhamento",
    "Verificacao de Maquina",
    "Reservado",
    "Reservado",
    "Reservado",
    "Reservado",
    "Reservado",
    "Reservado",
    "Reservado",
    "Reservado",
    "Reservado",
    "Reservado",
    "Reservado",
    "Reservado",
    "Reservado"
};

void ISR_ProcessaErro(Regs_t * regs)
{
    Mensagem3("ISR", "Processada ISR{0:u} [Erro Nro.: {1:u}]: {2:C}", regs->Interrupcao, regs->CodigoDeErro, (Tam_t)ISR_Nome[regs->Interrupcao]);
}


void ISR()
{
    extern void ISR00(); IDT_Config(0, (UInt_t)&ISR00, 0x8, 0x8e);
    extern void ISR01(); IDT_Config(1, (UInt_t)&ISR01, 0x8, 0x8e);
    extern void ISR02(); IDT_Config(2, (UInt_t)&ISR02, 0x8, 0x8e);
    extern void ISR03(); IDT_Config(3, (UInt_t)&ISR03, 0x8, 0x8e);
    extern void ISR04(); IDT_Config(4, (UInt_t)&ISR04, 0x8, 0x8e);
    extern void ISR05(); IDT_Config(5, (UInt_t)&ISR05, 0x8, 0x8e);
    extern void ISR06(); IDT_Config(6, (UInt_t)&ISR06, 0x8, 0x8e);
    extern void ISR07(); IDT_Config(7, (UInt_t)&ISR07, 0x8, 0x8e);
    extern void ISR08(); IDT_Config(8, (UInt_t)&ISR08, 0x8, 0x8e);
    extern void ISR09(); IDT_Config(9, (UInt_t)&ISR09, 0x8, 0x8e);
    extern void ISR10(); IDT_Config(10, (UInt_t)&ISR10, 0x8, 0x8e);
    extern void ISR11(); IDT_Config(11, (UInt_t)&ISR11, 0x8, 0x8e);
    extern void ISR12(); IDT_Config(12, (UInt_t)&ISR12, 0x8, 0x8e);
    extern void ISR13(); IDT_Config(13, (UInt_t)&ISR13, 0x8, 0x8e);
    extern void ISR14(); IDT_Config(14, (UInt_t)&ISR14, 0x8, 0x8e);
    extern void ISR15(); IDT_Config(15, (UInt_t)&ISR15, 0x8, 0x8e);
    extern void ISR16(); IDT_Config(16, (UInt_t)&ISR16, 0x8, 0x8e);
    extern void ISR17(); IDT_Config(17, (UInt_t)&ISR17, 0x8, 0x8e);
    extern void ISR18(); IDT_Config(18, (UInt_t)&ISR18, 0x8, 0x8e);
    extern void ISR19(); IDT_Config(19, (UInt_t)&ISR19, 0x8, 0x8e);
    extern void ISR20(); IDT_Config(20, (UInt_t)&ISR20, 0x8, 0x8e);
    extern void ISR21(); IDT_Config(21, (UInt_t)&ISR21, 0x8, 0x8e);
    extern void ISR22(); IDT_Config(22, (UInt_t)&ISR22, 0x8, 0x8e);
    extern void ISR23(); IDT_Config(23, (UInt_t)&ISR23, 0x8, 0x8e);
    extern void ISR24(); IDT_Config(24, (UInt_t)&ISR24, 0x8, 0x8e);
    extern void ISR25(); IDT_Config(25, (UInt_t)&ISR25, 0x8, 0x8e);
    extern void ISR26(); IDT_Config(26, (UInt_t)&ISR26, 0x8, 0x8e);
    extern void ISR27(); IDT_Config(27, (UInt_t)&ISR27, 0x8, 0x8e);
    extern void ISR28(); IDT_Config(28, (UInt_t)&ISR28, 0x8, 0x8e);
    extern void ISR29(); IDT_Config(29, (UInt_t)&ISR29, 0x8, 0x8e);
    extern void ISR30(); IDT_Config(30, (UInt_t)&ISR30, 0x8, 0x8e);
    extern void ISR31(); IDT_Config(31, (UInt_t)&ISR31, 0x8, 0x8e);
}