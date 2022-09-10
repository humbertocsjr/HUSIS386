#include <HUSIS.h>



#define SISTEMA_DE_ARQUIVOS_NOME_TAM 32

typedef struct
{
    Pos_t SistemaDeArquivos;
    Boleano_t Existe;
    SByte_t NomeConst[SISTEMA_DE_ARQUIVOS_NOME_TAM];
    Status_t (*AcaoValida)(Pos_t unidade);
    Status_t (*AcaoMonta)(Pos_t unidade);
    Status_t (*AcaoFormata)(Pos_t unidade);
} SistemaDeArquivos_t;

#define UNIDADE_NOME_TAM 64

typedef struct
{
    Pos_t Unidade;
    Pos_t Dispositivo;
    Pos_t SistemaDeArquivos;
    Boleano_t Existe;
    Boleano_t Montado;
    SByte_t NomeConst[UNIDADE_NOME_TAM];
    Byte_t * ObjetoMontado;
    Status_t (*AcaoRaiz)(Pos_t unidade, Item_t * * raiz);
    Status_t (*AcaoDesmonta)(Pos_t unidade);
    Status_t (*AcaoItemSubItem)(Item_t * item, Pos_t posicao, Item_t * * subItem);
    Status_t (*AcaoItemQtdSubItem)(Item_t * item, Tam_t * qtd);
    Status_t (*AcaoItemVaPara)(Item_t * item, Pos_t posicao);
    Tam_t (*AcaoItemLeia)(Item_t * item, Byte_t * destino, Tam_t tam);
    Tam_t (*AcaoItemEscreva)(Item_t * item, Byte_t * origem, Tam_t tam);
    Status_t (*AcaoItemExclui)(Item_t * item);
    Status_t (*AcaoItemFecha)(Item_t * item);
    Status_t (*AcaoItemCriaDiretorio)(Item_t * item, SByte_t * constanteNome);
    Status_t (*AcaoItemCriaArquivo)(Item_t * item, SByte_t * constanteNome);
} Unidade_t;

#define UNIDADE_CAPACIDADE 64

Unidade_t _Unidade_Lista[UNIDADE_CAPACIDADE];
Unidade_t * _Unidade_Principal = 0;

#define SISTEMA_DE_ARQUIVOS_CAPACIDADE 32

SistemaDeArquivos_t _Unidade_SisArq[SISTEMA_DE_ARQUIVOS_CAPACIDADE];


Status_t Unidade_DefinePrincipal(Pos_t unidade)
{
    if(unidade >= UNIDADE_CAPACIDADE) return STATUS_POSICAO_INVALIDA;
    if((!_Unidade_Lista[unidade].Montado) | (!_Unidade_Lista[unidade].Existe)) return STATUS_UNIDADE_INVALIDA;
    _Unidade_Principal = &_Unidade_Lista[unidade];
    return STATUS_OK;
}

Status_t Unidade_LeiaPrincipal(Pos_t * unidade)
{
    if(_Unidade_Principal == 0) return STATUS_NAO_ENCONTRADO;
    *unidade = _Unidade_Principal->Unidade;
    return STATUS_OK;
}

Status_t Unidade_Registra(SByte_t * constanteNome, Pos_t * unidade, Pos_t dispositivo)
{
    Unidade_t * ret = 0;
    for (Pos_t i = 0; i < UNIDADE_CAPACIDADE; i++)
    {
        if(!_Unidade_Lista[i].Existe)
        {
            ret = &_Unidade_Lista[i];
        }
    }
    if(ret == 0) return STATUS_ESTOURO_DA_CAPACIDADE;
    ret->Montado = NAO;
    ret->Dispositivo = dispositivo;
    ret->Existe = SIM;
    ret->SistemaDeArquivos = 0;
    Const_Copia(ret->NomeConst, constanteNome, UNIDADE_NOME_TAM);
    *unidade = ret->Unidade;
    return STATUS_OK;
}

Status_t Unidade_Desregistra(Pos_t unidade)
{
    Status_t ret = STATUS_OK;
    if(unidade >= UNIDADE_CAPACIDADE) return STATUS_UNIDADE_INVALIDA;
    if(!_Unidade_Lista[unidade].Existe) return STATUS_UNIDADE_INVALIDA;
    if(_Unidade_Lista[unidade].Montado)
    {
        ret = _Unidade_Lista[unidade].AcaoDesmonta(unidade);
        if(_Unidade_Lista[unidade].ObjetoMontado != 0) Mem_Local_Libera(_Unidade_Lista[unidade].ObjetoMontado);
    }
    if(ret == STATUS_OK)
    {
        _Unidade_Lista[unidade].Montado = NAO;
        _Unidade_Lista[unidade].Existe = NAO;
    }
    return ret;
}

