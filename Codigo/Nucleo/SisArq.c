#include <HUSIS.h>

#ifdef SISARQ_HARQ
#include "SisArq/HARQ.c"
#endif

void SisArq()
{
    #ifdef SISARQ_HARQ
    HARQ();
    #endif
}