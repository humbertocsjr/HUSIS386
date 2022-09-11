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

void FalhaGrave(SByte_t * modulo, SByte_t * msg, Tam_t valor0, Tam_t valor1)
{
    _Husis_EscreveMensagens = SIM;
    Mensagem2(modulo, msg, valor0, valor1);
    while(SIM)
    {
       __asm__ __volatile__ ("hlt");
    }
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

void MensagemConst(SByte_t * modulo, SByte_t * msg, SByte_t * constAdicional, Tam_t constAdicionalTam)
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
        Term_EscrevaFmt0(msg);
        Term_EscrevaC('\'');
        Term_EscrevaConst(constAdicional, constAdicionalTam);
        Term_EscrevaC('\'');
        Term_EscrevaC('\n');
        Term_AlteraCor(cor_fundo, cor_frente);
    }
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

void Inicio()
{

    Term();
    Term_Limpa(TERM_COR_PRETO, TERM_COR_CIANO, ' ');
    Term_EscrevaFmt4("HUSIS v{0:u}.{1:u} R{2:u} {3:C}\n\n", (Tam_t)_Husis_Versao.Versao, (Tam_t)_Husis_Versao.SubVersao, (Tam_t)_Husis_Versao.Revisao, (Tam_t)&_Husis_Edicao);

    Mensagem("Principal", "Iniciando Nucleo",0);
    Multiboot();
    GDT();
    IDT();
    ISR();
    IRQ();
    Unidade();
    Dispositivo();
    Processo();
    Mem();
    Cronometro();
    Teclado();
    Comando();
    Multitarefa();
    Multitarefa_Habilita();

    // Dispositivos
    Armazenamento();

    // Sistemas de arquivos
    SisArq();

    /*
    
    COMO IMPLEMENTAR MULTITAREFA NO FUTURO

     - Implementar envio de mensagens entre processos
     - Todo programa (incluindo o nucleo), rodara um loop infinito recebendo mensagens e tratando-as
     - Essas mensagens podem, por exemplo, conter uma requisicao de abertura de arquivo para o nucleo
     - O nucleo vai ler a mensagem, chamar a rotina de abertura de arquivo e responder a mensagem 
     - O programa solicitante fica suspenso aguardando a resposta
     - Quando a resposta vier a multitarefa reativa a tarefa para que ela possa ler a resposta

    Fazendo dessa forma simplifica e deita todos os processos monotarefas, evitando reentrada e corrupcao de dados


    Implementar um ultimo comando "inicial", que executa um aplicativo e inicia o loop de mensagens do nucleo
    
    
    
    */

    Comando_ProcessarConstAuto(Multiboot_ArgsConst(),&Comando_TermSaidaTexto);

    Comando_Term();

    for (;;)
    {
       __asm__ __volatile__ ("hlt");
    }
}
		