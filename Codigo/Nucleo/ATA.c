#include <HUSIS.h>

#define ATA_PORTA_ES_DADOS 0
#define ATA_PORTA_E__ERRO 1
#define ATA_PORTA__S_RECURSOS 2
#define ATA_PORTA_ES_QTD_SETORES 3
#define ATA_PORTA_ES_SETOR 4
#define ATA_PORTA_ES_CILINDRO_BAIXO 5
#define ATA_PORTA_ES_CILINDRO_ALTO 6
#define ATA_PORTA_ES_DISP_E_CABECA 7
#define ATA_PORTA_E__STATUS 8
#define ATA_PORTA__S_COMANDO 9
#define ATA_PORTA_E__STATUS_ALT 10
#define ATA_PORTA__S_CONTROLE_DISP 11
#define ATA_PORTA_E__ENDERECO_DISP 12

Status_t _ATA_Leia(void * disp, Pos_t posicao, Byte_t * destino, Tam_t tam )
{
    return STATUS_ACESSO_NEGADO;
}
Status_t _ATA_Escreva(void * disp, Pos_t posicao, Byte_t * origem, Tam_t tam )
{
    return STATUS_ACESSO_NEGADO;
}
Status_t _ATA_CriaDispositivo(UShort_t portaBase, UShort_t portaControleBase, Pos_t numero, Pos_t id)
{
    Pos_t disp = 0;
    Status_t ret = Dispositivo_Registra(&disp, 0, "ATA ", SIM, numero, id, 512, &_ATA_Leia, &_ATA_Escreva);
    Dispositivo_DefinePorta(disp, ATA_PORTA_ES_DADOS, portaBase + 0);
    Dispositivo_DefinePorta(disp, ATA_PORTA_E__ERRO, portaBase + 1);
    Dispositivo_DefinePorta(disp, ATA_PORTA__S_RECURSOS, portaBase + 1);
    Dispositivo_DefinePorta(disp, ATA_PORTA_ES_QTD_SETORES, portaBase + 2);
    Dispositivo_DefinePorta(disp, ATA_PORTA_ES_SETOR, portaBase + 3);
    Dispositivo_DefinePorta(disp, ATA_PORTA_ES_CILINDRO_BAIXO, portaBase + 4);
    Dispositivo_DefinePorta(disp, ATA_PORTA_ES_CILINDRO_ALTO, portaBase + 5);
    Dispositivo_DefinePorta(disp, ATA_PORTA_ES_DISP_E_CABECA, portaBase + 6);
    Dispositivo_DefinePorta(disp, ATA_PORTA_E__STATUS, portaBase + 7);
    Dispositivo_DefinePorta(disp, ATA_PORTA__S_COMANDO, portaBase + 7);
    Dispositivo_DefinePorta(disp, ATA_PORTA_E__STATUS_ALT, portaControleBase + 0);
    Dispositivo_DefinePorta(disp, ATA_PORTA__S_CONTROLE_DISP, portaControleBase + 1);
    Dispositivo_DefinePorta(disp, ATA_PORTA_E__ENDERECO_DISP, portaControleBase + 1);
    if(ret != STATUS_OK) return ret;
    return ret;
}

void ATA()
{
    _ATA_CriaDispositivo(0x1f0, 0x3f6, 0, 0);
    _ATA_CriaDispositivo(0x1f0, 0x3f6, 1, 1);
    _ATA_CriaDispositivo(0x170, 0x376, 2, 0);
    _ATA_CriaDispositivo(0x170, 0x376, 3, 1);
}