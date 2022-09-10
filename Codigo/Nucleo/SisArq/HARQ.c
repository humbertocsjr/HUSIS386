#include <HUSIS.h>

Byte_t HARQ_ExibeMensagensNivel = 8;

#define HARQ_INDICE_DESVIO 440
#define HARQ_INDICE_NOME_TAM 20

typedef struct
{
    SByte_t Assinatura[4];
    Byte_t Bits;
    Byte_t Versao;
    UInt_t TotalBlocos;
    UInt_t TotalItens;
    UInt_t MapaInicio;
    UInt_t MapaTotalBlocos;
    UInt_t ItensInicio;
    UInt_t ItensTotalBlocos;
    UInt_t AlteracoesInicio;
    UInt_t AlteracoesTotalBlocos;
    UInt_t Inicio;
    Byte_t Nome[HARQ_INDICE_NOME_TAM];
    Byte_t Zero;
} HARQ_Indice_t;

typedef struct
{
    Byte_t ProximoBaixo;
    Byte_t ProximoMedio;
    Byte_t ProximoAlto;
} HARQ_0_24_MapaRegistro_t;

typedef struct
{
    Byte_t Baixo;
    Byte_t Medio;
    Byte_t Alto;
} HARQ_0_24_Ponteiro_t;

#define HARQ_0_24_MAPA_REGS_POR_BLOCO 340

typedef struct
{
    UShort_t Qtd;
    UShort_t QtdLivre;
    HARQ_0_24_MapaRegistro_t Regs[HARQ_0_24_MAPA_REGS_POR_BLOCO];
} HARQ_0_24_MapaBloco_t;

#define HARQ_TIPO_VAZIO 0
#define HARQ_TIPO_ARQUIVO 0x10
#define HARQ_TIPO_DIRETORIO 0x20
#define HARQ_TIPO_RESERVADO 0xFF


typedef struct
{
    UShort_t Ano;
    UShort_t MesDiaHora;
    UShort_t MinSeg;
} HARQ_Data_t;

typedef struct
{
    Byte_t UsuarioLeitura;
    Byte_t UsuarioGravacao : 1;
    Byte_t GrupoLeitura : 2;
    Byte_t GrupoGravacao : 3;
    Byte_t OutrosLeitura : 4;
    Byte_t OutrosGravacao : 5;
    Byte_t Executavel : 6;
    Byte_t Oculto : 7;
} HARQ_Permissao_t;

typedef struct
{
    Byte_t Tipo;
    HARQ_Permissao_t Permissao;
    Byte_t Ligacoes;
    HARQ_Data_t Criacao;
    HARQ_Data_t Alteracao;
    UShort_t Usuario;
    UShort_t Grupo;
    HARQ_0_24_Ponteiro_t PrimeiroBloco;
    UInt_t Tamanho;
    UShort_t TamUltimoBloco;
    Byte_t Reservado[4];
} HARQ_0_24_Item_t;

#define HARQ_0_24_ITENS_POR_BLOCO 32

typedef struct
{
    HARQ_0_24_Item_t Itens[HARQ_0_24_ITENS_POR_BLOCO];
} HARQ_0_24_ListaItemBloco_t;

#define HARQ_ALTERACAO_TIPO_VAZIO 0
#define HARQ_ALTERACAO_TIPO_INDICE 1
#define HARQ_ALTERACAO_TIPO_CRIACAO 2
#define HARQ_ALTERACAO_TIPO_ALTERACAO 3
#define HARQ_ALTERACAO_TIPO_EXCLUSAO 4
#define HARQ_ALTERACAO_TIPO_INDISPONIVEL 0xff

typedef struct
{
    Byte_t Tipo;
    Byte_t Posicao;
    UInt_t Alteracao;
    UInt_t Bloco;
    UInt_t Item;
    UShort_t Reservado;
} HARQ_0_Alteracao_t;

#define HARQ_0_ALTERACOES_POR_BLOCO 63

#define HARQ_0_STATUS_DESMONTADO 0
#define HARQ_0_STATUS_MONTADO 1

typedef struct
{
    HARQ_0_Alteracao_t Alteracoes[HARQ_0_ALTERACOES_POR_BLOCO];
    Byte_t Status;
    Byte_t Capacidade;
} HARQ_0_AlteracaoBloco_t;


