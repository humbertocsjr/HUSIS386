
#include <HUSIS.h>

Byte_t HARQ16_ExibeMensagensNivel = 0;

#define HARQ16_INDICE_DESVIO 440
#define HARQ16_INDICE_NOME_TAM 20

#define HARQ16_MapaItem_t UShort_t

typedef struct __attribute__ ((__packed__))
{
    SByte_t Assinatura[4];
    Byte_t Bits;
    Byte_t Versao;
    UInt_t TotalBlocos;
    UInt_t TotalItens;
    UInt_t MapaInicio;
    UInt_t MapaTotalBlocos;
    UInt_t RegistrosInicio;
    UInt_t RegistrosTotalBlocos;
    UInt_t AlteracoesInicio;
    UInt_t AlteracoesTotalBlocos;
    UInt_t Inicio;
    Byte_t Nome[HARQ16_INDICE_NOME_TAM];
    Byte_t Zero;
} HARQ16_Indice_t;

typedef struct __attribute__ ((__packed__))
{
    Byte_t Baixo;
    Byte_t Medio;
    Byte_t Alto;
} HARQ16_Ponteiro_t;

#define HARQ16_STATUS_VAZIO 0
#define HARQ16_STATUS_DESCRICAO 1
#define HARQ16_STATUS_DADOS 2

typedef struct __attribute__ ((__packed__))
{
    Byte_t Conteudo[1000];
    Byte_t Status;
    UShort_t Item;
    HARQ16_Ponteiro_t Posicao;
    HARQ16_Ponteiro_t Anterior;
    HARQ16_Ponteiro_t Proximo;
} HARQ16_Bloco_t;

typedef struct __attribute__ ((__packed__))
{
    UShort_t Ano;
    UShort_t MesDiaHora;
    UShort_t MinSeg;
} HARQ16_Data_t;

#define HARQ16_NOME_TAM 64

typedef struct __attribute__ ((__packed__))
{
    SByte_t Nome[HARQ16_NOME_TAM];
    UShort_t Zero;
    HARQ16_Data_t Criacao;
    HARQ16_Data_t Alteracao;
    UShort_t Usuario;
    UShort_t Grupo;
    UShort_t Permissao;
    HARQ16_Ponteiro_t InicioDados;
    HARQ16_Ponteiro_t InicioConfig;
    HARQ16_Ponteiro_t InicioExtra[4];
    UInt_t Tamanho;
    UShort_t TamanhoUltimoBloco;
    UShort_t Reservado[4];
} HARQ16_Descricao_t;

#define HARQ16_DESCRICAO_SUBITENS sizeof(HARQ16_Descricao_t)
#define HARQ16_DESCRICAO_SUBITENS_CAPACIDADE (((1024 - HARQ16_DESCRICAO_SUBITENS) / 2) - 1)

#define HARQ16_TIPO_VAZIO 0
#define HARQ16_TIPO_ARQUIVO 0x10
#define HARQ16_TIPO_DIRETORIO 0x20
#define HARQ16_TIPO_RESERVADO 0xff

typedef struct __attribute__ ((__packed__))
{
    Byte_t Tipo;
    HARQ16_Ponteiro_t Descricao;
} HARQ16_Registro_t;

#define HARQ16_QTD_REGISTROS_POR_BLOCO (1024 / sizeof(HARQ16_Registro_t))

#define HARQ16_QTD_ITENS_DO_MAPA_POR_BLOCO (1024 / sizeof(HARQ16_MapaItem_t))

typedef struct
{
    HARQ16_Indice_t Indice;
    Pos_t BlocoTempPos;
    Byte_t BlocoTemp[1024];
    Pos_t BlocoRegistroPos;
    Byte_t BlocoRegistro[1024];
    Pos_t BlocoMapaPos;
    Byte_t BlocoMapa[1024];
    Pos_t BlocoAlteracaoIndicePos;
    Byte_t BlocoAlteracaoIndice[1024];
    Pos_t BlocoAlteracaoPos;
    Byte_t BlocoAlteracao[1024];
    Tam_t QtdItensAbertos;
    Pos_t Dispositivo;
    Pos_t Unidade;
} HARQ16_Montado_t;

typedef struct
{
    Item_t Item;
    Pos_t TempPos;
    Byte_t Temp[1024];
    HARQ16_Ponteiro_t Descricao;
    HARQ16_Montado_t * Montado;
} HARQ16_SisArq_Item_t;

Status_t _HARQ16_LeiaBloco1024(HARQ16_Montado_t * montado, Pos_t posicao, Byte_t * destino)
{
    Pos_t disp = montado->Dispositivo;
    if(HARQ16_ExibeMensagensNivel > 9) Mensagem2("HARQ16", "Inicio da Leitura do Bloco {1:u} no Dispositivo {0:u}", disp, posicao);
    Tam_t tam = Dispositivo_Leia(disp, posicao << 1, destino, 512);
    if(tam == 0) return STATUS_NAO_FOI_POSSIVEL_LER;
    tam = Dispositivo_Leia(disp, (posicao << 1) + 1, destino + 512, 512);
    if(tam == 0) return STATUS_NAO_FOI_POSSIVEL_LER;
    if(HARQ16_ExibeMensagensNivel > 8) Mensagem2("HARQ16", "Fim da Leitura do Bloco {1:u} no Dispositivo {0:u}", disp, posicao);
    return STATUS_OK;
}

