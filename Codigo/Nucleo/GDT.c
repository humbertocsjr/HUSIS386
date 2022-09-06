#include <HUSIS.h>


typedef struct __attribute__ ((__packed__))
{
    UShort_t LimiteBaixo;
    UShort_t BaseBaixa;
    Byte_t BaseMedia;
    Byte_t Acesso;
    Byte_t Granularidade;
    Byte_t BaseAlta;
} GDT_Item_t;

typedef struct __attribute__ ((__packed__))
{
    UShort_t Limite;
    UInt_t Base;
} GDT_Ponteiro_t;

#define GDT_CAPACIDADE 3

GDT_Item_t _GDT_Itens[GDT_CAPACIDADE];
GDT_Ponteiro_t _GDT_Ponteiro;


void GDT_Config(Pos_t pos, UInt_t base, UInt_t limite, Byte_t acesso, Byte_t granularidade)
{
    _GDT_Itens[pos].BaseBaixa = base & 0xffff;
    _GDT_Itens[pos].BaseMedia = (base >> 16) & 0xff;
    _GDT_Itens[pos].BaseAlta = (base >> 24) & 0xff;

    _GDT_Itens[pos].LimiteBaixo = limite & 0xffff;
    _GDT_Itens[pos].Granularidade = (limite >> 16) & 0xf;

    _GDT_Itens[pos].Granularidade |= granularidade & 0xf0;
    _GDT_Itens[pos].Acesso = acesso;
}

void GDT()
{
    _GDT_Ponteiro.Limite = (sizeof(GDT_Item_t) * GDT_CAPACIDADE) - 1;
    _GDT_Ponteiro.Base = (UInt_t)&_GDT_Itens;

    if(MSG_EXIBE_DADOS_GDT)
    {
        Mensagem("GDT", "Base: {0:u}", _GDT_Ponteiro.Base);
        Mensagem("GDT", "Tamanho: {0:u}", _GDT_Ponteiro.Limite);
    }

    GDT_Config(0, 0,0,0,0);
    GDT_Config(1, 0,0xffffffff, 0x9a, 0xcf);
    GDT_Config(2, 0,0xffffffff, 0x92, 0xcf);

    if(MSG_EXIBE_MAPA_GDT) MensagemMapa("GDT", "Tabela: ", (Byte_t *)_GDT_Itens, (sizeof(GDT_Item_t) * GDT_CAPACIDADE), sizeof(GDT_Item_t));

    GDT_Atualiza();

}