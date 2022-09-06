#include <HUSIS.h>

TeclaRegistro_t _Teclado_Teclas[256];

TeclaRegistro_t * _Teclado_Atual = 0;
Byte_t _Teclado_Atual_Ascii = 0;
Tecla_t _Teclado_Atual_Tecla = 0;
Tam_t _Teclado_Atual_Inicio = 0;
Boleano_t _Teclado_Shift = NAO;
Boleano_t _Teclado_Alt = NAO;
Boleano_t _Teclado_Ctrl = NAO;
Boleano_t _Teclado_Extendido = NAO;

Status_t (* _Teclado_Acao) (Tecla_t tecla, Byte_t ascii, Boleano_t shift, Boleano_t alt, Boleano_t ctrl, Boleano_t ext, Boleano_t pressionado) = 0;

void Teclado_Processa(Regs_t * regs)
{
    Byte_t codigo = ES_LeiaByte(0x60);
    TeclaRegistro_t * tecla = &_Teclado_Teclas[codigo];
    Boleano_t ignoraExtendido = NAO;
    Boleano_t encontrado = NAO;
    if(tecla->Tecla == TECLA_EXTENDIDO)
    {
        _Teclado_Extendido = SIM;
        ignoraExtendido = SIM;
        encontrado = SIM;
    }
    else if(tecla->Tecla & TECLA_ESPECIAL_SHIFT)
    {
        _Teclado_Shift = tecla->Pressionado;
        encontrado = SIM;
    }
    else if(tecla->Tecla & TECLA_ESPECIAL_ALT)
    {
        _Teclado_Alt = tecla->Pressionado;
        encontrado = SIM;
    }
    else if(tecla->Tecla & TECLA_ESPECIAL_CTRL)
    {
        _Teclado_Ctrl = tecla->Pressionado;
        encontrado = SIM;
    }
    else if(tecla->Pressionado == SIM)
    {
        if((_Teclado_Shift) & (tecla->TeclaComShift > 0))
        {
            _Teclado_Atual_Ascii = tecla->AsciiComShift;
            _Teclado_Atual_Tecla = tecla->TeclaComShift;
            encontrado = SIM;
        }
        else if((_Teclado_Alt) & (tecla->TeclaComAlt > 0))
        {
            _Teclado_Atual_Ascii = tecla->AsciiComAlt;
            _Teclado_Atual_Tecla = tecla->TeclaComAlt;
            encontrado = SIM;
        }
        else if((_Teclado_Extendido) & (tecla->TeclaExtendido > 0))
        {
            _Teclado_Atual_Ascii = tecla->AsciiExtendido;
            _Teclado_Atual_Tecla = tecla->TeclaExtendido;
            encontrado = SIM;
        }
        else if (tecla->Tecla > 0)
        {
            _Teclado_Atual_Ascii = tecla->Ascii;
            _Teclado_Atual_Tecla = tecla->Tecla;
            encontrado = SIM;
        }
        if(encontrado)
        {
            _Teclado_Atual = tecla;
            _Teclado_Atual_Inicio = Cronometro_Contador();
        }
    }
    else if(_Teclado_Atual > 0 && _Teclado_Atual->Pos == tecla->Pos)
    {
        encontrado = SIM;
        _Teclado_Atual = 0;
        _Teclado_Atual_Ascii = 0;
        _Teclado_Atual_Tecla = 0;
        _Teclado_Atual_Inicio = 0;
    }
    if(encontrado)
    {
        if(_Teclado_Acao != 0) _Teclado_Acao(_Teclado_Atual_Tecla, _Teclado_Atual_Ascii, _Teclado_Shift, _Teclado_Alt, _Teclado_Ctrl, _Teclado_Extendido, tecla->Pressionado);
    }
    else if(MSG_EXIBE_TECLA_DESCONHECIDA)
    {
        Mensagem7("Teclado", "Codigo: 0x{0:h}={0:u}: TECLA {1:u} = {2:c} SHIFT {3:s} ALT {4:s} CTRL {5:s} EXT {6:s}", codigo, (Tam_t)_Teclado_Atual_Tecla, (Tam_t)_Teclado_Atual_Ascii, (Tam_t)_Teclado_Shift, (Tam_t)_Teclado_Alt, (Tam_t)_Teclado_Ctrl, (Tam_t)_Teclado_Extendido);
    }
    if(ignoraExtendido == NAO) _Teclado_Extendido = NAO;
}

