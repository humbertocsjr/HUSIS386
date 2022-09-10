#include <HUSIS.h>

typedef struct 
{
    Boleano_t Ativo;
    Byte_t Comando[17];
    Byte_t Descricao[129];
    Status_t (* Acao) (SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0));
} Comando_t;

#define COMANDO_LISTA_TAM 256
Comando_t _Comando_Lista[COMANDO_LISTA_TAM];


Pos_t _Comando_IgnoraEspacosConst(SByte_t * constante, Pos_t inicio, Tam_t tam)
{
    for (Pos_t i = inicio; i < tam; i++)
    {
        if(constante[i] != ' ') return i;
    }
    return tam;
}

Pos_t _Comando_BuscaEspacosConst(SByte_t * constante, Pos_t inicio, Tam_t tam)
{
    for (Pos_t i = inicio; i < tam; i++)
    {
        if(constante[i] == ' ') return i;
        if(constante[i] == 0) return i;
    }
    return tam;
}

Pos_t _Comando_BuscaDoisPontosConst(SByte_t * constante, Pos_t inicio, Tam_t tam)
{
    for (Pos_t i = inicio; i < tam; i++)
    {
        if(i > 0)
        {
            if((constante[i] == ':') & (constante[i-1] != '\\')) return i;
        }
        else
        {
            if(constante[i] == ':') return i;
        }
        if(constante[i] == 0) return i;
    }
    return tam;
}

Boleano_t _Comando_Igual(SByte_t * constante, Tam_t tam, SByte_t * comando)
{
    if(Const_Tam(comando) != tam) return NAO;
    return Const_Igual(constante, comando, tam);
}

Status_t _Comando_ProcessarConstIndividual(SByte_t * constante, Tam_t tam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    if(tam == 0) return STATUS_OK;
    Pos_t comandoInicio = _Comando_IgnoraEspacosConst(constante, 0, tam);
    if(comandoInicio >= tam) return STATUS_COMANDO_DESCONHECIDO;
    Pos_t comandoFim = _Comando_BuscaEspacosConst(constante, comandoInicio, tam);
    Tam_t comandoTam = comandoFim - comandoInicio;
    SByte_t * comandoConst = (SByte_t *)(constante + comandoInicio);
    Pos_t argInicio = _Comando_IgnoraEspacosConst(constante, comandoFim, tam);
    Pos_t argFim = Const_TamLimitado(constante, tam);
    Tam_t argTam = argFim - argInicio;
    SByte_t * argConst = (SByte_t *)(constante + argInicio);
    Comando_t * cmdAtual = 0;
    Boleano_t encontrado = NAO;

    if(comandoTam > 0)
    {
        for (Pos_t i = 0; i < COMANDO_LISTA_TAM; i++)
        {
            cmdAtual = &_Comando_Lista[i];
            if(cmdAtual->Ativo)
            {
                if(_Comando_Igual(comandoConst, comandoTam, (SByte_t *)&cmdAtual->Comando))
                {
                    if(cmdAtual->Acao != 0)
                    {
                        cmdAtual->Acao(argConst, argTam, saidaTexto);
                        encontrado = SIM;
                        break;
                    }
                }
            }
        }
        if(!encontrado) saidaTexto("Comando desconhecido\n\n",0);
        
    }

    return STATUS_OK;
}

Status_t Comando_ProcessarConst(SByte_t * constante, Tam_t tam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    Pos_t inicio = 0;
    Pos_t fim = 0;
    Status_t ret = STATUS_OK;
    while(inicio < tam)
    {
        fim = _Comando_BuscaDoisPontosConst(constante, inicio, tam);
        ret = _Comando_ProcessarConstIndividual(constante + inicio, fim - inicio, saidaTexto);
        if(constante[fim] == 0)
        {
            return ret;
        }
        inicio = fim + 1;
    }
    return ret;
}

Status_t Comando_ProcessarConstAuto(SByte_t * constante, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    return Comando_ProcessarConst(constante, Const_Tam(constante), saidaTexto);
}

void Comando_TermSaidaTexto(SByte_t * constanteTexto, Tam_t valor0)
{
    Term_EscrevaFmt1(constanteTexto, valor0);
}

