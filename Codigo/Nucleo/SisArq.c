#include <HUSIS.h>

#ifdef SISARQ_HARQ
#include "SisArq/HARQ16.c"
#endif

void SisArq()
{
    #ifdef SISARQ_HARQ
    HARQ16();
    #endif
}