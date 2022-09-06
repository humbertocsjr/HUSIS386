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
    #define NAO 0
    #define SIM -1
    #define STATUS_OK 0
    #define STATUS_QUANTIDADE_INVALIDA 1
    #define STATUS_FORMATO_INVALIDO 2
    #define STATUS_POSICAO_INVALIDA 3
    #define STATUS_VAZIO 4
    #define STATUS_COMANDO_DESCONHECIDO 5
    #define STATUS_ESTOURO_DA_CAPACIDADE 6
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

#endif