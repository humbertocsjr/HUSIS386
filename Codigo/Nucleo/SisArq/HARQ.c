#include <HUSIS.h>

#ifdef SISARQ_HARQ


Status_t HARQ_Monta(Pos_t unidade)
{
    return STATUS_NAO_IMPLEMENTADO;

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
            Mem_CopiaBinario(bloco + 480, (Byte_t *)"HARQ", 4);
            tam = Dispositivo_Escreva(disp, 0, bloco, 512);
            if(tam == 0) ret = STATUS_NAO_FOI_POSSIVEL_ESCREVER;
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
        if(Const_Igual((SByte_t *)bloco + 480, "HARQ", 4))
        {
            ret = STATUS_OK;
        }
        else
        {
            MensagemConst("HARQ", "Assinatura: ", (SByte_t *)bloco+480, 4);
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

#endif