#include <HUSIS.h>

typedef struct __attribute__ ((__packed__))
{
    Byte_t Comando;
    Byte_t Posicao;
    Byte_t PosicaoNegativa;
    Byte_t Dados[128];
    Byte_t Soma;
} UnidadeRemota_Pacote_t;

typedef struct 
{
    UnidadeRemota_Pacote_t Entrada;
    Pos_t EntradaPosRecebimento;
    Boleano_t EntradaRecebido;
    UnidadeRemota_Pacote_t Saida;
    Pos_t SaidaPosEnvio;
    Boleano_t SaidaEnviado;
} UnidadeRemota_Montado_t;

#define UNIDADEREMOTA_CAPACIDADE 9

Pos_t _UnidadeRemota_Lista[UNIDADEREMOTA_CAPACIDADE];

Status_t _UnidadeRemota_Raiz(Pos_t unidade, Item_t **raiz)
{
    return STATUS_NAO_IMPLEMENTADO;
}
Status_t _UnidadeRemota_Desmonta(Pos_t unidade)
{
    return STATUS_NAO_IMPLEMENTADO;
}
Status_t _UnidadeRemota_ItemSubItem(Item_t *item, Pos_t posicao, Item_t **subItem)
{
    return STATUS_NAO_IMPLEMENTADO;
}
Status_t _UnidadeRemota_ItemQtdSubItem(Item_t *item, Tam_t *qtd)
{
    return STATUS_NAO_IMPLEMENTADO;
}
Status_t _UnidadeRemota_ItemVaPara(Item_t *item, Pos_t posicao)
{
    return STATUS_NAO_IMPLEMENTADO;
}
Tam_t _UnidadeRemota_ItemLeia(Item_t *item, Byte_t *destino, Tam_t tam)
{
    return STATUS_NAO_IMPLEMENTADO;
}
Tam_t _UnidadeRemota_ItemEscreva(Item_t *item, Byte_t *origem, Tam_t tam)
{
    return STATUS_NAO_IMPLEMENTADO;
}
Status_t _UnidadeRemota_ItemExclui(Item_t *item)
{
    return STATUS_NAO_IMPLEMENTADO;
}
Status_t _UnidadeRemota_ItemFecha(Item_t *item)
{
    return STATUS_NAO_IMPLEMENTADO;
}
Status_t _UnidadeRemota_ItemCriaDiretorio(Item_t *item, SByte_t *constanteNome)
{
    return STATUS_NAO_IMPLEMENTADO;
}
Status_t _UnidadeRemota_ItemCriaArquivo(Item_t *item, SByte_t *constanteNome)
{
    return STATUS_NAO_IMPLEMENTADO;
}

Status_t UnidadeRemota_ConectaViaArq(SByte_t * enderecoConst, Tam_t enderecoTam)
{
    Pos_t pos = 0;
    Status_t ret = STATUS_ESTOURO_DA_CAPACIDADE;
    for (Pos_t i = 0; i < UNIDADEREMOTA_CAPACIDADE; i++)
    {
        if(_UnidadeRemota_Lista[i] == 0)
        {
            pos = i;
            ret = STATUS_OK;
            break;
        }
    }
    if(ret != STATUS_OK) return ret;
    Item_t * item = 0;
    ret = Item_AbreConst(enderecoConst, enderecoTam, &item);
    if(ret != STATUS_OK) return ret;

    Pos_t unidade = 0;
    SByte_t nome[] = "Remoto  ";
    nome[7] = '0' + pos;
    ret = Unidade_Registra(nome, &unidade, 0);
    if(ret != STATUS_OK) return ret;
    ret = Unidade_RegistraMontagem(unidade, sizeof(UnidadeRemota_Pacote_t), &_UnidadeRemota_Raiz, &_UnidadeRemota_Desmonta, &_UnidadeRemota_ItemSubItem, &_UnidadeRemota_ItemQtdSubItem, &_UnidadeRemota_ItemVaPara, &_UnidadeRemota_ItemLeia, &_UnidadeRemota_ItemEscreva, &_UnidadeRemota_ItemExclui, &_UnidadeRemota_ItemFecha, &_UnidadeRemota_ItemCriaDiretorio, &_UnidadeRemota_ItemCriaArquivo);
    if(ret != STATUS_OK) return ret;
    

    return ret;
}

void UnidadeRemota()
{
    for (Pos_t i = 0; i < UNIDADEREMOTA_CAPACIDADE; i++)
    {
        _UnidadeRemota_Lista[i] = 0;
    }
    
}