Status_t Unidade_RegistraSisArq(SByte_t * constanteNome, Pos_t * sisArq, Status_t (*acaoMonta)(Pos_t unidade), Status_t (*acaoValida)(Pos_t unidade), Status_t (*acaoFormata)(Pos_t unidade))
{
    SistemaDeArquivos_t * ret = 0;
    for (Pos_t i = 0; i < SISTEMA_DE_ARQUIVOS_CAPACIDADE; i++)
    {
        if(!_Unidade_SisArq[i].Existe)
        {
            ret = &_Unidade_SisArq[i];
        }
    }
    if(ret == 0) return STATUS_ESTOURO_DA_CAPACIDADE;
    Mem_CopiaBinario((Byte_t *)ret->NomeConst, (Byte_t *)constanteNome, SISTEMA_DE_ARQUIVOS_NOME_TAM);
    ret->AcaoMonta = acaoMonta;
    ret->AcaoValida = acaoValida;
    ret->AcaoFormata = acaoFormata;
    ret->Existe = SIM;
    *sisArq = ret->SistemaDeArquivos;
    return STATUS_OK;
}

Status_t Unidade_DesregistraSisArq(Pos_t sisArq)
{
    Status_t ret = STATUS_OK;
    if(sisArq >= SISTEMA_DE_ARQUIVOS_CAPACIDADE) return STATUS_SISARQ_INVALIDO;
    if(!_Unidade_SisArq[sisArq].Existe) return STATUS_SISARQ_INVALIDO;
    for (Pos_t i = 0; i < UNIDADE_CAPACIDADE; i++)
    {
        if(_Unidade_Lista[i].Existe & _Unidade_Lista[i].Montado)
        {
            if(_Unidade_Lista[i].SistemaDeArquivos == sisArq)
            {
                ret = _Unidade_Lista[i].AcaoDesmonta(_Unidade_Lista[i].Unidade);
                if(ret != STATUS_OK)
                {
                    break;
                }
            }
        }
    }
    _Unidade_SisArq[sisArq].Existe = NAO;
    return ret;
}

Status_t Unidade_Dispositivo(Pos_t unidade, Pos_t * dispositivo)
{
    Status_t ret = STATUS_OK;
    if(unidade >= UNIDADE_CAPACIDADE) return STATUS_UNIDADE_INVALIDA;
    if(!_Unidade_Lista[unidade].Existe) return STATUS_UNIDADE_INVALIDA;
    *dispositivo = _Unidade_Lista[unidade].Dispositivo;
    return ret;
}

Status_t Unidade_Raiz(Pos_t unidade, Item_t * * item)
{
    Status_t ret = STATUS_OK;
    if(unidade >= UNIDADE_CAPACIDADE) return STATUS_UNIDADE_INVALIDA;
    if(!_Unidade_Lista[unidade].Existe) return STATUS_UNIDADE_INVALIDA;
    if(!_Unidade_Lista[unidade].Montado) return STATUS_UNIDADE_INVALIDA;
    if(_Unidade_Lista[unidade].AcaoRaiz == 0) return STATUS_COMANDO_DESCONHECIDO;
    ret = _Unidade_Lista[unidade].AcaoRaiz(unidade, item);
    return ret;
}

Status_t _Item_Validacao(Item_t * item)
{
    Status_t ret = STATUS_OK;
    Pos_t unidade = item->Unidade;
    if(unidade >= UNIDADE_CAPACIDADE) return STATUS_UNIDADE_INVALIDA;
    if(!_Unidade_Lista[unidade].Existe) return STATUS_UNIDADE_INVALIDA;
    if(!_Unidade_Lista[unidade].Montado) return STATUS_UNIDADE_INVALIDA;
    return ret;
}

Status_t _Item_ValidacaoArquivo(Item_t * item)
{
    Status_t ret = _Item_Validacao(item);
    if(item->Tipo != ITEM_TIPO_ARQUIVO) return STATUS_TIPO_INVALIDO;
    return ret;
}

Status_t _Item_ValidacaoDiretorio(Item_t * item)
{
    Status_t ret = _Item_Validacao(item);
    if(item->Tipo != ITEM_TIPO_DIRETORIO) return STATUS_TIPO_INVALIDO;
    return ret;
}