typedef struct
{
    HARQ_Indice_t Indice;
    Pos_t BlocoTempPos;
    Byte_t BlocoTemp[1024];
    Pos_t BlocoListaItemPos;
    Byte_t BlocoListaItem[1024];
    Pos_t BlocoMapaPos;
    Byte_t BlocoMapa[1024];
    Pos_t BlocoAlteracaoPos;
    Byte_t BlocoAlteracao[1024];
    Tam_t QtdItensAbertos;
    Pos_t Dispositivo;
    Pos_t Unidade;
} HARQ_Montado_t;

#define HARQ_0_NOME_TAM 60
#define HARQ_0_REGISTROS_POR_BLOCO_DIR 16

typedef struct
{
    UInt_t Item;
    SByte_t Nome[HARQ_0_NOME_TAM];
} HARQ_0_DiretorioRegistro_t;

typedef struct __attribute__ ((__packed__))
{
    Item_t Item;
    Pos_t TempPos;
    Byte_t Temp[1024];
    HARQ_Montado_t * Montado;
} HARQ_SisArq_Item_t;

Status_t _HARQ_LeiaBloco1024(Pos_t disp, Pos_t posicao, Byte_t * destino)
{
    if(HARQ_ExibeMensagensNivel > 9) Mensagem2("HARQ", "Inicio da Leitura do Bloco {1:u} no Dispositivo {0:u}", disp, posicao);
    Tam_t tam = Dispositivo_Leia(disp, posicao << 1, destino, 512);
    if(tam == 0) return STATUS_NAO_FOI_POSSIVEL_LER;
    tam = Dispositivo_Leia(disp, (posicao << 1) + 1, destino + 512, 512);
    if(tam == 0) return STATUS_NAO_FOI_POSSIVEL_LER;
    if(HARQ_ExibeMensagensNivel > 8) Mensagem2("HARQ", "Fim da Leitura do Bloco {1:u} no Dispositivo {0:u}", disp, posicao);
    return STATUS_OK;
}

Status_t _HARQ_EscrevaBloco1024(Pos_t disp, Pos_t posicao, Byte_t * origem)
{
    if(HARQ_ExibeMensagensNivel > 9) Mensagem2("HARQ", "Inicio da Escrita do Bloco {1:u} no Dispositivo {0:u}", disp, posicao);
    Tam_t tam = Dispositivo_Escreva(disp, posicao << 1, origem, 512);
    if(tam == 0) return STATUS_NAO_FOI_POSSIVEL_ESCREVER;
    tam = Dispositivo_Escreva(disp, (posicao << 1) + 1, origem + 512, 512);
    if(tam == 0) return STATUS_NAO_FOI_POSSIVEL_ESCREVER;
    if(HARQ_ExibeMensagensNivel > 8) Mensagem2("HARQ", "Fim da Escrita do Bloco {1:u} no Dispositivo {0:u}", disp, posicao);
    return STATUS_OK;
}

void _HARQ_Hoje(HARQ_Data_t * data)
{
    data->Ano = 0;
    data->MesDiaHora = 0;
    data->MinSeg = 0;
}

void _HARQ_PermissaoPadrao(HARQ_Permissao_t * perm)
{
    perm->Executavel = 0;
    perm->GrupoGravacao = 1;
    perm->GrupoLeitura = 1;
    perm->UsuarioGravacao = 1;
    perm->UsuarioLeitura = 1;
    perm->OutrosGravacao = 1;
    perm->OutrosLeitura = 1;
    perm->Oculto = 0;
}

