#ifndef HUSIS_H
#define HUSIS_H

#include <Base.h>
#include <Tecla.h>

/* Configuracoes Gerais */
    #define ARMAZENAMENTO_ATA
    #define SISARQ_HARQ


    #define MSG_ESCREVE_NO_TERM -1
    #define MSG_EXIBE_TECLA_DESCONHECIDA 0
    #define MSG_EXIBE_MAPA_IDT 0
    #define MSG_EXIBE_DADOS_IDT 0 
    #define MSG_EXIBE_MAPA_GDT 0
    #define MSG_EXIBE_DADOS_GDT 0 
    #define MSG_EXIBE_REGISTRO_IRQ 0
    #define MSG_EXIBE_CHAMADA_IRQ 0
    #define MSG_EXIBE_REGISTRO_ISR 0
    #define MSG_EXIBE_CHAMADA_ISR 0
    #define MSG_EXIBE_ERRO_ATA 1


/* Principal.c */
    extern void MensagemConst(SByte_t * modulo, SByte_t * msg, SByte_t * constAdicional, Tam_t constAdicionalTam);
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
    extern void FalhaGrave(SByte_t * modulo, SByte_t * msg, Tam_t valor0, Tam_t valor1);

/* ES.c */
    extern Byte_t ES_LeiaByte (UShort_t porta);
    extern void ES_EscrevaByte (UShort_t porta, Byte_t dados);
    extern UShort_t ES_LeiaUShort (UShort_t porta);
    extern void ES_EscrevaUShort (UShort_t porta, UShort_t dados);

/* Mem.c */
    /* Manipular memoria */
    extern void Mem_CopiaBinario(Byte_t * destino, Byte_t * origem, Tam_t tam);
    extern void Mem_RepeteByte(Byte_t * destino, Byte_t valor, Tam_t quantidade);
    extern void Mem_RepeteUShort(UShort_t * destino, UShort_t valor, Tam_t quantidade);
    /* Memoria Local do Nucleo */
    extern Status_t Mem_Local_Libera(Byte_t * endereco);
    extern Tam_t Mem_Local_EspacoLivre();
    extern Tam_t Mem_Local_Tam(Byte_t * endereco);
    extern Byte_t * Mem_Local_Aloca(Tam_t tam);
    extern Byte_t * Mem_Local_AlocaAlinhado(Tam_t tam);
    extern Tam_t Mem_Local_Capacidade();
    /* Paginacao */
    typedef struct
    {
        UInt_t Presente         :1;
        UInt_t Escrita          :1;
        UInt_t Usuario          :1;
        UInt_t FoiAcessado      :1;
        UInt_t FoiGravado       :1;
        UInt_t Outro            :7;
        UInt_t Posicao          :20;
    } Mem_Pagina_t;
    extern Mem_Pagina_t * Mem_Pagina(UInt_t endereco, Boleano_t cria, Pos_t * diretorio);
    extern Pos_t * Mem_DiretorioAtual();
    extern void Mem_AlocaPagina(Mem_Pagina_t * pagina, Boleano_t podeEscrever, Boleano_t pertenceAoNucleo);
    extern void Mem_LiberaPagina(Mem_Pagina_t * pagina);
    extern Tam_t Mem_PaginasLivres();
    extern Tam_t Mem_TotalDePaginas();
    extern Pos_t * Mem_CriaDiretorio(Boleano_t clonaDiretorioDoNucleo);
    extern void Mem_DefineDiretorio(Pos_t * diretorio);
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
    extern Boleano_t Const_Igual(SByte_t * destino, SByte_t * origem, Tam_t tam);
    extern void Const_Copia(SByte_t * destino, SByte_t * origem, Tam_t tam);
    extern Status_t Const_ParaNumero(SByte_t * origem, Tam_t tam, UInt_t * valor);
    extern Status_t Const_DeNumero(SByte_t * destino, UInt_t valor, Tam_t tam);

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
    extern Status_t ISR_Registra(Pos_t irq, void (*processador)(Regs_t *regs));
    extern Status_t ISR_Desregistra(Pos_t irq);

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
    extern void Teclado_EnUsIntl();
    extern void Teclado_ExibeDesconhecidas();
    extern void Teclado_ExibeTodas();
    extern void Teclado_Oculta();

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

