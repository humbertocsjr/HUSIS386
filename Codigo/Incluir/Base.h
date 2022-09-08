#ifndef BASE_H
#define BASE_H

/* Tipos padrao C*/
    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned int uint32_t;
    typedef char int8_t;
    typedef short int16_t;
    typedef int int32_t;
    typedef unsigned int size_t;

/* Tipos HUSIS */
    typedef unsigned char Byte_t;
    typedef unsigned short UShort_t;
    typedef unsigned int UInt_t;
    typedef char SByte_t;
    typedef short Short_t;
    typedef int Int_t;
    typedef unsigned int Tam_t;
    typedef unsigned int Pos_t;
    typedef unsigned int Status_t;
    typedef unsigned int Boleano_t;
    typedef unsigned short Processo_t;
    #define NAO 0
    #define SIM -1
    #define STATUS_OK 0
    #define STATUS_QUANTIDADE_INVALIDA 1
    #define STATUS_FORMATO_INVALIDO 2
    #define STATUS_POSICAO_INVALIDA 3
    #define STATUS_VAZIO 4
    #define STATUS_COMANDO_DESCONHECIDO 5
    #define STATUS_ESTOURO_DA_CAPACIDADE 6
    #define STATUS_UNIDADE_INVALIDA 7
    #define STATUS_SISARQ_INVALIDO 8
    #define STATUS_TIPO_INVALIDO 9
    #define STATUS_DISPOSITIVO_INVALIDO 10
    #define STATUS_ACESSO_NEGADO 11
    #define STATUS_NAO_ENCONTRADO 12
    typedef struct
    {
        UShort_t Versao;
        UShort_t SubVersao;
        UShort_t Revisao;
    } Versao_t;

    typedef struct 
    {
        UInt_t GS;
        UInt_t FS;
        UInt_t ES;
        UInt_t DS;
        UInt_t EDI;
        UInt_t ESI;
        UInt_t EBP;
        UInt_t Ignora;
        UInt_t EBX;
        UInt_t EDX;
        UInt_t ECX;
        UInt_t EAX;
        UInt_t Interrupcao;
        UInt_t CodigoDeErro;
        UInt_t EIP;
        UInt_t CS;
        UInt_t EFLAGS;
        UInt_t ESP;
        UInt_t SS;
    } Regs_t;

    #define DISPOSITIVO_NOME_TAM 32

    #define DISPOSITIVO_TIPO_DISCO 10
    #define DISPOSITIVO_PORTAS_CAPACIDADE 16

    typedef struct 
    {
        Pos_t Dispositivo;
        Pos_t DispositivoAcima;
        Boleano_t Existe;
        Boleano_t Ativo;
        SByte_t NomeConst[DISPOSITIVO_NOME_TAM];
        Pos_t DispositivoNumero;
        UShort_t Tipo;
        UShort_t Porta[DISPOSITIVO_PORTAS_CAPACIDADE];
        UInt_t Identificador;
        Tam_t TamanhoDeUmBloco;
        Tam_t (*AcaoLeia)(void * disp, Pos_t posicao, Byte_t * destino, Tam_t tam);
        Tam_t (*AcaoEscreva)(void * disp, Pos_t posicao, Byte_t * destino, Tam_t tam);
    } Dispositivo_t;

    #define ITEM_NOME_TAM 64

    #define ITEM_TIPO_NENHUM 0
    #define ITEM_TIPO_ARQUIVO 0x10
    #define ITEM_TIPO_DIRETORIO 0x20

    #define ITEM_ESPECIAL_NENHUM 0
    #define ITEM_ESPECIAL_DISPOSITIVO 0x10

    typedef struct
    {
        Pos_t Unidade;
        Pos_t Identificador;
        SByte_t Nome[ITEM_NOME_TAM];
        UShort_t Tipo;
        UShort_t Especial;
        Pos_t PosicaoNoItem;
        Pos_t PosicaoEspecial;
    } Item_t;

#endif