Status_t _HARQ_GeraMapa(Pos_t disp, HARQ_Indice_t * indice)
{
    Pos_t posBloco = indice->MapaInicio;
    Byte_t * bloco = Mem_Local_Aloca(1024);
    Pos_t pos = 0;
    Status_t ret = STATUS_OK;
    for (Pos_t i = 0; i < (indice->MapaTotalBlocos); i++)
    {
        HARQ_0_24_MapaBloco_t * dados = (HARQ_0_24_MapaBloco_t *)bloco;
        pos = i * HARQ_0_24_MAPA_REGS_POR_BLOCO;
        dados->Qtd = HARQ_0_24_MAPA_REGS_POR_BLOCO;
        dados->QtdLivre = HARQ_0_24_MAPA_REGS_POR_BLOCO;
        if((pos + HARQ_0_24_MAPA_REGS_POR_BLOCO) >= indice->TotalBlocos)
        {
            dados->Qtd = (indice->TotalBlocos - pos);
            dados->QtdLivre = dados->Qtd;
        }
        for (Pos_t j = 0; j < HARQ_0_24_MAPA_REGS_POR_BLOCO; j++)
        {
            if((j >= dados->Qtd) | ((pos + j) < indice->Inicio))
            {
                dados->Regs[j].ProximoBaixo = 0xff;
                dados->Regs[j].ProximoMedio = 0xff;
                dados->Regs[j].ProximoAlto = 0xff;
            }
            else
            {
                dados->Regs[j].ProximoBaixo = 0;
                dados->Regs[j].ProximoMedio = 0;
                dados->Regs[j].ProximoAlto = 0;
            }
        }
        ret = _HARQ_EscrevaBloco1024(disp, posBloco, bloco);
        posBloco++;
        if(ret != STATUS_OK) break;
    }
    Mem_Local_Libera(bloco);
    return ret;
}

Status_t _HARQ_GeraListaItens(Pos_t disp, HARQ_Indice_t * indice)
{
    Pos_t posBloco = indice->ItensInicio;
    Byte_t * bloco = Mem_Local_Aloca(1024);
    Status_t ret = STATUS_OK;
    for (Pos_t i = 0; i < (indice->ItensTotalBlocos); i++)
    {
        HARQ_0_24_ListaItemBloco_t * dados = (HARQ_0_24_ListaItemBloco_t *)bloco;
        for (Pos_t j = 0; j < HARQ_0_24_ITENS_POR_BLOCO; j++)
        {
            Pos_t pos = i * HARQ_0_24_ITENS_POR_BLOCO + j;
            if(pos == 0)
            {
                dados->Itens[j].Tipo = HARQ_TIPO_RESERVADO;
            }
            else if(pos == 0)
            {
                dados->Itens[j].Tipo = HARQ_TIPO_DIRETORIO;
                dados->Itens[j].Usuario = 0;
                dados->Itens[j].Grupo = 0;
                _HARQ_PermissaoPadrao(&dados->Itens[j].Permissao);
                _HARQ_Hoje(&dados->Itens[j].Criacao);
                _HARQ_Hoje(&dados->Itens[j].Alteracao);
                dados->Itens[j].Tamanho = 0;
                dados->Itens[j].PrimeiroBloco.Baixo = 0;
                dados->Itens[j].PrimeiroBloco.Medio = 0;
                dados->Itens[j].PrimeiroBloco.Alto = 0;
                dados->Itens[j].TamUltimoBloco = 0;
                dados->Itens[j].Ligacoes = 0;
            }
            else
            {
                dados->Itens[j].Tipo = HARQ_TIPO_VAZIO;
                dados->Itens[j].Tamanho = 0;
                dados->Itens[j].TamUltimoBloco = 0;
                dados->Itens[j].Ligacoes = 0;
            }
        }
        ret = _HARQ_EscrevaBloco1024(disp, posBloco, bloco);
        posBloco++;
        if(ret != STATUS_OK) break;
    }
    Mem_Local_Libera(bloco);
    return ret;
}

Status_t _HARQ_GeraAlteracoes(Pos_t disp, HARQ_Indice_t * indice)
{
    Byte_t * bloco = Mem_Local_Aloca(1024);
    Status_t ret = STATUS_OK;
    HARQ_0_AlteracaoBloco_t * dados = (HARQ_0_AlteracaoBloco_t *)bloco;
    for (Pos_t j = 0; j < HARQ_0_ALTERACOES_POR_BLOCO; j++)
    {
        dados->Alteracoes[j].Tipo = (j < (indice->AlteracoesTotalBlocos - 2)) ? HARQ_ALTERACAO_TIPO_VAZIO : HARQ_ALTERACAO_TIPO_INDISPONIVEL;
    }
    dados->Capacidade = indice->AlteracoesTotalBlocos - 2;
    dados->Status = HARQ_0_STATUS_DESMONTADO;
    ret = _HARQ_EscrevaBloco1024(disp, indice->AlteracoesInicio, bloco);
    ret = _HARQ_EscrevaBloco1024(disp, indice->AlteracoesInicio+1, bloco);
    Mem_Local_Libera(bloco);
    return ret;
}