Status_t Unidade_RegistraMontagem
(
    Pos_t unidade, 
    Tam_t tamanhoObjetoMontado,
    Status_t (*acaoRaiz)(Pos_t unidade, Item_t * * raiz),
    Status_t (*acaoDesmonta)(Pos_t unidade),
    Status_t (*acaoItemSubItem)(Item_t * item, Pos_t posicao, Item_t * * subItem),
    Status_t (*acaoItemQtdSubItem)(Item_t * item, Tam_t * qtd),
    Status_t (*acaoItemVaPara)(Item_t * item, Pos_t posicao),
    Tam_t (*acaoItemLeia)(Item_t * item, Byte_t * destino, Tam_t tam),
    Tam_t (*acaoItemEscreva)(Item_t * item, Byte_t * origem, Tam_t tam),
    Status_t (*acaoItemExclui)(Item_t * item),
    Status_t (*acaoItemFecha)(Item_t * item),
    Status_t (*acaoItemCriaDiretorio)(Item_t * item, SByte_t * constanteNome),
    Status_t (*acaoItemCriaArquivo)(Item_t * item, SByte_t * constanteNome)
)
{
    Status_t ret = STATUS_OK;
    if(unidade >= UNIDADE_CAPACIDADE) return STATUS_UNIDADE_INVALIDA;
    if(!_Unidade_Lista[unidade].Existe) return STATUS_UNIDADE_INVALIDA;
    if(_Unidade_Lista[unidade].Montado) return STATUS_UNIDADE_INVALIDA;
    Unidade_t * u = &_Unidade_Lista[unidade];
    u->AcaoRaiz = acaoRaiz;
    u->AcaoDesmonta = acaoDesmonta;
    u->AcaoItemSubItem = acaoItemSubItem;
    u->AcaoItemQtdSubItem = acaoItemQtdSubItem;
    u->AcaoItemVaPara = acaoItemVaPara;
    u->AcaoItemLeia = acaoItemLeia;
    u->AcaoItemEscreva = acaoItemEscreva;
    u->AcaoItemExclui = acaoItemExclui;
    u->AcaoItemFecha = acaoItemFecha;
    u->AcaoItemCriaDiretorio = acaoItemCriaDiretorio;
    u->AcaoItemCriaArquivo = acaoItemCriaArquivo;
    if(tamanhoObjetoMontado > 0)
    {
        u->ObjetoMontado = Mem_Local_Aloca(tamanhoObjetoMontado);
    }
    else
    {
        u->ObjetoMontado = 0;
    }
    u->Montado = SIM;
    return ret;
}

Status_t Item_SubItem(Item_t * item, Pos_t posicao, Item_t * * subItem)
{
    Status_t ret = _Item_ValidacaoDiretorio(item);
    if(ret != STATUS_OK) return ret;
    if(_Unidade_Lista[item->Unidade].AcaoItemSubItem == 0) return STATUS_COMANDO_DESCONHECIDO;
    ret = _Unidade_Lista[item->Unidade].AcaoItemSubItem(item, posicao, subItem);
    return ret;
}

Status_t Item_QtdSubItens(Item_t * item, Tam_t * qtd)
{
    Status_t ret = _Item_ValidacaoDiretorio(item);
    if(ret != STATUS_OK) return ret;
    if(_Unidade_Lista[item->Unidade].AcaoItemQtdSubItem == 0) return STATUS_COMANDO_DESCONHECIDO;
    ret = _Unidade_Lista[item->Unidade].AcaoItemQtdSubItem(item, qtd);
    return ret;
}

Boleano_t Item_EhDiretorio(Item_t * item)
{
    return (_Item_ValidacaoDiretorio(item) == STATUS_OK) ? SIM : NAO;
}

Boleano_t Item_EhArquivo(Item_t * item)
{
    return (_Item_ValidacaoArquivo(item) == STATUS_OK) ? SIM : NAO;
}

Status_t Item_VaPara(Item_t * item, Pos_t posicao)
{
    Status_t ret = _Item_ValidacaoArquivo(item);
    if(ret != STATUS_OK) return ret;
    if(_Unidade_Lista[item->Unidade].AcaoItemVaPara == 0) return STATUS_COMANDO_DESCONHECIDO;
    ret = _Unidade_Lista[item->Unidade].AcaoItemVaPara(item, posicao);
    return ret;
}

Status_t Item_Leia(Item_t * item, Byte_t * destino, Tam_t tam, Tam_t * lido)
{
    Status_t ret = _Item_ValidacaoArquivo(item);
    if(ret != STATUS_OK) return ret;
    if(_Unidade_Lista[item->Unidade].AcaoItemLeia == 0) return STATUS_COMANDO_DESCONHECIDO;
    *lido = _Unidade_Lista[item->Unidade].AcaoItemLeia(item, destino, tam);
    return ret;
}