Status_t _HARQ16_EscrevaBloco1024(HARQ16_Montado_t * montado, Pos_t posicao, Byte_t * origem)
{
    Pos_t disp = montado->Dispositivo;
    if(HARQ16_ExibeMensagensNivel > 9) Mensagem2("HARQ16", "Inicio da Escrita do Bloco {1:u} no Dispositivo {0:u}", disp, posicao);
    Tam_t tam = Dispositivo_Escreva(disp, posicao << 1, origem, 512);
    if(tam == 0) return STATUS_NAO_FOI_POSSIVEL_ESCREVER;
    tam = Dispositivo_Escreva(disp, (posicao << 1) + 1, origem + 512, 512);
    if(tam == 0) return STATUS_NAO_FOI_POSSIVEL_ESCREVER;
    if(HARQ16_ExibeMensagensNivel > 8) Mensagem2("HARQ16", "Fim da Escrita do Bloco {1:u} no Dispositivo {0:u}", disp, posicao);
    return STATUS_OK;
}

#define HARQ16_MONTADO_TIPO_ALTERACAO 1
#define HARQ16_MONTADO_TIPO_ALTERACAO_INDICE 2
#define HARQ16_MONTADO_TIPO_MAPA 3
#define HARQ16_MONTADO_TIPO_REGISTRO 4
#define HARQ16_MONTADO_TIPO_TEMP 5

Status_t _HARQ16_LeiaMontado(HARQ16_Montado_t * montado, Byte_t tipo, Pos_t posicao)
{
    Byte_t * destino = 0;
    switch (tipo)
    {
        case HARQ16_MONTADO_TIPO_ALTERACAO:
        {
            if(HARQ16_ExibeMensagensNivel > 3) Mensagem2("HARQ16", "ALTERACAO: Leitura do Bloco {1:u} no Dispositivo {0:u}", montado->Dispositivo, posicao);
            if((montado->BlocoAlteracaoPos == posicao) & (posicao > 0)) return STATUS_OK;
            montado->BlocoAlteracaoPos = posicao;
            destino = (Byte_t *)&montado->BlocoAlteracao;
            break;
        }
        case HARQ16_MONTADO_TIPO_ALTERACAO_INDICE:
        {
            if(HARQ16_ExibeMensagensNivel > 3) Mensagem2("HARQ16", "ALTERACAO INDICE: Leitura do Bloco {1:u} no Dispositivo {0:u}", montado->Dispositivo, posicao);
            if((montado->BlocoAlteracaoIndicePos == posicao) & (posicao > 0)) return STATUS_OK;
            montado->BlocoAlteracaoIndicePos = posicao;
            destino = (Byte_t *)&montado->BlocoAlteracaoIndice;
            break;
        }
        case HARQ16_MONTADO_TIPO_MAPA:
        {
            if(HARQ16_ExibeMensagensNivel > 3) Mensagem2("HARQ16", "MAPA: Leitura do Bloco {1:u} no Dispositivo {0:u}", montado->Dispositivo, posicao);
            if((montado->BlocoMapaPos == posicao) & (posicao > 0)) return STATUS_OK;
            montado->BlocoMapaPos = posicao;
            destino = (Byte_t *)&montado->BlocoMapa;
            break;
        }
        case HARQ16_MONTADO_TIPO_REGISTRO:
        {
            if(HARQ16_ExibeMensagensNivel > 3) Mensagem2("HARQ16", "REGISTRO: Leitura do Bloco {1:u} no Dispositivo {0:u}", montado->Dispositivo, posicao);
            if((montado->BlocoRegistroPos == posicao) & (posicao > 0)) return STATUS_OK;
            montado->BlocoRegistroPos = posicao;
            destino = (Byte_t *)&montado->BlocoRegistro;
            break;
        }
        case HARQ16_MONTADO_TIPO_TEMP:
        {
            if(HARQ16_ExibeMensagensNivel > 3) Mensagem2("HARQ16", "TEMP: Leitura do Bloco {1:u} no Dispositivo {0:u}", montado->Dispositivo, posicao);
            if((montado->BlocoTempPos == posicao) & (posicao > 0)) return STATUS_OK;
            montado->BlocoTempPos = posicao;
            destino = (Byte_t *)&montado->BlocoTemp;
            break;
        }
        default:
        {
            return STATUS_NAO_IMPLEMENTADO;
        }
    }
    return _HARQ16_LeiaBloco1024(montado, posicao, destino);
}