Status_t HARQ_Formata(Pos_t unidade)
{
    Tam_t tam = 0;
    Pos_t disp = 0;
    Status_t ret = Unidade_LeiaTamanhoDeUmBloco(unidade, &tam);
    if(ret != STATUS_OK) return ret;
    if(tam != 512) return STATUS_DISPOSITIVO_INVALIDO;
    Byte_t * bloco = Mem_Local_Aloca(512);
    ret = Unidade_Dispositivo(unidade, &disp);
    if(ret == STATUS_OK)
    {
        tam = Dispositivo_Leia(disp, 0, bloco, 512);
        if(tam == 0)
        {
            ret = STATUS_NAO_FOI_POSSIVEL_LER;
        } 
        else
        {
            HARQ_Indice_t * indice = (HARQ_Indice_t *)(bloco + HARQ_INDICE_DESVIO);
            Mem_CopiaBinario((Byte_t *)indice->Assinatura, (Byte_t *)"HARQ", 4);
            indice->Bits = 24;
            indice->Versao = 0;
            Dispositivo_LeiaTamanho(disp, &indice->TotalBlocos);
            indice->MapaInicio = 8;
            indice->MapaTotalBlocos = indice->TotalBlocos / HARQ_0_24_MAPA_REGS_POR_BLOCO;
            if((indice->TotalBlocos % HARQ_0_24_MAPA_REGS_POR_BLOCO) != 0)
            {
                indice->MapaTotalBlocos++;
            }
            indice->ItensInicio = indice->MapaInicio + indice->MapaTotalBlocos;
            indice->ItensTotalBlocos = indice->TotalBlocos / 4 / HARQ_0_24_ITENS_POR_BLOCO;
            if(indice->ItensTotalBlocos < 4) indice->ItensTotalBlocos = 4;
            indice->TotalItens = indice->ItensTotalBlocos * HARQ_0_24_ITENS_POR_BLOCO;
            indice->AlteracoesInicio = indice->ItensInicio + indice->ItensTotalBlocos;
            indice->AlteracoesTotalBlocos = indice->ItensTotalBlocos / 8;
            if(indice->AlteracoesTotalBlocos > HARQ_0_ALTERACOES_POR_BLOCO) indice->AlteracoesTotalBlocos = HARQ_0_ALTERACOES_POR_BLOCO + 1;
            if(indice->AlteracoesTotalBlocos < 6) indice->AlteracoesTotalBlocos = 6;
            indice->Inicio = indice->AlteracoesInicio + indice->AlteracoesTotalBlocos;
            Unidade_LeiaNomeConst(unidade, (SByte_t *)indice->Nome, HARQ_INDICE_NOME_TAM);
            indice->Zero = 0;
            tam = Dispositivo_Escreva(disp, 0, bloco, 512);
            if(tam == 0) ret = STATUS_NAO_FOI_POSSIVEL_ESCREVER;
            if(HARQ_ExibeMensagensNivel > 0) Mensagem5("HARQ", "Formatacao do Dispositivo {0:u}:\n {1:u} Blocos\n {2:u} Itens\n {3:u} Alteracoes registraveis\n {4:u} Blocos reservados", disp, indice->TotalBlocos, indice->TotalItens, indice->AlteracoesTotalBlocos - 1, indice->Inicio - 1);
            if(HARQ_ExibeMensagensNivel > 1) Mensagem7("HARQ", "Formatacao do Dispositivo [Informacao Extra] {0:u}:\n {1:u} Blocos Mapa\n {2:u} Blocos Itens\n {3:u} Inicio Mapa\n {4:u} Inicio Itens\n {5:u} Inicio Alteracoes\n {6:u} Inicio ", disp, indice->MapaTotalBlocos, indice->ItensTotalBlocos, indice->MapaInicio, indice->ItensInicio, indice->AlteracoesInicio, indice->Inicio);
            ret = _HARQ_GeraMapa(disp, indice);
            ret = _HARQ_GeraListaItens(disp, indice);
            ret = _HARQ_GeraAlteracoes(disp, indice);
        }
    }
    Mem_Local_Libera(bloco);
    return ret;
}

Status_t _HARQ_LeiaMontadoMapa(HARQ_Montado_t * montado, Pos_t bloco)
{
    if(montado->BlocoMapaPos == bloco) return STATUS_OK;
    montado->BlocoMapaPos = bloco;
    return _HARQ_LeiaBloco1024(montado->Dispositivo, bloco, montado->BlocoMapa);
}

