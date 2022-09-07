#include <HUSIS.h>

typedef struct 
{
    Boleano_t Ocupado;
    UShort_t Posicao;
} Mem_Local_Registro_t;


Mem_Local_Registro_t * _Mem_Local = 0;
Tam_t _Mem_Local_Tam = 0;
Byte_t * _Mem_Local_Inicio = 0;
Byte_t * _Mem_Local_Fim = 0;

Pos_t * _Mem_Diretorio = 0;
Pos_t * _Mem_DiretorioAtual = 0;
Mem_Pagina_t * _Mem_Tabela = 0;
Tam_t _Mem_TotalPaginas = 0;

Byte_t * _Mem_Mapa;
Tam_t _Mem_MapaTam = 0;

extern Tam_t _Mem_LeiaNucleoCodigo();
extern Tam_t _Mem_LeiaNucleoBSS();
extern Tam_t _Mem_LeiaNucleoFim();
extern Tam_t _Mem_LeiaCR0();
extern void _Mem_EscrevaCR0(Tam_t novo);
extern Tam_t _Mem_LeiaCR2();
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
    Tam_t necessario = tam / 256;
    if((tam%256)!= 0) necessario++;
    if(tam > 0xffffff) return 0;
    for (Pos_t i = 0; i < _Mem_Local_Tam; i++)
    {
        if(_Mem_Local[i].Ocupado)
        {
            verificando = NAO;
        }
        else if(!verificando)
        {
            verificando = SIM;
            inicio = i;
            if(necessario == 1)
            {
                fim = i;
                encontrado = SIM;
                break;
            }
        }
        else if((i-inicio) >= necessario)
        {
            fim = i;
            encontrado = SIM;
            break;
        }
    }
    if(!encontrado) return 0;
    for (Pos_t i = inicio; i <= fim; i++)
    {
        _Mem_Local[i].Ocupado = SIM;
        _Mem_Local[i].Posicao = i - inicio;
    }
    Mem_RepeteByte((Byte_t *)(inicio * 256 + _Mem_Local_Inicio), 0, tam);
    return (Byte_t *)(inicio * 256 + _Mem_Local_Inicio);
}

