#ifndef TECLA_HEADER
#define TECLA_HEADER

#include <Base.h>


typedef unsigned short Tecla_t;

typedef struct
{
    Pos_t Pos;
    Tecla_t Tecla;
    Byte_t Ascii;
    Tecla_t TeclaComShift;
    Byte_t AsciiComShift;
    Tecla_t TeclaComAlt;
    Byte_t AsciiComAlt;
    Tecla_t TeclaExtendido;
    Byte_t AsciiExtendido;
    Boleano_t Pressionado;

} TeclaRegistro_t;

#define TECLA_NENHUMA 0
#define TECLA_A 1
#define TECLA_B 2
#define TECLA_C 3
#define TECLA_D 4
#define TECLA_E 5
#define TECLA_F 6
#define TECLA_G 7
#define TECLA_H 8
#define TECLA_I 9
#define TECLA_J 10
#define TECLA_K 11
#define TECLA_L 12
#define TECLA_M 13
#define TECLA_N 14
#define TECLA_O 15
#define TECLA_P 16
#define TECLA_Q 17
#define TECLA_R 18
#define TECLA_S 19
#define TECLA_T 20
#define TECLA_U 21
#define TECLA_V 22
#define TECLA_W 23
#define TECLA_X 24
#define TECLA_Y 25
#define TECLA_Z 26
#define TECLA_0 27
#define TECLA_1 28
#define TECLA_2 29
#define TECLA_3 30
#define TECLA_4 31
#define TECLA_5 32
#define TECLA_6 33
#define TECLA_7 34
#define TECLA_8 35
#define TECLA_9 36
#define TECLA_ESC 37
#define TECLA_F1 38
#define TECLA_F2 39
#define TECLA_F3 40
#define TECLA_F4 41
#define TECLA_F5 42
#define TECLA_F6 43
#define TECLA_F7 44
#define TECLA_F8 45
#define TECLA_F9 46
#define TECLA_F10 47
#define TECLA_F11 48
#define TECLA_F12 49
#define TECLA_F13 50
#define TECLA_F14 51
#define TECLA_F15 52
#define TECLA_F16 53
#define TECLA_F17 54
#define TECLA_F18 55
#define TECLA_F19 56
#define TECLA_BRILHO_DIMINUI 57
#define TECLA_BRILHO_AUMENTA 58
#define TECLA_VOLUME_DIMINUI 59
#define TECLA_VOLUME_AUMENTA 60
#define TECLA_VOLUME_MUDO 61
#define TECLA_BRILHO2_DIMINUI 62
#define TECLA_BRILHO2_AUMENTA 63
#define TECLA_PLAY 64
#define TECLA_PAUSE 65
#define TECLA_PROXIMO 66
#define TECLA_ANTERIOR 67
#define TECLA_TIL 68
#define TECLA_APOSTROFO 69
#define TECLA_EXCLAMACAO 70
#define TECLA_ARROBA 71
#define TECLA_CERQUILHA 72
#define TECLA_CIFRAO 73
#define TECLA_PORCENTAGEM 74
#define TECLA_CIRCUNFLEXO 75
#define TECLA_E_COMERCIAL 76
#define TECLA_ASTERISCO 77
#define TECLA_PARENTESES_ABRE 78
#define TECLA_PARENTESES_FECHA 79
#define TECLA_MENOS 80
#define TECLA_SUBSCRITO 81
#define TECLA_IGUAL 82
#define TECLA_MAIS 83
#define TECLA_DELETE 84
#define TECLA_TAB 85
#define TECLA_CHAVES_ABRE 86
#define TECLA_CHAVES_FECHA 87
#define TECLA_COLCHETES_ABRE 88
#define TECLA_COLCHETES_FECHA 89
#define TECLA_BARRA_INVERTIDA 90
#define TECLA_BARRA_VERTICAL 91
#define TECLA_ESPACO 92
#define TECLA_PONTO_E_VIRGULA 93
#define TECLA_DOIS_PONTOS 94
#define TECLA_ASPAS_SIMPLES 95
#define TECLA_ASPAS_DUPLAS 96
#define TECLA_MENOR_QUE 97
#define TECLA_MAIOR_QUE 98
#define TECLA_PONTO 99
#define TECLA_VIRGULA 100
#define TECLA_BARRA 101
#define TECLA_INTERROGACAO 102
#define TECLA_SETA_ACIMA 103
#define TECLA_SETA_ABAIXO 104
#define TECLA_SETA_ESQUERDA 105
#define TECLA_SETA_DIREITA 106
#define TECLA_INSERT 107
#define TECLA_PAGE_UP 108
#define TECLA_PAGE_DOWN 109
#define TECLA_BACKSPACE 110
#define TECLA_EXTENDIDO 111
#define TECLA_ENTER 112
#define TECLA_ESPECIAL_DIREITA 128
#define TECLA_ESPECIAL_ESQUERDA 256
#define TECLA_ESPECIAL_FN 512
#define TECLA_ESPECIAL_SHIFT 1024
#define TECLA_ESPECIAL_CTRL 2048
#define TECLA_ESPECIAL_ALT 4096
#define TECLA_ESPECIAL_NUM 8192
#define TECLA_ESPECIAL_SCROLL 16384
#define TECLA_ESPECIAL_CAPS 32768


#endif