Status_t _HARQ_EscrevaMontadoMapa(HARQ_Montado_t * montado, Pos_t bloco)
{
    montado->BlocoMapaPos = bloco;
    return _HARQ_EscrevaBloco1024(montado->Dispositivo, bloco, montado->BlocoMapa);
}

Status_t _HARQ_LeiaMontadoListaItem(HARQ_Montado_t * montado, Pos_t bloco)
{
    if(montado->BlocoListaItemPos == bloco) return STATUS_OK;
    montado->BlocoListaItemPos = bloco;
    return _HARQ_LeiaBloco1024(montado->Dispositivo, bloco, montado->BlocoListaItem);
}

Status_t _HARQ_EscrevaMontadoListaItem(HARQ_Montado_t * montado, Pos_t bloco)
{
    montado->BlocoListaItemPos = bloco;
    return _HARQ_EscrevaBloco1024(montado->Dispositivo, bloco, montado->BlocoListaItem);
}

Status_t _HARQ_LeiaMontadoTemp(HARQ_Montado_t * montado, Pos_t bloco)
{
    if(montado->BlocoTempPos == bloco) return STATUS_OK;
    montado->BlocoTempPos = bloco;
    return _HARQ_LeiaBloco1024(montado->Dispositivo, bloco, montado->BlocoTemp);
}

Status_t _HARQ_EscrevaMontadoTemp(HARQ_Montado_t * montado, Pos_t bloco)
{
    montado->BlocoTempPos = bloco;
    return _HARQ_EscrevaBloco1024(montado->Dispositivo, bloco, montado->BlocoTemp);
}

Status_t _HARQ_LeiaItemTemp(HARQ_SisArq_Item_t * item, Pos_t bloco)
{
    if(item->TempPos == bloco) return STATUS_OK;
    item->TempPos = bloco;
    return _HARQ_LeiaBloco1024(item->Montado->Dispositivo, bloco, item->Temp);
}

Status_t _HARQ_EscrevaItemTemp(HARQ_SisArq_Item_t * item, Pos_t bloco)
{
    item->TempPos = bloco;
    return _HARQ_EscrevaBloco1024(item->Montado->Dispositivo, bloco, item->Temp);
}

Status_t _HARQ_0_24_LeiaListaItem(Item_t * item, HARQ_SisArq_Item_t * hitem, HARQ_0_24_Item_t * * litem)
{
    Status_t ret = _HARQ_LeiaMontadoListaItem(hitem->Montado, item->Identificador / HARQ_0_24_ITENS_POR_BLOCO + hitem->Montado->Indice.ItensInicio);
    if(ret != STATUS_OK) return ret;
    HARQ_0_24_ListaItemBloco_t * listaItem = (HARQ_0_24_ListaItemBloco_t *)hitem->Montado->BlocoListaItem;
    *litem = &listaItem->Itens[item->Identificador % HARQ_0_24_ITENS_POR_BLOCO];
    return ret;
}

Status_t _HARQ_0_24_EscrevaListaItemAtual(Item_t * item, HARQ_SisArq_Item_t * hitem)
{
    if((item->Identificador / HARQ_0_24_ITENS_POR_BLOCO + hitem->Montado->Indice.ItensInicio) != hitem->Montado->BlocoListaItemPos)
    {
        return STATUS_POSICAO_INVALIDA;
    }
    Status_t ret = _HARQ_EscrevaMontadoListaItem(hitem->Montado, item->Identificador / HARQ_0_24_ITENS_POR_BLOCO + hitem->Montado->Indice.ItensInicio);
    return ret;
}

Status_t _HARQ_Raiz(Pos_t unidade, Item_t * * raiz)
{
    HARQ_Montado_t * montado = 0;
    Status_t ret = Unidade_CarregaObjetoMontado(unidade, (Byte_t * *)&montado);
    if (ret != STATUS_OK) return ret;
    *raiz = (Item_t *)Mem_Local_Aloca(sizeof(HARQ_SisArq_Item_t));
    Unidade_LeiaNomeConst(unidade, (*raiz)->Nome, ITEM_NOME_TAM);
    (*raiz)->Identificador = 1;
    (*raiz)->Especial = ITEM_ESPECIAL_NENHUM;
    (*raiz)->Tipo = ITEM_TIPO_DIRETORIO;
    (*raiz)->Unidade = unidade;
    ((HARQ_SisArq_Item_t *)*raiz)->TempPos = 0;
    ((HARQ_SisArq_Item_t *)*raiz)->Montado = montado;
    return STATUS_OK;
}