Status_t Item_Escreva(Item_t * item, Byte_t * origem, Tam_t tam, Tam_t * escrito)
{
    Status_t ret = _Item_ValidacaoArquivo(item);
    if(ret != STATUS_OK) return ret;
    if(_Unidade_Lista[item->Unidade].AcaoItemEscreva == 0) return STATUS_COMANDO_DESCONHECIDO;
    *escrito = _Unidade_Lista[item->Unidade].AcaoItemEscreva(item, origem, tam);
    return ret;
}

Status_t Item_Exclui(Item_t * item)
{
    Status_t ret = _Item_ValidacaoArquivo(item);
    if(ret != STATUS_OK) return ret;
    if(_Unidade_Lista[item->Unidade].AcaoItemExclui == 0) return STATUS_COMANDO_DESCONHECIDO;
    ret = _Unidade_Lista[item->Unidade].AcaoItemExclui(item);
    return ret;
}

Status_t Item_Fecha(Item_t * item)
{
    Status_t ret = _Item_Validacao(item);
    if(ret != STATUS_OK) return ret;
    if(_Unidade_Lista[item->Unidade].AcaoItemFecha == 0) return STATUS_COMANDO_DESCONHECIDO;
    ret = _Unidade_Lista[item->Unidade].AcaoItemFecha(item);
    return ret;
}

Status_t Item_CriaDiretorio(Item_t * item, SByte_t * constanteNome)
{
    Status_t ret = _Item_ValidacaoDiretorio(item);
    if(ret != STATUS_OK) return ret;
    if(_Unidade_Lista[item->Unidade].AcaoItemCriaDiretorio == 0) return STATUS_COMANDO_DESCONHECIDO;
    ret = _Unidade_Lista[item->Unidade].AcaoItemCriaDiretorio(item, constanteNome);
    return ret;
}

Status_t Item_CriaArquivo(Item_t * item, SByte_t * constanteNome)
{
    Status_t ret = _Item_ValidacaoDiretorio(item);
    if(ret != STATUS_OK) return ret;
    if(_Unidade_Lista[item->Unidade].AcaoItemCriaArquivo == 0) return STATUS_COMANDO_DESCONHECIDO;
    ret = _Unidade_Lista[item->Unidade].AcaoItemCriaArquivo(item, constanteNome);
    return ret;
}

Status_t Item_AbreConst(SByte_t * constanteEndereco, Tam_t tam, Item_t * * item)
{
    Status_t ret = STATUS_OK;
    Tam_t fim = Const_TamLimitado(constanteEndereco, tam);
    Pos_t etapa = 0;
    Pos_t unidadeInicio = 0;
    Pos_t unidadeFim = 0;
    Tam_t unidadeTam = 0;
    Unidade_t * unidade = _Unidade_Principal;
    Item_t * atual = 0;
    Item_t * proximo = 0;
    Pos_t atualInicio = 0;
    Pos_t atualFim = 0;
    Tam_t atualTam = 0;
    Tam_t qtd = 0;
    if(unidade != 0)
    {
        ret = Unidade_Raiz(unidade->Unidade, &atual);
        if(ret != STATUS_OK) return ret;
    }
    for (Pos_t i = 0; i < fim; i++)
    {
        switch (etapa)
        {
            case 0:
            {
                if(constanteEndereco[i] == '[')
                {
                    etapa = 1;
                    unidadeInicio = i+1;
                }
                else if(constanteEndereco[i] == '/')
                {
                    etapa = 2;
                    atualInicio = i+1;
                }
                else if(constanteEndereco[i] != ' ')
                {
                    return STATUS_FORMATO_INVALIDO;
                }
                break;
            }
            case 1:
            {
                if(constanteEndereco[i] == ']')
                {
                    unidadeFim = i;
                    unidadeTam = unidadeFim - unidadeInicio;
                    for (Pos_t i = 0; i < UNIDADE_CAPACIDADE; i++)
                    {
                        if(_Unidade_Lista[i].Existe)
                        {
                            Tam_t unidadeNomeTam = Const_TamLimitado(_Unidade_Lista[i].NomeConst, UNIDADE_NOME_TAM);
                            if(unidadeNomeTam == unidadeTam)
                            {
                                if(Const_Igual(constanteEndereco + unidadeInicio, _Unidade_Lista[i].NomeConst, unidadeTam))
                                {
                                    if(atual != 0) Item_Fecha(atual);
                                    unidade = &_Unidade_Lista[i];
                                    ret = Unidade_Raiz(i, &atual);
                                    if(ret != STATUS_OK) return ret;
                                    break;
                                }
                            }
                        }
                    }
                    if(unidade == 0) return STATUS_UNIDADE_INVALIDA;
                    etapa = 0;
                }
                break;
            }
            case 2:
            {
                if((constanteEndereco[i] == '/') | ((i+1) >= fim))
                {
                    proximo = 0;
                    atualFim = i;
                    if(((i+1) >= fim)) atualFim++;
                    atualTam = atualFim - atualInicio;
                    ret = Item_QtdSubItens(atual, &qtd);
                    if(ret != STATUS_OK) return ret;
                    for (Pos_t i = 0; i < qtd; i++)
                    {
                        ret = Item_SubItem(atual, i, &proximo);
                        if(ret != STATUS_OK) return ret;
                        if(atualTam == Const_TamLimitado(proximo->Nome, ITEM_NOME_TAM))
                        {
                            if(Const_Igual(constanteEndereco + atualInicio, proximo->Nome, atualTam))
                            {
                                break;
                            }
                            else
                            {
                                Item_Fecha(proximo);
                                proximo = 0;
                            }
                        }
                        else
                        {
                            Item_Fecha(proximo);
                            proximo = 0;
                        }
                    }
                    if(proximo == 0) return STATUS_NAO_ENCONTRADO;
                    Item_Fecha(atual);
                    atual = proximo;

                }
                break;
            }
        }
    }
    if(unidade == 0) return STATUS_NAO_ENCONTRADO;
    *item = atual;
    return ret;
}

