#include "Arduino.h"
#include "AS5200L.h"

AS5200L::AS5200L(uint16_t address)
{
    _sensorAddress = address;
    Wire.begin();
}

AS5200L::AS5200L(uint16_t SDA, uint16_t SCL, uint16_t address)
{
    _sensorSDAPin = SDA;
    _sensorSCLPin = SCL;
    _sensorAddress = address;
    Wire.begin(_sensorSDAPin, _sensorSCLPin);
}

uint8_t AS5200L::_get8BitRegister(byte registerAddress)
{
    _msb = 0;

    Wire.beginTransmission(_sensorAddress);
    Wire.write(registerAddress);
    Wire.endTransmission();

    Wire.requestFrom(_sensorAddress, (uint8_t)1);
    if (Wire.available() <= 1)
    {
        _msb = Wire.read();
    }

    return _msb;
}

uint16_t AS5200L::_get12BitRegister(byte registerMSBAddress, byte registerLSBAddress)
{
    _lsb = 0;
    _msb = 0;

    Wire.beginTransmission(_sensorAddress);
    Wire.write(registerMSBAddress);
    Wire.endTransmission();
    delay(10);

    Wire.requestFrom(_sensorAddress, (uint8_t)1);

    if (Wire.available() <= 1)
    {
        _msb = Wire.read();
    }

    Wire.requestFrom(_sensorAddress, (uint8_t)1);

    Wire.beginTransmission(_sensorAddress);
    Wire.write(registerLSBAddress);
    Wire.endTransmission();

    if (Wire.available() <= 1)
    {
        _lsb = Wire.read();
    }

    return (_lsb) + (_msb & _msbMask) * 256;
}

bool AS5200L::_getBit(byte value, uint8_t bitNumber)
{
    return (bool)((value & (1 << bitNumber)) >> bitNumber);
}

void AS5200L::_writeToRegister(byte value, byte registerAddress) // needs to be checked for corectness (probably won't work for variables greater than 8 bit)
{

    Wire.beginTransmission(_sensorAddress);
    Wire.write(registerAddress);
    Wire.write(value);
    Wire.endTransmission();
}

uint16_t AS5200L::getRawAngle()
{
    return _get12BitRegister(_outputRawAngleMSBAddress, _outputRawAngleLSBAddress);
}

bool AS5200L::isRawAngleFromFastFilter()
{
    return _getBit(_get8BitRegister(_outputRawAngleMSBAddress), 7);
}

uint16_t AS5200L::getAngle()
{
    return _get12BitRegister(_outputAngleMSBAddress, _outputAngleLSBAddress);
}

bool AS5200L::isSlowFilterBusy()
{
    return _getBit(_get8BitRegister(_outputRawAngleMSBAddress), 6);
}

uint16_t AS5200L::getStartPosition()
{
    return _get12BitRegister(_configurationZposMSBAddress, _configurationZposLSBAddress);
}

void AS5200L::setStartPosition(uint16_t angle)
{
    _lsb = angle & 0xff;
    _msb = angle >> 8;
    _writeToRegister(_msb, _configurationZposMSBAddress);
    _writeToRegister(_lsb, _configurationZposLSBAddress);
}

uint16_t AS5200L::getStopPosition()
{
    return _get12BitRegister(_configurationMposMSBAddress, _configurationMposLSBAddress);
}

void AS5200L::setStopPosition(uint16_t angle)
{
    if (angle - getStartPosition() >= 18)
    {
        _lsb = angle & 0xff;
        _msb = angle >> 8;
        _writeToRegister(_msb, _configurationMposMSBAddress);
        _writeToRegister(_lsb, _configurationMposLSBAddress);
    }
}

void AS5200L::setPowerMode(uint8_t mode)
{
    _msb = _get8BitRegister(_configurationConfLSBAddress);
    switch (mode)
    {
    default:
        bitClear(_msb, 0);
        bitClear(_msb, 1);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case AS5200L_power_mode_lpm1:
        bitSet(_msb, 0);
        bitClear(_msb, 1);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case AS5200L_power_mode_lpm2:
        bitClear(_msb, 0);
        bitSet(_msb, 1);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case AS5200L_power_mode_lpm3:
        bitSet(_msb, 0);
        bitSet(_msb, 1);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;
    }
}

void AS5200L::setHysteresis(uint8_t mode)
{
    _msb = _get8BitRegister(_configurationConfLSBAddress);
    switch (mode)
    {
    default:
        bitClear(_msb, 2);
        bitClear(_msb, 3);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case AS5200L_hysteresis_lsbx1:
        bitSet(_msb, 2);
        bitClear(_msb, 3);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case AS5200L_hysteresis_lsbx2:
        bitClear(_msb, 2);
        bitSet(_msb, 3);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case AS5200L_hysteresis_lsbx3:
        bitSet(_msb, 2);
        bitSet(_msb, 3);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;
    }
}

