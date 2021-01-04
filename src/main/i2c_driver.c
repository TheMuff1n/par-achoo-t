/**
 * Treiber fuer I2C-Kommunikation
 * Neben grundlegenen Schreib- und Leseoperationen auch Funktionen fuer
 * Operationen auf Registern wie auf vielen I2C-Geraten ueblich
 * 
 * Getestet mit Arduino Nano (ATmega328P)
 **/

#include "i2c_driver.h"
#include <compat/twi.h>

/**
 * Startet Schreioperation an gegebene Adresse
 * @param address I2C-Zieladresse
 * @return Bei Erfolg 1, sonst 0
 **/
uint8_t i2c_begin_write(uint8_t address)
{
    // Startbedingung senden
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
    // Auf Abschluss warten
    while (!(TWCR & _BV(TWINT)))
        ;
    // Auf Erfolg pruefen
    if ((TWSR & 0xF8) != TW_START)
        return 0;

    // Zieladresse senden
    TWDR = address << 1;
    TWCR = _BV(TWINT) | _BV(TWEN);
    while (!(TWCR & _BV(TWINT)))
        ;
    if ((TWSR & 0xF8) != TW_MT_SLA_ACK)
        return 0;

    return 1;
}

/**
 * Liest eine Anzahl von Bytes von gegebener Adresse
 * @param address I2C-Zieladresse
 * @param len Anzahl zu lesender Bytes
 * @param data Array fuer gelesene Daten
 * @return Bei Erfolg 1, sonst 0
 **/
uint8_t i2c_read(uint8_t address, uint8_t len, uint8_t *data)
{
    // Startbedingung senden
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
    // Auf Abschluss warten
    while (!(TWCR & _BV(TWINT)))
        ;
    // Auf Erfolg pruefen
    if ((TWSR & 0xF8) != TW_START)
        return 0;

    // Zieladresse senden
    TWDR = (address << 1) | 1;
    TWCR = _BV(TWINT) | _BV(TWEN);
    while (!(TWCR & _BV(TWINT)))
        ;
    if ((TWSR & 0xF8) != TW_MR_SLA_ACK)
        return 0;

    // Bei nur einem Byte NACK senden
    if (len > 1)
        TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN);

    // Bytes lesen
    while (len > 1)
    {
        while (!(TWCR & _BV(TWINT)))
            ;
        if ((TWSR & 0xF8) != TW_MR_DATA_ACK)
            return 0;
        data[--len] = TWDR;
        // Wenn noch Bytes zu lesen ACK senden
        if (len)
            TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN);
    }

    // Letztes Byte lesen
    TWCR = _BV(TWINT) | _BV(TWEN);
    while (!(TWCR & _BV(TWINT)))
        ;
    if ((TWSR & 0xF8) != TW_MR_DATA_NACK)
        return 0;
    data[0] = TWDR;

    // I2C-Transaktion beenden
    //TOOD:TWEN benoetigt?
    TWCR = _BV(TWINT) | _BV(TWSTO);

    return 1;
}

/**
 * Uebertraegt ein Byte
 * @param data Datenbyte
 * @return Bei Erfolg 1, sonst 0
 **/
uint8_t i2c_write(uint8_t data)
{
    // Daten in Register laden und senden
    TWDR = data;
    TWCR = _BV(TWINT) | _BV(TWEN);
    // Auf Abschluss warten
    while (!(TWCR & _BV(TWINT)))
        ;
    // Auf Erfolg pruefen
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
        return 0;

    return 1;
}

/**
 * Beendet I2C-Transaktion
 **/
void i2c_end(void)
{
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
}

/**
 * @param address I2C-Zieladresse
 * @param reg Registeradresse
 * @param data Zu schreibende Daten
 * @return Bei Erfolg 1, sonst 0
 **/
uint8_t i2c_write_register(uint8_t address, uint8_t reg, uint8_t data)
{
    if (!i2c_begin_write(address))
        return 0;

    // Registeradresse schreiben
    if (!i2c_write(reg))
        return 0;

    // Daten in Register schreiben
    if (!i2c_write(data))
        return 0;

    i2c_end();

    return 1;
}

/**
 * Liest gegebene Anzahl von Bytes von I2C-Ziel aus Registern auf diesem
 * @param address I2C-Zieladresse
 * @param reg Registeradresse
 * @param len Anzahl zu lesender Bytes
 * @param data Array fuer gelesene Daten
 * @return Bei Erfolg 1, sonst 0
 **/
uint8_t i2c_read_register(uint8_t address, uint8_t reg, uint8_t len, uint8_t *data)
{
    if (!i2c_begin_write(address))
        return 0;

    // Registeradresse schreiben
    if (!i2c_write(reg))
        return 0;

    i2c_end();

    return i2c_read(address, len, data);
}