Status_t Teclado_Registra(Pos_t codigo, Byte_t ascii, Tecla_t tecla)
{
    if(codigo > 256) return STATUS_POSICAO_INVALIDA;
    _Teclado_Teclas[codigo].Pos = codigo;
    _Teclado_Teclas[codigo].Ascii = ascii;
    _Teclado_Teclas[codigo].Tecla = tecla;
    _Teclado_Teclas[codigo].Pressionado = SIM;
    if(codigo < 128)
    {
        _Teclado_Teclas[codigo | 0b10000000].Pos = codigo;
        _Teclado_Teclas[codigo | 0b10000000].Ascii = ascii;
        _Teclado_Teclas[codigo | 0b10000000].Tecla = tecla;
        _Teclado_Teclas[codigo | 0b10000000].Pressionado = NAO;
    }
    return STATUS_OK;
}

Status_t Teclado_RegistraAlt(Pos_t codigo, Byte_t ascii, Tecla_t tecla)
{
    if(codigo > 127) return STATUS_POSICAO_INVALIDA;
    _Teclado_Teclas[codigo].Pos = codigo;
    _Teclado_Teclas[codigo].AsciiComAlt = ascii;
    _Teclado_Teclas[codigo].TeclaComAlt = tecla;
    _Teclado_Teclas[codigo].Pressionado = SIM;
    _Teclado_Teclas[codigo | 0b10000000].Pos = codigo;
    _Teclado_Teclas[codigo | 0b10000000].AsciiComAlt = ascii;
    _Teclado_Teclas[codigo | 0b10000000].TeclaComAlt = tecla;
    _Teclado_Teclas[codigo | 0b10000000].Pressionado = NAO;
    return STATUS_OK;
}

Status_t Teclado_RegistraShift(Pos_t codigo, Byte_t ascii, Tecla_t tecla)
{
    if(codigo > 127) return STATUS_POSICAO_INVALIDA;
    _Teclado_Teclas[codigo].Pos = codigo;
    _Teclado_Teclas[codigo].AsciiComShift = ascii;
    _Teclado_Teclas[codigo].TeclaComShift = tecla;
    _Teclado_Teclas[codigo].Pressionado = SIM;
    _Teclado_Teclas[codigo | 0b10000000].Pos = codigo;
    _Teclado_Teclas[codigo | 0b10000000].AsciiComShift = ascii;
    _Teclado_Teclas[codigo | 0b10000000].TeclaComShift = tecla;
    _Teclado_Teclas[codigo | 0b10000000].Pressionado = NAO;
    return STATUS_OK;
}

Status_t Teclado_RegistraExt(Pos_t codigo, Byte_t ascii, Tecla_t tecla)
{
    if(codigo > 127) return STATUS_POSICAO_INVALIDA;
    _Teclado_Teclas[codigo].Pos = codigo;
    _Teclado_Teclas[codigo].AsciiExtendido = ascii;
    _Teclado_Teclas[codigo].TeclaExtendido = tecla;
    _Teclado_Teclas[codigo].Pressionado = SIM;
    _Teclado_Teclas[codigo | 0b10000000].Pos = codigo;
    _Teclado_Teclas[codigo | 0b10000000].AsciiExtendido = ascii;
    _Teclado_Teclas[codigo | 0b10000000].TeclaExtendido = tecla;
    _Teclado_Teclas[codigo | 0b10000000].Pressionado = NAO;
    return STATUS_OK;
}

void Teclado_RegistraAcao(Status_t (* acao) (Tecla_t tecla, Byte_t ascii, Boleano_t shift, Boleano_t alt, Boleano_t ctrl, Boleano_t ext, Boleano_t pressionado))
{
    _Teclado_Acao = acao;
}

void Teclado()
{
    for (Tam_t i = 0; i < 128; i++)
    {
        Teclado_Registra(i, 0, 0);
        Teclado_RegistraAlt(i, 0, 0);
        Teclado_RegistraShift(i, 0, 0);
    }
    Teclado_EnUsIntl();
    
    Teclado_RegistraAcao(&Term_ProcessaTeclado);

    IRQ_Registra(1, &Teclado_Processa);
}