#include <HUSIS.h>


typedef struct __attribute__ ((__packed__))
{
    UShort_t BaseBaixa;
    UShort_t Seletor;
    Byte_t Zero;
    Byte_t Marcadores;
    UShort_t BaseAlta;
} IDT_Item_t;

typedef struct __attribute__ ((__packed__))
{
    UShort_t Limite;
    UInt_t Base;
} IDT_Ponteiro_t;

#define IDT_CAPACIDADE 256

IDT_Item_t _IDT_Itens[IDT_CAPACIDADE];
IDT_Ponteiro_t _IDT_Ponteiro;


void IDT_Config(Pos_t pos, UInt_t base, UShort_t seletor, Byte_t marcadores)
{
    _IDT_Itens[pos].BaseBaixa = base & 0xffff;
    _IDT_Itens[pos].BaseAlta = (base>> 16) & 0xffff;

    _IDT_Itens[pos].Zero = 0;
    _IDT_Itens[pos].Marcadores = marcadores;
    _IDT_Itens[pos].Seletor = seletor;
}

void IDT()
{
    _IDT_Ponteiro.Limite = (sizeof(IDT_Item_t) * IDT_CAPACIDADE) - 1;
    _IDT_Ponteiro.Base = (UInt_t)&_IDT_Itens;

    if(MSG_EXIBE_DADOS_IDT)
    {
        Mensagem("IDT", "Base: {0:u}", _IDT_Ponteiro.Base);
        Mensagem("IDT", "Tamanho: {0:u}", _IDT_Ponteiro.Limite);
    }

    Mem_RepeteByte((Byte_t *)&_IDT_Itens, 0, (sizeof(IDT_Item_t) * IDT_CAPACIDADE));

    if(MSG_EXIBE_MAPA_IDT) MensagemMapa("IDT", "Tabela: ", (Byte_t *)_IDT_Itens, (sizeof(IDT_Item_t) * IDT_CAPACIDADE), sizeof(IDT_Item_t));
    
    IDT_Atualiza();   
}