void Comando_Term()
{
    #define COMANDO_LINHA_TAM 70
    SByte_t comando[COMANDO_LINHA_TAM];
    Term_EscrevaFmt0("\nHUSIS Processador de Comandos\n\n Digite 'ajuda' para lista de comandos.\n");

    while(SIM)
    {
        Term_EscrevaFmt0("\n=> ");
        Term_LeiaLinhaConst(comando, COMANDO_LINHA_TAM);
        Comando_ProcessarConst(comando, COMANDO_LINHA_TAM, &Comando_TermSaidaTexto);
    }
}

Status_t Comando_RegistraConst(SByte_t * constanteNome, SByte_t * constanteDescricao, Status_t (* acao) (SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0)))
{
    Comando_t * atual = 0;
    Tam_t tam = Const_Tam(constanteNome);
    Tam_t descTam = Const_Tam(constanteDescricao);
    if(tam > 15) return STATUS_ESTOURO_DA_CAPACIDADE;
    if(tam > 128) descTam = 128;
    for (Pos_t i = 0; i < COMANDO_LISTA_TAM; i++)
    {
        if(!_Comando_Lista[i].Ativo)
        {
            atual = &_Comando_Lista[i];
            atual->Ativo = SIM;
            break;
        }
    }
    if(atual == 0) return STATUS_ESTOURO_DA_CAPACIDADE;
    atual->Acao = acao;
    Mem_CopiaBinario(atual->Comando, (Byte_t *)constanteNome, tam);
    Mem_CopiaBinario(atual->Descricao, (Byte_t *)constanteDescricao, descTam);
    atual->Comando[16] = 0;
    atual->Descricao[128] = 0;
    return STATUS_OK;
}

Status_t _Comando_CmdAjuda(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    saidaTexto("Aceita multiplos comandos por linha separados por ':'\n", 0);
    saidaTexto("Comandos:", 0);
    for (Pos_t i = 0; i < COMANDO_LISTA_TAM; i++)
    {
        if(_Comando_Lista[i].Ativo)
        {
            if(Const_Tam((SByte_t *)_Comando_Lista[i].Comando) >= 7)
            {
                saidaTexto("\n {0:C}\t= ", (Tam_t)_Comando_Lista[i].Comando);
            }
            else
            {
                saidaTexto("\n {0:C}\t\t= ", (Tam_t)_Comando_Lista[i].Comando);
            }
            saidaTexto("{0:C}", (Tam_t)_Comando_Lista[i].Descricao);
        }
    }
    saidaTexto("\n", 0);
    return STATUS_OK;
}

Status_t _Comando_CmdEscreva(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    for (Pos_t i = 0; i < argsTam; i++)
    {
        if(args[i] == 0) break;
        if(args[i] == '\\')
        {
            i++;
            if(args[i] == 0) break;
            switch (args[i])
            {
                case 'n':
                {
                    saidaTexto("\n", 0);
                    break;
                }
                case 't':
                {
                    saidaTexto("\t", 0);
                    break;
                }
                default:
                {
                    saidaTexto("{0:c}", (Tam_t)args[i]);
                    break;
                }
            }
        }
        else
        {
            saidaTexto("{0:c}", (Tam_t)args[i]);
        }
    }
    return STATUS_OK;
}

Status_t _Comando_CmdEscrevaL(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    _Comando_CmdEscreva(args, argsTam, saidaTexto);
    saidaTexto("\n", (Tam_t)args);
    return STATUS_OK;
}

Status_t _Comando_CmdMsg(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    Pos_t fim = _Comando_BuscaEspacosConst(args, 0, argsTam);
    Pos_t arg2 = _Comando_IgnoraEspacosConst(args, fim, argsTam);

    if(_Comando_Igual(args, fim, "ativa"))
    {
        MensagemAtiva();
    }
    else if(_Comando_Igual(args, fim, "desativa"))
    {
        MensagemDesativa();
    }
    else if(_Comando_Igual(args, fim, "escreva"))
    {
        Mensagem("Comandos", "Mensagem: {0:C}", (Tam_t)(args + arg2));
    }
    else
    {
        saidaTexto("Sub-Comandos aceitos:\n ativa\t\t= Ativa a escrita no terminal\n desativa\t= Desativa a escrita no terminal\n escreva\t= Escreve uma mensagem via nucleo",0);
    }
    return STATUS_OK;
}