void AS5200L::setPWMOutput(uint16_t pwmFrequency)
{
    _msb = _get8BitRegister(_configurationConfLSBAddress);
    switch (pwmFrequency)
    {
    default:
        bitClear(_msb, 5);
        bitClear(_msb, 6);
        bitClear(_msb, 7);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case 115:
        bitSet(_msb, 5);
        bitClear(_msb, 6);
        bitClear(_msb, 7);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case 230:
        bitSet(_msb, 5);
        bitSet(_msb, 6);
        bitClear(_msb, 7);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case 460:
        bitSet(_msb, 5);
        bitClear(_msb, 6);
        bitSet(_msb, 7);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case 920:
        bitSet(_msb, 5);
        bitSet(_msb, 6);
        bitSet(_msb, 7);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;
    }
}

void AS5200L::setSlowFilter(uint8_t mode)
{
    _msb = _get8BitRegister(_configurationConfMSBAddress);
    switch (mode)
    {
    default:
        bitClear(_msb, 8);
        bitClear(_msb, 9);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case AS5200L_slow_filter_lsbx8:
        bitSet(_msb, 8);
        bitClear(_msb, 9);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case AS5200L_slow_filter_lsbx4:
        bitClear(_msb, 8);
        bitSet(_msb, 9);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case AS5200L_slow_filter_lsbx2:
        bitSet(_msb, 8);
        bitSet(_msb, 9);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;
    }
}

void AS5200L::setFastFilter(uint8_t mode)
{
    _msb = _get8BitRegister(_configurationConfMSBAddress);
    switch (mode)
    {
    default:
        bitClear(_msb, 10);
        bitClear(_msb, 11);
        bitClear(_msb, 12);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case AS5200L_fast_filter_lsbx6:
        bitSet(_msb, 10);
        bitClear(_msb, 11);
        bitClear(_msb, 12);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case AS5200L_fast_filter_lsbx7:
        bitClear(_msb, 10);
        bitSet(_msb, 11);
        bitClear(_msb, 12);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case AS5200L_fast_filter_lsbx9:
        bitSet(_msb, 10);
        bitSet(_msb, 11);
        bitClear(_msb, 12);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case AS5200L_fast_filter_lsbx18:
        bitClear(_msb, 10);
        bitClear(_msb, 11);
        bitSet(_msb, 12);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case AS5200L_fast_filter_lsbx21:
        bitSet(_msb, 10);
        bitClear(_msb, 11);
        bitSet(_msb, 12);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case AS5200L_fast_filter_lsbx24:
        bitClear(_msb, 10);
        bitSet(_msb, 11);
        bitSet(_msb, 12);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;

    case AS5200L_fast_filter_lsbx10:
        bitSet(_msb, 10);
        bitSet(_msb, 11);
        bitSet(_msb, 12);
        _writeToRegister(_msb, _configurationConfLSBAddress);
        break;
    }
}

void AS5200L::setLPMTimer(bool value)
{
    _msb = _get8BitRegister(_configurationConfMSBAddress);
    if (value)
    {
        bitSet(_msb, 13);
    }
    else
    {
        bitClear(_msb, 13);
    }

    _writeToRegister(_msb, _configurationConfLSBAddress);
}

uint8_t AS5200L::getMagnetStatus()
{
    _msb = _get8BitRegister(_statusStatusAddress);
    uint8_t MH = _getBit(_msb, 3), ML = _getBit(_msb, 4), MD = _getBit(_msb, 5);

    if (!MD)
    {
        return AS5200L_magnet_notDetected;
    }
    else if (ML)
    {
        return AS5200L_magnet_tooWeak;
    }
    else if (MH)
    {
        return AS5200L_magnet_tooStrong;
    }
    else
    {
        return  AS5200L_magnet_correct;
    }
}

uint8_t AS5200L::getGain()
{
    return _get8BitRegister(_statusGainAddress);
}

uint16_t AS5200L::getMagnitude()
{
    return _get12BitRegister(_statusMagnitudeMSBAddress, _statusMagnitudeLSBAddress);
}

bool AS5200L::isSensorOverflowed()
{
    return _getBit(_get8BitRegister(_statusGainAddress), 6);
}

bool AS5200L::burnAngle()
{
    if (getMagnetStatus() ==  AS5200L_magnet_correct)
    {
        _writeToRegister(_burnAngleValue, _burnAdress);
        return true;
    }
    else
    {
        return false;
    }
}

void AS5200L::burnSettings()
{
    _writeToRegister(_burnSettingsValue, _burnAdress);
}
