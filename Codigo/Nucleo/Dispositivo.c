#include <HUSIS.h>

#define DISPOSITIVO_CAPACIDADE 256

Dispositivo_t _Dispositivo_Lista[DISPOSITIVO_CAPACIDADE];

Status_t Dispositivo_Registra(Pos_t * novoDispositivo, Pos_t dispositivoAcima, SByte_t * constanteNome, Boleano_t adicionaNumero, UInt_t dispNumero, UInt_t id, Tam_t blocoTam, Tam_t (*acaoLeia)(void * disp, Pos_t posicao, Byte_t * destino, Tam_t tam), Tam_t (*acaoEscreva)(void * disp, Pos_t posicao, Byte_t * origem, Tam_t tam))
{
    Dispositivo_t * disp = 0;
    for (Pos_t i = 0; i < DISPOSITIVO_CAPACIDADE; i++)
    {
        if(!_Dispositivo_Lista[i].Existe)
        {
            disp = &_Dispositivo_Lista[i];
            break;
        }
    }
    if(dispNumero > 9) return STATUS_ESTOURO_DA_CAPACIDADE;
    if(disp == 0) return STATUS_ESTOURO_DA_CAPACIDADE;
    disp->Existe = SIM;
    Const_Copia(disp->NomeConst, constanteNome, DISPOSITIVO_NOME_TAM - 2);
    if(adicionaNumero)
    {
        Pos_t posNumero = Const_TamLimitado(disp->NomeConst, DISPOSITIVO_NOME_TAM);
        disp->NomeConst[posNumero] = dispNumero + '0';
        disp->NomeConst[posNumero+1] = 0;
    }
    disp->Identificador = id;
    disp->DispositivoNumero = dispNumero;
    disp->DispositivoAcima = dispositivoAcima;
    disp->TamanhoDeUmBloco = blocoTam;
    disp->AcaoLeia = acaoLeia;
    disp->AcaoEscreva = acaoEscreva;
    for (Pos_t i = 0; i < DISPOSITIVO_PORTAS_CAPACIDADE; i++)
    {
        disp->Porta[i] = 0;
    }
    disp->Ativo = SIM;
    *novoDispositivo = disp->Dispositivo;
    return STATUS_OK;
}

Status_t _Dispositivo_Valida(Pos_t dispositivo)
{
    if(dispositivo >= DISPOSITIVO_CAPACIDADE) return STATUS_DISPOSITIVO_INVALIDO;
    if(!_Dispositivo_Lista[dispositivo].Existe) return STATUS_DISPOSITIVO_INVALIDO;
    if(!_Dispositivo_Lista[dispositivo].Ativo) return STATUS_DISPOSITIVO_INVALIDO;
    return STATUS_OK;
}

Status_t Dispositivo_DefinePorta(Pos_t dispositivo, Pos_t aux, UShort_t porta)
{
    if(dispositivo >= DISPOSITIVO_CAPACIDADE) return STATUS_DISPOSITIVO_INVALIDO;
    if(aux >= DISPOSITIVO_PORTAS_CAPACIDADE) return STATUS_DISPOSITIVO_INVALIDO;
    if(!_Dispositivo_Lista[dispositivo].Existe) return STATUS_DISPOSITIVO_INVALIDO;
    if(!_Dispositivo_Lista[dispositivo].Ativo) return STATUS_DISPOSITIVO_INVALIDO;
    _Dispositivo_Lista[dispositivo].Porta[aux] = porta;
    return STATUS_OK;
}

Status_t Dispositivo_LeiaBytePorta(Pos_t dispositivo, Pos_t aux, Byte_t * valor)
{
    if(dispositivo >= DISPOSITIVO_CAPACIDADE) return STATUS_DISPOSITIVO_INVALIDO;
    if(aux >= DISPOSITIVO_PORTAS_CAPACIDADE) return STATUS_DISPOSITIVO_INVALIDO;
    if(!_Dispositivo_Lista[dispositivo].Existe) return STATUS_DISPOSITIVO_INVALIDO;
    if(!_Dispositivo_Lista[dispositivo].Ativo) return STATUS_DISPOSITIVO_INVALIDO;
    if(_Dispositivo_Lista[dispositivo].Porta[aux] == 0) return STATUS_DISPOSITIVO_INVALIDO;
    *valor = ES_LeiaByte(_Dispositivo_Lista[dispositivo].Porta[aux]);
    return STATUS_OK;
}

