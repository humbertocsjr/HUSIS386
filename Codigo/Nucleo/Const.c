#include <HUSIS.h>

Tam_t Const_Tam(SByte_t * constanteTexto)
{
    Tam_t ret = 0;
    while(*constanteTexto != 0)
    {
        constanteTexto++;
        ret++;
    }
    return ret;
}

Tam_t Const_TamLimitado(SByte_t * constanteTexto, Tam_t capacidade)
{
    Tam_t ret = 0;
    while(*constanteTexto != 0 && ret < capacidade)
    {
        constanteTexto++;
        ret++;
    }
    return ret;
}

Boleano_t Const_Igual(SByte_t * origem, SByte_t * destino, Tam_t tam)
{
    for (Tam_t i = 0; i < tam; i++)
    {
        if(origem[i] != destino[i]) return NAO;
    }
    return SIM;
}