Status_t _HARQ_Desmonta(Pos_t unidade)
{
    HARQ_Montado_t * montado = 0;
    Status_t ret = Unidade_CarregaObjetoMontado(unidade, (Byte_t * * )&montado);
    if(ret != STATUS_OK) return ret;
    if(montado->QtdItensAbertos >= 0) return STATUS_ACESSO_NEGADO;
    return STATUS_OK;
}
Status_t _HARQ_ItemSubItem(Item_t * item, Pos_t posicao, Item_t * * subItem)
{
    return STATUS_NAO_IMPLEMENTADO;
}

Status_t _HARQ_ItemQtdSubItem(Item_t * item, Tam_t * qtd)
{
    HARQ_SisArq_Item_t * hitem = (HARQ_SisArq_Item_t *)item;
    HARQ_0_24_Item_t * litem = 0;
    Status_t ret = _HARQ_0_24_LeiaListaItem(item, hitem, &litem);
    if(ret != STATUS_OK) return ret;
    *qtd = litem->Tamanho;
    return STATUS_OK;
}

Status_t _HARQ_ItemVaPara(Item_t * item, Pos_t posicao)
{
    item->PosicaoNoItem = posicao;
    return STATUS_OK;
}

Tam_t _HARQ_ItemLeia(Item_t * item, Byte_t * destino, Tam_t tam)
{
    return STATUS_NAO_IMPLEMENTADO;
}

Tam_t _HARQ_ItemEscreva(Item_t * item, Byte_t * origem, Tam_t tam)
{
    return STATUS_NAO_IMPLEMENTADO;
}

Status_t _HARQ_ItemExclui(Item_t * item)
{
    return STATUS_NAO_IMPLEMENTADO;
}

Status_t _HARQ_ItemFecha(Item_t * item)
{
    return Mem_Local_Libera((Byte_t *)item);
}

Status_t _HARQ_0_24_ReservaBloco(HARQ_SisArq_Item_t * hitem, HARQ_0_24_Ponteiro_t * ptr)
{
    Boleano_t encontrado = NAO;
    HARQ_0_24_MapaBloco_t * blocoMapa = 0;
    Pos_t blocoMapaPosDisco = 0;
    Pos_t blocoMapaPos = 0;
    Pos_t blocoPos = 0;
    for (Pos_t i = 0; i < hitem->Montado->Indice.MapaTotalBlocos; i++)
    {
        blocoMapaPosDisco = i + hitem->Montado->Indice.MapaInicio;
        _HARQ_LeiaMontadoMapa(hitem->Montado, blocoMapaPosDisco);
        blocoMapa = (HARQ_0_24_MapaBloco_t *)&hitem->Montado->BlocoMapa;
        if(blocoMapa->QtdLivre > 0)
        {
            for (Pos_t j = 0; j < HARQ_0_24_MAPA_REGS_POR_BLOCO; j++)
            {
                if((blocoMapa->Regs[j].ProximoBaixo == 0) & (blocoMapa->Regs[j].ProximoMedio == 0) & (blocoMapa->Regs[j].ProximoAlto == 0))
                {
                    encontrado = SIM;
                    blocoMapaPos = j;
                    blocoPos = i * HARQ_0_24_MAPA_REGS_POR_BLOCO + j;
                    break;
                }
            }
        }
        if(encontrado) break;;
    }
    if(!encontrado)
    {
        return STATUS_ESTOURO_DA_CAPACIDADE;
    }
    blocoMapa->Regs[blocoMapaPos].ProximoBaixo = 0xfe;
    blocoMapa->Regs[blocoMapaPos].ProximoMedio = 0xff;
    blocoMapa->Regs[blocoMapaPos].ProximoAlto = 0xff;
    ptr->Baixo = blocoPos * 0xff;
    ptr->Medio = (blocoPos >> 8) * 0xff;
    ptr->Alto = (blocoPos >> 16) * 0xff;
    return _HARQ_EscrevaMontadoMapa(hitem->Montado, blocoMapaPosDisco);
}


