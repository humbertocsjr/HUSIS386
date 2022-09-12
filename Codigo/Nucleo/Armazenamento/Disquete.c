#include <HUSIS.h>

#define DISQUETE_PORTA_E__STATUS_A 0
#define DISQUETE_PORTA_E__STATUS_B 1
#define DISQUETE_PORTA_ES_SAIDA_DIGITAL 2
#define DISQUETE_PORTA_ES_DISP_FITA 3
#define DISQUETE_PORTA__S_SELETOR_VELOCIDADE 4
#define DISQUETE_PORTA_ES_FILA_DADOS 5
#define DISQUETE_PORTA_E__ENTRADA_DIGITAL 6
#define DISQUETE_PORTA__S_CONTROLE_CONFIG 7
#define DISQUETE_PORTA_E__STATUS_PRINCIPAL 8

#define DISQUETE_MASCARA_SAIDA_DIGITAL_MOTORA 0x10
#define DISQUETE_MASCARA_SAIDA_DIGITAL_MOTORB 0x20
#define DISQUETE_MASCARA_SAIDA_DIGITAL_MOTORC 0x40
#define DISQUETE_MASCARA_SAIDA_DIGITAL_MOTORD 0x80
#define DISQUETE_MASCARA_SAIDA_DIGITAL_IRQ 0x08
#define DISQUETE_MASCARA_SAIDA_DIGITAL_REDEFINE 0x04
#define DISQUETE_MASCARA_SAIDA_DIGITAL_SELECIONA0 0x02
#define DISQUETE_MASCARA_SAIDA_DIGITAL_SELECIONA1 0x01

#define DISQUETE_MASCARA_STATUS_PRONTO 0x80 // RQM
#define DISQUETE_MASCARA_STATUS_PRONTO_LEITURA 0x40 // DIO
#define DISQUETE_MASCARA_STATUS_SEM_DMA 0x20 // NDMA
#define DISQUETE_MASCARA_STATUS_MOVENDOA 0x01 // ACTA
#define DISQUETE_MASCARA_STATUS_MOVENDOB 0x02 // ACTB
#define DISQUETE_MASCARA_STATUS_MOVENDOC 0x04 // ACTC
#define DISQUETE_MASCARA_STATUS_MOVENDOD 0x08 // ACTD

#define DISQUETE_INFO_CILINDROS 0
#define DISQUETE_INFO_CABECAS 1
#define DISQUETE_INFO_SETORES 2
#define DISQUETE_INFO_SETORES_POR_CILINDRO 3
#define DISQUETE_INFO_TOTAL_BLOCOS 4
#define DISQUETE_INFO_VERSAO 5

#define DISQUETE_CMD_LEIA_TRILHA 2
#define DISQUETE_CMD_DEFINE_PARAMS 3
#define DISQUETE_CMD_STATUS_DISP 4  
#define DISQUETE_CMD_ESCREVA_DADOS 5
#define DISQUETE_CMD_LEIA_DADOS 6
#define DISQUETE_CMD_RECALIBRAR 7
#define DISQUETE_CMD_STATUS_INTERRUPCAO 8
#define DISQUETE_CMD_ESCREVA_DADOS_EXCLUIDOS 9
#define DISQUETE_CMD_LEIA_ID 10
#define DISQUETE_CMD_LEIA_DADOS_EXCLUIDOS 12
#define DISQUETE_CMD_FORMATA_TRILHA 13
#define DISQUETE_CMD_EXTRAI_REG 14
#define DISQUETE_CMD_VA_PARA 15
#define DISQUETE_CMD_VERSAO 16
#define DISQUETE_CMD_ESCANEIE_IGUAL 17
#define DISQUETE_CMD_MODO_PERPENDICULAR 18
#define DISQUETE_CMD_CONFIGURAR 19 
#define DISQUETE_CMD_BLOQUEIO 20
#define DISQUETE_CMD_VERIFICA 22
#define DISQUETE_CMD_ESCANEIA_BAIXO_OU_IGUAL 25
#define DISQUETE_CMD_ESCANEIA_ALTO_OU_IGUAL 29

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

Boleano_t _Disquete_ExibeErro = SIM;
Byte_t * _Disquete_Bloco = 0;
Pos_t _Disquete_Posicao = 0;
Boleano_t _Disquete_Leitura = NAO;
Boleano_t _Disquete_Gravacao = NAO;
Boleano_t _Disquete_Pronto = NAO;

