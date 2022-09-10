#include <HUSIS.h>

#ifdef ARMAZENAMENTO_ATA
#include "Armazenamento/ATA.c"
#endif

void Armazenamento()
{
    #ifdef ARMAZENAMENTO_ATA
    ATA();
    #endif
}