Tam_t Unidade_Quantidade()
{
    Tam_t ret = 0;
    for (Pos_t i = 0; i < UNIDADE_CAPACIDADE; i++)
    {
        if(_Unidade_Lista[i].Existe) ret = i + 1;
    }
    return ret;
}

Status_t Unidade_LeiaNomeConst(Pos_t unidade, SByte_t * destino, Tam_t tam)
{
    if(unidade >= UNIDADE_CAPACIDADE) return STATUS_POSICAO_INVALIDA;
    Mem_RepeteByte((Byte_t *)destino, 0, tam);
    Const_Copia(destino, _Unidade_Lista[unidade].NomeConst, tam > UNIDADE_NOME_TAM ? UNIDADE_NOME_TAM : tam);
    return STATUS_OK;
}

Status_t Unidade_EscrevaNomeConst(Pos_t unidade, SByte_t * origem, Tam_t tam)
{
    if(unidade >= UNIDADE_CAPACIDADE) return STATUS_POSICAO_INVALIDA;
    Mem_RepeteByte((Byte_t *)_Unidade_Lista[unidade].NomeConst, 0, UNIDADE_NOME_TAM);
    Const_Copia(_Unidade_Lista[unidade].NomeConst, origem, tam > UNIDADE_NOME_TAM ? UNIDADE_NOME_TAM : tam);
    return STATUS_OK;
}

Status_t Unidade_LeiaTamanhoDeUmBloco(Pos_t unidade, Tam_t * tam)
{
    if(unidade >= UNIDADE_CAPACIDADE) return STATUS_POSICAO_INVALIDA;
    if(_Unidade_Lista[unidade].Dispositivo == 0) return STATUS_DISPOSITIVO_INVALIDO;
    return Dispositivo_LeiaTamanhoDeUmBloco(_Unidade_Lista[unidade].Dispositivo, tam);
}

Status_t Unidade_Monta(SByte_t * constSisArq, Tam_t constSisArqTam, SByte_t * constUnidade, Tam_t constUnidadeTam)
{
    Status_t ret = STATUS_OK;
    Boleano_t encontrado = NAO;
    SistemaDeArquivos_t * sisArq = 0;
    Unidade_t * unidade = 0;
    for (Pos_t i = 0; i < SISTEMA_DE_ARQUIVOS_CAPACIDADE; i++)
    {
        if(_Unidade_SisArq[i].Existe)
        {
            if((Const_TamLimitado(_Unidade_SisArq[i].NomeConst, SISTEMA_DE_ARQUIVOS_NOME_TAM) == constSisArqTam) && Const_Igual(_Unidade_SisArq[i].NomeConst, constSisArq, constSisArqTam))
            {
                encontrado = SIM;
                sisArq = &_Unidade_SisArq[i];
                break;
            }
        }
    }
    if(!encontrado) return STATUS_SISARQ_INVALIDO;
    encontrado = NAO;
    for (Pos_t i = 0; i < UNIDADE_CAPACIDADE; i++)
    {
        if(_Unidade_Lista[i].Existe & (!_Unidade_Lista[i].Montado))
        {
            if((Const_TamLimitado(_Unidade_Lista[i].NomeConst, UNIDADE_NOME_TAM) == constUnidadeTam) && Const_Igual(_Unidade_Lista[i].NomeConst, constUnidade, constUnidadeTam))
            {
                encontrado = SIM;
                unidade = &_Unidade_Lista[i];
                break;
            }
        }
    }
    if(!encontrado) return STATUS_UNIDADE_INVALIDA;
    ret = sisArq->AcaoValida(unidade->Unidade);
    if(ret != STATUS_OK) return ret;
    return sisArq->AcaoMonta(unidade->Unidade);
}

