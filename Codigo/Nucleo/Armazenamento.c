#include <HUSIS.h>

#ifdef ARMAZENAMENTO_ATA
#include "Armazenamento/ATA.c"
#endif

#ifdef ARMAZENAMENTO_DISQUETE
#include "Armazenamento/Disquete.c"
#endif

void Armazenamento()
{
    #ifdef ARMAZENAMENTO_ATA
    ATA();
    #endif
    #ifdef ARMAZENAMENTO_DISQUETE
    Disquete();
    #endif
}