#include <HUSIS.h>

#define SERIAL_PORTA_DADOS 0
#define SERIAL_PORTA_INTERRUPCAO 1
#define SERIAL_PORTA_INTERRUPCAO_IDENTIFICACAO 2
#define SERIAL_PORTA_FILA_DADOS 3
#define SERIAL_PORTA_CONTROLE_LINHA 4
#define SERIAL_PORTA_CONTROLE_MODEM 5
#define SERIAL_PORTA_ESTADO_LINHA 6
#define SERIAL_PORTA_ESTADO_MODEM 7
#define SERIAL_PORTA_ETC 8
#define SERIAL_PORTA_DLAB0 9
#define SERIAL_PORTA_DLAB1 10

#define SERIAL_INFO_VELOCIDADE 0
#define SERIAL_INFO_BITS_DADOS 1
#define SERIAL_INFO_BITS_PARADA 2
#define SERIAL_INFO_PARIEDADE 1

UShort_t _Serial_CalcularVelocidade(Tam_t velocidade)
{
    Tam_t ret = 115200 / velocidade;
    return ret;
}

Byte_t _Serial_CalcularBitsDados(Pos_t bits)
{
    switch (bits)
    {
        case 5:
        {
            return 0;
        }
        case 6:
        {
            return 1;
        }
        case 7:
        {
            return 2;
        }
        case 8:
        {
            return 3;
        }
        default:
        {
            return 3;
        }
    }
}

Byte_t _Serial_CalcularBitsDadosInverso(Pos_t bits)
{
    switch (bits)
    {
        case 0:
        {
            return 5;
        }
        case 1:
        {
            return 6;
        }
        case 2:
        {
            return 7;
        }
        case 3:
        {
            return 8;
        }
        default:
        {
            return 8;
        }
    }
}

Byte_t _Serial_CalcularBitsParada(Pos_t bits)
{
    switch (bits)
    {
        case 1:
        {
            return 0;
        }
        case 2:
        {
            return 4;
        }
        default:
        {
            return 0;
        }
    }
}

Byte_t _Serial_CalcularBitsParadaInverso(Pos_t bits)
{
    switch (bits)
    {
        case 0:
        {
            return 1;
        }
        case 4:
        {
            return 2;
        }
        default:
        {
            return 1;
        }
    }
}

Byte_t _Serial_CalcularPariedade(Pos_t bits)
{
    switch (bits)
    {
        case 0:
        {
            return 0;
        }
        case 1:
        {
            return 0x8;
        }
        case 2:
        {
            return 0x18;
        }
        case 3:
        {
            return 0x28;
        }
        case 4:
        {
            return 0x38;
        }
        default:
        {
            return 0;
        }
    }
}

Byte_t _Serial_CalcularPariedadeInverso(Pos_t bits)
{
    switch (bits)
    {
        case 0:
        {
            return 0;
        }
        case 0x8:
        {
            return 1;
        }
        case 0x18:
        {
            return 2;
        }
        case 0x28:
        {
            return 3;
        }
        case 0x38:
        {
            return 4;
        }
        default:
        {
            return 0;
        }
    }
}

void _Serial_AtivaDLAB(Pos_t disp)
{
    Byte_t valor = 0;
    Dispositivo_LeiaBytePorta(disp, SERIAL_PORTA_CONTROLE_LINHA, &valor);
    valor |= 0x80;
    Dispositivo_EscrevaBytePorta(disp, SERIAL_PORTA_CONTROLE_LINHA, valor);
}

void _Serial_DesativaDLAB(Pos_t disp)
{
    Byte_t valor = 0;
    Dispositivo_LeiaBytePorta(disp, SERIAL_PORTA_CONTROLE_LINHA, &valor);
    valor &= 0x7F;
    Dispositivo_EscrevaBytePorta(disp, SERIAL_PORTA_CONTROLE_LINHA, valor);
}

Tam_t Serial_Leia(Dispositivo_t *disp, Pos_t posicao, Byte_t *destino, Tam_t tam)
{
    Tam_t qtd = 0;
    Byte_t estado = 0;
    while(qtd < tam)
    {
        Dispositivo_LeiaBytePorta(disp->Dispositivo, SERIAL_PORTA_ESTADO_LINHA, &estado);
        if(!(estado & 1)) break;
        Dispositivo_LeiaBytePorta(disp->Dispositivo, SERIAL_PORTA_DADOS, &destino[qtd]);
        qtd++;
    }
    return qtd;
}