Byte_t * Mem_Local_AlocaAlinhado(Tam_t tam)
{
    Boleano_t encontrado = NAO;
    Boleano_t verificando = NAO;
    Pos_t inicio = 0;
    Pos_t fim = 0;
    Tam_t necessario = tam / 256;
    if((tam%256)!= 0) necessario++;
    if(tam > 0xffffff) return 0;
    for (Pos_t i = 0; i < _Mem_Local_Tam; i++)
    {
        if(_Mem_Local[i].Ocupado)
        {
            verificando = NAO;
            i+=16;
            i&=0xfff0;
            i--;
        }
        else if(!verificando)
        {
            verificando = SIM;
            inicio = i;
            if(necessario == 1)
            {
                fim = i;
                encontrado = SIM;
                break;
            }
        }
        else if((i-inicio) >= necessario)
        {
            fim = i;
            encontrado = SIM;
            break;
        }
    }
    if(!encontrado) return 0;
    for (Pos_t i = inicio; i <= fim; i++)
    {
        _Mem_Local[i].Ocupado = SIM;
        _Mem_Local[i].Posicao = i - inicio;
    }
    Mem_RepeteByte((Byte_t *)(inicio * 256 + _Mem_Local_Inicio), 0, tam);
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

Tam_t Mem_Local_EspacoLivre()
{
    Tam_t ret = 0;
    for (Pos_t i = 0; i < _Mem_Local_Tam; i++)
    {
        if(!_Mem_Local[i].Ocupado) ret++;
    }
    return ret * 256;
}

Tam_t Mem_Local_Capacidade()
{
    return _Mem_Local_Tam * 256;
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

void _Mem_MarcarPaginas(Pos_t inicio, Pos_t fim)
{
    Pos_t item = 0;
    Pos_t sub = 0;
    for (Pos_t i = inicio; i <= fim; i++)
    {
        if(i >= _Mem_TotalPaginas) break;
        item = i / 8;
        sub = i % 8;
        _Mem_Mapa[item] |= (1 << sub);
    }   
}

void _Mem_DesmarcarPaginas(Pos_t inicio, Pos_t fim)
{
    Pos_t item = 0;
    Pos_t sub = 0;
    for (Pos_t i = inicio; i <= fim; i++)
    {
        if(i >= _Mem_TotalPaginas) break;
        item = i / 8;
        sub = i % 8;
        _Mem_Mapa[item] &= ~(1 << sub);
    }   
}

void Mem_AlocaPagina(Mem_Pagina_t * pagina, Boleano_t podeEscrever, Boleano_t pertenceAoNucleo)
{
    if(pagina->Posicao != 0) return;
    Pos_t pos = 0;
    Boleano_t encontrado = NAO;
    Pos_t item = 0;
    Pos_t sub = 0;
    for (Pos_t i = 0; i < _Mem_TotalPaginas; i++)
    {
        item = i / 8;
        sub = i % 8;
        if((_Mem_Mapa[item] & (1 << sub)) == 0)
        {
            pos = i; 
            encontrado = SIM;
            break;
        }
    }
    if(!encontrado) FalhaGrave("Mem", "Memoria cheia. Tentativa de alocar uma pagina",0 , 0);
    _Mem_MarcarPaginas(pos, pos);
    pagina->Posicao = pos;
    pagina->Presente = 1;
    pagina->Usuario = (!pertenceAoNucleo?1:0);
    pagina->Escrita = (podeEscrever?1:0);
}

void Mem_LiberaPagina(Mem_Pagina_t * pagina)
{
    if(pagina->Posicao == 0) return;
    _Mem_DesmarcarPaginas(pagina->Posicao, pagina->Posicao);
    pagina->Posicao = 0;
}

Tam_t Mem_TotalDePaginas()
{
    return _Mem_TotalPaginas;
}

Tam_t Mem_PaginasLivres()
{
    Tam_t ret = 0;
    Pos_t item = 0;
    Pos_t sub = 0;
    for (Pos_t i = 0; i <= _Mem_TotalPaginas; i++)
    {
        if(i >= _Mem_TotalPaginas) break;
        item = i / 8;
        sub = i % 8;
        if((_Mem_Mapa[item] & (1 << sub)) ==0)
        {
            ret++;
        }
    }   
    return ret;
}

void _Mem_DefineDiretorio(Pos_t * diretorio)
{
    _Mem_EscrevaCR3((Tam_t)diretorio);
    _Mem_EscrevaCR0(_Mem_LeiaCR0() | 0x80000000);
}

Pos_t * Mem_DiretorioAtual()
{
    return _Mem_DiretorioAtual;
}

Mem_Pagina_t * Mem_Pagina(UInt_t endereco, Boleano_t cria, Pos_t * diretorio)
{
    Mem_Pagina_t * ret = (Mem_Pagina_t*)0;
    Pos_t indice = endereco / 4096;
    Pos_t indiceDiretorio = indice / 1024;
    Pos_t indicePagina = indice % 1024;
    if(diretorio[indiceDiretorio] & 0xfffff000)
    {
        ret = &((Mem_Pagina_t*)(diretorio[indiceDiretorio] & 0xfffff000))[indicePagina];
    }
    else if(cria)
    {
        ret = (Mem_Pagina_t *)Mem_Local_AlocaAlinhado(4096);
        if(ret == 0) FalhaGrave("Mem", "Nao foi possivel alocar 4096 Bytes para a Tabela de Paginacao",0,0);
        diretorio[indiceDiretorio] = (Pos_t)((UInt_t)ret | 0x7);
        ret = &ret[indicePagina];
    }
    return ret;
}

void _Mem_FalhaPaginacao(Regs_t * regs)
{
    Pos_t endereco = _Mem_LeiaCR2();

    Boleano_t presente = regs->CodigoDeErro & 0x1;
    Boleano_t tentouEscrever = regs->CodigoDeErro & 0x2;
    Boleano_t usuario = regs->CodigoDeErro & 0x4;
    Boleano_t reservado = regs->CodigoDeErro & 0x8;

    Mensagem5("Mem", "FALHA DE PAGINACAO:\n Endereco: {0:u} / 0x{0:H}\n Tentou escrever: {1:s}\n Modo usuario: {2:s}\n Reservado: {3:s}\n Pagina existe: {4:s}", endereco, tentouEscrever, usuario, reservado, presente);
    FalhaGrave("Mem","Falha de Paginacao em {0:u} / 0x{0:H}", endereco, 0);
}

void Mem()
{
    Mensagem2("Mem", "Nucleo: Binario: {0:u} KiB | Total: {1:u} KiB", (_Mem_LeiaNucleoBSS() - _Mem_LeiaNucleoCodigo()) / 1024, (_Mem_LeiaNucleoFim() - _Mem_LeiaNucleoCodigo()) / 1024);

    /* Inicia memoria local */
    _Mem_Local = (Mem_Local_Registro_t *)_Mem_LeiaNucleoFim();
    if((Multiboot_MemoriaAltaKiB() / 1024) >= 2048)
    {
        _Mem_Local_Tam = 49152;
    }
    else if((Multiboot_MemoriaAltaKiB() / 1024) >= 1024)
    {
        _Mem_Local_Tam = 32768;
    }
    else if((Multiboot_MemoriaAltaKiB() / 1024) >= 512)
    {
        _Mem_Local_Tam = 16284;
    }
    else if((Multiboot_MemoriaAltaKiB() / 1024) >= 30)
    {
        _Mem_Local_Tam = 8192;
    }
    else if((Multiboot_MemoriaAltaKiB() / 1024) >= 15)
    {
        _Mem_Local_Tam = 4096;
    }
    else if((Multiboot_MemoriaAltaKiB() / 1024) >= 4)
    {
        _Mem_Local_Tam = 2048;
    }
    else
    {
        _Mem_Local_Tam = 1536;
    }

    _Mem_Local_Inicio = (Byte_t *)(_Mem_LeiaNucleoFim() + (sizeof(Mem_Local_Registro_t) * _Mem_Local_Tam));
    _Mem_Local_Fim = _Mem_Local_Inicio + (_Mem_Local_Tam * 256);

    for (Pos_t i = 0; i < _Mem_Local_Tam; i++)
    {
        _Mem_Local[i].Ocupado = NAO;
    }
    
    Mensagem("Mem", "Memoria de Nucleo: {0:u} KiB", Mem_Local_EspacoLivre() / 1024);

    /* Define o tamanho da memoria em paginas*/    
    _Mem_TotalPaginas = Multiboot_MemoriaAltaPaginas();

    _Mem_Diretorio = (Pos_t *)Mem_Local_AlocaAlinhado(4096);
    _Mem_DiretorioAtual = _Mem_Diretorio;
    if(_Mem_Diretorio == 0) FalhaGrave("Mem", "Nao foi possivel alocar 4096 Bytes para o Diretorio de Paginacao",0,0);
    _Mem_Tabela = (Mem_Pagina_t *)Mem_Local_AlocaAlinhado(4096);
    if(_Mem_Diretorio == 0) FalhaGrave("Mem", "Nao foi possivel alocar 4096 Bytes para a Tabela de Paginacao",0,0);


    /* Inicializa todos os registros */
    for (Tam_t i = 0; i < 1024; i++)
    {
        _Mem_Tabela[i].Presente = 1;
        _Mem_Tabela[i].Escrita = 1;
        _Mem_Tabela[i].Posicao = i;
        if(i == 0)
        {
            _Mem_Diretorio[i] = (Tam_t)_Mem_Tabela | 3;
        }
        else
        {
            _Mem_Diretorio[i] = 0 | 2;
        }
    }

    _Mem_MapaTam = _Mem_TotalPaginas / 8;
    _Mem_Mapa = (Byte_t *)Mem_Local_Aloca(_Mem_MapaTam);
    if(_Mem_Mapa == 0) FalhaGrave("Mem", "Nao foi possivel alocar {0:u} Bytes para o Mapa de Memoria",_Mem_MapaTam,0);

    for (Tam_t i = 0; i < _Mem_MapaTam; i++)
    {
        _Mem_Mapa[i] = 0;
    }
    /* Reserva os primeiros 64 KiB */
    _Mem_MarcarPaginas(0, 16);
    /* Reserva area reservada ente 640 KiB e 1 MiB */
    _Mem_MarcarPaginas(Multiboot_MemoriaBaixaPaginas() - 1, 256);
    /* Reserva Espaco do Nucleo */
    _Mem_MarcarPaginas(_Mem_LeiaNucleoCodigo() / 4096, (Pos_t)_Mem_Local_Fim / 4096 + 1);
    /* Reserva area reservada entre 15 MiB e 16 MiB */
    _Mem_MarcarPaginas(3840, 4096);
    

    /* Registra e ativa a paginacao */
    ISR_Registra(14, &_Mem_FalhaPaginacao);
    _Mem_DefineDiretorio(_Mem_Diretorio);
    
    

}