Status_t _Comando_CmdVer(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    Versao_t ver = Husis_Versao();
    saidaTexto("HUSIS v{0:u}", (Tam_t)ver.Versao);
    saidaTexto(".{0:u}", (Tam_t)ver.SubVersao);
    saidaTexto(" R{0:u}\nCopyright (c) 2022, Humberto Costa dos Santos Junior (humbertocsjr)\n", (Tam_t)ver.Revisao);
    saidaTexto("Argumentos do Nucleo: {0:C}\n", (Tam_t)Multiboot_ArgsConst());
    return STATUS_OK;
}

Status_t _Comando_CmdTempo(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    saidaTexto("Tempo ativo: {0:u}h", Cronometro_Horas());
    saidaTexto("{0:u}m", Cronometro_Minutos());
    saidaTexto("{0:u}s\n", Cronometro_Segundos());
    return STATUS_OK;
}

Status_t _Comando_CmdLivre(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    saidaTexto("Memoria Nucleo Livre....: {0:u} KiB", Mem_Local_EspacoLivre() / 1024);
    saidaTexto(" de {0:u} KiB\n", Mem_Local_Capacidade() / 1024);
    saidaTexto("Memoria Livre...........: {0:u} KiB", Mem_PaginasLivres() * 4);
    saidaTexto(" de {0:u} KiB\n", Mem_TotalDePaginas() * 4);
    return STATUS_OK;
}

Status_t _Comando_CmdTeclado(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    Pos_t fim = _Comando_BuscaEspacosConst(args, 0, argsTam);

    if(_Comando_Igual(args, fim, "desconhecidas"))
    {
        Teclado_ExibeDesconhecidas();
    }
    else if(_Comando_Igual(args, fim, "todas"))
    {
        Teclado_ExibeTodas();
    }
    else if(_Comando_Igual(args, fim, "oculta"))
    {
        Teclado_Oculta();
    }
    else
    {
        saidaTexto("Sub-Comandos aceitos:\n desconhecidas\t= Ativa a exibicao das teclas desconhecidas\n todas\t\t= Ativa a exibicao de todas teclas\n oculta\t\t= Oculta a escrita de qualquer tecla",0);
    }
    return STATUS_OK;
}

Status_t _Comando_CmdLista(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    Item_t * item = 0;
    Item_t * sub = 0;
    SByte_t nome[ITEM_NOME_TAM +1];
    Tam_t qtd = 0;
    nome[ITEM_NOME_TAM] = 0;
    Status_t ret = Item_AbreConst(args, argsTam, &item);
    if(ret != STATUS_OK)
    {
        switch(ret)
        {
            case STATUS_UNIDADE_INVALIDA:
            {
                saidaTexto("Erro: {0:u} - Unidade nao montada ou invalida", ret);
                break;
            }
            case STATUS_NAO_ENCONTRADO:
            {
                saidaTexto("Erro: {0:u} - Endereco nao encontrado", ret);
                break;
            }
            default:
            {
                saidaTexto("Erro: {0:u} - Desconhecido", ret);
                break;
            }
        }
        return ret;
    }
    Const_Copia(nome, item->Nome, ITEM_NOME_TAM);
    if(item->Tipo != ITEM_TIPO_DIRETORIO)
    {
        saidaTexto("'{0:C}' nao eh um diretorio", (Tam_t)&nome);
        return STATUS_TIPO_INVALIDO;
    }
    saidaTexto("Diretorio '{0:C}':\n\n", (Tam_t)&nome);
    ret = Item_QtdSubItens(item, &qtd);
    if(ret != STATUS_OK)
    {
        Item_Fecha(item);
        saidaTexto("Erro: {0:u} - Nao foi possivel buscar a quantidade de itens no diretorio", ret);
        return ret;
    }
    for (Pos_t i = 0; i < qtd; i++)
    {
        ret = Item_SubItem(item, i, &sub);
        if(ret != STATUS_OK)
        {
            Item_Fecha(item);
            saidaTexto("Erro: {0:u} - Nao foi possivel abrir um item no diretorio", ret);
            return ret;
        }
        Const_Copia(nome, sub->Nome, ITEM_NOME_TAM);
        if(sub->Tipo == ITEM_TIPO_DIRETORIO)
        {
            saidaTexto(" {0:C} [Diretorio]\n", (Tam_t)&nome);
        }
        else
        {
            saidaTexto(" {0:C}\n", (Tam_t)&nome);
        }
        Item_Fecha(sub);
    }
    saidaTexto("\nQuantidade de itens: {0:u}\n", qtd);
    Item_Fecha(item);
    return ret;
}