Status_t Dispositivo_EscrevaBytePorta(Pos_t dispositivo, Pos_t aux, Byte_t valor)
{
    if(dispositivo >= DISPOSITIVO_CAPACIDADE) return STATUS_DISPOSITIVO_INVALIDO;
    if(aux >= DISPOSITIVO_PORTAS_CAPACIDADE) return STATUS_DISPOSITIVO_INVALIDO;
    if(!_Dispositivo_Lista[dispositivo].Existe) return STATUS_DISPOSITIVO_INVALIDO;
    if(!_Dispositivo_Lista[dispositivo].Ativo) return STATUS_DISPOSITIVO_INVALIDO;
    if(_Dispositivo_Lista[dispositivo].Porta[aux] == 0) return STATUS_DISPOSITIVO_INVALIDO;
    ES_EscrevaByte(_Dispositivo_Lista[dispositivo].Porta[aux], valor);
    return STATUS_OK;
}

Status_t Dispositivo_LeiaUShortPorta(Pos_t dispositivo, Pos_t aux, UShort_t * valor)
{
    if(dispositivo >= DISPOSITIVO_CAPACIDADE) return STATUS_DISPOSITIVO_INVALIDO;
    if(aux >= DISPOSITIVO_PORTAS_CAPACIDADE) return STATUS_DISPOSITIVO_INVALIDO;
    if(!_Dispositivo_Lista[dispositivo].Existe) return STATUS_DISPOSITIVO_INVALIDO;
    if(!_Dispositivo_Lista[dispositivo].Ativo) return STATUS_DISPOSITIVO_INVALIDO;
    if(_Dispositivo_Lista[dispositivo].Porta[aux] == 0) return STATUS_DISPOSITIVO_INVALIDO;
    *valor = ES_LeiaUShort(_Dispositivo_Lista[dispositivo].Porta[aux]);
    return STATUS_OK;
}

Status_t Dispositivo_EscrevaUShortPorta(Pos_t dispositivo, Pos_t aux, UShort_t valor)
{
    if(dispositivo >= DISPOSITIVO_CAPACIDADE) return STATUS_DISPOSITIVO_INVALIDO;
    if(aux >= DISPOSITIVO_PORTAS_CAPACIDADE) return STATUS_DISPOSITIVO_INVALIDO;
    if(!_Dispositivo_Lista[dispositivo].Existe) return STATUS_DISPOSITIVO_INVALIDO;
    if(!_Dispositivo_Lista[dispositivo].Ativo) return STATUS_DISPOSITIVO_INVALIDO;
    if(_Dispositivo_Lista[dispositivo].Porta[aux] == 0) return STATUS_DISPOSITIVO_INVALIDO;
    ES_EscrevaUShort(_Dispositivo_Lista[dispositivo].Porta[aux], valor);
    return STATUS_OK;
}

Status_t _Dispositivo_Raiz(Pos_t unidade, Item_t * * raiz)
{
    *raiz = (Item_t *)Mem_Local_Aloca(sizeof(Item_t));
    Const_Copia((*raiz)->Nome, "Raiz", ITEM_NOME_TAM);
    (*raiz)->Identificador = 0;
    (*raiz)->PosicaoEspecial = 0;
    (*raiz)->Especial = ITEM_ESPECIAL_DISPOSITIVO;
    (*raiz)->Tipo = ITEM_TIPO_DIRETORIO;
    (*raiz)->PosicaoNoItem = 0;
    (*raiz)->Unidade = unidade;
    return STATUS_OK;
}

Status_t _Dispositivo_Desmonta(Pos_t unidade)
{
    return STATUS_OK;
}

Status_t _Dispositivo_ItemSubItem(Item_t * item, Pos_t posicao, Item_t * * subItem)
{
    Status_t ret = STATUS_OK;
    if(item->Especial != ITEM_ESPECIAL_DISPOSITIVO) return STATUS_TIPO_INVALIDO;
    if(item->PosicaoEspecial != 0) return STATUS_TIPO_INVALIDO;
    ret = _Dispositivo_Valida(posicao + 1);
    if(ret != STATUS_OK) return ret;
    Dispositivo_t * disp = &_Dispositivo_Lista[posicao + 1];
    *subItem = (Item_t *)Mem_Local_Aloca(sizeof(Item_t));
    Const_Copia((*subItem)->Nome, disp->NomeConst, ITEM_NOME_TAM > DISPOSITIVO_NOME_TAM ? DISPOSITIVO_NOME_TAM : ITEM_NOME_TAM);
    (*subItem)->Identificador = 0;
    (*subItem)->PosicaoEspecial = posicao + 1;
    (*subItem)->Especial = ITEM_ESPECIAL_DISPOSITIVO;
    (*subItem)->Tipo = ITEM_TIPO_ARQUIVO;
    (*subItem)->PosicaoNoItem = 0;
    (*subItem)->Unidade = item->Unidade;
    return ret;
}