Status_t _HARQ16_EscrevaMontadoManual(HARQ16_Montado_t * montado, Byte_t tipo, Pos_t posicao)
{
    Byte_t * origem = 0;
    switch (tipo)
    {
        case HARQ16_MONTADO_TIPO_ALTERACAO:
        {
            if(HARQ16_ExibeMensagensNivel > 3) Mensagem2("HARQ16", "ALTERACAO: Escrita do Bloco {1:u} no Dispositivo {0:u}", montado->Dispositivo, posicao);
            montado->BlocoAlteracaoPos = posicao;
            origem = (Byte_t *)&montado->BlocoAlteracao;
            break;
        }
        case HARQ16_MONTADO_TIPO_ALTERACAO_INDICE:
        {
            if(HARQ16_ExibeMensagensNivel > 3) Mensagem2("HARQ16", "ALTERACAO INDICE: Escrita do Bloco {1:u} no Dispositivo {0:u}", montado->Dispositivo, posicao);
            montado->BlocoAlteracaoIndicePos = posicao;
            origem = (Byte_t *)&montado->BlocoAlteracaoIndice;
            break;
        }
        case HARQ16_MONTADO_TIPO_MAPA:
        {
            if(HARQ16_ExibeMensagensNivel > 3) Mensagem2("HARQ16", "MAPA: Escrita do Bloco {1:u} no Dispositivo {0:u}", montado->Dispositivo, posicao);
            montado->BlocoMapaPos = posicao;
            origem = (Byte_t *)&montado->BlocoMapa;
            break;
        }
        case HARQ16_MONTADO_TIPO_REGISTRO:
        {
            if(HARQ16_ExibeMensagensNivel > 3) Mensagem2("HARQ16", "REGISTRO: Escrita do Bloco {1:u} no Dispositivo {0:u}", montado->Dispositivo, posicao);
            montado->BlocoRegistroPos = posicao;
            origem = (Byte_t *)&montado->BlocoRegistro;
            break;
        }
        case HARQ16_MONTADO_TIPO_TEMP:
        {
            if(HARQ16_ExibeMensagensNivel > 3) Mensagem2("HARQ16", "TEMP: Escrita do Bloco {1:u} no Dispositivo {0:u}", montado->Dispositivo, posicao);
            montado->BlocoTempPos = posicao;
            origem = (Byte_t *)&montado->BlocoTemp;
            break;
        }
        default:
        {
            return STATUS_NAO_IMPLEMENTADO;
        }
    }
    return _HARQ16_EscrevaBloco1024(montado, posicao, origem);
}

Status_t _HARQ16_EscrevaMontadoAuto(HARQ16_Montado_t * montado, Byte_t tipo)
{
    Byte_t * origem = 0;
    Pos_t posicao = 0;
    switch (tipo)
    {
        case HARQ16_MONTADO_TIPO_ALTERACAO:
        {
            if(HARQ16_ExibeMensagensNivel > 3) Mensagem2("HARQ16", "ALTERACAO: Escrita do Bloco {1:u} no Dispositivo {0:u}", montado->Dispositivo, posicao);
            posicao = montado->BlocoAlteracaoPos;
            origem = (Byte_t *)&montado->BlocoAlteracao;
            break;
        }
        case HARQ16_MONTADO_TIPO_ALTERACAO_INDICE:
        {
            if(HARQ16_ExibeMensagensNivel > 3) Mensagem2("HARQ16", "ALTERACAO INDICE: Escrita do Bloco {1:u} no Dispositivo {0:u}", montado->Dispositivo, posicao);
            posicao = montado->BlocoAlteracaoIndicePos;
            origem = (Byte_t *)&montado->BlocoAlteracaoIndice;
            break;
        }
        case HARQ16_MONTADO_TIPO_MAPA:
        {
            if(HARQ16_ExibeMensagensNivel > 3) Mensagem2("HARQ16", "MAPA: Escrita do Bloco {1:u} no Dispositivo {0:u}", montado->Dispositivo, posicao);
            posicao = montado->BlocoMapaPos;
            origem = (Byte_t *)&montado->BlocoMapa;
            break;
        }
        case HARQ16_MONTADO_TIPO_REGISTRO:
        {
            if(HARQ16_ExibeMensagensNivel > 3) Mensagem2("HARQ16", "REGISTRO: Escrita do Bloco {1:u} no Dispositivo {0:u}", montado->Dispositivo, posicao);
            posicao = montado->BlocoRegistroPos;
            origem = (Byte_t *)&montado->BlocoRegistro;
            break;
        }
        case HARQ16_MONTADO_TIPO_TEMP:
        {
            if(HARQ16_ExibeMensagensNivel > 3) Mensagem2("HARQ16", "TEMP: Escrita do Bloco {1:u} no Dispositivo {0:u}", montado->Dispositivo, posicao);
            posicao = montado->BlocoTempPos;
            origem = (Byte_t *)&montado->BlocoTemp;
            break;
        }
        default:
        {
            return STATUS_NAO_IMPLEMENTADO;
        }
    }
    return _HARQ16_EscrevaBloco1024(montado, posicao, origem);
}

Status_t _HARQ16_LeiaTemp(HARQ16_SisArq_Item_t * hitem, Pos_t bloco)
{
    if(hitem->TempPos == bloco)
    {
        return STATUS_OK;
    }
    hitem->TempPos = bloco;
    return _HARQ16_LeiaBloco1024(hitem->Montado, bloco, (Byte_t *)&hitem->Temp);
}

Status_t _HARQ16_EscrevaTempManual(HARQ16_SisArq_Item_t * hitem, Pos_t bloco)
{
    hitem->TempPos = bloco;
    return _HARQ16_EscrevaBloco1024(hitem->Montado, bloco, (Byte_t *)&hitem->Temp);
}

Status_t _HARQ16_EscrevaTempAuto(HARQ16_SisArq_Item_t * hitem)
{
    return _HARQ16_EscrevaBloco1024(hitem->Montado, hitem->TempPos, (Byte_t *)&hitem->Temp);
}

void _HARQ16_DefinePonteiro(HARQ16_Ponteiro_t * ptr, Pos_t valor)
{
    ptr->Baixo = valor & 0xff;
    ptr->Medio = (valor >> 8) & 0xff;
    ptr->Alto = (valor >> 16) & 0xff;
}