Status_t _Comando_CmdBinario(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    Item_t * item = 0;
    SByte_t nome[ITEM_NOME_TAM +1];
    Byte_t temp[16];
    Tam_t qtd = 0;
    Pos_t pos = 0;
    nome[ITEM_NOME_TAM] = 0;
    Status_t ret = Item_AbreConst(args, argsTam, &item);
    if(ret != STATUS_OK)
    {
        switch(ret)
        {
            case STATUS_UNIDADE_INVALIDA:
            {
                saidaTexto("Erro: {0:u} - Unidade nao montada ou invalida", ret);
                break;
            }
            case STATUS_NAO_ENCONTRADO:
            {
                saidaTexto("Erro: {0:u} - Endereco nao encontrado", ret);
                break;
            }
            default:
            {
                saidaTexto("Erro: {0:u} - Desconhecido", ret);
                break;
            }
        }
        return ret;
    }
    Const_Copia(nome, item->Nome, ITEM_NOME_TAM);
    if(item->Tipo != ITEM_TIPO_ARQUIVO)
    {
        saidaTexto("'{0:C}' nao eh um arquivo", (Tam_t)&nome);
        Item_Fecha(item);
        return STATUS_TIPO_INVALIDO;
    }
    saidaTexto("POS       x0x1x2x3x4x5x6x7x8x9xAxBxCxDxExF   ASCII\n", 0);
    while((ret = Item_Leia(item, temp, 16, &qtd)) == STATUS_OK)
    {
        if(qtd == 0) break;
        saidaTexto("{0:H}: ", pos);
        for (Pos_t i = 0; i < qtd; i++)
        {
            saidaTexto("{0:h}", temp[i]);
        }
        for (Pos_t i = 0; i < (16-qtd); i++)
        {
            saidaTexto("  ",0);
        }
        saidaTexto(" : ",0);
        for (Pos_t i = 0; i < qtd; i++)
        {
            saidaTexto("{0:c}", temp[i]);
        }
        saidaTexto("\n",0);
        pos += qtd;
    }
    Item_Fecha(item);
    return ret;
}

Status_t _Comando_CmdTexto(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    Item_t * item = 0;
    SByte_t nome[ITEM_NOME_TAM +1];
    Byte_t temp[256];
    Tam_t qtd = 0;
    Pos_t pos = 0;
    Boleano_t continua = SIM;
    nome[ITEM_NOME_TAM] = 0;
    Status_t ret = Item_AbreConst(args, argsTam, &item);
    if(ret != STATUS_OK)
    {
        switch(ret)
        {
            case STATUS_UNIDADE_INVALIDA:
            {
                saidaTexto("Erro: {0:u} - Unidade nao montada ou invalida", ret);
                break;
            }
            case STATUS_NAO_ENCONTRADO:
            {
                saidaTexto("Erro: {0:u} - Endereco nao encontrado", ret);
                break;
            }
            default:
            {
                saidaTexto("Erro: {0:u} - Desconhecido", ret);
                break;
            }
        }
        return ret;
    }
    Const_Copia(nome, item->Nome, ITEM_NOME_TAM);
    if(item->Tipo != ITEM_TIPO_ARQUIVO)
    {
        saidaTexto("'{0:C}' nao eh um arquivo", (Tam_t)&nome);
        Item_Fecha(item);
        return STATUS_TIPO_INVALIDO;
    }
    while(((ret = Item_Leia(item, temp, 256, &qtd)) == STATUS_OK) & continua)
    {
        if(qtd == 0) break;
        for (Pos_t i = 0; i < qtd; i++)
        {
            if(temp[i] == 0)
            {
                continua = NAO;
                break;
            }
            saidaTexto("{0:h}", temp[i]);
        }
        pos += qtd;
    }
    Item_Fecha(item);
    return ret;
}

Status_t _Comando_CmdUnidades(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    SByte_t nome[65];
    nome[64] = 0;
    Pos_t principal = 0;
    Status_t principalStatus = Unidade_LeiaPrincipal(&principal); 
    Pos_t qtd = Unidade_Quantidade();
    saidaTexto("Unidades:\n\n", 0);
    for (Pos_t i = 0; i < qtd; i++)
    {
        Unidade_LeiaNomeConst(i, nome, 64);
        if(Const_TamLimitado(nome,64) != 0)
        {
            if((principalStatus == STATUS_OK) & (principal == i))
            {
                saidaTexto(" [{0:C}] Unidade principal\n", (Tam_t)nome);
            }
            else if(Const_Igual(nome, "#", 64))
            {
                saidaTexto(" [{0:C}] Unidade especial que contem os dispositivos\n", (Tam_t)nome);
            }
            else
            {
                saidaTexto(" [{0:C}]\n", (Tam_t)nome);
            }
        }
    }
    return STATUS_OK;
}

