#ifndef HUSIS_H
#define HUSIS_H

#include <Base.h>
#include <Tecla.h>

/* Configuracoes Gerais */
    #define MSG_ESCREVE_NO_TERM -1
    #define MSG_EXIBE_TECLA_DESCONHECIDA 0
    #define MSG_EXIBE_MAPA_IDT 0
    #define MSG_EXIBE_DADOS_IDT 0 
    #define MSG_EXIBE_MAPA_GDT 0
    #define MSG_EXIBE_DADOS_GDT 0 
    #define MSG_EXIBE_REGISTRO_IRQ 0


/* Principal.c */
    extern void Mensagem(SByte_t * modulo, SByte_t * msg, Tam_t valor0);
    extern void Mensagem2(SByte_t * modulo, SByte_t * msg, Tam_t valor0, Tam_t valor1);
    extern void Mensagem3(SByte_t * modulo, SByte_t * msg, Tam_t valor0, Tam_t valor1, Tam_t valor2);
    extern void Mensagem4(SByte_t * modulo, SByte_t * msg, Tam_t valor0, Tam_t valor1, Tam_t valor2, Tam_t valor3);
    extern void Mensagem5(SByte_t * modulo, SByte_t * msg, Tam_t valor0, Tam_t valor1, Tam_t valor2, Tam_t valor3, Tam_t valor4);
    extern void Mensagem6(SByte_t * modulo, SByte_t * msg, Tam_t valor0, Tam_t valor1, Tam_t valor2, Tam_t valor3, Tam_t valor4, Tam_t valor5);
    extern void Mensagem7(SByte_t * modulo, SByte_t * msg, Tam_t valor0, Tam_t valor1, Tam_t valor2, Tam_t valor3, Tam_t valor4, Tam_t valor5, Tam_t valor6);
    extern void MensagemMapa(SByte_t * modulo, SByte_t * msg, Byte_t * ponteiro, Tam_t tam, Tam_t itensPorLinha);
    extern void MensagemAtiva();
    extern void MensagemDesativa();
    extern Versao_t Husis_Versao();

/* ES.c */
    extern Byte_t ES_LeiaByte (UShort_t porta);
    extern void ES_EscrevaByte (UShort_t porta, Byte_t dados);
    extern UShort_t ES_LeiaUShort (UShort_t porta);
    extern void ES_EscrevaUShort (UShort_t porta, UShort_t dados);

/* Mem.c */
    extern void Mem_CopiaBinario(Byte_t * destino, Byte_t * origem, Tam_t tam);
    extern void Mem_RepeteByte(Byte_t * destino, Byte_t valor, Tam_t quantidade);
    extern void Mem_RepeteUShort(UShort_t * destino, UShort_t valor, Tam_t quantidade);
    extern void Mem();

/* Term.c */
    typedef struct Term_t
    {
        UShort_t * Conteudo;
        Byte_t Largura;
        Byte_t Altura;
        Byte_t X;
        Byte_t Y;
        Byte_t CorFrente;
        Byte_t CorFundo;
        Byte_t EntradaTemp[16];
        Boleano_t EntradaUso[16];
        Byte_t EntradaInicio;
        Byte_t EntradaFim;
        void (*Atualiza)(void * term);
    } Term_t;
    /* Inicia o Terminal */
    extern void Term();
    /* Acao para processar uma tecla */
    Status_t Term_ProcessaTeclado (Tecla_t tecla, Byte_t ascii, Boleano_t shift, Boleano_t alt, Boleano_t ctrl, Boleano_t ext, Boleano_t pressionado);
    /* Limpa o terminal preenchendo com um caractere e define a cor */
    extern void Term_Limpa(Byte_t cor_fundo, Byte_t cor_frente, Byte_t caractere);
    /* Escreve um caractere no terminal */
    extern void Term_EscrevaC(Byte_t caractere);
    /* Escreve uma constante de texto no terminal definindo o tamanho */
    extern void Term_EscrevaConst(SByte_t * textoConstante, Tam_t tam);
    /* Escreve uma constante de texto no terminal calculando o tamanho automaticamente */
    extern void Term_EscrevaConstAuto(SByte_t * textoConstante);
    /* Escreve um UInt na tela */
    extern void Term_EscrevaUInt(UInt_t valor);
    /* Va para uma posicao no terminal, posicao inicial: X=0 Y=0 */
    extern void Term_IrPara(Byte_t x, Byte_t y);
    /* Le a largura do terminal */
    extern Byte_t Term_Largura();
    /* Le a altura do terminal */
    extern Byte_t Term_Altura();
    /*
    Escreve texto com parametros baseados em formatação
    Modo de uso:
        Dentro do texto deve inserir referencias aos valores externos, usando o formato abaixo:
        {POSICAO:FORMATO}
        Formatos válidos:
            i = Numero Int_t
            u = Numero UInt_t
            c = Caractere
            C = Constante de texto
        Exemplos:
            {0:i} {1:c} {0:C}
    */
    extern Status_t Term_EscrevaFmt(SByte_t * constanteFmt, Tam_t qtd, Tam_t v0, Tam_t v1, Tam_t v2, Tam_t v3, Tam_t v4, Tam_t v5, Tam_t v6, Tam_t v7, Tam_t v8, Tam_t v9);
    extern Status_t Term_EscrevaFmt10(SByte_t * constanteFmt, Tam_t v0, Tam_t v1, Tam_t v2, Tam_t v3, Tam_t v4, Tam_t v5, Tam_t v6, Tam_t v7, Tam_t v8, Tam_t v9);
    extern Status_t Term_EscrevaFmt9(SByte_t * constanteFmt, Tam_t v0, Tam_t v1, Tam_t v2, Tam_t v3, Tam_t v4, Tam_t v5, Tam_t v6, Tam_t v7, Tam_t v8);
    extern Status_t Term_EscrevaFmt8(SByte_t * constanteFmt, Tam_t v0, Tam_t v1, Tam_t v2, Tam_t v3, Tam_t v4, Tam_t v5, Tam_t v6, Tam_t v7);
    extern Status_t Term_EscrevaFmt7(SByte_t * constanteFmt, Tam_t v0, Tam_t v1, Tam_t v2, Tam_t v3, Tam_t v4, Tam_t v5, Tam_t v6);
    extern Status_t Term_EscrevaFmt6(SByte_t * constanteFmt, Tam_t v0, Tam_t v1, Tam_t v2, Tam_t v3, Tam_t v4, Tam_t v5);
    extern Status_t Term_EscrevaFmt5(SByte_t * constanteFmt, Tam_t v0, Tam_t v1, Tam_t v2, Tam_t v3, Tam_t v4);
    extern Status_t Term_EscrevaFmt4(SByte_t * constanteFmt, Tam_t v0, Tam_t v1, Tam_t v2, Tam_t v3);
    extern Status_t Term_EscrevaFmt3(SByte_t * constanteFmt, Tam_t v0, Tam_t v1, Tam_t v2);
    extern Status_t Term_EscrevaFmt2(SByte_t * constanteFmt, Tam_t v0, Tam_t v1);
    extern Status_t Term_EscrevaFmt1(SByte_t * constanteFmt, Tam_t v0);
    extern Status_t Term_EscrevaFmt0(SByte_t * constanteFmt);
    extern Tam_t Term_LeiaLinhaConst(SByte_t * destino, Tam_t tam);
    extern Byte_t Term_LeiaC();
    /* Le e Manipula as cores */
    extern Byte_t Term_LeiaCorFundo();
    extern Byte_t Term_LeiaCorFrente();
    extern void Term_AlteraCorFundo(Byte_t cor);
    extern void Term_AlteraCorFrente(Byte_t cor);
    extern void Term_AlteraCor(Byte_t cor_fundo, Byte_t cor_frente);
    /* Cores suportadas pelo terminal */
    #define TERM_COR_PRETO 0
    #define TERM_COR_AZUL 1
    #define TERM_COR_VERDE 2
    #define TERM_COR_CIANO 3
    #define TERM_COR_VERMELHO 4
    #define TERM_COR_CINZA 7
    #define TERM_COR_CINZA_CLARO 8
    #define TERM_COR_BRANCO 15