Status_t _Dispositivo_ItemQtdSubItem(Item_t * item, Tam_t * qtd)
{
    Status_t ret = STATUS_OK;
    if(item->Especial != ITEM_ESPECIAL_DISPOSITIVO) return STATUS_TIPO_INVALIDO;
    if(item->PosicaoEspecial != 0) return STATUS_TIPO_INVALIDO;
    Tam_t qtdRet = 0;
    for (Pos_t i = 1; i < DISPOSITIVO_CAPACIDADE; i++)
    {
        if(_Dispositivo_Lista[i].Ativo & _Dispositivo_Lista[i].Existe)
        {
            qtdRet++;
        }
    }
    *qtd = qtdRet;
    return ret;
}

Status_t _Dispositivo_ItemVaPara(Item_t * item, Pos_t posicao)
{
    item->PosicaoNoItem = posicao;
    return STATUS_OK;
}

Status_t _Dispositivo_ItemLeia(Item_t * item, Byte_t * destino, Tam_t tam)
{
    Status_t ret = STATUS_OK;
    if(item->Especial != ITEM_ESPECIAL_DISPOSITIVO) return STATUS_TIPO_INVALIDO;
    if(item->PosicaoEspecial == 0) return STATUS_TIPO_INVALIDO;
    ret = _Dispositivo_Valida(item->PosicaoEspecial);
    if(ret != STATUS_OK) return ret;
    if(_Dispositivo_Lista[item->PosicaoEspecial].AcaoLeia == 0) return STATUS_DISPOSITIVO_INVALIDO;
    ret = _Dispositivo_Lista[item->PosicaoEspecial].AcaoLeia(&_Dispositivo_Lista[item->PosicaoEspecial], item->PosicaoNoItem, destino, tam);
    item->PosicaoNoItem += ret;
    return ret;
}

Status_t _Dispositivo_ItemEscreva(Item_t * item, Byte_t * origem, Tam_t tam)
{
    Status_t ret = STATUS_OK;
    if(item->Especial != ITEM_ESPECIAL_DISPOSITIVO) return STATUS_TIPO_INVALIDO;
    if(item->PosicaoEspecial == 0) return STATUS_TIPO_INVALIDO;
    ret = _Dispositivo_Valida(item->PosicaoEspecial);
    if(ret != STATUS_OK) return ret;
    if(_Dispositivo_Lista[item->PosicaoEspecial].AcaoLeia == 0) return STATUS_DISPOSITIVO_INVALIDO;
    ret = _Dispositivo_Lista[item->PosicaoEspecial].AcaoEscreva(&_Dispositivo_Lista[item->PosicaoEspecial], item->PosicaoNoItem, origem, tam);
    item->PosicaoNoItem += ret;
    return ret;
}

Status_t _Dispositivo_Exclui(Item_t * item)
{
    return STATUS_ACESSO_NEGADO;
}

Status_t _Dispositivo_Fecha(Item_t * item)
{
    return Mem_Local_Libera((Byte_t *)item);
}

Status_t _Dispositivo_CriaDiretorio(Item_t * item, SByte_t * constanteNome)
{
    return STATUS_ACESSO_NEGADO;
}

Status_t _Dispositivo_CriaArquivo(Item_t * item, SByte_t * constanteNome)
{
    return STATUS_ACESSO_NEGADO;
}

void Dispositivo()
{
    for (Tam_t i = 0; i < DISPOSITIVO_CAPACIDADE; i++)
    {
        _Dispositivo_Lista[i].Ativo = NAO;
        _Dispositivo_Lista[i].Existe = NAO;
    }
    Pos_t raiz = 0;
    Dispositivo_Registra(&raiz, 0, "Raiz", NAO, 0, 0, 0, 0, 0);
    Pos_t unidade = 0;
    Unidade_Registra("Dispositivos", &unidade, 0);
    Unidade_RegistraMontagem(unidade, &_Dispositivo_Raiz, &_Dispositivo_Desmonta, &_Dispositivo_ItemSubItem, &_Dispositivo_ItemQtdSubItem, &_Dispositivo_ItemVaPara, &_Dispositivo_ItemLeia, &_Dispositivo_ItemEscreva, &_Dispositivo_Exclui, &_Dispositivo_Fecha, &_Dispositivo_CriaDiretorio, &_Dispositivo_CriaArquivo);
}