#include <HUSIS.h>

TeclaRegistro_t _Teclado_Teclas[256];

TeclaRegistro_t * _Teclado_Atual = 0;
Byte_t _Teclado_AtualAscii = 0;
Tecla_t _Teclado_AtualTecla = 0;
Tam_t _Teclado_AtualInicio = 0;
Boleano_t _Teclado_Shift = NAO;
Boleano_t _Teclado_Alt = NAO;
Boleano_t _Teclado_Ctrl = NAO;
Boleano_t _Teclado_Extendido = NAO;
Boleano_t _Teclado_ExibeTeclasDesconhecidas = NAO;
Boleano_t _Teclado_ExibeTodasAsTeclas = NAO;

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
    else if(tecla->Pressionado)
    {
        if((_Teclado_Shift) & (tecla->TeclaComShift > 0))
        {
            _Teclado_AtualAscii = tecla->AsciiComShift;
            _Teclado_AtualTecla = tecla->TeclaComShift;
            encontrado = SIM;
        }
        else if((_Teclado_Alt) & (tecla->TeclaComAlt > 0))
        {
            _Teclado_AtualAscii = tecla->AsciiComAlt;
            _Teclado_AtualTecla = tecla->TeclaComAlt;
            encontrado = SIM;
        }
        else if((_Teclado_Extendido) & (tecla->TeclaExtendido > 0))
        {
            _Teclado_AtualAscii = tecla->AsciiExtendido;
            _Teclado_AtualTecla = tecla->TeclaExtendido;
            encontrado = SIM;
        }
        else if (tecla->Tecla > 0)
        {
            _Teclado_AtualAscii = tecla->Ascii;
            _Teclado_AtualTecla = tecla->Tecla;
            encontrado = SIM;
        }
        if(encontrado)
        {
            _Teclado_Atual = tecla;
            _Teclado_AtualInicio = Cronometro_Contador();
        }
    }
    else if(_Teclado_Atual > 0 && _Teclado_Atual->Pos == tecla->Pos)
    {
        encontrado = SIM;
        _Teclado_Atual = 0;
        _Teclado_AtualAscii = 0;
        _Teclado_AtualTecla = 0;
        _Teclado_AtualInicio = 0;
    }
    if(encontrado)
    {
        if(_Teclado_Acao != 0) _Teclado_Acao(_Teclado_AtualTecla, _Teclado_AtualAscii, _Teclado_Shift, _Teclado_Alt, _Teclado_Ctrl, _Teclado_Extendido, tecla->Pressionado);
    }
    else if(_Teclado_ExibeTeclasDesconhecidas)
    {
        Mensagem7("Teclado", "Codigo: 0x{0:h}={0:u}: TECLA {1:u} = {2:c} SHIFT {3:s} ALT {4:s} CTRL {5:s} EXT {6:s}", codigo, (Tam_t)_Teclado_AtualTecla, (Tam_t)_Teclado_AtualAscii, (Tam_t)_Teclado_Shift, (Tam_t)_Teclado_Alt, (Tam_t)_Teclado_Ctrl, (Tam_t)_Teclado_Extendido);
    }
    if(_Teclado_ExibeTodasAsTeclas)
    {
        Mensagem7("Teclado", "Codigo: 0x{0:h}={0:u}: TECLA {1:u} = {2:c} SHIFT {3:s} ALT {4:s} CTRL {5:s} EXT {6:s}", codigo, (Tam_t)_Teclado_AtualTecla, (Tam_t)_Teclado_AtualAscii, (Tam_t)_Teclado_Shift, (Tam_t)_Teclado_Alt, (Tam_t)_Teclado_Ctrl, (Tam_t)_Teclado_Extendido);
    }
    if(ignoraExtendido == NAO) _Teclado_Extendido = NAO;
}

void Teclado_ExibeDesconhecidas()
{
    _Teclado_ExibeTeclasDesconhecidas = SIM;
}

void Teclado_ExibeTodas()
{
    _Teclado_ExibeTodasAsTeclas = SIM;
    _Teclado_ExibeTeclasDesconhecidas = NAO;
}

void Teclado_Oculta()
{
    _Teclado_ExibeTeclasDesconhecidas = NAO;
    _Teclado_ExibeTodasAsTeclas = NAO;
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