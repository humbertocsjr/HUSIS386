#include <HUSIS.h>

#define SISARQ_HARQ

#include "SisArq/HARQ.c"

void SisArq()
{
    #ifdef SISARQ_HARQ
    HARQ();
    #endif
}