Status_t _Comando_CmdBloco(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    Pos_t arg1 = _Comando_IgnoraEspacosConst(args, 0, argsTam);
    Pos_t arg1Fim = _Comando_BuscaEspacosConst(args, arg1, argsTam);
    Pos_t arg1Tam = arg1Fim - arg1;
    Pos_t arg2 = _Comando_IgnoraEspacosConst(args, arg1Fim, argsTam);
    Pos_t arg2Fim = _Comando_BuscaEspacosConst(args, arg2, argsTam);
    Pos_t arg3 = _Comando_IgnoraEspacosConst(args, arg2Fim, argsTam);
    Pos_t arg3Fim = argsTam;
    Item_t * item = 0;
    SByte_t nome[ITEM_NOME_TAM +1];
    Byte_t * temp;
    Tam_t qtd = 0;
    Pos_t pos = 0;
    nome[ITEM_NOME_TAM] = 0;
    if((arg1 == arg1Fim) |(arg2 == arg2Fim) |(arg3 == arg3Fim))
    {
        saidaTexto("Uso: bloco [Tamanho do bloco: 512 | 1024 | 2048 | 4096] [Posicao] [Arquivo/Dispositivo]", 0);
        return STATUS_FORMATO_INVALIDO;
    }
    if(arg1Tam == 3 && Const_Igual("512", args+ arg1, arg1Fim - arg1) )
    {
        qtd = 512;
        temp = Mem_Local_Aloca(512);
    }
    else if(arg1Tam == 4 && Const_Igual("1024", args+ arg1, arg1Fim - arg1) )
    {
        qtd = 1024;
        temp = Mem_Local_Aloca(1024);
    }
    else if(arg1Tam == 4 && Const_Igual("2048", args+ arg1, arg1Fim - arg1) )
    {
        qtd = 2048;
        temp = Mem_Local_Aloca(2048);
    }
    else if(arg1Tam == 4 && Const_Igual("4096", args+ arg1, arg1Fim - arg1) )
    {
        qtd = 4096;
        temp = Mem_Local_Aloca(4096);
    }
    else
    {
        saidaTexto("Tamanho de bloco nao suportado", 0);
        return STATUS_FORMATO_INVALIDO;
    }
    
    Status_t ret = Const_ParaNumero(args+ arg2, arg2Fim - arg2, &pos);
    if(ret != STATUS_OK)
    {
        saidaTexto("Posicao do bloco nao numerica", 0);
    }
    ret = Item_AbreConst(args + arg3, arg3Fim - arg3, &item);
    if(ret != STATUS_OK)
    {
        switch(ret)
        {
            case STATUS_UNIDADE_INVALIDA:
            {
                saidaTexto("Erro: {0:u} - Unidade nao montada ou invalida", ret);
                break;
            }
            case STATUS_NAO_ENCONTRADO:
            {
                saidaTexto("Erro: {0:u} - Endereco nao encontrado", ret);
                break;
            }
            default:
            {
                saidaTexto("Erro: {0:u} - Desconhecido", ret);
                break;
            }
        }
        Mem_Local_Libera(temp);
        return ret;
    }
    Const_Copia(nome, item->Nome, ITEM_NOME_TAM);
    if(item->Tipo != ITEM_TIPO_ARQUIVO)
    {
        saidaTexto("'{0:C}' nao eh um arquivo", (Tam_t)&nome);
        Item_Fecha(item);
        Mem_Local_Libera(temp);
        return STATUS_TIPO_INVALIDO;
    }
    ret = Item_VaPara(item, pos);
    if(ret == STATUS_OK)
    {
        if((ret = Item_Leia(item, temp, qtd, &qtd)) == STATUS_OK)
        {
            if(qtd > 0)
            {
                saidaTexto("Bloco: {0:u}\n", pos);
                saidaTexto("POS       x0x1x2x3x4x5x6x7x8x9xAxBxCxDxExFx0x1x2x3x4x5x6x7x8x9xAxBxCxDxExF\n", 0);
                for (Pos_t i = 0; i < qtd; i+=32)
                {
                    saidaTexto("{0:H}: ", i);
                    for (Pos_t j = 0; j < 32; j++)
                    {
                        saidaTexto("{0:h}", temp[i+j]);
                    }
                    saidaTexto("\n",0);
                }
                saidaTexto("Bloco: {0:u}\n", pos);
            }
            else
            {
                saidaTexto("Bloco vazio\n", 0);
            }
        }
        else
        {
            saidaTexto("Nao foi possivel ler. Erro: {0:u}\n", ret);
        }
    }
    Item_Fecha(item);
    Mem_Local_Libera(temp);
    return ret;
}