Pos_t _HARQ16_LeiaPonteiro(HARQ16_Ponteiro_t * ptr)
{
    return (ptr->Baixo) | (ptr->Medio << 8) | (ptr->Alto << 16);
}

Status_t _HARQ16_CriaBloco(HARQ16_Montado_t * montado, Pos_t item, HARQ16_Ponteiro_t * saida)
{
    for (Pos_t i = 0; i < montado->Indice.MapaTotalBlocos; i++)
    {
        Status_t ret = _HARQ16_LeiaMontado(montado, HARQ16_MONTADO_TIPO_MAPA, i + montado->Indice.MapaInicio);
        if(ret != STATUS_OK) return ret;
        HARQ16_MapaItem_t * mapa = (HARQ16_MapaItem_t *)&montado->BlocoMapa;
        for (Pos_t j = 0; j < HARQ16_QTD_ITENS_DO_MAPA_POR_BLOCO; j++)
        {
            Pos_t pos = i * HARQ16_QTD_ITENS_DO_MAPA_POR_BLOCO + j;
            if(mapa[j] == 0)
            {
                mapa[j] = item;
                _HARQ16_DefinePonteiro(saida, pos);
                ret = _HARQ16_EscrevaMontadoManual(montado, HARQ16_MONTADO_TIPO_MAPA, i + montado->Indice.MapaInicio);
                return STATUS_OK;
            }
        }
    }
    return STATUS_ESTOURO_DA_CAPACIDADE;
}

Status_t _HARQ16_CriaItem(HARQ16_Montado_t * montado, Byte_t tipo, Pos_t * id)
{
    for (Pos_t i = 0; i < montado->Indice.RegistrosTotalBlocos; i++)
    {
        Status_t ret = _HARQ16_LeiaMontado(montado, HARQ16_MONTADO_TIPO_REGISTRO, i + montado->Indice.RegistrosInicio);
        if(ret != STATUS_OK) return ret;
        HARQ16_Registro_t * regs = (HARQ16_Registro_t *)&montado->BlocoRegistro;
        for (Pos_t j = 0; j < HARQ16_QTD_REGISTROS_POR_BLOCO; j++)
        {
            Pos_t pos = i * HARQ16_QTD_REGISTROS_POR_BLOCO + j;
            if(regs[j].Tipo == HARQ16_TIPO_VAZIO)
            {
                regs[j].Tipo = tipo;
                ret = _HARQ16_CriaBloco(montado, pos, &regs[j].Descricao);
                if(ret != STATUS_OK) return ret;
                *id = pos;
                ret = _HARQ16_EscrevaMontadoManual(montado, HARQ16_MONTADO_TIPO_REGISTRO, i + montado->Indice.RegistrosInicio);
                return ret;
            }
        }
    }
    return STATUS_ESTOURO_DA_CAPACIDADE;
}

Status_t _HARQ16_AbreItemSimples(HARQ16_Montado_t * montado, Pos_t id, HARQ16_SisArq_Item_t * * hitem)
{
    if(id >= montado->Indice.TotalItens) return STATUS_POSICAO_INVALIDA;
    Status_t ret = _HARQ16_LeiaMontado(montado, HARQ16_MONTADO_TIPO_REGISTRO, id / HARQ16_QTD_REGISTROS_POR_BLOCO + montado->Indice.RegistrosInicio);
    if(ret != STATUS_OK) return ret;
    HARQ16_Registro_t * regs = (HARQ16_Registro_t*)(&montado->BlocoRegistro);
    Pos_t regPos = (id % HARQ16_QTD_REGISTROS_POR_BLOCO);
    *hitem = (HARQ16_SisArq_Item_t *)Mem_Local_Aloca(sizeof(HARQ16_SisArq_Item_t));
    HARQ16_SisArq_Item_t * saida = *hitem;
    saida->Item.Identificador = id;
    saida->Item.Especial = ITEM_ESPECIAL_NENHUM;
    for (Pos_t i = 0; i < ITEM_NOME_TAM; i++)
    {
        saida->Item.Nome[i] = 0;
    }
    saida->Item.PosicaoEspecial = 0;
    saida->Item.PosicaoNoItem = 0;
    switch (regs[regPos].Tipo)
    {
        case HARQ16_TIPO_ARQUIVO:
        {
            saida->Item.Tipo = ITEM_TIPO_ARQUIVO;
            break;
        }
        case HARQ16_TIPO_DIRETORIO:
        {
            saida->Item.Tipo = ITEM_TIPO_DIRETORIO;
            break;
        }
        default:
        {
            Mem_Local_Libera((Byte_t *)(*hitem));
            return STATUS_TIPO_INVALIDO;
        }
    }
    saida->Item.Unidade = montado->Unidade;
    saida->TempPos = 0;
    saida->Montado = montado;
    saida->Descricao.Baixo = regs[regPos].Descricao.Baixo;
    saida->Descricao.Medio = regs[regPos].Descricao.Medio;
    saida->Descricao.Alto = regs[regPos].Descricao.Alto;
    montado->QtdItensAbertos ++;
    return STATUS_OK;
}

Status_t _HARQ16_FechaItem(HARQ16_SisArq_Item_t * hitem)
{
    if(hitem->Montado->QtdItensAbertos > 0)hitem->Montado->QtdItensAbertos --;
    return Mem_Local_Libera((Byte_t *)hitem);
}

