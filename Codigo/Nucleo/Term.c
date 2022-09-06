#include <HUSIS.h>

Term_t _Term_Fisico;

Term_t * _Term_Atual()
{
    return &_Term_Fisico;
}

void _Term_Fisico_Atualiza(void * obj)
{
    Term_t * term = (Term_t *)obj;
    if(term->X >= term->Largura)
    {
        term->X = 0;
        term->Y++;
    }
    if(term->Y >= term->Altura)
    {
        term->Y = term->Altura - 1;
        for (Pos_t y = 1; y < term->Altura; y++)
        {
            Mem_CopiaBinario((Byte_t *)(term->Conteudo + (y-1) * term->Largura), (Byte_t *)(term->Conteudo + y * term->Largura), term->Largura * 2);
        }
        UShort_t valor = (term->CorFundo << 12) | (term->CorFrente << 8) | ' ';
        Mem_RepeteUShort(term->Conteudo + term->Y * term->Largura, valor, term->Largura);
    }
    Pos_t pos = term->Y * term->Largura + term->X;
    ES_EscrevaByte(0x3D4, 14);
    ES_EscrevaByte(0x3D5, pos >> 8);
    ES_EscrevaByte(0x3D4, 15);
    ES_EscrevaByte(0x3D5, pos);
}

void Term()
{
    _Term_Fisico.Conteudo = (UShort_t *) 0xb8000;
    _Term_Fisico.Altura = 25;
    _Term_Fisico.Largura = 80;
    _Term_Fisico.X = 0;
    _Term_Fisico.Y = 0;
    _Term_Fisico.CorFrente = 15;
    _Term_Fisico.CorFundo = 0;
    _Term_Fisico.Atualiza = &_Term_Fisico_Atualiza;
    for (Pos_t i = 0; i < 0xf; i++)
    {
        _Term_Fisico.EntradaUso[i] = NAO;
    }
    _Term_Fisico.EntradaInicio = 0;
    _Term_Fisico.EntradaFim = 0;
}

void Term_Limpa(Byte_t cor_fundo, Byte_t cor_frente, Byte_t caractere)
{
    Term_t * term = _Term_Atual();
    UShort_t valor = (cor_fundo << 12) | (cor_frente << 8) | caractere;
    for (Pos_t y = 0; y < term->Altura; y++)
    {
        Mem_RepeteUShort(term->Conteudo + y * term->Largura, valor, term->Largura);
    }
    term->X = 0;
    term->Y = 0;
    term->CorFrente = cor_frente;
    term->CorFundo = cor_fundo;
    term->Atualiza((void *)term);
}

void Term_EscrevaC(Byte_t caractere)
{
    Term_t * term = _Term_Atual();
    if(term->X >= term->Largura)
    {
        term->X = 0;
        term->Y++;
    }
    if(term->Y >= term->Altura)
    {
        term->Atualiza((void *)term);
    }
    Pos_t pos = term->Y * term->Largura + term->X;
    switch(caractere)
    {
        case 8:
        {
            if(term->X != 0) term->X--;
            break;
        }
        case 9:
        {
            term->X = (term->X + 8) & ~(8 - 1);
            break;
        }
        case 13:
        {
            term->X = 0;
            break;
        }
        case 10:
        {
            term->X = 0;
            term->Y++;
            break;
        }
        default:
        {
            if(caractere >= ' ')
            {
                term->Conteudo[pos] = (term->CorFundo << 12) | (term->CorFrente << 8) | caractere;
                term->X++;
            }
            break;
        }
    }
    term->Atualiza((void *)term);
}

void Term_EscrevaConst(SByte_t * textoConstante, Tam_t tam)
{
    for (Tam_t i = 0; i < tam; i++)
    {
        if(textoConstante[i] == 0) break;
        Term_EscrevaC((Byte_t)(textoConstante[i]));
    }
}

void Term_EscrevaConstAuto(SByte_t * textoConstante)
{
    Term_EscrevaConst(textoConstante, Const_Tam(textoConstante));
}

