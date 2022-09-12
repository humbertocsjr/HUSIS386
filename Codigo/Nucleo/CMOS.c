#include <HUSIS.h>

Byte_t _CMOS_Tabela[128];
Boleano_t _CMOS_DesativaNMI = NAO;
Byte_t _CMOS_Segundo = 0;
Byte_t _CMOS_Minuto = 0;
Byte_t _CMOS_Hora = 0;
Byte_t _CMOS_Dia = 0;
Byte_t _CMOS_Mes = 0;
UShort_t _CMOS_Ano = 0;

__volatile__ void CMOS_Escreva(Pos_t registrador, Byte_t valor)
{
    if(registrador > 127) return;
    ES_EscrevaByte(0x70, registrador | ((_CMOS_DesativaNMI & 0x1) << 7));
    ES_EscrevaByte(0x71, valor);
}

__volatile__ Byte_t CMOS_Leia(Pos_t registrador)
{
    if(registrador > 127) return 0;
    Byte_t ret = 0;
    ES_EscrevaByte(0x70, registrador | ((_CMOS_DesativaNMI & 0x1) << 7));
    ret = ES_LeiaByte(0x71);
    return ret;
}

Byte_t _CMOS_LeiaBCD(Byte_t bcd)
{
    return (bcd & 0xf) + ((bcd / 16) * 10);
}

void _CMOS_AtualizaRelogio()
{
    Byte_t tmp, seg, min, hora, mes, dia, ampm;
    UShort_t ano;
    Boleano_t igual = NAO;
    while(CMOS_Leia(10) & 0x80) Cronometro_Espera(1);
    while(!igual)
    {
        igual = SIM;
        for (Pos_t i = 0; i < 12; i++)
        {
            tmp = CMOS_Leia(i);
            if(tmp != _CMOS_Tabela[i]) igual = NAO;
            _CMOS_Tabela[i] = tmp;
        }
    }
    // Le dados do CMOS
    seg = _CMOS_Tabela[0];
    min = _CMOS_Tabela[2];
    hora = _CMOS_Tabela[4];
    ampm = hora & 0x80;
    hora &= 0x7f;
    dia = _CMOS_Tabela[7];
    mes = _CMOS_Tabela[8];
    ano = _CMOS_Tabela[9];
    // Converte se estiver no formato BCD
    if(!(_CMOS_Tabela[11] & 0x4))
    {
        seg = _CMOS_LeiaBCD(seg);
        min = _CMOS_LeiaBCD(min);
        hora = _CMOS_LeiaBCD(hora);
        dia = _CMOS_LeiaBCD(dia);
        mes = _CMOS_LeiaBCD(mes);
        ano = _CMOS_LeiaBCD(ano);
    }
    // Como vai ser usado em maquinas antigas, nao confiar no seculo da CMOS
    if(ano < 80)
    {
        ano += 2000;
    }
    else
    {
        ano += 1990;
    }
    // Corrige para relogio de 24 horas
    if((!(_CMOS_Tabela[11] & 2)))
    {
        if(ampm)
        {
            hora += 12;
            if(hora >= 24) hora %= 24;
        }
    }
    // Grava na hora global
    _CMOS_Segundo = seg;
    _CMOS_Minuto = min;
    _CMOS_Hora = hora;
    _CMOS_Dia = dia;
    _CMOS_Mes = mes;
    _CMOS_Ano = ano;
}

void Relogio_LeiaHora(Byte_t * segundo, Byte_t * minuto, Byte_t * hora)
{
    _CMOS_AtualizaRelogio();
    *segundo = _CMOS_Segundo;
    *minuto = _CMOS_Minuto;
    *hora = _CMOS_Hora;
}

void Relogio_LeiaData(Byte_t * dia, Byte_t * mes, UShort_t * ano)
{
    _CMOS_AtualizaRelogio();
    *dia = _CMOS_Dia;
    *mes = _CMOS_Mes;
    *ano = _CMOS_Ano;
}

void Relogio_Leia(DataHora_t * datahora)
{
    _CMOS_AtualizaRelogio();
    datahora->Segundo = _CMOS_Segundo;
    datahora->Minuto = _CMOS_Minuto;
    datahora->Hora = _CMOS_Hora;
    datahora->Dia = _CMOS_Dia;
    datahora->Mes = _CMOS_Mes;
    datahora->Ano = _CMOS_Ano;
}

Byte_t CMOS_LeiaRegistrador(Pos_t registrador)
{
    if(registrador > 127) return 0;
    return _CMOS_Tabela[registrador];
}

void CMOS()
{
    for (Pos_t i = 0; i < 128; i++)
    {
        _CMOS_Tabela[i] = CMOS_Leia(i);
    }
}