Status_t _Disquete_EnviaComando(Pos_t disp, Byte_t cmd, Byte_t paramsQtd, Byte_t param1, Byte_t param2, Byte_t param3, Boleano_t escrita, Byte_t * dados, Tam_t tam)
{
    if(paramsQtd > 3) return STATUS_QUANTIDADE_INVALIDA;
    Status_t ret = STATUS_OK;
    Byte_t status = 0;
    Pos_t pos = 0;
    Byte_t valor = 0;
    // Envia comando
    do
    {
        ret = Dispositivo_LeiaBytePorta(disp, DISQUETE_PORTA_E__STATUS_PRINCIPAL, &status);
        if(ret != STATUS_OK) return ret;
    } while (!((status & DISQUETE_MASCARA_STATUS_PRONTO) & (!(status & DISQUETE_MASCARA_STATUS_PRONTO_LEITURA))));
    ret = Dispositivo_EscrevaBytePorta(disp, DISQUETE_PORTA_ES_FILA_DADOS, cmd); 
    if(ret != STATUS_OK) return ret;
    // Envia Parametros
    if(paramsQtd >= 1)
    {
        do
        {
            ret = Dispositivo_LeiaBytePorta(disp, DISQUETE_PORTA_E__STATUS_PRINCIPAL, &status);
            if(ret != STATUS_OK) return ret;
        } while (!((status & DISQUETE_MASCARA_STATUS_PRONTO) & (!(status & DISQUETE_MASCARA_STATUS_PRONTO_LEITURA))));
        ret = Dispositivo_EscrevaBytePorta(disp, DISQUETE_PORTA_ES_FILA_DADOS, param1); 
        if(ret != STATUS_OK) return ret;
    }
    if(paramsQtd >= 2)
    {
        do
        {
            ret = Dispositivo_LeiaBytePorta(disp, DISQUETE_PORTA_E__STATUS_PRINCIPAL, &status);
            if(ret != STATUS_OK) return ret;
        } while (!((status & DISQUETE_MASCARA_STATUS_PRONTO) & (!(status & DISQUETE_MASCARA_STATUS_PRONTO_LEITURA))));
        ret = Dispositivo_EscrevaBytePorta(disp, DISQUETE_PORTA_ES_FILA_DADOS, param2); 
        if(ret != STATUS_OK) return ret;
    }
    if(paramsQtd >= 3)
    {
        do
        {
            ret = Dispositivo_LeiaBytePorta(disp, DISQUETE_PORTA_E__STATUS_PRINCIPAL, &status);
            if(ret != STATUS_OK) return ret;
        } while (!((status & DISQUETE_MASCARA_STATUS_PRONTO) & (!(status & DISQUETE_MASCARA_STATUS_PRONTO_LEITURA))));
        ret = Dispositivo_EscrevaBytePorta(disp, DISQUETE_PORTA_ES_FILA_DADOS, param3); 
        if(ret != STATUS_OK) return ret;
    }
    // Verifica fase de execucao
    ret = Dispositivo_LeiaBytePorta(disp, DISQUETE_PORTA_E__STATUS_PRINCIPAL, &status);
    if(ret != STATUS_OK) return ret;
    if(ret & DISQUETE_MASCARA_STATUS_SEM_DMA)
    {
        // Fase Execucao
        pos = 0;
        do
        {
            do
            {
                ret = Dispositivo_LeiaBytePorta(disp, DISQUETE_PORTA_E__STATUS_PRINCIPAL, &status);
                if(ret != STATUS_OK) return ret;
            } while (!(status & DISQUETE_MASCARA_STATUS_PRONTO_LEITURA));
            do
            {
                if(escrita)
                {
                    valor = 0;
                    if(pos < tam) valor = dados[pos];
                    ret = Dispositivo_EscrevaBytePorta(disp, DISQUETE_PORTA_ES_FILA_DADOS, valor);
                    if(ret != STATUS_OK) return ret;
                }
                else
                {
                    ret = Dispositivo_LeiaBytePorta(disp, DISQUETE_PORTA_ES_FILA_DADOS, &valor);
                    if(ret != STATUS_OK) return ret;
                    if(pos < tam) dados[pos] = valor;
                }
                ret = Dispositivo_LeiaBytePorta(disp, DISQUETE_PORTA_E__STATUS_PRINCIPAL, &status);
                if(ret != STATUS_OK) return ret;
                pos++;
            } while ((status & DISQUETE_MASCARA_STATUS_PRONTO_LEITURA) & (status & DISQUETE_MASCARA_STATUS_SEM_DMA));
        } while ((status & DISQUETE_MASCARA_STATUS_SEM_DMA) & (pos < tam));
    }
    return STATUS_OK;
}