void Term_EscrevaSimNao(Boleano_t valor)
{
    if(valor)
    {
        Term_EscrevaConstAuto("Sim");
    }
    else
    {
        Term_EscrevaConstAuto("Nao");
    }
}

void Term_EscrevaVerdadeiroFalso(Boleano_t valor)
{
    if(valor)
    {
        Term_EscrevaConstAuto("Verdadeiro");
    }
    else
    {
        Term_EscrevaConstAuto("Falso");
    }
}

void Term_EscrevaHex(UInt_t valor)
{
    const Byte_t hex[] = "0123456789ABCDEF";
    UInt_t tmp = valor;
    Byte_t c = 0;
    for (Pos_t i = 0; i < 8; i++)
    {
        c = hex[tmp >> 28];
        tmp = tmp << 4;
        Term_EscrevaC(c);
    }
}

void Term_EscrevaHexByte(Byte_t valor)
{
    const Byte_t hex[] = "0123456789ABCDEF";
    Byte_t tmp = valor;
    Byte_t c = 0;
    for (Pos_t i = 0; i < 2; i++)
    {
        c = hex[tmp >> 4];
        tmp = tmp << 4;
        Term_EscrevaC(c);
    }
}

void Term_EscrevaUInt(UInt_t valor)
{
    UInt_t tmp;
    Byte_t c = 0;
    if(valor > 0)
    {
        c = valor % 10;
        tmp = valor / 10;
        if(tmp > 0) Term_EscrevaUInt(tmp);
    }
    Term_EscrevaC(c + '0');
}

void Term_EscrevaInt(Int_t valor)
{
    UInt_t tmp = (UInt_t)valor;
    if(valor < 0)
    {
        tmp = (UInt_t)(-valor);
        Term_EscrevaC('-');
    }
    Term_EscrevaUInt(tmp);
}

Status_t Term_EscrevaFmt(SByte_t * constanteFmt, Tam_t qtd, Tam_t v0, Tam_t v1, Tam_t v2, Tam_t v3, Tam_t v4, Tam_t v5, Tam_t v6, Tam_t v7, Tam_t v8, Tam_t v9)
{
    Tam_t nro = 0;
    if(qtd > 10) return STATUS_QUANTIDADE_INVALIDA;
    while(*constanteFmt != 0)
    {
        if(*constanteFmt == '\\')
        {
            constanteFmt++;
            if(*constanteFmt == 0) return STATUS_FORMATO_INVALIDO;
            switch (*constanteFmt)
            {
                case 'n':
                {
                    Term_EscrevaC(10);
                    
                    break;
                }
                case 'r':
                {
                    Term_EscrevaC(13);
                    break;
                }
                case 't':
                {
                    Term_EscrevaC(9);
                    break;
                }
                default:
                {
                    Term_EscrevaC(*constanteFmt);
                    break;
                }
            }
        }
        else if(*constanteFmt == '{')
        {
            nro = 0;
            constanteFmt++;
            while(Caract_EhNumero(*constanteFmt))
            {
                nro = (nro * 10) + (*constanteFmt - '0');
                constanteFmt++;
            }
            if(*constanteFmt == 0) return STATUS_FORMATO_INVALIDO;
            switch(nro)
            {
                case 0:
                {
                    nro = v0;
                    break;
                }
                case 1:
                {
                    nro = v1;
                    break;
                }
                case 2:
                {
                    nro = v2;
                    break;
                }
                case 3:
                {
                    nro = v3;
                    break;
                }
                case 4:
                {
                    nro = v4;
                    break;
                }
                case 5:
                {
                    nro = v5;
                    break;
                }
                case 6:
                {
                    nro = v6;
                    break;
                }
                case 7:
                {
                    nro = v7;
                    break;
                }
                case 8:
                {
                    nro = v8;
                    break;
                }
                case 9:
                {
                    nro = v9;
                    break;
                }
                default:
                {
                    return STATUS_POSICAO_INVALIDA;
                }
            }
            if((*constanteFmt != ':') & (*constanteFmt != '}')) return STATUS_FORMATO_INVALIDO;
            if(*constanteFmt == ':')
            {
                constanteFmt++;
                if(*constanteFmt == 0) return STATUS_FORMATO_INVALIDO;
                switch (*constanteFmt)
                {
                    case 'c':
                    {
                        Term_EscrevaC((Byte_t)nro);
                        break;
                    }
                    case 'i':
                    {
                        Term_EscrevaInt(nro);
                        break;
                    }
                    case 'u':
                    {
                        Term_EscrevaUInt(nro);
                        break;
                    }
                    case 'C':
                    {
                        Term_EscrevaConstAuto((SByte_t *)nro);
                        break;
                    }
                    case 'H':
                    {
                        Term_EscrevaHex(nro);
                        break;
                    }
                    case 'h':
                    {
                        Term_EscrevaHexByte(nro);
                        break;
                    }
                    case 's':
                    {
                        Term_EscrevaSimNao(nro);
                        break;
                    }
                    case 'v':
                    {
                        Term_EscrevaVerdadeiroFalso(nro);
                        break;
                    }
                    default:
                    {
                        return STATUS_FORMATO_INVALIDO;
                    }
                }
                constanteFmt++;
                if(*constanteFmt != '}') return STATUS_FORMATO_INVALIDO;
            }
            else
            {
                Term_EscrevaInt(nro);
            }
        }
        else
        {
            Term_EscrevaC(*constanteFmt);
        }
        constanteFmt++;
    }
    return STATUS_OK;
}