Status_t HARQ16_Raiz(Pos_t unidade, Item_t **raiz)
{
    HARQ16_Montado_t * montado = 0;
    Status_t ret = Unidade_CarregaObjetoMontado(unidade, (Byte_t * *)&montado);
    if (ret != STATUS_OK) return ret;
    *raiz = (Item_t *)Mem_Local_Aloca(sizeof(HARQ16_SisArq_Item_t));
    Unidade_LeiaNomeConst(unidade, (*raiz)->Nome, ITEM_NOME_TAM);
    (*raiz)->Identificador = 1;
    (*raiz)->Especial = ITEM_ESPECIAL_NENHUM;
    (*raiz)->Tipo = ITEM_TIPO_DIRETORIO;
    (*raiz)->Unidade = unidade;
    HARQ16_SisArq_Item_t * hitem = (HARQ16_SisArq_Item_t *)(*raiz);
    hitem->TempPos = 0;
    hitem->Montado = montado;
    _HARQ16_LeiaMontado(montado, HARQ16_MONTADO_TIPO_REGISTRO, montado->Indice.RegistrosInicio);
    HARQ16_Registro_t * regs = (HARQ16_Registro_t *)&montado->BlocoRegistro;
    hitem->Descricao.Baixo = regs[1].Descricao.Baixo;
    hitem->Descricao.Medio = regs[1].Descricao.Medio;
    hitem->Descricao.Alto = regs[1].Descricao.Alto;
    return STATUS_OK;
}

Status_t HARQ16_Desmonta(Pos_t unidade)
{
    HARQ16_Montado_t * montado = 0;
    Status_t ret = Unidade_CarregaObjetoMontado(unidade, (Byte_t **)&montado);
    if(ret != STATUS_OK) return ret;
    if(montado->QtdItensAbertos != 0)
    {
        if(HARQ16_ExibeMensagensNivel > 0)Mensagem2("HARQ16", "Nao foi possivel desmontar por ter {0:u} itens abertos na unidade {1:u}", montado->QtdItensAbertos, unidade);
        return STATUS_QUANTIDADE_INVALIDA;
    }
    return STATUS_OK;
}

Status_t HARQ16_ItemSubItem(Item_t *item, Pos_t posicao, Item_t **subItem)
{
    if(posicao >= HARQ16_DESCRICAO_SUBITENS_CAPACIDADE) return STATUS_ESTOURO_DA_CAPACIDADE;
    HARQ16_SisArq_Item_t * hitem = (HARQ16_SisArq_Item_t *)(item);
    Status_t ret = _HARQ16_LeiaTemp(hitem, _HARQ16_LeiaPonteiro(&hitem->Descricao));
    if(ret != STATUS_OK) return ret;
    HARQ16_MapaItem_t * subItens = (HARQ16_MapaItem_t *)(((Pos_t)&hitem->Temp) + HARQ16_DESCRICAO_SUBITENS);
    if(subItens[posicao] == 0) return STATUS_POSICAO_INVALIDA;
    HARQ16_SisArq_Item_t * hsubItem = 0;

    ret = _HARQ16_AbreItemSimples(hitem->Montado, subItens[posicao], &hsubItem);
    if(ret != STATUS_OK) return ret;
    ret = _HARQ16_LeiaTemp(hsubItem, _HARQ16_LeiaPonteiro(&hsubItem->Descricao));
    if(ret != STATUS_OK) return ret;
    HARQ16_Descricao_t * desc = (HARQ16_Descricao_t *) &hsubItem->Temp;

    Const_Copia((SByte_t *)&hsubItem->Item.Nome, (SByte_t *)&desc->Nome, HARQ16_NOME_TAM > ITEM_NOME_TAM ? ITEM_NOME_TAM : HARQ16_NOME_TAM);
    
    Item_t * subItemRet = (Item_t *)((Pos_t)hsubItem);

    if(HARQ16_ExibeMensagensNivel > 1) MensagemConst("HARQ16", "Abrindo item: ", subItemRet->Nome, ITEM_NOME_TAM);

    *subItem = subItemRet;
    return STATUS_OK;
}

Status_t HARQ16_ItemQtdSubItem(Item_t *item, Tam_t *qtd)
{
    HARQ16_SisArq_Item_t * hitem = (HARQ16_SisArq_Item_t *)(item);
    Status_t ret = _HARQ16_LeiaTemp(hitem, _HARQ16_LeiaPonteiro(&hitem->Descricao));
    if(ret != STATUS_OK) return ret;
    Tam_t contagem = 0;
    HARQ16_MapaItem_t * subItens = (HARQ16_MapaItem_t *)(((Pos_t)&hitem->Temp) + HARQ16_DESCRICAO_SUBITENS);
    for (Pos_t i = 0; i < HARQ16_DESCRICAO_SUBITENS_CAPACIDADE; i++)
    {
        if(subItens[i] != 0)
        {
            contagem++;
        }
    }
    *qtd = contagem;
    return STATUS_OK;
}

Status_t HARQ16_ItemVaPara(Item_t *item, Pos_t posicao)
{
    item->PosicaoNoItem = posicao;
    return STATUS_OK;
}

Tam_t HARQ16_ItemLeia(Item_t *item, Byte_t *destino, Tam_t tam)
{
    return STATUS_NAO_IMPLEMENTADO;
}

Tam_t HARQ16_ItemEscreva(Item_t *item, Byte_t *origem, Tam_t tam)
{
    return STATUS_NAO_IMPLEMENTADO;
}

Status_t HARQ16_ItemExclui(Item_t *item)
{
    return STATUS_NAO_IMPLEMENTADO;
}

