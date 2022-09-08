#include <HUSIS.h>

volatile UInt_t _Cronometro_Contador = 0;
volatile UInt_t _Cronometro_Segundos = 0;
volatile UInt_t _Cronometro_Minutos = 0;
volatile UInt_t _Cronometro_Horas = 0;

void Cronometro_Processa(Regs_t * regs)
{
    _Cronometro_Contador++;
    if(_Cronometro_Contador % 100 == 0)
    {
        _Cronometro_Segundos ++;
        if(_Cronometro_Segundos % 60 == 0)
        {
            _Cronometro_Segundos = 0;
            _Cronometro_Minutos ++;
            if(_Cronometro_Minutos % 60 == 0)
            {
                _Cronometro_Minutos = 0;
                _Cronometro_Horas ++;
            }
        }
    }

    if(_Cronometro_Contador % 5 == 0)
    {
        Multitarefa_Cronometro(regs);
    }
}


void Cronometro_Espera(UInt_t centesimos)
{
    UInt_t tempo;
 
    tempo = _Cronometro_Contador + centesimos;
    while(_Cronometro_Contador < tempo) 
    {
        __asm__ __volatile__ ("hlt");
    }
}

Tam_t Cronometro_Contador()
{
    return _Cronometro_Contador;
}

Tam_t Cronometro_Segundos()
{
    return _Cronometro_Segundos;
}

Tam_t Cronometro_Minutos()
{
    return _Cronometro_Minutos;
}

Tam_t Cronometro_Horas()
{
    return _Cronometro_Horas;
}

Tam_t _Cronometro_Leia(void * disp, Pos_t posicao, Byte_t * destino, Tam_t tam)
{
    Tam_t ret = 0;
    switch (posicao)
    {
        case 0:
        {
            switch (((Dispositivo_t *)disp)->Identificador)
            {
                case 0:
                {
                    if(tam < 4) return 0;
                    ((UInt_t *) destino)[0] = _Cronometro_Contador;
                    ret = 4;
                    break;
                }
                case 1:
                {
                    if(tam < 12) return 0;
                    if(Const_DeNumero((SByte_t *)destino, _Cronometro_Contador, tam) == STATUS_OK)
                    {
                        ret = Const_TamLimitado((SByte_t *)destino, tam);
                    }
                    break;
                }
            }
            break;
        }
        default:
        {
            ret = 0;
        }
    }
    return ret;
}

Tam_t _Cronometro_Escreva(void * disp, Pos_t posicao, Byte_t * origem, Tam_t tam)
{
    return 0;
}

void Cronometro()
{
    IRQ_Registra(0, &Cronometro_Processa);

    UInt_t divisao = 1193180 / 100;
    ES_EscrevaByte(0x43, 0x36);
    ES_EscrevaByte(0x40, divisao & 0xff);
    ES_EscrevaByte(0x40, divisao >> 8);
    __asm__ __volatile__ ("sti");

    Pos_t disp = 0;
    Dispositivo_Registra(&disp, 0, "Cronometro Binario", NAO, 0, 0, 4, &_Cronometro_Leia, &_Cronometro_Escreva);
    Dispositivo_Registra(&disp, 0, "Cronometro ASCII", NAO, 0, 1, 12, &_Cronometro_Leia, &_Cronometro_Escreva);
}