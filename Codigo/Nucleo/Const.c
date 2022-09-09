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

Boleano_t Const_Igual(SByte_t * destino, SByte_t * origem, Tam_t tam)
{
    for (Tam_t i = 0; i < tam; i++)
    {
        if(origem[i] != destino[i]) return NAO;
        if(origem[i] == 0) return SIM;
    }
    return SIM;
}

void Const_Copia(SByte_t * destino, SByte_t * origem, Tam_t tam)
{
    for (Tam_t i = 0; i < tam; i++)
    {
        destino[i] = origem[i];
        if(destino[i] == 0) break;
    }
}

Status_t Const_ParaNumero(SByte_t * origem, Tam_t tam, UInt_t * valor)
{
    if(tam == 0) return STATUS_ESTOURO_DA_CAPACIDADE;
    *valor = 0;
    Boleano_t iniciado = NAO;
    for (Pos_t i = 0; i < tam; i++)
    {
        if(!iniciado)
        {
            if(origem[i] != ' ')
            {
                iniciado = SIM;
                if((origem[i] >= '0') & (origem[i] <= '9'))
                {
                    *valor = (origem[i] - '0');
                }
                else
                {
                    return STATUS_FORMATO_INVALIDO;
                }
            }
        }
        else if((origem[i] >= '0') & (origem[i] <= '9'))
        {
            *valor = *valor * 10 + (origem[i] - '0');
        }
        else 
        {
            break;
        }
    }
    return STATUS_OK;
}

Status_t Const_DeNumero(SByte_t * destino, UInt_t valor, Tam_t tam)
{
    if(tam == 0) return STATUS_ESTOURO_DA_CAPACIDADE;
    Status_t ret = STATUS_OK;
    UInt_t digito[10];
    UInt_t tmp = valor;
    Pos_t pos = 0;
    for (Pos_t i = 0; i < 10; i++)
    {
        digito[pos] = (tmp % 10) + '0';
        tmp /= 10;
        pos++;
        if(tmp == 0) break;
    }
    for (Pos_t i = 0; i < tam; i++)
    {
        destino[i] = 0;
    }
    if((pos + 1) >= tam) return STATUS_ESTOURO_DA_CAPACIDADE;
    for (Pos_t i = 0; i < pos; i++)
    {
        destino[i] = digito[pos - i - 1];
    }
    return ret;
}