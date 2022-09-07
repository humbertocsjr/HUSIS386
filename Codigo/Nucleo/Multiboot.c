#include <HUSIS.h>

typedef struct __attribute__ ((__packed__)) 
{
    UInt_t Marcadores;
    UInt_t MemoriaBaixa;
    UInt_t MemoriaAlta;
    UInt_t DispositivoInicial;
    SByte_t * Argumentos;

} Multiboot_t;

extern Multiboot_t * _Multiboot_Leia();

Multiboot_t _Multiboot_Copia;
SByte_t _Multiboot_ArgumentoVazio[1] = 
{
    0
};


Tam_t Multiboot_MemoriaBaixaKiB()
{
    return _Multiboot_Copia.MemoriaBaixa;
}

Tam_t Multiboot_MemoriaBaixaPaginas()
{
    return _Multiboot_Copia.MemoriaBaixa / 4;
}

Tam_t Multiboot_MemoriaAltaKiB()
{
    return _Multiboot_Copia.MemoriaAlta;
}

Tam_t Multiboot_MemoriaAltaPaginas()
{
    return _Multiboot_Copia.MemoriaAlta / 4 - 1;
}

SByte_t * Multiboot_ArgsConst()
{
    return _Multiboot_Copia.Argumentos;
}

void Multiboot()
{
    Mem_CopiaBinario((Byte_t *) &_Multiboot_Copia, (Byte_t *) _Multiboot_Leia(), sizeof(Multiboot_t));
    if(!(_Multiboot_Copia.Marcadores & 1))
    {
        _Multiboot_Copia.MemoriaBaixa = 639;
        _Multiboot_Copia.MemoriaAlta = 1024;
    }
    if(!(_Multiboot_Copia.Marcadores & 2))
    {
        _Multiboot_Copia.DispositivoInicial = 0;
    }
    if(!(_Multiboot_Copia.Marcadores & 4))
    {
        _Multiboot_Copia.Argumentos = _Multiboot_ArgumentoVazio;
    }
}