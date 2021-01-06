/**
 * Treiber fuer Barometersensor mit I2C
 * 
 * Getestet mit BMP180
 * Angeschlossen an Pin 4 und 5 am Arduino Nano (SDA, SCL)
 **/

#include "baro_driver.h"
#include "i2c_driver.h"
#include <util/delay.h>

#define BMP180_ADR 0x77
#define BARO_READ_REGISTER(reg, len) \
    i2c_read_register((BMP180_ADR), (reg), (len), (buf))
#define BARO_WRITE_REGISTER(reg, data) \
    i2c_write_register((BMP180_ADR), (reg), (data))

// Konstanten aus dem EEPROM des Sensors
// Werden zur Umrechnung der Sensorwerte verwendet
int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD;
uint16_t AC4, AC5, AC6;

// Zwischenwerte fuer Umrechnung der Sensorwerte
int32_t X1, X2, X3, B3, B5, B6, UT, UP, T, p;
uint32_t B4, B7;

// Puffer fuer Auslesen von Sensorwerten
uint8_t buf[3];

/**
 * Initialisiert den Sensor, 
 * liest Konstanten aus fuer spaetere Umrechnung der Sensorwerte aus
 * @return Bei Erfolg 1, sonst 0
 **/
uint8_t baro_init(void)
{
    // Wenn alle Konstanten erfolgreich ausgelesen
    if (baro_read_int(0xAA, &AC1) &&
        baro_read_int(0xAC, &AC2) &&
        baro_read_int(0xAE, &AC3) &&
        baro_read_uint(0xB0, &AC4) &&
        baro_read_uint(0xB2, &AC5) &&
        baro_read_uint(0xB4, &AC6) &&
        baro_read_int(0xB6, &B1) &&
        baro_read_int(0xB8, &B2) &&
        baro_read_int(0xBA, &MB) &&
        baro_read_int(0xBC, &MC) &&
        baro_read_int(0xBE, &MD))
        return 1;
    return 0;
}

/**
 * Liest einen vorzeichenbehafteten 16-Bit Integer aus einem Register aus
 * @param reg Registeradresse
 * @param var Pointer fuer ausgelesenen Wert
 * @return Bei Erfolg 1, sonst 0
 **/
uint8_t baro_read_int(uint8_t reg, int16_t *var)
{
    // Zwei Byte auslesen
    if (!BARO_READ_REGISTER(reg, 2))
        return 0;
    // Zusammensetzen
    *var = (((int16_t)buf[1]) << 8) | buf[0];

    return 1;
}

/**
 * Liest einen vorzeichenunbehafteten 16-Bit Integer aus einem Register aus
 * @param reg Registeradresse
 * @param var Pointer fuer ausgelesenen Wert
 **/
uint8_t baro_read_uint(uint8_t reg, uint16_t *var)
{
    // Zwei Byte auslesen
    if (!BARO_READ_REGISTER(reg, 2))
        return 0;
    // Zusammensetzen
    *var = (((uint16_t)buf[1]) << 8) | buf[0];

    return 1;
}

/**
 * Liest den Rohwert des Temperatursensors aus
 * @param temp Pointer fuer ausgelesenen Wert
 * @return Bei Erfolg 1, sonst 0
 **/
uint8_t baro_temp_raw(int32_t *temp)
{
    // Temperaturmessung anfordern
    if (!BARO_WRITE_REGISTER(0xF4, 0x2E))
        return 0;

    // Warten auf Abschluss der Messung
    _delay_ms(5);

    // Zwei Byte auslesen
    if (!BARO_READ_REGISTER(0xF6, 2))
        return 0;

    // Zusammensetzen
    UT = (((int32_t)buf[1]) << 8) | buf[0];
    *temp = UT;

    return 1;
}

/**
 * Liest die Temperatur aus und rechnet sie in 0.1 Grad Celsius (C*10^-1) um
 * @param temp Pointer fuer ausgelesene Temperatur
 * @return Bei Erfolg 1, sonst 0
 **/
uint8_t baro_temp(int32_t *temp)
{
    // Rohwert auslesen
    if (!baro_temp_raw(&UT))
        return 0;

    // Berechnung Temperatur in 0.1 Grad Celsius
    // Formel siehe Datenblatt
    X1 = (UT - AC6) * AC5 / 32768;
    X2 = (int32_t)MC * 2048 / (X1 + MD);
    B5 = X1 + X2;

    T = (B5 + 8) / 16;
    *temp = T;

    return 1;
}

/**
 * Liest den Rohwert des Luftdrucksensors aus, 
 * Genaugkeit durch Oversamplingrate bestimmbar
 * @param pres Pointer fuer ausgelesenen Wert
 * @param oss Oversamplingwert (0 bis 3)
 * @return Bei Erfolg 1, sonst 0
 **/
uint8_t baro_pres_raw(int32_t *pres, uint8_t oss)
{
    // Oversamplingwert begrenzen
    oss &= 0x03;

    // Luftdruckmessung anfordern
    if (!BARO_WRITE_REGISTER(0xF4, 0x34 | (oss << 6)))
        return 0;

    // Warten auf Abschluss der Messung
    // Dauer von Oversamplingrate abhaengig
    switch (oss)
    {
    case 0:
        _delay_ms(5);
        break;
    case 1:
        _delay_ms(8);
        break;
    case 2:
        _delay_ms(14);
        break;
    default:
        _delay_ms(26);
    }

    // Zwei Byte auslesen
    if (!BARO_READ_REGISTER(0xF6, 3))
        return 0;

    // Zusammensetzen
    UP = ((((int32_t)buf[2]) << 16) | ((int32_t)buf[1] << 8) | buf[0]) >> (8 - oss);
    *pres = UP;

    return 1;
}

/**
 * Liest den Luftdruck aus und rechnet ihn in Pascal um
 * @param pres Pointer fuer ausgelesenen Wert
 * @param oss Oversamplingwert (0 bis 3)
 * @return Bei Erfolg 1, sonst 0
 **/
uint8_t baro_pres(int32_t *pres, uint8_t oss)
{
    // Temperatur wird fuer Luftdruckumrechnung benoetigt
    if (!baro_temp(&T))
        return 0;
    // Rohwert des Luftdrucks auslesen
    if (!baro_pres_raw(&UP, oss))
        return 0;

    // Berechnung Luftdruck in Pascal
    // Formel siehe Datenblatt
    B6 = B5 - 4000;
    X1 = (B2 * (B6 * B6 / 4096)) / 2048;
    X2 = AC2 * B6 / 2048;
    X3 = X1 + X2;
    B3 = ((((int32_t)AC1 * 4 + X3) << oss) + 2) / 4;
    X1 = AC3 * B6 / 8192;
    X2 = (B1 * (B6 * B6 / 4096)) / 65536;
    X3 = ((X1 + X2) + 2) / 4;
    B4 = AC4 * (uint32_t)(X3 + 32768) / 32768;
    B7 = ((uint32_t)UP - B3) * (50000 >> oss);
    if (B7 < 0x80000000)
        p = (B7 * 2) / B4;
    else
        p = (B7 / B4) * 2;
    X1 = (p / 256) * (p / 256);
    X1 = (X1 * 3038) / 65536;
    X2 = (-7357 * p) / 65536;

    p = p + (X1 + X2 + 3791) / 16;
    *pres = p;

    return 1;
}
