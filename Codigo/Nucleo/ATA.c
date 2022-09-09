#include <HUSIS.h>

Boleano_t _ATA_ExibeErro = MSG_EXIBE_ERRO_ATA;

#define ATA_PORTA_ES_DADOS 0
#define ATA_PORTA_E__ERRO 1
#define ATA_PORTA__S_RECURSOS 2
#define ATA_PORTA_ES_QTD_SETORES 3
#define ATA_PORTA_ES_SETOR 4
#define ATA_PORTA_ES_LBA_BAIXO 4
#define ATA_PORTA_ES_CILINDRO_BAIXO 5
#define ATA_PORTA_ES_LBA_MEDIO 5
#define ATA_PORTA_ES_CILINDRO_ALTO 6
#define ATA_PORTA_ES_LBA_ALTO 6
#define ATA_PORTA_ES_DISP_E_CABECA 7
#define ATA_PORTA_E__STATUS 8
#define ATA_PORTA__S_COMANDO 9
#define ATA_PORTA_E__STATUS_ALT 10
#define ATA_PORTA__S_CONTROLE_DISP 11
#define ATA_PORTA_E__ENDERECO_DISP 12

#define ATA_MASCARA_STATUS_ERRO 1
#define ATA_MASCARA_STATUS_INDICE 2
#define ATA_MASCARA_STATUS_DADOS_CORRIGIDOS 4
#define ATA_MASCARA_STATUS_REQ_DADOS_PRONTA 8
#define ATA_MASCARA_STATUS_VA_PARA_CONCLUIDO 16
#define ATA_MASCARA_STATUS_ESCRITA_FALHA 32
#define ATA_MASCARA_STATUS_DISP_PRONTO 64
#define ATA_MASCARA_STATUS_OCUPADO 128



#define ATA_INFO_STATUS 0
#define ATA_INFO_TIPO 1
#define ATA_INFO_CILINDROS 2
#define ATA_INFO_CABECAS 3
#define ATA_INFO_SETORES 4
#define ATA_INFO_SETORES_POR_CILINDROS 5

#define ATA_STATUS_DISP_CRIADO 0
#define ATA_STATUS_IDENTIFICADO 1
#define ATA_STATUS_UNIDADE_CRIADA 2

#define ATA_TIPO_DESCONHECIDO 0
#define ATA_TIPO_CHS 1
#define ATA_TIPO_LBA28 2
#define ATA_TIPO_LBA48 3
#define ATA_TIPO_ATAPI 4
#define ATA_TIPO_SATA 5
#define ATA_TIPO_SATAPI 6

