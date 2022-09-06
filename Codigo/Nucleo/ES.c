#include <HUSIS.h>

/* Le um byte de uma porta */
Byte_t ES_LeiaByte (UShort_t porta)
{
    Byte_t ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (ret) : "dN" (porta));
    return ret;
}

/* Escreve um byte em uma porta */
void ES_EscrevaByte (UShort_t porta, Byte_t dados)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (porta), "a" (dados));
}
/* Le um uint16 de uma porta */
UShort_t ES_LeiaUShort (UShort_t porta)
{
    UShort_t ret;
    __asm__ __volatile__ ("inw %1, %0" : "=a" (ret) : "dN" (porta));
    return ret;
}

/* Escreve um uint16 em uma porta */
void ES_EscrevaUShort (UShort_t porta, UShort_t dados)
{
    __asm__ __volatile__ ("outw %1, %0" : : "dN" (porta), "a" (dados));
}