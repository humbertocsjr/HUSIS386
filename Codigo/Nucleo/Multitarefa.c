#include <HUSIS.h>

Boleano_t _Multitarefa_Suspensa = SIM;

__volatile__ Pos_t _Multitarefa_ProcessoAtual = 0;
__volatile__ UInt_t _Multitarefa_Ponteiro = 0;
__volatile__ UInt_t _Multitarefa_Pilha = 0;
__volatile__ Pos_t * _Multitarefa_Diretorio = 0;
__volatile__ Pos_t _Multitarefa_CR0 = 0;


extern void _Multitarefa_MudaPilha(Pos_t pilhaTopo);
extern Pos_t _Multitarefa_LeiaPilha();

__volatile__ void Multitarefa_Cronometro(Regs_t * regs)
{
    if(_Multitarefa_Suspensa) return;
    Processo_Info_t * atual = Processo_Leia(_Multitarefa_ProcessoAtual);
    Processo_Info_t * prox = Processo_Proximo(_Multitarefa_ProcessoAtual);
    if(prox == 0) return;
    __asm__ __volatile__ ("cli");
    __asm__ __volatile__ ("mov %%esp, %0": "=r" (_Multitarefa_Pilha));
    __asm__ __volatile__ ("mov %%ebp, %0": "=r" (_Multitarefa_Ponteiro));
    atual->DiretorioPaginacao = Mem_DiretorioAtual();
    atual->Pilha = _Multitarefa_Pilha;
    atual->Ponteiro = _Multitarefa_Ponteiro;
    _Multitarefa_Pilha = prox->Pilha;
    _Multitarefa_Ponteiro = prox->Ponteiro;
    _Multitarefa_Diretorio = prox->DiretorioPaginacao;
    __asm__ __volatile__ ("mov %0, %%cr3": : "r" (_Multitarefa_Diretorio)); 
    __asm__ __volatile__ ("mov %%cr0, %0": "=r"(_Multitarefa_CR0));
    __asm__ __volatile__ ("mov %0, %%cr0":: "r" (_Multitarefa_CR0 | 0x80000000));
    __asm__ __volatile__ ("mov %0, %%esp": : "r" (_Multitarefa_Pilha));
    __asm__ __volatile__ ("mov %0, %%ebp": : "r" (_Multitarefa_Ponteiro));
    __asm__ __volatile__ ("sti");
}

void Multitarefa_Habilita()
{
    _Multitarefa_Suspensa = NAO;
}

void Multitarefa_Desabilita()
{
    _Multitarefa_Suspensa = SIM;
}

void Multitarefa()
{

}