#include <HUSIS.h>

const Versao_t _Husis_Versao =
{
    .Versao = 22,
    .SubVersao = 9,
    .Revisao = 1
};
const SByte_t _Husis_Edicao[] = "Alpha";
Boleano_t _Husis_EscreveMensagens = MSG_ESCREVE_NO_TERM;

Versao_t Husis_Versao()
{
    return _Husis_Versao;
}

void MensagemAtiva()
{
    _Husis_EscreveMensagens = SIM;
    Mensagem("Principal", "Alterado impressao das mensagens no terminal para: {0:s}", _Husis_EscreveMensagens);
}

void MensagemDesativa()
{
    _Husis_EscreveMensagens = NAO;
    Mensagem("Principal", "Alterado impressao das mensagens no terminal para: {0:s}", _Husis_EscreveMensagens);
}

void Mensagem(SByte_t * modulo, SByte_t * msg, Tam_t valor0)
{
    Byte_t cor_frente;
    Byte_t cor_fundo;
    if(_Husis_EscreveMensagens)
    {
        cor_frente = Term_LeiaCorFrente();
        cor_fundo = Term_LeiaCorFundo();
        Term_AlteraCor(TERM_COR_PRETO, TERM_COR_VERMELHO);
        Term_EscrevaFmt1("NUCLEO<{0:C}>: ", (Tam_t)modulo);
        Term_AlteraCor(TERM_COR_PRETO, TERM_COR_BRANCO);
        Term_EscrevaFmt1(msg, valor0);
        Term_EscrevaC('\n');
        Term_AlteraCor(cor_fundo, cor_frente);
    }
}

void Mensagem2(SByte_t * modulo, SByte_t * msg, Tam_t valor0, Tam_t valor1)
{
    Byte_t cor_frente;
    Byte_t cor_fundo;
    if(_Husis_EscreveMensagens)
    {
        cor_frente = Term_LeiaCorFrente();
        cor_fundo = Term_LeiaCorFundo();
        Term_AlteraCor(TERM_COR_PRETO, TERM_COR_VERMELHO);
        Term_EscrevaFmt1("NUCLEO<{0:C}>: ", (Tam_t)modulo);
        Term_AlteraCor(TERM_COR_PRETO, TERM_COR_BRANCO);
        Term_EscrevaFmt2(msg, valor0, valor1);
        Term_EscrevaC('\n');
        Term_AlteraCor(cor_fundo, cor_frente);
    }
}

void Mensagem3(SByte_t * modulo, SByte_t * msg, Tam_t valor0, Tam_t valor1, Tam_t valor2)
{
    Byte_t cor_frente;
    Byte_t cor_fundo;
    if(_Husis_EscreveMensagens)
    {
        cor_frente = Term_LeiaCorFrente();
        cor_fundo = Term_LeiaCorFundo();
        Term_AlteraCor(TERM_COR_PRETO, TERM_COR_VERMELHO);
        Term_EscrevaFmt1("NUCLEO<{0:C}>: ", (Tam_t)modulo);
        Term_AlteraCor(TERM_COR_PRETO, TERM_COR_BRANCO);
        Term_EscrevaFmt3(msg, valor0, valor1, valor2);
        Term_EscrevaC('\n');
        Term_AlteraCor(cor_fundo, cor_frente);
    }
}

void Mensagem4(SByte_t * modulo, SByte_t * msg, Tam_t valor0, Tam_t valor1, Tam_t valor2, Tam_t valor3)
{
    Byte_t cor_frente;
    Byte_t cor_fundo;
    if(_Husis_EscreveMensagens)
    {
        cor_frente = Term_LeiaCorFrente();
        cor_fundo = Term_LeiaCorFundo();
        Term_AlteraCor(TERM_COR_PRETO, TERM_COR_VERMELHO);
        Term_EscrevaFmt1("NUCLEO<{0:C}>: ", (Tam_t)modulo);
        Term_AlteraCor(TERM_COR_PRETO, TERM_COR_BRANCO);
        Term_EscrevaFmt4(msg, valor0, valor1, valor2, valor3);
        Term_EscrevaC('\n');
        Term_AlteraCor(cor_fundo, cor_frente);
    }
}

void Mensagem5(SByte_t * modulo, SByte_t * msg, Tam_t valor0, Tam_t valor1, Tam_t valor2, Tam_t valor3, Tam_t valor4)
{
    Byte_t cor_frente;
    Byte_t cor_fundo;
    if(_Husis_EscreveMensagens)
    {
        cor_frente = Term_LeiaCorFrente();
        cor_fundo = Term_LeiaCorFundo();
        Term_AlteraCor(TERM_COR_PRETO, TERM_COR_VERMELHO);
        Term_EscrevaFmt1("NUCLEO<{0:C}>: ", (Tam_t)modulo);
        Term_AlteraCor(TERM_COR_PRETO, TERM_COR_BRANCO);
        Term_EscrevaFmt5(msg, valor0, valor1, valor2, valor3, valor4);
        Term_EscrevaC('\n');
        Term_AlteraCor(cor_fundo, cor_frente);
    }
}