/* Const.c */
    extern Tam_t Const_Tam(SByte_t * constanteTexto);
    extern Tam_t Const_TamLimitado(SByte_t * constanteTexto, Tam_t capacidade);
    extern Boleano_t Const_Igual(SByte_t * origem, SByte_t * destino, Tam_t tam);

/* Caract.c */
    extern Boleano_t Caract_EhNumero(Byte_t c);

/* GDT.c */
    extern void GDT();
    extern void GDT_Config(Pos_t pos, UInt_t base, UInt_t limite, Byte_t acesso, Byte_t granularidade);
    extern void GDT_Atualiza(); /* Inicial.asm */

/* IDT.c */
    extern void IDT();
    extern void IDT_Config(Pos_t pos, UInt_t base, UShort_t seletor, Byte_t marcadores);
    extern void IDT_Atualiza(); /* Inicial.asm */

/* ISR.c */
    extern void ISR();

/* IRQ.c */
    extern void IRQ();
    extern Status_t IRQ_Registra(Pos_t irq, void (*processador)(Regs_t *regs));
    extern Status_t IRQ_Desregistra(Pos_t irq);

/* Cronometro.c */
    extern void Cronometro();
    extern void Cronometro_Espera(UInt_t centesimos);
    extern Tam_t Cronometro_Contador();
    extern Tam_t Cronometro_Segundos();
    extern Tam_t Cronometro_Minutos();
    extern Tam_t Cronometro_Horas();

/* Teclado.c */
    extern void Teclado();
    extern Status_t Teclado_Registra(Pos_t codigo, Byte_t ascii, Tecla_t tecla);
    extern Status_t Teclado_RegistraAlt(Pos_t codigo, Byte_t ascii, Tecla_t tecla);
    extern Status_t Teclado_RegistraShift(Pos_t codigo, Byte_t ascii, Tecla_t tecla);
    extern Status_t Teclado_RegistraExt(Pos_t codigo, Byte_t ascii, Tecla_t tecla);
    extern void Teclado_RegistraAcao(Status_t (* acao) (Tecla_t tecla, Byte_t ascii, Boleano_t shift, Boleano_t alt, Boleano_t ctrl, Boleano_t ext, Boleano_t pressionado));
    extern void Teclado_EnUsIntl();;

/* Comandos.c */
    extern Status_t Comando_ProcessarConst(SByte_t * constante, Tam_t tam, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0));
    extern Status_t Comando_ProcessarConstAuto(SByte_t * constante, void (*saidaTexto)(SByte_t * constanteTexto, Tam_t valor0));
    extern void Comando_Term();
    extern void Comando();
    extern void Comando_TermSaidaTexto(SByte_t * constanteTexto, Tam_t valor0);

/* Multiboot.c */
    extern void Multiboot();
    extern Tam_t Multiboot_MemoriaBaixaKiB();
    extern Tam_t Multiboot_MemoriaBaixaPaginas();
    extern Tam_t Multiboot_MemoriaAltaKiB();
    extern Tam_t Multiboot_MemoriaAltaPaginas();
    extern SByte_t * Multiboot_ArgsConst();

#endif