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

void Cronometro()
{
    IRQ_Registra(0, &Cronometro_Processa);

    UInt_t divisao = 1193180 / 100;
    ES_EscrevaByte(0x43, 0x36);
    ES_EscrevaByte(0x40, divisao & 0xff);
    ES_EscrevaByte(0x40, divisao >> 8);
    __asm__ __volatile__ ("sti");
}