Status_t Term_EscrevaFmt0(SByte_t * constanteTexto)
{
    return Term_EscrevaFmt(constanteTexto, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

Status_t Term_EscrevaFmt1(SByte_t * constanteTexto, Tam_t v0)
{
    return Term_EscrevaFmt(constanteTexto, 1, v0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

Status_t Term_EscrevaFmt2(SByte_t * constanteTexto, Tam_t v0, Tam_t v1)
{
    return Term_EscrevaFmt(constanteTexto, 2, v0, v1, 0, 0, 0, 0, 0, 0, 0, 0);
}

Status_t Term_EscrevaFmt3(SByte_t * constanteTexto, Tam_t v0, Tam_t v1, Tam_t v2)
{
    return Term_EscrevaFmt(constanteTexto, 3, v0, v1, v2, 0, 0, 0, 0, 0, 0, 0);
}

Status_t Term_EscrevaFmt4(SByte_t * constanteTexto, Tam_t v0, Tam_t v1, Tam_t v2, Tam_t v3)
{
    return Term_EscrevaFmt(constanteTexto, 4, v0, v1, v2, v3, 0, 0, 0, 0, 0, 0);
}

Status_t Term_EscrevaFmt5(SByte_t * constanteTexto, Tam_t v0, Tam_t v1, Tam_t v2, Tam_t v3, Tam_t v4)
{
    return Term_EscrevaFmt(constanteTexto, 5, v0, v1, v2, v3, v4, 0, 0, 0, 0, 0);
}

Status_t Term_EscrevaFmt6(SByte_t * constanteTexto, Tam_t v0, Tam_t v1, Tam_t v2, Tam_t v3, Tam_t v4, Tam_t v5)
{
    return Term_EscrevaFmt(constanteTexto, 6, v0, v1, v2, v3, v4, v5, 0, 0, 0, 0);
}

Status_t Term_EscrevaFmt7(SByte_t * constanteTexto, Tam_t v0, Tam_t v1, Tam_t v2, Tam_t v3, Tam_t v4, Tam_t v5, Tam_t v6)
{
    return Term_EscrevaFmt(constanteTexto, 7, v0, v1, v2, v3, v4, v5, v6, 0, 0, 0);
}

Status_t Term_EscrevaFmt8(SByte_t * constanteTexto, Tam_t v0, Tam_t v1, Tam_t v2, Tam_t v3, Tam_t v4, Tam_t v5, Tam_t v6, Tam_t v7)
{
    return Term_EscrevaFmt(constanteTexto, 8, v0, v1, v2, v3, v4, v5, v6, v7, 0, 0);
}

Status_t Term_EscrevaFmt9(SByte_t * constanteTexto, Tam_t v0, Tam_t v1, Tam_t v2, Tam_t v3, Tam_t v4, Tam_t v5, Tam_t v6, Tam_t v7, Tam_t v8)
{
    return Term_EscrevaFmt(constanteTexto, 9, v0, v1, v2, v3, v4, v5, v6, v7, v8, 0);
}

Status_t Term_EscrevaFmt10(SByte_t * constanteTexto, Tam_t v0, Tam_t v1, Tam_t v2, Tam_t v3, Tam_t v4, Tam_t v5, Tam_t v6, Tam_t v7, Tam_t v8, Tam_t v9)
{
    return Term_EscrevaFmt(constanteTexto, 10, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
}

void Term_IrPara(Byte_t x, Byte_t y)
{
    Term_t * term = _Term_Atual();
    term->X = x;
    term->Y = y;
    term->Atualiza((void*)term);
}

Byte_t Term_Largura()
{
    Term_t * term = _Term_Atual();
    return term->Largura;
}

Byte_t Term_Altura()
{
    Term_t * term = _Term_Atual();
    return term->Altura;
}

Byte_t Term_LeiaCorFundo()
{
    Term_t * term = _Term_Atual();
    return term->CorFundo;
}

Byte_t Term_LeiaCorFrente()
{
    Term_t * term = _Term_Atual();
    return term->CorFrente;
}

void Term_AlteraCorFundo(Byte_t cor)
{
    Term_t * term = _Term_Atual();
    term->CorFundo = cor;
}

void Term_AlteraCorFrente(Byte_t cor)
{
    Term_t * term = _Term_Atual();
    term->CorFrente = cor;
}

void Term_AlteraCor(Byte_t cor_fundo, Byte_t cor_frente)
{
    Term_t * term = _Term_Atual();
    term->CorFundo = cor_fundo;
    term->CorFrente = cor_frente;
}

Status_t Term_ProcessaTeclado (Tecla_t tecla, Byte_t ascii, Boleano_t shift, Boleano_t alt, Boleano_t ctrl, Boleano_t ext, Boleano_t pressionado)
{
    Term_t * term = _Term_Atual();
    if(pressionado & (ascii > 0))
    {
        if(!term->EntradaUso[term->EntradaFim])
        {
            term->EntradaTemp[term->EntradaFim] = ascii;
            term->EntradaUso[term->EntradaFim] = SIM;
            term->EntradaFim = ((term->EntradaFim + 1) &0xf);
        }
    }
    return STATUS_OK;
}

Byte_t Term_LeiaC()
{
    Byte_t ret = 0;
    Term_t * term = _Term_Atual();
    if(term->EntradaUso[term->EntradaInicio])
    {
        ret = term->EntradaTemp[term->EntradaInicio];
        term->EntradaUso[term->EntradaInicio] =NAO;
        term->EntradaInicio = ((term->EntradaInicio + 1) &0xf);
    }
    return ret;
}

Tam_t Term_LeiaLinhaConst(SByte_t * destino, Tam_t tam)
{
    Byte_t caractere = 0;
    Tam_t ret = 0;
    if(tam == 0) return 0;
    destino[0] = 0;
    while (SIM)
    {
        caractere = Term_LeiaC();
        if(caractere == '\n')
        {
            Term_EscrevaC(caractere);
            return ret;
        }
        else if(caractere == 8)
        {
            if(ret > 0)
            {
                Term_EscrevaC(caractere);
                Term_EscrevaC(' ');
                Term_EscrevaC(caractere);
                ret--;
                destino[ret] = 0;
            }   
        }
        else if((caractere >= ' ') | (caractere == '\t'))
        {
            if((ret + 1) < tam)
            {
                Term_EscrevaC(caractere);
                destino[ret] = caractere;
                destino[ret + 1] = 0;
                ret++;
            }
        }
        __asm__ __volatile__ ("hlt");
    }
    
}