void Disquete_IRQ6(Regs_t * regs)
{

}

Tam_t Disquete_Leia(Dispositivo_t *dispositivo, Pos_t posicao, Byte_t *destino, Tam_t tam)
{
    return 0;
}

Tam_t Disquete_Escreva(Dispositivo_t *disp, Pos_t posicao, Byte_t *origem, Tam_t tam)
{
    return 0;
}

Status_t Disquete_Registra(Pos_t posicao, Byte_t tipoCMOS, Pos_t * dispositivo)
{
    Pos_t disp = 0;
    Status_t ret = STATUS_OK;
    Boleano_t micro;
    Tam_t cilindros;
    Tam_t cabecas;
    Tam_t setores;
    Tam_t totalBlocos;
    switch(tipoCMOS)
    {
        case 1:
        {
            micro = NAO;
            cilindros = 40;
            cabecas = 2;
            setores = 9;
            totalBlocos = 720;
            Mensagem("Disquete", "Encontrado MiniDisquete (5 1/4) de 360 KiB na Posicao {0:u}", posicao);
            break;
        }
        case 2:
        {
            micro = NAO;
            cilindros = 80;
            cabecas = 2;
            setores = 18;
            totalBlocos = 2400;
            Mensagem("Disquete", "Encontrado MiniDisquete (5 1/4) de 1200 KiB na Posicao {0:u}", posicao);
            break;
        }
        case 3:
        {
            micro = SIM;
            cilindros = 40;
            cabecas = 2;
            setores = 18;
            totalBlocos = 1440;
            Mensagem("Disquete", "Encontrado MiniDisquete (3 1/2) de 720 KiB na Posicao {0:u}", posicao);
            break;
        }
        case 4:
        {
            micro = SIM;
            cilindros = 80;
            cabecas = 2;
            setores = 18;
            totalBlocos = 2880;
            Mensagem("Disquete", "Encontrado MiniDisquete (3 1/2) de 1440 KiB na Posicao {0:u}", posicao);
            break;
        }
        default:
        {
            *dispositivo = 0;
            return STATUS_DISPOSITIVO_INVALIDO;
        }
    }
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
    Dispositivo_DefinePorta(disp, DISQUETE_PORTA_E__STATUS_PRINCIPAL, 0x3f4);
    Dispositivo_DefineInfo(disp, DISQUETE_INFO_CILINDROS, cilindros);
    Dispositivo_DefineInfo(disp, DISQUETE_INFO_CABECAS, cabecas);
    Dispositivo_DefineInfo(disp, DISQUETE_INFO_SETORES, setores);
    Dispositivo_DefineInfo(disp, DISQUETE_INFO_SETORES_POR_CILINDRO, setores * cabecas);
    Dispositivo_DefineInfo(disp, DISQUETE_INFO_TOTAL_BLOCOS, totalBlocos);
    Byte_t valor = 0;
    // Desliga os motores
    Dispositivo_LeiaBytePorta(disp, DISQUETE_PORTA_ES_SAIDA_DIGITAL, &valor);
    valor &= 0xf;
    Dispositivo_EscrevaBytePorta(disp, DISQUETE_PORTA_ES_SAIDA_DIGITAL, valor);
    *dispositivo = disp;
    return ret;
}

void Disquete()
{
    Pos_t disp = 0;
    Byte_t cmos = CMOS_LeiaRegistrador(16);
    Byte_t cmosA = (cmos >> 4) & 0xf;
    Byte_t cmosB = cmos & 0xf;

    IRQ_Registra(6, &Disquete_IRQ6);
    
    if(cmosA != 0) Disquete_Registra(0, cmosA, &disp);
    if(cmosB != 0) Disquete_Registra(1, cmosB, &disp);


}