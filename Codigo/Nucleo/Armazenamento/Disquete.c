#include <HUSIS.h>

#define DISQUETE_PORTA_E__STATUS_A 0
#define DISQUETE_PORTA_E__STATUS_B 1
#define DISQUETE_PORTA_ES_SAIDA_DIGITAL 2
#define DISQUETE_PORTA_ES_DISP_FITA 3
#define DISQUETE_PORTA__S_SELETOR_VELOCIDADE 4
#define DISQUETE_PORTA_ES_FILA_DADOS 5
#define DISQUETE_PORTA_E__ENTRADA_DIGITAL 6
#define DISQUETE_PORTA__S_CONTROLE_CONFIG 7

#define DISQUETE_MASCARA_SAIDA_DIGITAL_MOTORA 0x10
#define DISQUETE_MASCARA_SAIDA_DIGITAL_MOTORB 0x20
#define DISQUETE_MASCARA_SAIDA_DIGITAL_MOTORC 0x40
#define DISQUETE_MASCARA_SAIDA_DIGITAL_MOTORD 0x80
#define DISQUETE_MASCARA_SAIDA_DIGITAL_IRQ 0x08
#define DISQUETE_MASCARA_SAIDA_DIGITAL_REDEFINE 0x04
#define DISQUETE_MASCARA_SAIDA_DIGITAL_SELECIONA0 0x02
#define DISQUETE_MASCARA_SAIDA_DIGITAL_SELECIONA1 0x01

#define DISQUETE_MASCARA_STATUS_PRONTO 0x80
#define DISQUETE_MASCARA_STATUS_PRONTO_LEITURA 0x40
#define DISQUETE_MASCARA_STATUS_SEM_DMA 0x20
#define DISQUETE_MASCARA_STATUS_MOVENDOA 0x01
#define DISQUETE_MASCARA_STATUS_MOVENDOB 0x02
#define DISQUETE_MASCARA_STATUS_MOVENDOC 0x04
#define DISQUETE_MASCARA_STATUS_MOVENDOD 0x08

/*

Vai operar em mono PIO, lendo via IRQ6 com as seguintes etapas:

 - Define o endereco do bloco
 - Define a posicao como 0
 - Ativa Leitura ou Grvacao
 - Define Pronto como Nao
 - Envia o comando para o controlador 
 - Aguarda Pronto voltar a ser SIM

Em paralelo a IRQ6 sera chamada e lido/gravado no bloco incrementando a posicao,
quando completar 512 bytes lidos encerra a leitura e marca como Pronto 

*/

Byte_t * _Disquete_Bloco = 0;
Pos_t _Disquete_Posicao = 0;
Boleano_t _Disquete_Leitura = NAO;
Boleano_t _Disquete_Gravacao = NAO;
Boleano_t _Disquete_Pronto = NAO;

void Disquete_IRQ6(Regs_t * regs)
{

}

Tam_t Disquete_Leia(Dispositivo_t *disp, Pos_t posicao, Byte_t *destino, Tam_t tam)
{
    return 0;
}

Tam_t Disquete_Escreva(Dispositivo_t *disp, Pos_t posicao, Byte_t *origem, Tam_t tam)
{
    return 0;
}

Status_t Disquete_Registra(Pos_t posicao, Boleano_t micro, Pos_t * dispositivo)
{
    Pos_t disp = 0;
    Status_t ret = STATUS_OK;
    if(micro)
    {
        ret = Dispositivo_Registra(&disp, 0, "MicroDisquete ", SIM, posicao, posicao, 512, 2880, &Disquete_Leia, &Disquete_Escreva);
    }
    else
    {
        ret = Dispositivo_Registra(&disp, 0, "MiniDisquete ", SIM, posicao, posicao, 512, 2400, &Disquete_Leia, &Disquete_Escreva);
    }
    if(ret != STATUS_OK)
    {
        Mensagem2("Disquete", "Nao foi possivel registrar o Disquete {0:u} pelo erro {1:u}", posicao, ret);
        return ret;
    }
    Dispositivo_DefinePorta(disp, DISQUETE_PORTA_E__STATUS_A, 0x3f0);
    Dispositivo_DefinePorta(disp, DISQUETE_PORTA_E__STATUS_B, 0x3f1);
    Dispositivo_DefinePorta(disp, DISQUETE_PORTA_ES_SAIDA_DIGITAL, 0x3f2);
    Dispositivo_DefinePorta(disp, DISQUETE_PORTA_ES_DISP_FITA, 0x3f3);
    Dispositivo_DefinePorta(disp, DISQUETE_PORTA__S_SELETOR_VELOCIDADE, 0x3f4);
    Dispositivo_DefinePorta(disp, DISQUETE_PORTA_ES_FILA_DADOS, 0x3f5);
    Dispositivo_DefinePorta(disp, DISQUETE_PORTA_E__ENTRADA_DIGITAL, 0x3f7);
    Dispositivo_DefinePorta(disp, DISQUETE_PORTA__S_CONTROLE_CONFIG, 0x3f7);
    *dispositivo = disp;
    return ret;
}

void Disquete()
{
    Pos_t disp = 0;
    Mensagem("Disquete", "Registrando dispositivos", 0);
    Disquete_Registra(0, SIM, &disp);
    Disquete_Registra(1, SIM, &disp);
    Disquete_Registra(0, NAO, &disp);
    Disquete_Registra(1, NAO, &disp);

    IRQ_Registra(6, &Disquete_IRQ6);

    Byte_t valor = 0;

    // Desliga os motores
    Dispositivo_LeiaBytePorta(disp, DISQUETE_PORTA_ES_SAIDA_DIGITAL, &valor);
    valor &= 0xf;
    Dispositivo_EscrevaBytePorta(disp, DISQUETE_PORTA_ES_SAIDA_DIGITAL, valor);
}