Tam_t Serial_Escreva(Dispositivo_t *disp, Pos_t posicao, Byte_t *origem, Tam_t tam)
{
    Tam_t qtd = 0;
    Byte_t estado = 0;
    while(qtd < tam)
    {
        Dispositivo_LeiaBytePorta(disp->Dispositivo, SERIAL_PORTA_ESTADO_LINHA, &estado);
        if(!(estado & 0x20)) break;
        Dispositivo_EscrevaBytePorta(disp->Dispositivo, SERIAL_PORTA_DADOS, origem[qtd]);
        qtd++;
    }
    return qtd;
}

Tam_t Serial_LeiaVelocidade(Dispositivo_t *disp, Pos_t posicao, Byte_t *destino, Tam_t tam)
{
    if(posicao != 0) return 0;
    UInt_t velocidade = 0;
    Dispositivo_LeiaInfo(disp->Dispositivo, SERIAL_INFO_VELOCIDADE, &velocidade);
    destino[0] = velocidade & 0xff;
    destino[1] = (velocidade >> 8) & 0xff;
    return 2;
}

Tam_t Serial_EscrevaVelocidade(Dispositivo_t *disp, Pos_t posicao, Byte_t *origem, Tam_t tam)
{
    if(posicao != 0) return 0;
    if(tam != 2) return 0;
    _Serial_AtivaDLAB(disp->Dispositivo);
    Dispositivo_EscrevaBytePorta(disp->Dispositivo, SERIAL_PORTA_DLAB0, origem[0]);
    Dispositivo_EscrevaBytePorta(disp->Dispositivo, SERIAL_PORTA_DLAB1, origem[1]);
    _Serial_DesativaDLAB(disp->Dispositivo);
    Dispositivo_DefineInfo(disp->Dispositivo, SERIAL_INFO_VELOCIDADE, origem[0] | (origem[1] << 8));
    return 2;
}

Tam_t Serial_LeiaBitsDados(Dispositivo_t *disp, Pos_t posicao, Byte_t *destino, Tam_t tam)
{
    if(posicao != 0) return 0;
    UInt_t dado = 0;
    Dispositivo_LeiaInfo(disp->Dispositivo, SERIAL_INFO_BITS_DADOS, &dado);
    destino[0] = _Serial_CalcularBitsDadosInverso(dado);
    return 1;
}

Tam_t Serial_EscrevaBitsDados(Dispositivo_t *disp, Pos_t posicao, Byte_t *origem, Tam_t tam)
{
    if(posicao != 0) return 0;
    if(tam != 1) return 0;
    Byte_t registradorLinha = 0;
    Dispositivo_DefineInfo(disp->Dispositivo, SERIAL_INFO_BITS_PARADA, origem[0]);
    Dispositivo_LeiaBytePorta(disp->Dispositivo, SERIAL_PORTA_CONTROLE_LINHA, &registradorLinha);
    registradorLinha &= 0xfc;
    registradorLinha |= _Serial_CalcularBitsDados(origem[0]);
    Dispositivo_EscrevaBytePorta(disp->Dispositivo, SERIAL_PORTA_CONTROLE_LINHA, registradorLinha);
    return 1;
}

Tam_t Serial_LeiaBitsParada(Dispositivo_t *disp, Pos_t posicao, Byte_t *destino, Tam_t tam)
{
    if(posicao != 0) return 0;
    UInt_t dado = 0;
    Dispositivo_LeiaInfo(disp->Dispositivo, SERIAL_INFO_BITS_PARADA, &dado);
    destino[0] = _Serial_CalcularBitsParadaInverso(dado);
    return 1;
}

Tam_t Serial_EscrevaBitsParada(Dispositivo_t *disp, Pos_t posicao, Byte_t *origem, Tam_t tam)
{
    if(posicao != 0) return 0;
    if(tam != 1) return 0;
    Byte_t registradorLinha = 0;
    Dispositivo_DefineInfo(disp->Dispositivo, SERIAL_INFO_BITS_PARADA, origem[0]);
    Dispositivo_LeiaBytePorta(disp->Dispositivo, SERIAL_PORTA_CONTROLE_LINHA, &registradorLinha);
    registradorLinha &= 0xfb;
    registradorLinha |= _Serial_CalcularBitsParada(origem[0]);
    Dispositivo_EscrevaBytePorta(disp->Dispositivo, SERIAL_PORTA_CONTROLE_LINHA, registradorLinha);
    return 1;
}