Status_t _Comando_CmdMonta(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    Pos_t arg1 = _Comando_IgnoraEspacosConst(args, 0, argsTam);
    Pos_t arg1Fim = _Comando_BuscaEspacosConst(args, arg1, argsTam);
    Pos_t arg1Tam = arg1Fim - arg1;
    Pos_t arg2 = _Comando_IgnoraEspacosConst(args, arg1Fim, argsTam);
    Pos_t arg2Fim = argsTam;
    Pos_t arg2Tam = arg2Fim - arg2;
    if((arg1 == arg1Fim) |(arg2 == arg2Fim))
    {
        saidaTexto("Uso: monta [Sistema de Arquivos] [Unidade]", 0);
        return STATUS_FORMATO_INVALIDO;
    }
    Status_t ret = Unidade_Monta(args + arg1, arg1Tam, args + arg2, arg2Tam);
    switch (ret)
    {
        case STATUS_OK:
        {
            saidaTexto("Montado com sucesso", 0);
            break;
        }
        case STATUS_SISARQ_INVALIDO:
        {
            saidaTexto("Sistema de arquivos invalido", 0);
            break;
        }
        case STATUS_UNIDADE_INVALIDA:
        {
            saidaTexto("Unidade invalida", 0);
            break;
        }
        case STATUS_FORMATO_INVALIDO:
        {
            saidaTexto("Formatacao incompativel, verifique o sistema de arquivos", 0);
            break;
        }
        case STATUS_NAO_IMPLEMENTADO:
        {
            saidaTexto("Funcao nao implementada neste sistema de arquivos", 0);
            break;
        }
        default:
        {
            saidaTexto("Erro: {0:u}", ret);
            break;
        }
    }
    return ret;
}

Status_t _Comando_CmdFormata(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    Pos_t arg1 = _Comando_IgnoraEspacosConst(args, 0, argsTam);
    Pos_t arg1Fim = _Comando_BuscaEspacosConst(args, arg1, argsTam);
    Pos_t arg1Tam = arg1Fim - arg1;
    Pos_t arg2 = _Comando_IgnoraEspacosConst(args, arg1Fim, argsTam);
    Pos_t arg2Fim = argsTam;
    Pos_t arg2Tam = arg2Fim - arg2;
    if((arg1 == arg1Fim) |(arg2 == arg2Fim))
    {
        saidaTexto("Uso: formata [Sistema de Arquivos] [Unidade]", 0);
        return STATUS_FORMATO_INVALIDO;
    }
    Status_t ret = Unidade_Formata(args + arg1, arg1Tam, args + arg2, arg2Tam);
    switch (ret)
    {
        case STATUS_OK:
        {
            saidaTexto("Formatado com sucesso", 0);
            break;
        }
        case STATUS_SISARQ_INVALIDO:
        {
            saidaTexto("Sistema de arquivos invalido", 0);
            break;
        }
        case STATUS_UNIDADE_INVALIDA:
        {
            saidaTexto("Unidade invalida", 0);
            break;
        }
        case STATUS_FORMATO_INVALIDO:
        {
            saidaTexto("Formatacao incompativel, verifique o sistema de arquivos", 0);
            break;
        }
        case STATUS_NAO_IMPLEMENTADO:
        {
            saidaTexto("Funcao nao implementada neste sistema de arquivos", 0);
            break;
        }
        default:
        {
            saidaTexto("Erro: {0:u}", ret);
            break;
        }
    }
    return ret;
}