/* Processo.c */
    #define PROCESSO_CAPACIDADE 256
    typedef struct 
    {
        Processo_t Processo;
        Pos_t * DiretorioPaginacao;
        Pos_t Binario;
        Tam_t BinarioTam;
        Pos_t Pilha;
        Pos_t PilhaBase;
        Tam_t PilhaTam;
        Pos_t Ponteiro;
        Boleano_t Existe;
        Boleano_t Suspenso;
    } Processo_Info_t;
    extern Processo_Info_t * Processo_Nucleo();
    extern Processo_Info_t * Processo_Cria();
    extern Processo_Info_t * Processo_Leia(Processo_t processo);
    extern Status_t Processo_Destroi(Processo_t processo);
    extern Processo_Info_t * Processo_Proximo(Processo_t processo);
    extern void Processo();

/* Multitarefa.c */
    extern void Multitarefa_Cronometro(Regs_t * regs);
    extern void Multitarefa_Habilita();
    extern void Multitarefa_Desabilita();
    extern void Multitarefa();

/* Unidade.c */
    extern void Unidade();
    extern Status_t Unidade_LeiaPrincipal(Pos_t * unidade);
    extern Status_t Unidade_DefinePrincipal(Pos_t unidade);
    extern Status_t Unidade_Monta(SByte_t * constSisArq, Tam_t constSisArqTam, SByte_t * constUnidade, Tam_t constUnidadeTam);
    extern Status_t Unidade_Formata(SByte_t * constSisArq, Tam_t constSisArqTam, SByte_t * constUnidade, Tam_t constUnidadeTam);
    extern Status_t Unidade_LeiaNomeConst(Pos_t unidade, SByte_t * destino, Tam_t tam);
    extern Status_t Unidade_EscrevaNomeConst(Pos_t unidade, SByte_t * origem, Tam_t tam);
    extern Tam_t Unidade_Quantidade();
    extern Status_t Unidade_Registra(SByte_t * constanteNome, Pos_t * unidade, Pos_t dispositivo);
    extern Status_t Unidade_Desregistra(Pos_t unidade);
    extern Status_t Unidade_RegistraSisArq(SByte_t * constanteNome, Pos_t * sisArq, Status_t (*acaoMonta)(Pos_t unidade), Status_t (*acaoValida)(Pos_t unidade), Status_t (*acaoFormata)(Pos_t unidade));
    extern Status_t Unidade_DesregistraSisArq(Pos_t sisArq);
    extern Status_t Unidade_Dispositivo(Pos_t unidade, Pos_t * dispositivo);
    extern Status_t Unidade_Raiz(Pos_t unidade, Item_t * * item);
    extern Status_t Unidade_LeiaTamanhoDeUmBloco(Pos_t unidade, Tam_t * tam);
    extern Status_t Unidade_CarregaObjetoMontado(Pos_t unidade, Byte_t * * objetoMontado);
    extern Status_t Unidade_RegistraMontagem
    (
        Pos_t unidade, 
        Tam_t tamanhoObjetoMontado,
        Status_t (*acaoRaiz)(Pos_t unidade, Item_t * * raiz),
        Status_t (*acaoDesmonta)(Pos_t unidade),
        Status_t (*acaoItemSubItem)(Item_t * item, Pos_t posicao, Item_t * * subItem),
        Status_t (*acaoItemQtdSubItem)(Item_t * item, Tam_t * qtd),
        Status_t (*acaoItemVaPara)(Item_t * item, Pos_t posicao),
        Tam_t (*acaoItemLeia)(Item_t * item, Byte_t * destino, Tam_t tam),
        Tam_t (*acaoItemEscreva)(Item_t * item, Byte_t * origem, Tam_t tam),
        Status_t (*acaoItemExclui)(Item_t * item),
        Status_t (*acaoItemFecha)(Item_t * item),
        Status_t (*acaoItemCriaDiretorio)(Item_t * item, SByte_t * constanteNome),
        Status_t (*acaoItemCriaArquivo)(Item_t * item, SByte_t * constanteNome)
    );
    extern Status_t Item_SubItem(Item_t * item, Pos_t posicao, Item_t * * subItem);
    extern Status_t Item_QtdSubItens(Item_t * item, Tam_t * qtd);
    extern Boleano_t Item_EhDiretorio(Item_t * item);
    extern Boleano_t Item_EhArquivo(Item_t * item);
    extern Status_t Item_VaPara(Item_t * item, Pos_t posicao);
    extern Status_t Item_Leia(Item_t * item, Byte_t * destino, Tam_t tam, Tam_t * lido);
    extern Status_t Item_Escreva(Item_t * item, Byte_t * origem, Tam_t tam, Tam_t * escrito);
    extern Status_t Item_Exclui(Item_t * item);
    extern Status_t Item_Fecha(Item_t * item);
    extern Status_t Item_CriaDiretorio(Item_t * item, SByte_t * constanteNome);
    extern Status_t Item_CriaArquivo(Item_t * item, SByte_t * constanteNome);
    extern Status_t Item_AbreConst(SByte_t * constanteEndereco, Tam_t tam, Item_t * * item);
    extern Status_t Item_CriaArquivoConst(SByte_t * constanteEndereco, Tam_t tam);