Tam_t Serial_LeiaPariedade(Dispositivo_t *disp, Pos_t posicao, Byte_t *destino, Tam_t tam)
{
    if(posicao != 0) return 0;
    UInt_t dado = 0;
    Dispositivo_LeiaInfo(disp->Dispositivo, SERIAL_INFO_PARIEDADE, &dado);
    destino[0] = _Serial_CalcularPariedadeInverso(dado);
    return 1;
}

Tam_t Serial_EscrevaPariedade(Dispositivo_t *disp, Pos_t posicao, Byte_t *origem, Tam_t tam)
{
    if(posicao != 0) return 0;
    if(tam != 1) return 0;
    Byte_t registradorLinha = 0;
    Dispositivo_DefineInfo(disp->Dispositivo, SERIAL_INFO_PARIEDADE, origem[0]);
    Dispositivo_LeiaBytePorta(disp->Dispositivo, SERIAL_PORTA_CONTROLE_LINHA, &registradorLinha);
    registradorLinha &= 0xc7;
    registradorLinha |= _Serial_CalcularPariedade(origem[0]);
    Dispositivo_EscrevaBytePorta(disp->Dispositivo, SERIAL_PORTA_CONTROLE_LINHA, registradorLinha);
    return 1;
}

Status_t Serial_Registra(Pos_t * disp, UShort_t porta, Pos_t id)
{
    Status_t ret = STATUS_OK;
    
    // Dispositivo que define a velocidade
    ret = Dispositivo_Registra(disp, 0, "Serial Velocidade ", SIM, id, id, 2, 1, &Serial_LeiaVelocidade, &Serial_EscrevaVelocidade);
    if(ret != STATUS_OK) return ret;
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_DADOS, porta);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_INTERRUPCAO, porta + 1);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_INTERRUPCAO_IDENTIFICACAO, porta + 2);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_CONTROLE_LINHA, porta + 3);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_CONTROLE_MODEM, porta + 4);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_ESTADO_LINHA, porta + 5);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_ESTADO_MODEM, porta + 6);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_ETC, porta + 7);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_DLAB0, porta);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_DLAB1, porta + 1);
    UShort_t velocidade = 2400;
    UShort_t velocidadeCalc = _Serial_CalcularVelocidade(velocidade);
    Dispositivo_DefineInfo(*disp, SERIAL_INFO_VELOCIDADE, velocidade);
    _Serial_AtivaDLAB(*disp);
    Dispositivo_EscrevaBytePorta(*disp, SERIAL_PORTA_DLAB0, velocidadeCalc & 0xff);
    Dispositivo_EscrevaBytePorta(*disp, SERIAL_PORTA_DLAB1, (velocidadeCalc >> 8) & 0xff);
    _Serial_DesativaDLAB(*disp);
    
    // Dispositivo que define a bits de dados
    ret = Dispositivo_Registra(disp, 0, "Serial Bits Dados ", SIM, id, id, 1, 1, &Serial_LeiaBitsDados, &Serial_EscrevaBitsDados);
    if(ret != STATUS_OK) return ret;
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_DADOS, porta);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_INTERRUPCAO, porta + 1);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_INTERRUPCAO_IDENTIFICACAO, porta + 2);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_CONTROLE_LINHA, porta + 3);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_CONTROLE_MODEM, porta + 4);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_ESTADO_LINHA, porta + 5);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_ESTADO_MODEM, porta + 6);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_ETC, porta + 7);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_DLAB0, porta);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_DLAB1, porta + 1);
    Pos_t bitDados = 8;
    Dispositivo_DefineInfo(*disp, SERIAL_INFO_BITS_DADOS, bitDados);
    Byte_t registradorLinha = 0;
    Dispositivo_LeiaBytePorta(*disp, SERIAL_PORTA_CONTROLE_LINHA, &registradorLinha);
    registradorLinha &= 0xfc;
    registradorLinha |= _Serial_CalcularBitsDados(bitDados);
    Dispositivo_EscrevaBytePorta(*disp, SERIAL_PORTA_CONTROLE_LINHA, registradorLinha);
    
    // Dispositivo que define a bits de parada
    ret = Dispositivo_Registra(disp, 0, "Serial Bits Parada ", SIM, id, id, 1, 1, &Serial_LeiaBitsParada, &Serial_EscrevaBitsParada);
    if(ret != STATUS_OK) return ret;
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_DADOS, porta);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_INTERRUPCAO, porta + 1);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_INTERRUPCAO_IDENTIFICACAO, porta + 2);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_CONTROLE_LINHA, porta + 3);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_CONTROLE_MODEM, porta + 4);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_ESTADO_LINHA, porta + 5);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_ESTADO_MODEM, porta + 6);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_ETC, porta + 7);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_DLAB0, porta);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_DLAB1, porta + 1);
    Pos_t bitParada = 1;
    Dispositivo_DefineInfo(*disp, SERIAL_INFO_BITS_PARADA, bitParada);
    Dispositivo_LeiaBytePorta(*disp, SERIAL_PORTA_CONTROLE_LINHA, &registradorLinha);
    registradorLinha &= 0xfb;
    registradorLinha |= _Serial_CalcularBitsParada(bitParada);
    Dispositivo_EscrevaBytePorta(*disp, SERIAL_PORTA_CONTROLE_LINHA, registradorLinha);
    
    // Dispositivo que define a bits de parada
    ret = Dispositivo_Registra(disp, 0, "Serial Pariedade ", SIM, id, id, 1, 1, &Serial_LeiaPariedade, &Serial_EscrevaPariedade);
    if(ret != STATUS_OK) return ret;
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_DADOS, porta);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_INTERRUPCAO, porta + 1);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_INTERRUPCAO_IDENTIFICACAO, porta + 2);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_CONTROLE_LINHA, porta + 3);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_CONTROLE_MODEM, porta + 4);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_ESTADO_LINHA, porta + 5);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_ESTADO_MODEM, porta + 6);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_ETC, porta + 7);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_DLAB0, porta);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_DLAB1, porta + 1);
    Pos_t pariedade = 0;
    Dispositivo_DefineInfo(*disp, SERIAL_INFO_PARIEDADE, pariedade);
    Dispositivo_LeiaBytePorta(*disp, SERIAL_PORTA_CONTROLE_LINHA, &registradorLinha);
    registradorLinha &= 0xc7;
    registradorLinha |= _Serial_CalcularPariedade(pariedade);
    Dispositivo_EscrevaBytePorta(*disp, SERIAL_PORTA_CONTROLE_LINHA, registradorLinha);

    // Dispositivo principal
    ret = Dispositivo_Registra(disp, 0, "Serial ", SIM, id, id, 1, 0, &Serial_Leia, &Serial_Escreva);
    if(ret != STATUS_OK) return ret;
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_DADOS, porta);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_INTERRUPCAO, porta + 1);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_INTERRUPCAO_IDENTIFICACAO, porta + 2);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_CONTROLE_LINHA, porta + 3);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_CONTROLE_MODEM, porta + 4);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_ESTADO_LINHA, porta + 5);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_ESTADO_MODEM, porta + 6);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_ETC, porta + 7);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_DLAB0, porta);
    Dispositivo_DefinePorta(*disp, SERIAL_PORTA_DLAB1, porta + 1);
    Dispositivo_EscrevaBytePorta(*disp, SERIAL_PORTA_INTERRUPCAO, 0);
    Dispositivo_EscrevaBytePorta(*disp, SERIAL_PORTA_INTERRUPCAO_IDENTIFICACAO, 0xc7);
    Dispositivo_EscrevaBytePorta(*disp, SERIAL_PORTA_CONTROLE_MODEM, 0x0b);
    Dispositivo_EscrevaBytePorta(*disp, SERIAL_PORTA_CONTROLE_MODEM, 0x1e);
    Byte_t teste = 0xae;
    Dispositivo_EscrevaBytePorta(*disp, SERIAL_PORTA_DADOS, teste);
    Dispositivo_LeiaBytePorta(*disp, SERIAL_PORTA_DADOS, &teste);
    if(teste != 0xae) return STATUS_DISPOSITIVO_INVALIDO;
    Dispositivo_EscrevaBytePorta(*disp, SERIAL_PORTA_CONTROLE_MODEM, 0xf);
    return STATUS_OK;
}

void Serial()
{
    Pos_t disp = 0;
    Serial_Registra(&disp, 0x3f8, 1);
    Serial_Registra(&disp, 0x2f8, 2);
}