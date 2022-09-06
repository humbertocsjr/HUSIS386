#include <HUSIS.h>

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

void Mem()
{
    Mensagem3("Mem", "Nucleo: Codigo: {0:u} BSS: {1:u} Fim: {2:u}", _Mem_LeiaNucleoCodigo(), _Mem_LeiaNucleoBSS(), _Mem_LeiaNucleoFim());

    /* Define o tamanho da memoria em paginas*/    
    _Mem_Total_Paginas = Multiboot_MemoriaAltaPaginas();

    _Mem_Diretorio = (Pos_t *)((Multiboot_MemoriaBaixaPaginas() - 2) * 4096);
    _Mem_Tabela = (Pos_t *)((Multiboot_MemoriaBaixaPaginas() - 1) * 4096);

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