void Mensagem6(SByte_t * modulo, SByte_t * msg, Tam_t valor0, Tam_t valor1, Tam_t valor2, Tam_t valor3, Tam_t valor4, Tam_t valor5)
{
    Byte_t cor_frente;
    Byte_t cor_fundo;
    if(_Husis_EscreveMensagens)
    {
        cor_frente = Term_LeiaCorFrente();
        cor_fundo = Term_LeiaCorFundo();
        Term_AlteraCor(TERM_COR_PRETO, TERM_COR_VERMELHO);
        Term_EscrevaFmt1("NUCLEO<{0:C}>: ", (Tam_t)modulo);
        Term_AlteraCor(TERM_COR_PRETO, TERM_COR_BRANCO);
        Term_EscrevaFmt6(msg, valor0, valor1, valor2, valor3, valor4, valor5);
        Term_EscrevaC('\n');
        Term_AlteraCor(cor_fundo, cor_frente);
    }
}

void Mensagem7(SByte_t * modulo, SByte_t * msg, Tam_t valor0, Tam_t valor1, Tam_t valor2, Tam_t valor3, Tam_t valor4, Tam_t valor5, Tam_t valor6)
{
    Byte_t cor_frente;
    Byte_t cor_fundo;
    if(_Husis_EscreveMensagens)
    {
        cor_frente = Term_LeiaCorFrente();
        cor_fundo = Term_LeiaCorFundo();
        Term_AlteraCor(TERM_COR_PRETO, TERM_COR_VERMELHO);
        Term_EscrevaFmt1("NUCLEO<{0:C}>: ", (Tam_t)modulo);
        Term_AlteraCor(TERM_COR_PRETO, TERM_COR_BRANCO);
        Term_EscrevaFmt7(msg, valor0, valor1, valor2, valor3, valor4, valor5, valor6);
        Term_EscrevaC('\n');
        Term_AlteraCor(cor_fundo, cor_frente);
    }
}

void MensagemMapa(SByte_t * modulo, SByte_t * msg, Byte_t * ponteiro, Tam_t tam, Tam_t itensPorLinha)
{
    const Byte_t hex[] = "0123456789ABCDEF";
    Byte_t cor_frente;
    Byte_t cor_fundo;
    Byte_t temp;
    Pos_t cont = 0;
    if(_Husis_EscreveMensagens)
    {
        cor_frente = Term_LeiaCorFrente();
        cor_fundo = Term_LeiaCorFundo();
        Term_AlteraCor(TERM_COR_PRETO, TERM_COR_VERMELHO);
        Term_EscrevaFmt1("NUCLEO<{0:C}>: ", (Tam_t)modulo);
        Term_AlteraCor(TERM_COR_PRETO, TERM_COR_BRANCO);
        Term_EscrevaFmt0(msg);
        Term_EscrevaFmt1("[{0:u}]:\n", (Tam_t)ponteiro);
        for (Pos_t i = 0; i < tam; i++)
        {
            temp = ponteiro[i];
            Term_EscrevaC(hex[(temp >> 4) & 0xf]);
            Term_EscrevaC(hex[temp & 0xf]);
            Term_EscrevaC(' ');
            cont++;
            if(cont >= itensPorLinha)
            {
                cont = 0;
                Term_EscrevaC('\n');
            }
        }
        

        Term_EscrevaC('\n');
        Term_AlteraCor(cor_fundo, cor_frente);
    }

}

/* This is a very simple main() function. All it does is sit in an
*  infinite loop. This will be like our 'idle' loop */
void Inicio()
{

    Term();
    Term_Limpa(TERM_COR_PRETO, TERM_COR_CIANO, ' ');
    Term_EscrevaFmt4("HUSIS v{0:u}.{1:u} R{2:u} {3:C}\n\n", (Tam_t)_Husis_Versao.Versao, (Tam_t)_Husis_Versao.SubVersao, (Tam_t)_Husis_Versao.Revisao, (Tam_t)&_Husis_Edicao);

    Mensagem("Principal", "Iniciando Nucleo",0);
    Multiboot();
    Mensagem("Principal", "Iniciando GDT",0);
    GDT();
    Mensagem("Principal", "Iniciando IDT",0);
    IDT();
    Mensagem("Principal", "Iniciando ISR",0);
    ISR();
    Mensagem("Principal", "Iniciando IRQ",0);
    IRQ();
    Mensagem("Principal", "Iniciando Gerenciador de Memoria",0);
    Mem();
    Mensagem("Principal", "Iniciando Cronometro",0);
    Cronometro();
    Mensagem("Principal", "Iniciando Teclado",0);
    Teclado();
    Mensagem("Principal", "Iniciando Processador de Comandos",0);
    Comando();

    Comando_ProcessarConstAuto(Multiboot_ArgsConst(),&Comando_TermSaidaTexto);

    Comando_Term();

    /* ...and leave this loop in. There is an endless loop in
    *  'start.asm' also, if you accidentally delete this next line */
    for (;;)
    {
       __asm__ __volatile__ ("hlt");
    }
}
		