Status_t _HARQ_0_24_ReservaItem(HARQ_SisArq_Item_t * hitem, Pos_t * item, Byte_t tipo)
{
    Boleano_t encontrado = NAO;
    HARQ_0_24_ListaItemBloco_t * blocoLista = 0;
    Pos_t blocoListaPosDisco = 0;
    Pos_t blocoListaPos = 0;
    Pos_t blocoPos = 0;
    for (Pos_t i = 0; i < hitem->Montado->Indice.ItensTotalBlocos; i++)
    {
        blocoListaPosDisco = i + hitem->Montado->Indice.MapaInicio;
        _HARQ_LeiaMontadoListaItem(hitem->Montado, blocoListaPosDisco);
        blocoLista = (HARQ_0_24_ListaItemBloco_t *)&hitem->Montado->BlocoListaItem;
        for (Pos_t j = 0; j < HARQ_0_24_ITENS_POR_BLOCO; j++)
        {
            if(blocoLista->Itens[j].Tipo == HARQ_TIPO_VAZIO)
            {
                encontrado = SIM;
                blocoListaPos = j;
                blocoPos = i * HARQ_0_24_ITENS_POR_BLOCO + j;
                break;
            }
        }
        if(encontrado) break;;
    }
    if(!encontrado)
    {
        return STATUS_ESTOURO_DA_CAPACIDADE;
    }
    blocoLista->Itens[blocoListaPos].Tipo = tipo;
    blocoLista->Itens[blocoListaPos].Tamanho = 0;
    blocoLista->Itens[blocoListaPos].TamUltimoBloco = 0;
    blocoLista->Itens[blocoListaPos].Usuario = 0;
    blocoLista->Itens[blocoListaPos].Grupo = 0;
    _HARQ_PermissaoPadrao(&blocoLista->Itens[blocoListaPos].Permissao);
    _HARQ_Hoje(&blocoLista->Itens[blocoListaPos].Criacao);
    _HARQ_Hoje(&blocoLista->Itens[blocoListaPos].Alteracao);
    blocoLista->Itens[blocoListaPos].Ligacoes = 0;
    *item = blocoPos;
    return _HARQ_EscrevaMontadoListaItem(hitem->Montado, blocoListaPosDisco);
}

Pos_t _HARQ_0_24_ConverteParaPos(HARQ_0_24_Ponteiro_t * ptr)
{
    return ptr->Baixo | (ptr->Medio << 8) | (ptr->Alto << 16);
}

Status_t _HARQ_ItemCriaDiretorio(Item_t * item, SByte_t * constanteNome)
{
    return STATUS_NAO_IMPLEMENTADO;
}

Status_t _HARQ_ItemCriaArquivo(Item_t * item, SByte_t * constanteNome)
{
    if(HARQ_ExibeMensagensNivel > 1) Mensagem("HARQ", "Criando arquivo '{0:C}'", (Tam_t)constanteNome);
    HARQ_SisArq_Item_t * hitem = (HARQ_SisArq_Item_t *)item;
    HARQ_0_24_Item_t * litem = 0;
    Pos_t sub = 0;
    HARQ_0_DiretorioRegistro_t * lista = 0;
    Pos_t listaPos = 0;
    Status_t ret = _HARQ_0_24_ReservaItem(hitem, &sub, HARQ_TIPO_ARQUIVO);
    if(ret != STATUS_OK) return ret;
    ret = _HARQ_0_24_LeiaListaItem(item, hitem, &litem);
    if(ret != STATUS_OK) return ret;
    if((litem->PrimeiroBloco.Baixo == 0) & (litem->PrimeiroBloco.Medio == 0) & (litem->PrimeiroBloco.Alto == 0))
    {
        ret = _HARQ_0_24_ReservaBloco(hitem, &litem->PrimeiroBloco);
        if(ret != STATUS_OK) return ret;
        ret = _HARQ_LeiaItemTemp(hitem, _HARQ_0_24_ConverteParaPos(&litem->PrimeiroBloco));
        if(ret != STATUS_OK) return ret;
        lista = (HARQ_0_DiretorioRegistro_t *)&hitem->Temp;
        for (Pos_t i = 0; i < HARQ_0_REGISTROS_POR_BLOCO_DIR; i++)
        {
            lista[i].Item = 0;
            lista[i].Nome[0] = 0;
        }
        listaPos = 0;
    }
    else
    {
        ret = _HARQ_LeiaItemTemp(hitem, _HARQ_0_24_ConverteParaPos(&litem->PrimeiroBloco));
        if(ret != STATUS_OK) return ret;
        return STATUS_NAO_IMPLEMENTADO;
    }
    lista[listaPos].Item = sub;
    Const_Copia(lista[listaPos].Nome, constanteNome, HARQ_0_NOME_TAM);
    ret = _HARQ_EscrevaItemTemp(hitem, hitem->TempPos);
    if(ret != STATUS_OK) return ret;
    ret = _HARQ_0_24_EscrevaListaItemAtual(item, hitem);
    return ret;
}

