#include <HUSIS.h>

Boleano_t Caract_EhNumero(Byte_t c)
{
    if((c >= '0') & (c <= '9'))
    {
        return SIM;
    }
    return NAO;
}