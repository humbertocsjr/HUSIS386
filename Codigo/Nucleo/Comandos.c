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
}