Status_t _Comando_CmdPrincipal(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    SByte_t nome[65];
    nome[64] = 0;
    Pos_t qtd = Unidade_Quantidade();
    for (Pos_t i = 0; i < qtd; i++)
    {
        Unidade_LeiaNomeConst(i, nome, 64);
        if((Const_TamLimitado(nome,64) == argsTam) && Const_Igual(nome, args, argsTam))
        {
            saidaTexto("Alterado para [{0:C}]\n", (Tam_t)nome);
            return Unidade_DefinePrincipal(i);
        }
    }
    saidaTexto("Nao encontrado [{0:C}]\n", (Tam_t)nome);
    return STATUS_NAO_ENCONTRADO;
}

Status_t _Comando_CmdMontaP(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    Pos_t arg1 = _Comando_IgnoraEspacosConst(args, 0, argsTam);
    Pos_t arg1Fim = _Comando_BuscaEspacosConst(args, arg1, argsTam);
    Pos_t arg2 = _Comando_IgnoraEspacosConst(args, arg1Fim, argsTam);
    Pos_t arg2Fim = argsTam;
    Pos_t arg2Tam = arg2Fim - arg2;
    if((arg1 == arg1Fim) |(arg2 == arg2Fim))
    {
        saidaTexto("Uso: montap [Sistema de Arquivos] [Unidade]", 0);
        return STATUS_FORMATO_INVALIDO;
    }
    Status_t ret = _Comando_CmdMonta(args, argsTam, saidaTexto);
    if(ret != STATUS_OK) return ret;
    saidaTexto("\n",0);
    ret = _Comando_CmdPrincipal(args + arg2, arg2Tam, saidaTexto);
    return ret;
}

Status_t _Comando_CmdCriaArq(SByte_t * args, Tam_t argsTam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0))
{
    if(argsTam == 0)
    {
        saidaTexto("Informe o nome do arquivo com endereço completo",0);
        return STATUS_FORMATO_INVALIDO;
    }
    Status_t ret = Item_CriaArquivoConst(args, argsTam);
    switch (ret)
    {
        case STATUS_OK:
        {
            saidaTexto("Arquivo criado com sucesso",0);
            break;
        }
        default:
        {
            saidaTexto("Erro: {0:u}", ret);
            break;
        }
    }
    return ret;
}

void Comando()
{
    for (Pos_t i = 0; i < COMANDO_LISTA_TAM; i++)
    {
        _Comando_Lista[i].Ativo = NAO;
    }
    
    Comando_RegistraConst("ajuda", "Exibe os comandos suportados", &_Comando_CmdAjuda);
    Comando_RegistraConst("escreva", "Escreve um texto na tela", &_Comando_CmdEscreva);
    Comando_RegistraConst("escreval", "Escreve um texto na tela e quebra a linha", &_Comando_CmdEscrevaL);
    Comando_RegistraConst("msg", "Gerencia as mensagens do Nucleo", &_Comando_CmdMsg);
    Comando_RegistraConst("ver", "Exibe a versao do Nucleo", &_Comando_CmdVer);
    Comando_RegistraConst("tempo", "Exibe a quanto tempo a maquina esta ativa", &_Comando_CmdTempo);
    Comando_RegistraConst("livre", "Exibe a quanto tem de memoria disponivel para alocacao", &_Comando_CmdLivre);
    Comando_RegistraConst("teclado", "Funcoes do modulo de teclado", &_Comando_CmdTeclado);
    Comando_RegistraConst("lista", "Lista o conteudo de um diretorio", &_Comando_CmdLista);
    Comando_RegistraConst("binario", "Lista o conteudo de um arquivo binario", &_Comando_CmdBinario);
    Comando_RegistraConst("texto", "Lista o conteudo de um arquivo de texto", &_Comando_CmdTexto);
    Comando_RegistraConst("unidades", "Lista as unidades existentes", &_Comando_CmdUnidades);
    Comando_RegistraConst("bloco", "Exibe um bloco", &_Comando_CmdBloco);
    Comando_RegistraConst("monta", "Monta uma unidade", &_Comando_CmdMonta);
    Comando_RegistraConst("montap", "Monta uma unidade como principal", &_Comando_CmdMontaP);
    Comando_RegistraConst("formata", "Formata uma unidade", &_Comando_CmdFormata);
    Comando_RegistraConst("principal", "Altera a unidade principal", &_Comando_CmdPrincipal);
    Comando_RegistraConst("criaarq", "Cria um arquivo", &_Comando_CmdCriaArq);
}