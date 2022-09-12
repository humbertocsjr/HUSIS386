#include <HUSIS.h>

#ifdef COMUNICACAO_SERIAL
#include "Comunicacao/Serial.c"
#endif

void Comunicacao()
{
    #ifdef COMUNICACAO_SERIAL
    Serial();
    #endif
}