#include <HUSIS.h>

typedef struct 
{
    Boleano_t Ocupado;
    UShort_t Posicao;
} Mem_Local_Registro_t;

Mem_Local_Registro_t * _Mem_Local = 0;
Tam_t _Mem_Local_Tam = 0;
Byte_t * _Mem_Local_Inicio = 0;

Pos_t * _Mem_Diretorio = 0;
Pos_t * _Mem_Tabela = 0;
Pos_t _Mem_Total_Paginas = 0;

extern Tam_t _Mem_LeiaNucleoCodigo();
extern Tam_t _Mem_LeiaNucleoBSS();
extern Tam_t _Mem_LeiaNucleoFim();
extern Tam_t _Mem_LeiaCR0();
extern void _Mem_EscrevaCR0(Tam_t novo);
extern Tam_t _Mem_LeiaCR3();
extern void _Mem_EscrevaCR3(Tam_t novo);

void Mem_CopiaBinario(Byte_t * destino, Byte_t * origem, Tam_t tam)
{
    for (Tam_t i = 0; i < tam; i++)
    {
        destino[i] = origem[i];
    }
}

void Mem_RepeteByte(Byte_t * destino, Byte_t valor, Tam_t quantidade)
{
    for (Tam_t i = 0; i < quantidade; i++)
    {
        destino[i] = valor;
    }
}

void Mem_RepeteUShort(UShort_t * destino, UShort_t valor, Tam_t quantidade)
{
    for (Tam_t i = 0; i < quantidade; i++)
    {
        destino[i] = valor;
    }
}

Byte_t * Mem_Local_Aloca(Tam_t tam)
{
    Boleano_t encontrado = NAO;
    Boleano_t verificando = NAO;
    Pos_t inicio = 0;
    Pos_t fim = 0;
    if(tam > 0xffffff) return 0;
    for (Pos_t i = 1; i < _Mem_Local_Tam; i++)
    {
        if(_Mem_Local[i].Ocupado)
        {
            verificando = NAO;
        }
        else if(!verificando)
        {
            verificando = SIM;
            inicio = i;
            if(tam == 1)
            {
                fim = i;
                encontrado = SIM;
                 break;
            }
        }
        else if((i-inicio) >= tam)
        {
            fim = i;
            encontrado = SIM;
            break;
        }
    }
    if(!encontrado) return 0;
    for (Pos_t i = inicio; i < fim; i++)
    {
        _Mem_Local[i].Ocupado = SIM;
        _Mem_Local[i].Posicao = i - inicio;
    }
    
    return (Byte_t *)(inicio * 256 + _Mem_Local_Inicio);
}

Tam_t Mem_Local_Tam(Byte_t * endereco)
{
    Pos_t posicao = (Pos_t)endereco / 256 - (Pos_t)_Mem_Local_Inicio;
    Pos_t anterior = _Mem_Local[posicao].Posicao;
    for (Pos_t i = posicao + 1; i < _Mem_Local_Tam; i++)
    {
        if(_Mem_Local[i].Posicao == 0) break;
        anterior = _Mem_Local[i].Posicao;
    }
    return (anterior + 1) * 256;
}

Tam_t Mem_Local_Espaco_Livre()
{
    Tam_t ret = 0;
    for (Pos_t i = 0; i < _Mem_Local_Tam; i++)
    {
        if(!_Mem_Local[i].Ocupado) ret++;
    }
    return ret * 256;
}

Status_t Mem_Local_Libera(Byte_t * endereco)
{
    Pos_t posicao = (Pos_t)endereco / 256 - (Pos_t)_Mem_Local_Inicio;
    if(_Mem_Local[posicao].Ocupado & (_Mem_Local[posicao].Posicao == 0))
    {
        _Mem_Local[posicao].Ocupado = NAO;
        for (Pos_t i = posicao + 1; i < _Mem_Local_Tam; i++)
        {
            if(_Mem_Local[i].Posicao == 0) break;
            _Mem_Local[i].Ocupado = NAO;
        }
    }
    else
    {
        return STATUS_POSICAO_INVALIDA;
    }
    return STATUS_OK;
}

void Mem()
{
    Mensagem3("Mem", "Nucleo: Codigo: {0:u} BSS: {1:u} Fim: {2:u}", _Mem_LeiaNucleoCodigo(), _Mem_LeiaNucleoBSS(), _Mem_LeiaNucleoFim());

    /* Inicia memoria local */
    _Mem_Local = (Mem_Local_Registro_t *)_Mem_LeiaNucleoFim();
    _Mem_Local_Tam = 8192;

    _Mem_Local_Inicio = (Byte_t *)(_Mem_LeiaNucleoFim() + (sizeof(Mem_Local_Registro_t) * _Mem_Local_Tam));

    for (Pos_t i = 0; i < _Mem_Local_Tam; i++)
    {
        _Mem_Local[i].Ocupado = NAO;
    }
    
    Mensagem("Mem", "Memoria de Nucleo: {0:u} KiB", Mem_Local_Espaco_Livre() / 1024);

    /* Define o tamanho da memoria em paginas*/    
    _Mem_Total_Paginas = Multiboot_MemoriaAltaPaginas();

    _Mem_Diretorio = (Pos_t *)((Multiboot_MemoriaBaixaPaginas() - 3) * 4096);
    _Mem_Tabela = (Pos_t *)((Multiboot_MemoriaBaixaPaginas() - 2) * 4096);

    /* Inicializa todos os registros */
    for (Tam_t i = 0; i < 1024; i++)
    {
        _Mem_Tabela[i] = (i * 4096) | 3;
        if(i == 0)
        {
            _Mem_Diretorio[i] = (Tam_t)_Mem_Tabela | 3;
        }
        else
        {
            _Mem_Diretorio[i] = 0 | 2;
        }
    }

    /* Registra e ativa a paginacao */
    _Mem_EscrevaCR3((Tam_t)_Mem_Diretorio);
    _Mem_EscrevaCR0(_Mem_LeiaCR0() | 0x80000000);
    
    

}