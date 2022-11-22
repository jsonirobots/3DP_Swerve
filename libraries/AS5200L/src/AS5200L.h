/*
  AS5200L.h - Library for handling AS5200L magnetic encoder over I2C bus.
  Created by Amadeusz Świerk and Juliusz Tarnowski, 11.03.2020
  Released into the public domain.
*/

#ifndef AS5200L_h
#define AS5200L_h

#include "Arduino.h"
#include <Wire.h>

#define bottomSensorAddress 0x40
#define topSensorAddress 0x41

// power mode
#define AS5200L_power_mode_normal 0
#define AS5200L_power_mode_lpm1 1
#define AS5200L_power_mode_lpm2 2
#define AS5200L_power_mode_lpm3 3

// hysteresis
#define AS5200L_hysteresis_off 0
#define AS5200L_hysteresis_lsbx1 1
#define AS5200L_hysteresis_lsbx2 2
#define AS5200L_hysteresis_lsbx3 3

// slow filter
#define AS5200L_slow_filter_lsbx16 0
#define AS5200L_slow_filter_lsbx8 1
#define AS5200L_slow_filter_lsbx4 2
#define AS5200L_slow_filter_lsbx2 3

// fast filter
#define AS5200L_fast_filter_off 0
#define AS5200L_fast_filter_lsbx6 1
#define AS5200L_fast_filter_lsbx7 2
#define AS5200L_fast_filter_lsbx9 3
#define AS5200L_fast_filter_lsbx18 4
#define AS5200L_fast_filter_lsbx21 5
#define AS5200L_fast_filter_lsbx24 6
#define AS5200L_fast_filter_lsbx10 7

// magnetState
#define AS5200L_magnet_notDetected 0
#define AS5200L_magnet_tooWeak 1
#define AS5200L_magnet_tooStrong 2
#define AS5200L_magnet_correct 3


class AS5200L
{
public:
    AS5200L(uint16_t adress);
    AS5200L(uint16_t SDA, uint16_t SCL, uint16_t adress);

    // Output registers
    uint16_t getRawAngle();          // RAW ANGLE: RAW ANGLE
    bool isRawAngleFromFastFilter(); // RAW ANGLE: RAWFF
    uint16_t getAngle();             // ANGLE: ANGLE
    bool isSlowFilterBusy();         // RAW ANGLE: SFBUSY

    // Configuration registers
    uint16_t getStartPosition();              // ZPOS: ZPOS
    void setStartPosition(uint16_t angle);    // ZPOS: ZPOS
    uint16_t getStopPosition();               // MPOS: MPOS
    void setStopPosition(uint16_t angle);     // MPOS: MPOS
    void setPowerMode(uint8_t mode);          // CONF: PM
    void setHysteresis(uint8_t mode);         // CONF: HYST
    void setPWMOutput(uint16_t pwmFrequency); // CONF: OUTS, PWMF
    void setSlowFilter(uint8_t mode);         // CONF: SF
    void setFastFilter(uint8_t mode);         // CONF: FTH
    void setLPMTimer(bool value);             // CONF: ALPM

    // Status registers
    uint8_t getMagnetStatus(); // STATUS: MD, ML, MH
    uint8_t getGain();         // STATUS: AGC
    uint16_t getMagnitude();   // MAGNITUDE: MAGNITUDE
    bool isSensorOverflowed(); // STATUS: ADCOF

    // Burn commands
    bool burnAngle();
    void burnSettings();

private:
    uint8_t _sensorSDAPin = 0;
    uint8_t _sensorSCLPin = 0;
    uint8_t _sensorAddress = 0;

    byte _configurationZposLSBAddress = 0x02;
    byte _configurationZposMSBAddress = 0x01;
    byte _configurationMposLSBAddress = 0x04;
    byte _configurationMposMSBAddress = 0x03;
    byte _configurationConfLSBAddress = 0x08;
    byte _configurationConfMSBAddress = 0x07;

    byte _outputRawAngleLSBAddress = 0x0D;
    byte _outputRawAngleMSBAddress = 0x0C;
    byte _outputAngleLSBAddress = 0x0F;
    byte _outputAngleMSBAddress = 0x0E;

    byte _statusStatusAddress = 0x0B;
    byte _statusGainAddress = 0x1A;
    byte _statusMagnitudeLSBAddress = 0x1C;
    byte _statusMagnitudeMSBAddress = 0x1B;

    byte _burnAdress = 0xFF;
    byte _burnAngleValue = 0x80;
    byte _burnSettingsValue = 0x40;

    uint8_t _msb;
    uint8_t _lsb;
    uint8_t _msbMask = 0b00001111;

    uint8_t _get8BitRegister(byte registerAddress);
    uint16_t _get12BitRegister(byte registerMSBAdress, byte registerLSBAddress);
    bool _getBit(byte value, uint8_t bitNumber);
    void _writeToRegister(byte value, byte registerAddress);
};

#endif