/* Dispositivo.c */
    extern Status_t Dispositivo_Registra(Pos_t * novoDispositivo, Pos_t dispositivoAcima, SByte_t * constanteNome, Boleano_t adicionaNumero, UInt_t dispNumero, UInt_t id, Tam_t blocoTam, Tam_t tamanho, Tam_t (*acaoLeia)(Dispositivo_t * disp, Pos_t posicao, Byte_t * destino, Tam_t tam), Tam_t (*acaoEscreva)(Dispositivo_t * disp, Pos_t posicao, Byte_t * origem, Tam_t tam));
    extern Status_t Dispositivo_DefinePorta(Pos_t dispositivo, Pos_t aux, UShort_t porta);
    extern Status_t Dispositivo_LeiaBytePorta(Pos_t dispositivo, Pos_t aux, Byte_t * valor);
    extern Status_t Dispositivo_EscrevaBytePorta(Pos_t dispositivo, Pos_t aux, Byte_t valor);
    extern Status_t Dispositivo_LeiaUShortPorta(Pos_t dispositivo, Pos_t aux, UShort_t * valor);
    extern Status_t Dispositivo_EscrevaUShortPorta(Pos_t dispositivo, Pos_t aux, UShort_t valor);
    extern Status_t Dispositivo_LeiaInfo(Pos_t dispositivo, Pos_t posicao, UInt_t * valor);
    extern Status_t Dispositivo_DefineInfo(Pos_t dispositivo, Pos_t posicao, UInt_t valor);
    extern Status_t Dispositivo_LeiaId(Pos_t dispositivo, Pos_t * id);
    extern Status_t Dispositivo_DefineTamanhoDeUmBloco(Pos_t dispositivo, Tam_t tam);
    extern Status_t Dispositivo_LeiaTamanhoDeUmBloco(Pos_t dispositivo, Tam_t * tam);
    extern Status_t Dispositivo_DefineTamanho(Pos_t dispositivo, Tam_t tam);
    extern Status_t Dispositivo_LeiaTamanho(Pos_t dispositivo, Tam_t * tam);
    extern Tam_t Dispositivo_Leia(Pos_t dispositivo, Pos_t posicao, Byte_t * destino, Tam_t tam);
    extern Tam_t Dispositivo_Escreva(Pos_t dispositivo, Pos_t posicao, Byte_t * origem, Tam_t tam);
    extern void Dispositivo();

/* ATA.c */
    extern void ATA();

#endif