Status_t Unidade_Formata(SByte_t * constSisArq, Tam_t constSisArqTam, SByte_t * constUnidade, Tam_t constUnidadeTam)
{
    Boleano_t encontrado = NAO;
    SistemaDeArquivos_t * sisArq = 0;
    Unidade_t * unidade = 0;
    for (Pos_t i = 0; i < SISTEMA_DE_ARQUIVOS_CAPACIDADE; i++)
    {
        if(_Unidade_SisArq[i].Existe)
        {
            if((Const_TamLimitado(_Unidade_SisArq[i].NomeConst, SISTEMA_DE_ARQUIVOS_NOME_TAM) == constSisArqTam) && Const_Igual(_Unidade_SisArq[i].NomeConst, constSisArq, constSisArqTam))
            {
                encontrado = SIM;
                sisArq = &_Unidade_SisArq[i];
                break;
            }
        }
    }
    if(!encontrado)
    {
        MensagemConst("Unidade", "Nao foi possivel formatar. Sistema de arquivos nao encontrado: ", constSisArq, constSisArqTam); 
        return STATUS_SISARQ_INVALIDO;
    }
    encontrado = NAO;
    for (Pos_t i = 0; i < UNIDADE_CAPACIDADE; i++)
    {
        if(_Unidade_Lista[i].Existe & (!_Unidade_Lista[i].Montado))
        {
            if((Const_TamLimitado(_Unidade_Lista[i].NomeConst, UNIDADE_NOME_TAM) == constUnidadeTam) && Const_Igual(_Unidade_Lista[i].NomeConst, constUnidade, constUnidadeTam))
            {
                encontrado = SIM;
                unidade = &_Unidade_Lista[i];
                break;
            }
        }
    }
    if(!encontrado)
    {
        MensagemConst("Unidade", "Nao foi possivel formatar. Unidade nao encontrada: ", constUnidade, constUnidadeTam); 
        return STATUS_UNIDADE_INVALIDA;
    }
    return sisArq->AcaoFormata(unidade->Unidade);
}

Status_t Unidade_Desmonta(SByte_t * constUnidade, Tam_t constUnidadeTam)
{
    Boleano_t encontrado = NAO;
    Unidade_t * unidade = 0;
    encontrado = NAO;
    for (Pos_t i = 0; i < UNIDADE_CAPACIDADE; i++)
    {
        if(_Unidade_Lista[i].Existe & _Unidade_Lista[i].Montado)
        {
            if((Const_TamLimitado(_Unidade_Lista[i].NomeConst, UNIDADE_NOME_TAM) == constUnidadeTam) && Const_Igual(_Unidade_Lista[i].NomeConst, constUnidade, constUnidadeTam))
            {
                encontrado = SIM;
                unidade = &_Unidade_Lista[i];
                break;
            }
        }
    }
    if(!encontrado)
    {
        MensagemConst("Unidade", "Nao foi possivel desmontar. Unidade nao encontrada: ", constUnidade, constUnidadeTam); 
        return STATUS_UNIDADE_INVALIDA;
    }
    Status_t ret = unidade->AcaoDesmonta(unidade->Unidade);
    if(ret != STATUS_OK) return ret;
    unidade->Montado = NAO;
    if(unidade->ObjetoMontado != 0)
    {
        Mem_Local_Libera((Byte_t *)unidade->ObjetoMontado);
        unidade->ObjetoMontado = 0;
    }
    if(_Unidade_Principal != 0 && (Pos_t)unidade == (Pos_t)_Unidade_Principal) _Unidade_Principal = 0;
    return ret;
}