Status_t HARQ16_ItemFecha(Item_t *item)
{
    return _HARQ16_FechaItem((HARQ16_SisArq_Item_t *)item);
}

Status_t HARQ16_ItemCriaDiretorio(Item_t *item, SByte_t *constanteNome)
{
    HARQ16_SisArq_Item_t * hitem = (HARQ16_SisArq_Item_t *)(item);
    Status_t ret = _HARQ16_LeiaTemp(hitem, _HARQ16_LeiaPonteiro(&hitem->Descricao));
    if(ret != STATUS_OK) return ret;
    HARQ16_MapaItem_t * subItens = (HARQ16_MapaItem_t *)(((Pos_t)&hitem->Temp) + HARQ16_DESCRICAO_SUBITENS);
    Boleano_t encontrado = NAO;
    Pos_t subItemPos = 0;
    for (Pos_t i = 0; i < HARQ16_DESCRICAO_SUBITENS_CAPACIDADE; i++)
    {
        if(subItens[i] == 0)
        {
            encontrado = SIM;
            subItemPos = i;
            break;
        }
    }
    if(!encontrado) return STATUS_ESTOURO_DA_CAPACIDADE;
    Pos_t id = 0;
    ret = _HARQ16_CriaItem(hitem->Montado, HARQ16_TIPO_DIRETORIO, &id);
    if(ret != STATUS_OK) return ret;
    HARQ16_SisArq_Item_t * hsubItem = 0;
    ret = _HARQ16_AbreItemSimples(hitem->Montado, id, &hsubItem);
    if(ret != STATUS_OK) return ret;
    ret = _HARQ16_LeiaTemp(hitem, _HARQ16_LeiaPonteiro(&hitem->Descricao));
    if(ret != STATUS_OK) return ret;
    ret = _HARQ16_LeiaTemp(hsubItem, _HARQ16_LeiaPonteiro(&hsubItem->Descricao));
    if(ret != STATUS_OK) return ret;
    HARQ16_Descricao_t * descSub = (HARQ16_Descricao_t *) &hsubItem->Temp;
    Const_Copia((SByte_t *)&descSub->Nome, constanteNome, HARQ16_NOME_TAM);
    subItens[subItemPos] = id;
    HARQ16_MapaItem_t * subSubItens = (HARQ16_MapaItem_t *)(((Pos_t)&hsubItem->Temp) + HARQ16_DESCRICAO_SUBITENS);
    for (Pos_t i = 0; i < HARQ16_DESCRICAO_SUBITENS_CAPACIDADE; i++)
    {
        subSubItens[i] = 0;
    }
    _HARQ16_EscrevaTempAuto(hitem);
    _HARQ16_EscrevaTempAuto(hsubItem);
    _HARQ16_FechaItem(hsubItem);
    return STATUS_OK;
}

Status_t HARQ16_ItemCriaArquivo(Item_t *item, SByte_t *constanteNome)
{
    HARQ16_SisArq_Item_t * hitem = (HARQ16_SisArq_Item_t *)(item);
    Status_t ret = _HARQ16_LeiaTemp(hitem, _HARQ16_LeiaPonteiro(&hitem->Descricao));
    if(ret != STATUS_OK) return ret;
    HARQ16_MapaItem_t * subItens = (HARQ16_MapaItem_t *)(((Pos_t)&hitem->Temp) + HARQ16_DESCRICAO_SUBITENS);
    Boleano_t encontrado = NAO;
    Pos_t subItemPos = 0;
    for (Pos_t i = 0; i < HARQ16_DESCRICAO_SUBITENS_CAPACIDADE; i++)
    {
        if(subItens[i] == 0)
        {
            encontrado = SIM;
            subItemPos = i;
            break;
        }
    }
    if(!encontrado) return STATUS_ESTOURO_DA_CAPACIDADE;
    Pos_t id = 0;
    ret = _HARQ16_CriaItem(hitem->Montado, HARQ16_TIPO_ARQUIVO, &id);
    if(ret != STATUS_OK) return ret;
    HARQ16_SisArq_Item_t * hsubItem = 0;
    ret = _HARQ16_AbreItemSimples(hitem->Montado, id, &hsubItem);
    if(ret != STATUS_OK) return ret;
    ret = _HARQ16_LeiaTemp(hitem, _HARQ16_LeiaPonteiro(&hitem->Descricao));
    if(ret != STATUS_OK) return ret;
    ret = _HARQ16_LeiaTemp(hsubItem, _HARQ16_LeiaPonteiro(&hsubItem->Descricao));
    if(ret != STATUS_OK) return ret;
    HARQ16_Descricao_t * descSub = (HARQ16_Descricao_t *) &hsubItem->Temp;
    Const_Copia((SByte_t *)&descSub->Nome, constanteNome, HARQ16_NOME_TAM);
    subItens[subItemPos] = id;
    _HARQ16_EscrevaTempAuto(hitem);
    _HARQ16_EscrevaTempAuto(hsubItem);
    _HARQ16_FechaItem(hsubItem);
    return STATUS_OK;
}

