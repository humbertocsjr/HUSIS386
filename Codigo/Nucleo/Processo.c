#include <HUSIS.h>

Processo_Info_t _Processo_Lista[PROCESSO_CAPACIDADE];

Processo_Info_t * Processo_Cria()
{
    Processo_Info_t * ret = 0;
    Mem_Pagina_t * pilha = 0;
    Pos_t enderecoPilha = 32*1024*1024;
    for (Pos_t i = 0; i < PROCESSO_CAPACIDADE; i++)
    {
        if(!_Processo_Lista[i].Existe)
        {
            ret = &_Processo_Lista[i];
            break;
        }
    }
    if(ret == 0) return 0;
    ret->Suspenso = SIM;
    ret->Existe = SIM;
    ret->Binario = 0;
    ret->BinarioTam = 0;
    ret->DiretorioPaginacao = Mem_CriaDiretorio(SIM);
    pilha = Mem_Pagina(enderecoPilha, SIM, ret->DiretorioPaginacao);
    Mem_AlocaPagina(pilha, SIM, NAO);
    ret->PilhaBase = enderecoPilha;
    ret->PilhaTam = 4096;
    ret->Pilha = ret->PilhaBase + ret->PilhaTam;
    return ret;
}

Processo_Info_t * Processo_Leia(Processo_t processo)
{
    if(processo >= PROCESSO_CAPACIDADE) return 0;
    return &_Processo_Lista[processo];
}

Processo_Info_t * Processo_Proximo(Processo_t processo)
{
    Processo_Info_t * ret = 0;
    if(processo >= PROCESSO_CAPACIDADE) return 0;
    for (Processo_t i = processo + 1; i < PROCESSO_CAPACIDADE; i++)
    {
        if(_Processo_Lista[i].Existe & !_Processo_Lista[i].Suspenso)
        {
            ret  = &_Processo_Lista[i];
        }
    }
    if(ret == 0)
    for (Processo_t i = 0; i < PROCESSO_CAPACIDADE; i++)
    {
        if(_Processo_Lista[i].Existe & !_Processo_Lista[i].Suspenso)
        {
            ret  = &_Processo_Lista[i];
        }
    }
    return ret;
}

Processo_Info_t * Processo_Nucleo()
{
    return Processo_Leia(0);
}

Status_t Processo_Destroi(Processo_t processo)
{
    return STATUS_OK;
}

void Processo()
{
    for (Pos_t i = 0; i < PROCESSO_CAPACIDADE; i++)
    {
        _Processo_Lista[i].Processo = i;
        _Processo_Lista[i].Existe = NAO;
        _Processo_Lista[i].Suspenso = SIM;
    }
    /* A inicalização é feita pelo Modulo de Memoria */
}