Status_t Unidade_CarregaObjetoMontado(Pos_t unidade, Byte_t * * objetoMontado)
{
    if(unidade >= UNIDADE_CAPACIDADE) return STATUS_POSICAO_INVALIDA;
    if((!_Unidade_Lista[unidade].Montado) | (!_Unidade_Lista[unidade].Existe)) return STATUS_UNIDADE_INVALIDA;
    if(_Unidade_Lista[unidade].ObjetoMontado == 0) return STATUS_ESTOURO_DA_CAPACIDADE;
    *objetoMontado = _Unidade_Lista[unidade].ObjetoMontado;
    return STATUS_OK;
}



Status_t Item_CriaArquivoConst(SByte_t * constanteEndereco, Tam_t tam)
{
    Status_t ret = STATUS_OK;
    Tam_t fim = Const_TamLimitado(constanteEndereco, tam);
    Pos_t etapa = 0;
    Pos_t unidadeInicio = 0;
    Pos_t unidadeFim = 0;
    Tam_t unidadeTam = 0;
    SByte_t nome[ITEM_NOME_TAM + 1];
    nome[ITEM_NOME_TAM] = 0;
    Unidade_t * unidade = _Unidade_Principal;
    Item_t * atual = 0;
    Item_t * proximo = 0;
    Pos_t atualInicio = 0;
    Pos_t atualFim = 0;
    Tam_t atualTam = 0;
    Tam_t qtd = 0;
    if(unidade != 0)
    {
        ret = Unidade_Raiz(unidade->Unidade, &atual);
        if(ret != STATUS_OK) return ret;
    }
    for (Pos_t i = 0; i < fim; i++)
    {
        switch (etapa)
        {
            case 0:
            {
                if(constanteEndereco[i] == '[')
                {
                    etapa = 1;
                    unidadeInicio = i+1;
                }
                else if(constanteEndereco[i] == '/')
                {
                    etapa = 2;
                    atualInicio = i+1;
                }
                else if(constanteEndereco[i] != ' ')
                {
                    return STATUS_FORMATO_INVALIDO;
                }
                break;
            }
            case 1:
            {
                if(constanteEndereco[i] == ']')
                {
                    unidadeFim = i;
                    unidadeTam = unidadeFim - unidadeInicio;
                    for (Pos_t i = 0; i < UNIDADE_CAPACIDADE; i++)
                    {
                        if(_Unidade_Lista[i].Existe)
                        {
                            Tam_t unidadeNomeTam = Const_TamLimitado(_Unidade_Lista[i].NomeConst, UNIDADE_NOME_TAM);
                            if(unidadeNomeTam == unidadeTam)
                            {
                                if(Const_Igual(constanteEndereco + unidadeInicio, _Unidade_Lista[i].NomeConst, unidadeTam))
                                {
                                    if(atual != 0) Item_Fecha(atual);
                                    unidade = &_Unidade_Lista[i];
                                    ret = Unidade_Raiz(i, &atual);
                                    if(ret != STATUS_OK) return ret;
                                    break;
                                }
                            }
                        }
                    }
                    if(unidade == 0) return STATUS_UNIDADE_INVALIDA;
                    etapa = 0;
                }
                break;
            }
            case 2:
            {
                if((constanteEndereco[i] == '/') | ((i+1) >= fim))
                {
                    proximo = 0;
                    atualFim = i;
                    if(((i+1) >= fim)) atualFim++;
                    atualTam = atualFim - atualInicio;
                    ret = Item_QtdSubItens(atual, &qtd);
                    if(ret != STATUS_OK) return ret;
                    for (Pos_t i = 0; i < qtd; i++)
                    {
                        ret = Item_SubItem(atual, i, &proximo);
                        if(ret != STATUS_OK) return ret;
                        if(atualTam == Const_TamLimitado(proximo->Nome, ITEM_NOME_TAM))
                        {
                            if(Const_Igual(constanteEndereco + atualInicio, proximo->Nome, atualTam))
                            {
                                break;
                            }
                            else
                            {
                                Item_Fecha(proximo);
                                proximo = 0;
                            }
                        }
                        else
                        {
                            Item_Fecha(proximo);
                            proximo = 0;
                        }
                    }
                    if(proximo == 0)
                    {
                        Const_Copia(nome, constanteEndereco + atualInicio, atualTam);
                        ret = Item_CriaArquivo(atual, nome);
                        Item_Fecha(atual);
                        return ret;
                    }
                    Item_Fecha(atual);
                    atual = proximo;

                }
                break;
            }
        }
    }
    return STATUS_NAO_ENCONTRADO;
}