Status_t HARQ16_Monta(Pos_t unidade)
{
    // Registra o ponto de montagem
    HARQ16_Montado_t * montado = 0;
    Pos_t disp = 0;
    Status_t ret = Unidade_RegistraMontagem(unidade, sizeof(HARQ16_Montado_t), &HARQ16_Raiz, &HARQ16_Desmonta, &HARQ16_ItemSubItem, &HARQ16_ItemQtdSubItem, &HARQ16_ItemVaPara, &HARQ16_ItemLeia, &HARQ16_ItemEscreva, &HARQ16_ItemExclui, &HARQ16_ItemFecha, &HARQ16_ItemCriaDiretorio, &HARQ16_ItemCriaArquivo);
    if(ret != STATUS_OK) return ret;
    ret = Unidade_CarregaObjetoMontado(unidade, (Byte_t * *)&montado);
    if(ret != STATUS_OK) return ret;
    ret = Unidade_Dispositivo(unidade, &disp);
    if(ret != STATUS_OK) return ret;
    montado->Dispositivo = disp;
    montado->Unidade = unidade;
    montado->BlocoMapaPos = 0;
    montado->BlocoTempPos = 0;
    montado->BlocoRegistroPos = 0;
    montado->BlocoAlteracaoPos = 0;
    montado->BlocoAlteracaoIndicePos = 0;
    ret = _HARQ16_LeiaMontado(montado, HARQ16_MONTADO_TIPO_TEMP, 0);
    if(ret != STATUS_OK) return ret;
    Mem_CopiaBinario((Byte_t *) &montado->Indice, (Byte_t *)(&montado->BlocoTemp) + HARQ16_INDICE_DESVIO, sizeof(HARQ16_Indice_t));
    if(HARQ16_ExibeMensagensNivel > 0) Mensagem2("HARQ16", "Montado unidade {0:u} de {1:u} KiB", unidade , montado->Indice.TotalBlocos);
    if(HARQ16_ExibeMensagensNivel > 0) Mensagem("HARQ16", "Cada diretorio pode conter {0:u} itens", HARQ16_DESCRICAO_SUBITENS_CAPACIDADE);
    return STATUS_OK;
}

Status_t HARQ16_Valida(Pos_t unidade)
{
    Status_t ret = STATUS_OK;
    Tam_t tamUmBloco = 0;
    Pos_t disp = 0;
    // Carrega dados essenciais
    ret = Unidade_LeiaTamanhoDeUmBloco(unidade, &tamUmBloco);
    if(ret != STATUS_OK) return ret;
    ret = Unidade_Dispositivo(unidade, &disp);
    if(ret != STATUS_OK) return ret;
    Byte_t tmp[512];
    if(Dispositivo_Leia(disp, 0, (Byte_t *)&tmp, 512) == 0) return STATUS_DISPOSITIVO_INVALIDO;
    HARQ16_Indice_t * indice = (HARQ16_Indice_t *)(((Pos_t)&tmp) + HARQ16_INDICE_DESVIO);
    if
    (
        (indice->Assinatura[0] == 'H') &
        (indice->Assinatura[1] == 'A') &
        (indice->Assinatura[2] == 'R') &
        (indice->Assinatura[3] == 'Q') &
        (indice->Bits == 16) &
        (indice->Versao == 0)
    )
    {
        return STATUS_OK;
    }
    if(HARQ16_ExibeMensagensNivel > 3)Mensagem6("HARQ16","Assinatura: '{0:c}{1:c}{2:c}{3:c}' Bits {4:u} Versao {5:u}", indice->Assinatura[0], indice->Assinatura[1], indice->Assinatura[2], indice->Assinatura[3], indice->Bits, indice->Versao);
    return STATUS_FORMATO_INVALIDO;
}