Tam_t _ATA_Leia(Dispositivo_t * dispositivo, Pos_t posicao, Byte_t * destino, Tam_t tam )
{
    Pos_t disp = dispositivo->Dispositivo;
    UInt_t tipo = 0;
    UInt_t cilindrosTotal = 0;
    UInt_t cabecasTotal = 0;
    UInt_t setoresTotal = 0;
    UInt_t setoresPorCilindroTotal = 0;
    UInt_t cilindro = 0;
    UInt_t cabeca = 0;
    UInt_t setor = 0;
    UInt_t id = 0;
    UInt_t tamBloco = 0;
    Byte_t status = 0;
    Boleano_t ok = SIM;

    // Le parametros do dispositivo
    Status_t ret = Dispositivo_LeiaInfo(disp, ATA_INFO_TIPO, &tipo);
    if((ret != STATUS_OK) && _ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da leitura", ret, 1);
    if(ret != STATUS_OK) return 0;
    if((tipo != ATA_TIPO_CHS) & (tipo != ATA_TIPO_SATA)) 
    {
        if(_ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da leitura", ret, 2);
        return 0;
    }
    ret = Dispositivo_LeiaInfo(disp, ATA_INFO_CILINDROS, &cilindrosTotal);
    if((ret != STATUS_OK) && _ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da leitura", ret, 3);
    if(ret != STATUS_OK) return 0;
    ret = Dispositivo_LeiaInfo(disp, ATA_INFO_CABECAS, &cabecasTotal);
    if((ret != STATUS_OK) && _ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da leitura", ret, 4);
    if(ret != STATUS_OK) return 0;
    ret = Dispositivo_LeiaInfo(disp, ATA_INFO_SETORES, &setoresTotal);
    if((ret != STATUS_OK) && _ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da leitura", ret, 5);
    if(ret != STATUS_OK) return 0;
    ret = Dispositivo_LeiaInfo(disp, ATA_INFO_SETORES_POR_CILINDROS, &setoresPorCilindroTotal);
    if((ret != STATUS_OK) && _ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da leitura", ret, 6);
    if(ret != STATUS_OK) return 0;
    ret = Dispositivo_LeiaTamanhoDeUmBloco(disp, &tamBloco);
    if((ret != STATUS_OK) && _ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da leitura", ret, 7);
    if(ret != STATUS_OK) return 0;
    if(tamBloco > tam)
    {
        if((ret != STATUS_OK) && _ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da leitura", ret, 8);
        return 0;
    }
    ret = Dispositivo_LeiaId(disp, &id);
    if((ret != STATUS_OK) && _ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da leitura", ret, 9);
    if(ret != STATUS_OK) return 0;

    // Calcula endereco
    setor = posicao % setoresTotal + 1;
    cabeca = (posicao + 1 - setor) % setoresPorCilindroTotal / setoresTotal;
    cilindro = (posicao + 1 - setor) / (setoresPorCilindroTotal);

    // Se for um endereco invalido encerra
    if(cilindro >= cilindrosTotal)
    {
        if(_ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da leitura", ret, 10);
        return 0;
    }

    // Seleciona dispositivo
    Dispositivo_EscrevaBytePorta(disp, ATA_PORTA_ES_DISP_E_CABECA, (id == 0? 0xa0 : 0xb0) | cabeca);

    // Escreve parametros
    Dispositivo_EscrevaBytePorta(disp, ATA_PORTA_ES_QTD_SETORES, 1);
    Dispositivo_EscrevaBytePorta(disp, ATA_PORTA_ES_SETOR, setor);
    Dispositivo_EscrevaBytePorta(disp, ATA_PORTA_ES_CILINDRO_BAIXO, cilindro & 0xff);
    Dispositivo_EscrevaBytePorta(disp, ATA_PORTA_ES_CILINDRO_ALTO, (cilindro >> 8) & 0xff);

    // Solicita um bloco
    Dispositivo_EscrevaBytePorta(disp, ATA_PORTA__S_COMANDO, 0x20);

    // Espera processamento
    ok = NAO;
    for(Pos_t i = 0; i < 200; i++)
    {
        Cronometro_Espera(1);
        Dispositivo_LeiaBytePorta(disp, ATA_PORTA_E__STATUS, &status);
        if((!(status & ATA_MASCARA_STATUS_OCUPADO)))
        {
            ok = SIM;
            break;
        }
    }
    if(ok == NAO)
    {
        if(_ATA_ExibeErro) Mensagem6("ATA", "Erro {0:u} na fase {1:u} da leitura. Status: {2:h} CHS: {3:u}:{4:u}:{5:u}", ret, 11, status, cilindro, cabeca, setor);
        return 0;
    }
    for (Pos_t i = 0; i < tamBloco; i+=2)
    {
        Dispositivo_LeiaUShortPorta(disp, ATA_PORTA_ES_DADOS, (UShort_t *)&destino[i]);
    }
    return tamBloco;
}

Tam_t _ATA_Escreva(Dispositivo_t * dispositivo, Pos_t posicao, Byte_t * origem, Tam_t tam )
{
    Pos_t disp = dispositivo->Dispositivo;
    UInt_t tipo = 0;
    UInt_t cilindrosTotal = 0;
    UInt_t cabecasTotal = 0;
    UInt_t setoresTotal = 0;
    UInt_t setoresPorCilindroTotal = 0;
    UInt_t cilindro = 0;
    UInt_t cabeca = 0;
    UInt_t setor = 0;
    UInt_t id = 0;
    UInt_t tamBloco = 0;
    Byte_t status = 0;
    Boleano_t ok = SIM;

    // Le parametros do dispositivo
    Status_t ret = Dispositivo_LeiaInfo(disp, ATA_INFO_TIPO, &tipo);
    if((ret != STATUS_OK) && _ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da escrita", ret, 1);
    if(ret != STATUS_OK) return 0;
    if((tipo != ATA_TIPO_CHS) & (tipo != ATA_TIPO_SATA)) 
    {
        if(_ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da escrita", ret, 2);
        return 0;
    }
    ret = Dispositivo_LeiaInfo(disp, ATA_INFO_CILINDROS, &cilindrosTotal);
    if((ret != STATUS_OK) && _ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da escrita", ret, 3);
    if(ret != STATUS_OK) return 0;
    ret = Dispositivo_LeiaInfo(disp, ATA_INFO_CABECAS, &cabecasTotal);
    if((ret != STATUS_OK) && _ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da escrita", ret, 4);
    if(ret != STATUS_OK) return 0;
    ret = Dispositivo_LeiaInfo(disp, ATA_INFO_SETORES, &setoresTotal);
    if((ret != STATUS_OK) && _ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da escrita", ret, 5);
    if(ret != STATUS_OK) return 0;
    ret = Dispositivo_LeiaInfo(disp, ATA_INFO_SETORES_POR_CILINDROS, &setoresPorCilindroTotal);
    if((ret != STATUS_OK) && _ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da escrita", ret, 6);
    if(ret != STATUS_OK) return 0;
    ret = Dispositivo_LeiaTamanhoDeUmBloco(disp, &tamBloco);
    if((ret != STATUS_OK) && _ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da escrita", ret, 7);
    if(ret != STATUS_OK) return 0;
    if(tamBloco > tam)
    {
        if((ret != STATUS_OK) && _ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da escrita", ret, 8);
        return 0;
    }
    ret = Dispositivo_LeiaId(disp, &id);
    if((ret != STATUS_OK) && _ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da escrita", ret, 9);
    if(ret != STATUS_OK) return 0;

    // Calcula endereco
    setor = posicao % setoresTotal + 1;
    cabeca = (posicao + 1 - setor) % setoresPorCilindroTotal / setoresTotal;
    cilindro = (posicao + 1 - setor) / (setoresPorCilindroTotal);

    // Se for um endereco invalido encerra
    if(cilindro >= cilindrosTotal)
    {
        if(_ATA_ExibeErro) Mensagem2("ATA", "Erro {0:u} na fase {1:u} da escrita", ret, 10);
        return 0;
    }

    // Seleciona dispositivo
    Dispositivo_EscrevaBytePorta(disp, ATA_PORTA_ES_DISP_E_CABECA, (id == 0? 0xa0 : 0xb0) | cabeca);

    // Escreve parametros
    Dispositivo_EscrevaBytePorta(disp, ATA_PORTA_ES_QTD_SETORES, 1);
    Dispositivo_EscrevaBytePorta(disp, ATA_PORTA_ES_SETOR, setor);
    Dispositivo_EscrevaBytePorta(disp, ATA_PORTA_ES_CILINDRO_BAIXO, cilindro & 0xff);
    Dispositivo_EscrevaBytePorta(disp, ATA_PORTA_ES_CILINDRO_ALTO, (cilindro >> 8) & 0xff);

    // Solicita um bloco
    Dispositivo_EscrevaBytePorta(disp, ATA_PORTA__S_COMANDO, 0x30);

    // Espera processamento
    ok = NAO;
    for(Pos_t i = 0; i < 200; i++)
    {
        Cronometro_Espera(1);
        Dispositivo_LeiaBytePorta(disp, ATA_PORTA_E__STATUS, &status);
        if((!(status & ATA_MASCARA_STATUS_OCUPADO)))
        {
            ok = SIM;
            break;
        }
    }
    if(ok == NAO)
    {
        if(_ATA_ExibeErro) Mensagem6("ATA", "Erro {0:u} na fase {1:u} da escrita. Status: {2:h} CHS: {3:u}:{4:u}:{5:u}", ret, 11, status, cilindro, cabeca, setor);
        return 0;
    }
    for (Pos_t i = 0; i < tamBloco; i+=2)
    {
        Dispositivo_EscrevaUShortPorta(disp, ATA_PORTA_ES_DADOS, (UShort_t)(origem[i] | (origem[i+1] << 8)));
    }
    return tamBloco;
}

Status_t _ATA_CriaDispositivo(UShort_t portaBase, UShort_t portaControleBase, Pos_t numero, Pos_t id, Pos_t * dispSaida)
{
    Pos_t disp = 0;
    Status_t ret = Dispositivo_Registra(&disp, 0, "ATA ", SIM, numero, id, 512, 0, &_ATA_Leia, &_ATA_Escreva);
    if(ret != STATUS_OK) return ret;
    Dispositivo_DefinePorta(disp, ATA_PORTA_ES_DADOS, portaBase + 0);
    Dispositivo_DefinePorta(disp, ATA_PORTA_E__ERRO, portaBase + 1);
    Dispositivo_DefinePorta(disp, ATA_PORTA__S_RECURSOS, portaBase + 1);
    Dispositivo_DefinePorta(disp, ATA_PORTA_ES_QTD_SETORES, portaBase + 2);
    Dispositivo_DefinePorta(disp, ATA_PORTA_ES_SETOR, portaBase + 3);
    Dispositivo_DefinePorta(disp, ATA_PORTA_ES_CILINDRO_BAIXO, portaBase + 4);
    Dispositivo_DefinePorta(disp, ATA_PORTA_ES_CILINDRO_ALTO, portaBase + 5);
    Dispositivo_DefinePorta(disp, ATA_PORTA_ES_DISP_E_CABECA, portaBase + 6);
    Dispositivo_DefinePorta(disp, ATA_PORTA_E__STATUS, portaBase + 7);
    Dispositivo_DefinePorta(disp, ATA_PORTA__S_COMANDO, portaBase + 7);
    Dispositivo_DefinePorta(disp, ATA_PORTA_E__STATUS_ALT, portaControleBase + 0);
    Dispositivo_DefinePorta(disp, ATA_PORTA__S_CONTROLE_DISP, portaControleBase + 1);
    Dispositivo_DefinePorta(disp, ATA_PORTA_E__ENDERECO_DISP, portaControleBase + 1);
    Dispositivo_DefineInfo(disp, ATA_INFO_STATUS, ATA_STATUS_DISP_CRIADO);
    Dispositivo_DefineInfo(disp, ATA_INFO_TIPO, ATA_TIPO_DESCONHECIDO);
    *dispSaida = disp;
    return ret;
}

Status_t _ATA_CriaUnidade(Pos_t disp)
{
    UInt_t status = 0;
    Status_t ret = Dispositivo_LeiaInfo(disp, ATA_INFO_STATUS, &status);
    if(ret != STATUS_OK) return ret;
    if(status != ATA_STATUS_IDENTIFICADO) return STATUS_DISPOSITIVO_INVALIDO;
    return STATUS_OK;   
}

void ATA()
{
    Byte_t * temp = Mem_Local_Aloca(2048);
    SByte_t nome[] = "ATA  ";

    Pos_t disp[4];
    if(_ATA_CriaDispositivo(0x1f0, 0x3f6, 0, 0, &disp[0]) != STATUS_OK)
    {
        Mensagem2("ATA", "Nao foi possivel registrar um dispositivo no barramento {0:u}:{1:u}", 0, 0);
        disp[0] = 0;
    }
    if(_ATA_CriaDispositivo(0x1f0, 0x3f6, 1, 1, &disp[1]) != STATUS_OK)
    {
        Mensagem2("ATA", "Nao foi possivel registrar um dispositivo no barramento {0:u}:{1:u}", 0, 1);
        disp[1] = 0;
    }
    if(_ATA_CriaDispositivo(0x170, 0x376, 2, 0, &disp[2]) != STATUS_OK)
    {
        Mensagem2("ATA", "Nao foi possivel registrar um dispositivo no barramento {0:u}:{1:u}", 1, 0);
        disp[2] = 0;
    }
    if(_ATA_CriaDispositivo(0x170, 0x376, 3, 1, &disp[3]) != STATUS_OK)
    {
        Mensagem2("ATA", "Nao foi possivel registrar um dispositivo no barramento {0:u}:{1:u}", 1, 1);
        disp[3] = 0;
    }

    // Desativa IRQ dos dois Controladores
    if(disp[0] != 0)
    {
        Dispositivo_EscrevaBytePorta(disp[0], ATA_PORTA__S_COMANDO, 2);
    }
    if(disp[2] != 0)
    {
        Dispositivo_EscrevaBytePorta(disp[2], ATA_PORTA__S_COMANDO, 2);
    }


    for (Pos_t i = 0; i < 4; i++)
    {
        if(disp[i] != 0)
        {
            Pos_t contador = 0;
            Pos_t id = 0;
            Byte_t status = 0;
            Byte_t cilindroBaixo = 0;
            Byte_t cilindroAlto = 0;
            UInt_t tipo = ATA_TIPO_DESCONHECIDO;
            Boleano_t ok = SIM;
            UShort_t cilindros = 0;
            UShort_t cabecas = 0;
            UShort_t setores = 0;
            if(Dispositivo_LeiaId(disp[i], &id) == STATUS_OK)
            {
                // Define dispositivo inicialmente como desconhecido
                Dispositivo_DefineInfo(disp[i], ATA_INFO_TIPO, tipo);

                // Seleciona o dispositivo
                Dispositivo_EscrevaBytePorta(disp[i], ATA_PORTA_ES_DISP_E_CABECA, id == 0? 0xa0 : 0xb0);

                // Zera
                Dispositivo_EscrevaBytePorta(disp[i], ATA_PORTA_ES_QTD_SETORES, 0);
                Dispositivo_EscrevaBytePorta(disp[i], ATA_PORTA_ES_SETOR, 0);
                Dispositivo_EscrevaBytePorta(disp[i], ATA_PORTA_ES_CILINDRO_BAIXO, 0);
                Dispositivo_EscrevaBytePorta(disp[i], ATA_PORTA_ES_CILINDRO_ALTO, 0);

                // Envia comando de identificacao
                Dispositivo_EscrevaBytePorta(disp[i], ATA_PORTA__S_COMANDO, 0xec);

                // Espera o processamento;
                Cronometro_Espera(4);

                // Le status
                Dispositivo_LeiaBytePorta(disp[i], ATA_PORTA_E__STATUS, &status);
                if(status != 0)
                {
                    // Contem um dispositivo
                    // Espera deixar de ficar ocupado
                    ok = NAO;
                    contador = 0;
                    while(contador < 40)
                    {
                        // Espera o processamento
                        Cronometro_Espera(1);
                        // Verifica se ainda ocupado
                        Dispositivo_LeiaBytePorta(disp[i], ATA_PORTA_E__STATUS, &status);
                        if(!(status & ATA_MASCARA_STATUS_OCUPADO))
                        {
                            ok = SIM;
                            break;
                        }
                        contador++;
                    }
                    if(!ok) continue;
                    // Verifica o tipo
                    Dispositivo_LeiaBytePorta(disp[i], ATA_PORTA_ES_CILINDRO_BAIXO, &cilindroBaixo);
                    Dispositivo_LeiaBytePorta(disp[i], ATA_PORTA_ES_CILINDRO_ALTO, &cilindroAlto);
                    if((cilindroBaixo == 0x69) & (cilindroAlto == 0x96))
                    {
                        Mensagem2("ATA", "Encontrado um dispositivo SATAPI no barramento {0:u}:{1:u}", i /2, id);
                        Dispositivo_DefineInfo(disp[i], ATA_INFO_TIPO, ATA_TIPO_SATAPI);
                    }
                    else if((cilindroBaixo == 0x14) & (cilindroAlto == 0xeb))
                    {
                        Mensagem2("ATA", "Encontrado um dispositivo ATAPI no barramento {0:u}:{1:u}", i /2, id);
                        Dispositivo_DefineInfo(disp[i], ATA_INFO_TIPO, ATA_TIPO_ATAPI);
                    }
                    else if((cilindroBaixo == 0x3c) & (cilindroAlto == 0xc3))
                    {
                        Mensagem2("ATA", "Encontrado um dispositivo SATA no barramento {0:u}:{1:u}", i /2, id);
                        Dispositivo_DefineInfo(disp[i], ATA_INFO_TIPO, ATA_TIPO_SATA);
                    }
                    else if((cilindroBaixo == 0) & (cilindroAlto == 0))
                    {
                        Mensagem2("ATA", "Encontrado um dispositivo ATA no barramento {0:u}:{1:u}", i /2, id);
                        Dispositivo_DefineInfo(disp[i], ATA_INFO_TIPO, ATA_TIPO_CHS);
                        contador = 0;
                        while (contador < 40)
                        {
                            // Espera o processamento
                            Cronometro_Espera(1);
                            // Se maior que 0, contem um dispositivo
                            Dispositivo_LeiaBytePorta(disp[i], ATA_PORTA_E__STATUS, &status);
                            if(status & ATA_MASCARA_STATUS_ERRO)
                            {
                                break;
                            }
                            if ((status & ATA_MASCARA_STATUS_DADOS_CORRIGIDOS) & (!(status & ATA_MASCARA_STATUS_OCUPADO)))
                            {
                                break;
                            }
                            contador++;
                        }
                    }
                    else
                    {
                        Mensagem4("ATA", "Dispositivo desconhecido no barramento {0:u}:{1:u} CL:0x{2:h} CH:0x{3:h}", i /2, id, cilindroBaixo, cilindroAlto);
                        // Dispositivo estranho
                        continue;
                    }
                }
                Dispositivo_LeiaInfo(disp[i], ATA_INFO_TIPO, &tipo);
                if(tipo != ATA_TIPO_DESCONHECIDO)
                {
                    // Envia pacote de identificacao
                    Dispositivo_EscrevaBytePorta(disp[i], ATA_PORTA__S_COMANDO, 0xa1);

                    // Espera
                    Cronometro_Espera(10);

                    // Dispositivo valido, le informação
                    for (Pos_t j = 0; j < 512; j+=2)
                    {
                        Dispositivo_LeiaUShortPorta(disp[i], ATA_PORTA_ES_DADOS, (UShort_t*)&temp[j]);
                    }
                    Mensagem5("ATA", "ATA {4:u}: [{0:u}] CHS: {1:u}:{2:u}:{3:u}", temp[1] << 8 | temp[0], temp[3] << 8 | temp[2], temp[7] << 8 | temp[6], temp[13] << 8 | temp[12], i);
                    cilindros = temp[3] << 8 | temp[2];
                    cabecas = temp[7] << 8 | temp[6];
                    setores = temp[13] << 8 | temp[12];
                    Dispositivo_DefineInfo(disp[i], ATA_INFO_CILINDROS, cilindros);
                    Dispositivo_DefineInfo(disp[i], ATA_INFO_CABECAS, cabecas);
                    Dispositivo_DefineInfo(disp[i], ATA_INFO_SETORES, setores);
                    Dispositivo_DefineInfo(disp[i], ATA_INFO_SETORES_POR_CILINDROS, cabecas * setores);
                    Dispositivo_DefineTamanho(disp[i], cilindros * cabecas * setores);
                    nome[4] = i + '0';
                    Pos_t unidade = 0;
                    Unidade_Registra(nome, &unidade, disp[i]);
                }
            }
        }
    }
    
    Mem_Local_Libera(temp);
}