Status_t Item_CriaDiretorioConst(SByte_t * constanteEndereco, Tam_t tam)
{
    Status_t ret = STATUS_OK;
    Tam_t fim = Const_TamLimitado(constanteEndereco, tam);
    Pos_t etapa = 0;
    Pos_t unidadeInicio = 0;
    Pos_t unidadeFim = 0;
    Tam_t unidadeTam = 0;
    SByte_t nome[ITEM_NOME_TAM + 1];
    nome[ITEM_NOME_TAM] = 0;
    Unidade_t * unidade = _Unidade_Principal;
    Item_t * atual = 0;
    Item_t * proximo = 0;
    Pos_t atualInicio = 0;
    Pos_t atualFim = 0;
    Tam_t atualTam = 0;
    Tam_t qtd = 0;
    if(unidade != 0)
    {
        ret = Unidade_Raiz(unidade->Unidade, &atual);
        if(ret != STATUS_OK) return ret;
    }
    for (Pos_t i = 0; i < fim; i++)
    {
        switch (etapa)
        {
            case 0:
            {
                if(constanteEndereco[i] == '[')
                {
                    etapa = 1;
                    unidadeInicio = i+1;
                }
                else if(constanteEndereco[i] == '/')
                {
                    etapa = 2;
                    atualInicio = i+1;
                }
                else if(constanteEndereco[i] != ' ')
                {
                    return STATUS_FORMATO_INVALIDO;
                }
                break;
            }
            case 1:
            {
                if(constanteEndereco[i] == ']')
                {
                    unidadeFim = i;
                    unidadeTam = unidadeFim - unidadeInicio;
                    for (Pos_t i = 0; i < UNIDADE_CAPACIDADE; i++)
                    {
                        if(_Unidade_Lista[i].Existe)
                        {
                            Tam_t unidadeNomeTam = Const_TamLimitado(_Unidade_Lista[i].NomeConst, UNIDADE_NOME_TAM);
                            if(unidadeNomeTam == unidadeTam)
                            {
                                if(Const_Igual(constanteEndereco + unidadeInicio, _Unidade_Lista[i].NomeConst, unidadeTam))
                                {
                                    if(atual != 0) Item_Fecha(atual);
                                    unidade = &_Unidade_Lista[i];
                                    ret = Unidade_Raiz(i, &atual);
                                    if(ret != STATUS_OK) return ret;
                                    break;
                                }
                            }
                        }
                    }
                    if(unidade == 0) return STATUS_UNIDADE_INVALIDA;
                    etapa = 0;
                }
                break;
            }
            case 2:
            {
                if((constanteEndereco[i] == '/') | ((i+1) >= fim))
                {
                    proximo = 0;
                    atualFim = i;
                    if(((i+1) >= fim)) atualFim++;
                    atualTam = atualFim - atualInicio;
                    ret = Item_QtdSubItens(atual, &qtd);
                    if(ret != STATUS_OK) return ret;
                    for (Pos_t i = 0; i < qtd; i++)
                    {
                        ret = Item_SubItem(atual, i, &proximo);
                        if(ret != STATUS_OK) return ret;
                        if(atualTam == Const_TamLimitado(proximo->Nome, ITEM_NOME_TAM))
                        {
                            if(Const_Igual(constanteEndereco + atualInicio, proximo->Nome, atualTam))
                            {
                                break;
                            }
                            else
                            {
                                Item_Fecha(proximo);
                                proximo = 0;
                            }
                        }
                        else
                        {
                            Item_Fecha(proximo);
                            proximo = 0;
                        }
                    }
                    if(proximo == 0)
                    {
                        Const_Copia(nome, constanteEndereco + atualInicio, atualTam);
                        ret = Item_CriaDiretorio(atual, nome);
                        Item_Fecha(atual);
                        return ret;
                    }
                    Item_Fecha(atual);
                    atual = proximo;

                }
                break;
            }
        }
    }
    return STATUS_NAO_ENCONTRADO;
}

void Unidade()
{
    for (Pos_t i = 0; i < UNIDADE_CAPACIDADE; i++)
    {
        _Unidade_Lista[i].Unidade = i;
        _Unidade_Lista[i].Existe = NAO;
        _Unidade_Lista[i].Montado = NAO;
    }
    for (Pos_t i = 0; i < SISTEMA_DE_ARQUIVOS_CAPACIDADE; i++)
    {
        _Unidade_SisArq[i].SistemaDeArquivos = i;
        _Unidade_SisArq[i].Existe = NAO;
    }
}