Status_t HARQ16_Formata(Pos_t unidade)
{
    Status_t ret = STATUS_OK;
    Tam_t tamUmBloco = 0;
    Pos_t disp = 0;
    Tam_t totalBlocos = 0;
    // Carrega dados essenciais
    ret = Unidade_LeiaTamanhoDeUmBloco(unidade, &tamUmBloco);
    if(ret != STATUS_OK) return ret;
    ret = Unidade_Dispositivo(unidade, &disp);
    if(ret != STATUS_OK) return ret;
    ret = Dispositivo_LeiaTamanho(disp, &totalBlocos);
    if(ret != STATUS_OK) return ret;
    // Cria objeto montado temporario
    HARQ16_Montado_t * montado = (HARQ16_Montado_t *)Mem_Local_Aloca(sizeof(HARQ16_Montado_t));
    montado->BlocoAlteracaoPos = 0;
    montado->BlocoAlteracaoIndicePos = 0;
    montado->BlocoMapaPos = 0;
    montado->BlocoRegistroPos = 0;
    montado->BlocoTempPos = 0;
    montado->Dispositivo = disp;
    montado->Unidade = unidade;
    HARQ16_Indice_t * indice = &montado->Indice;
    indice->Bits = 16;
    indice->Versao = 0;
    indice->Assinatura[0] = 'H';
    indice->Assinatura[1] = 'A';
    indice->Assinatura[2] = 'R';
    indice->Assinatura[3] = 'Q';
    indice->TotalBlocos = totalBlocos;
    indice->RegistrosTotalBlocos = totalBlocos / 2000;
    if(indice->RegistrosTotalBlocos == 0) indice->RegistrosTotalBlocos = 1;
    if(indice->RegistrosTotalBlocos > 256) indice->RegistrosTotalBlocos = 256;
    indice->TotalItens = indice->RegistrosTotalBlocos * HARQ16_QTD_REGISTROS_POR_BLOCO;
    indice->MapaInicio = 8;
    if((totalBlocos % HARQ16_QTD_ITENS_DO_MAPA_POR_BLOCO) == 0)
    {
        indice->MapaTotalBlocos = totalBlocos / HARQ16_QTD_ITENS_DO_MAPA_POR_BLOCO;
    }
    else
    {
        indice->MapaTotalBlocos = totalBlocos / HARQ16_QTD_ITENS_DO_MAPA_POR_BLOCO + 1;
    }
    indice->RegistrosInicio = indice->MapaInicio + indice->MapaTotalBlocos;
    indice->AlteracoesInicio = indice->RegistrosInicio + indice->RegistrosTotalBlocos;
    indice->AlteracoesTotalBlocos = 18; // 2 Copias indice e 8 Copias de antes e depois
    indice->Inicio = indice->AlteracoesInicio + indice->AlteracoesTotalBlocos;
    indice->Nome[0] = 0;
    indice->Zero = 0;
    for (Pos_t i = 0; i < indice->MapaTotalBlocos; i++)
    {
        HARQ16_MapaItem_t * mapa = (HARQ16_MapaItem_t *)&montado->BlocoMapa;
        for (Pos_t j = 0; j < HARQ16_QTD_ITENS_DO_MAPA_POR_BLOCO; j++)
        {
            Pos_t pos = i * HARQ16_QTD_ITENS_DO_MAPA_POR_BLOCO + j;
            if(pos < indice->Inicio)
            {
                mapa[j] = 0xffff;
            }
            else if(pos >= indice->TotalBlocos)
            {
                mapa[j] = 0xffff;
            }
            else
            {
                mapa[j] = 0;
            }
        }
        _HARQ16_EscrevaMontadoManual(montado, HARQ16_MONTADO_TIPO_MAPA, i + indice->MapaInicio);
    }
    for (Pos_t i = 0; i < indice->RegistrosTotalBlocos; i++)
    {
        HARQ16_Registro_t * regs = (HARQ16_Registro_t *)&montado->BlocoRegistro;
        for (Pos_t j = 0; j < HARQ16_QTD_REGISTROS_POR_BLOCO; j++)
        {
            Pos_t pos = i * HARQ16_QTD_REGISTROS_POR_BLOCO + j;
            if(pos == 0)
            {
                regs[j].Tipo = HARQ16_TIPO_RESERVADO;
            }
            else
            {
                regs[j].Tipo = HARQ16_TIPO_VAZIO;
            }
        }
        _HARQ16_EscrevaMontadoManual(montado, HARQ16_MONTADO_TIPO_REGISTRO, i + indice->RegistrosInicio);
    }
    // Cria o diretorio Raiz
    Pos_t raiz = 0;
    ret = _HARQ16_CriaItem(montado, HARQ16_TIPO_DIRETORIO, &raiz);
    if(ret != STATUS_OK) return ret;
    if(raiz != 1) ret = STATUS_POSICAO_INVALIDA;
    HARQ16_SisArq_Item_t * hitem;
    ret = _HARQ16_AbreItemSimples(montado, raiz, &hitem);
    if(ret != STATUS_OK) return ret;
    ret = _HARQ16_LeiaTemp(hitem, _HARQ16_LeiaPonteiro(&hitem->Descricao));
    if(ret != STATUS_OK) return ret;
    HARQ16_Bloco_t * bloco = (HARQ16_Bloco_t *)&hitem->Temp;
    HARQ16_Descricao_t * desc = (HARQ16_Descricao_t *)&hitem->Temp;
    desc->Nome[0] = 'R';
    desc->Nome[1] = 'a';
    desc->Nome[2] = 'i';
    desc->Nome[3] = 'z';
    desc->Nome[4] = 0;
    bloco->Item = raiz;
    _HARQ16_DefinePonteiro(&bloco->Posicao, 0);
    _HARQ16_DefinePonteiro(&bloco->Proximo, 0);
    _HARQ16_DefinePonteiro(&bloco->Anterior, 0);
    bloco->Status = HARQ16_STATUS_DESCRICAO;
    HARQ16_MapaItem_t * subItens = (HARQ16_MapaItem_t *)(((Pos_t)&hitem->Temp) + HARQ16_DESCRICAO_SUBITENS);
    for (Pos_t i = 0; i < HARQ16_DESCRICAO_SUBITENS_CAPACIDADE; i++)
    {
        subItens[i] = 0;
    }
    
    ret = _HARQ16_EscrevaTempAuto(hitem);
    if(ret != STATUS_OK) return ret;

    _HARQ16_FechaItem(hitem);
    // Grava indice
    ret = _HARQ16_LeiaMontado(montado, HARQ16_MONTADO_TIPO_TEMP, 0);
    if(ret != STATUS_OK) return ret;

    Mem_CopiaBinario((Byte_t *)(&montado->BlocoTemp) + HARQ16_INDICE_DESVIO, (Byte_t *) &montado->Indice, sizeof(HARQ16_Indice_t));

    ret = _HARQ16_EscrevaMontadoAuto(montado, HARQ16_MONTADO_TIPO_TEMP);
    if(ret != STATUS_OK) return ret;


    // Libera objeto montado temporario
    Mem_Local_Libera((Byte_t *)montado);
    return ret;
}

void HARQ16()
{
    Pos_t sisArq = 0;
    Unidade_RegistraSisArq("HARQ16", &sisArq, &HARQ16_Monta, &HARQ16_Valida, &HARQ16_Formata);
}