Status_t HARQ_Monta(Pos_t unidade)
{
    Tam_t tam = 0;
    Pos_t disp = 0;
    Status_t ret = Unidade_LeiaTamanhoDeUmBloco(unidade, &tam);
    if(ret != STATUS_OK) return ret;
    if(tam != 512) return STATUS_DISPOSITIVO_INVALIDO;
    Byte_t * bloco = Mem_Local_Aloca(512);
    ret = Unidade_Dispositivo(unidade, &disp);
    if(ret != STATUS_OK) return ret;
    tam = Dispositivo_Leia(disp, 0, bloco, 512);
    if(tam == 0)
    {
        ret = STATUS_NAO_FOI_POSSIVEL_LER;
    }
    else
    { 
        HARQ_Indice_t * indice = (HARQ_Indice_t *)(bloco + HARQ_INDICE_DESVIO);
        if(Const_Igual(indice->Assinatura, "HARQ", 4) & (indice->Bits == 24) & (indice->Versao == 0))
        {
            ret = Unidade_RegistraMontagem(unidade, sizeof(HARQ_Montado_t), &_HARQ_Raiz, &_HARQ_Desmonta, &_HARQ_ItemSubItem, &_HARQ_ItemQtdSubItem, &_HARQ_ItemVaPara, &_HARQ_ItemLeia, &_HARQ_ItemEscreva, &_HARQ_ItemExclui, &_HARQ_ItemFecha, &_HARQ_ItemCriaDiretorio, &_HARQ_ItemCriaArquivo);
            HARQ_Montado_t * montado = 0;
            ret = Unidade_CarregaObjetoMontado(unidade, (Byte_t * *)&montado);
            Mem_CopiaBinario((Byte_t *)(&montado->Indice), (Byte_t *)indice, sizeof(HARQ_Indice_t));
            montado->BlocoListaItemPos = 0;
            montado->BlocoAlteracaoPos = 0;
            montado->BlocoMapaPos = 0;
            montado->QtdItensAbertos = 0;
            montado->Dispositivo = disp;
            montado->Unidade = unidade;
        }
        else
        {
            MensagemConst("HARQ", "Assinatura: ", indice->Assinatura, 4);
            ret = STATUS_FORMATO_INVALIDO;
        }
    }
    Mem_Local_Libera(bloco);
    return ret;
}

Status_t HARQ_Valida(Pos_t unidade)
{
    Tam_t tam = 0;
    Pos_t disp = 0;
    Status_t ret = Unidade_LeiaTamanhoDeUmBloco(unidade, &tam);
    if(ret != STATUS_OK) return ret;
    if(tam != 512) return STATUS_DISPOSITIVO_INVALIDO;
    Byte_t * bloco = Mem_Local_Aloca(512);
    ret = Unidade_Dispositivo(unidade, &disp);
    if(ret != STATUS_OK) return ret;
    tam = Dispositivo_Leia(disp, 0, bloco, 512);
    if(tam == 0)
    {
        ret = STATUS_NAO_FOI_POSSIVEL_LER;
    }
    else
    { 
        HARQ_Indice_t * indice = (HARQ_Indice_t *)(bloco + HARQ_INDICE_DESVIO);
        if(Const_Igual(indice->Assinatura, "HARQ", 4) & (indice->Bits == 24) & (indice->Versao == 0))
        {
            ret = STATUS_OK;
        }
        else
        {
            MensagemConst("HARQ", "Assinatura: ", indice->Assinatura, 4);
            ret = STATUS_FORMATO_INVALIDO;
        }
    }
    Mem_Local_Libera(bloco);
    return ret;
}

void HARQ()
{
    Pos_t sisarq = 0;
    Unidade_RegistraSisArq("HARQ", &sisarq, &HARQ_Monta, &HARQ_Valida, &HARQ_Formata);
}
