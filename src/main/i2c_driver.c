#include "i2c_driver.h"

void i2c_end(void)
{
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);
}

uint8_t i2c_write(uint8_t data)
{
    // Daten in Register laden und senden
    TWDR = data;
    TWCR = _BV(TWINT) | _BV(TWEN);
    // Warten bis fertig
    while (!(TWCR & _BV(TWINT)))
        ;
    // Bei Fehler falsch zurueckgeben
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
        return 0;
    return 1;
}

uint8_t i2c_read(uint8_t address, uint8_t len, uint8_t *data)
{
    // Request START
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
    while (!(TWCR & _BV(TWINT)))
        ;
    if ((TWSR & 0xF8) != TW_START)
        return 0;

    // Send address
    TWDR = (address << 1) | 1;
    TWCR = _BV(TWINT) | _BV(TWEN);
    while (!(TWCR & _BV(TWINT)))
        ;
    if ((TWSR & 0xF8) != TW_MR_SLA_ACK)
        return 0;

    // Dont send ACK when reading last byte
    if (len > 1)
        TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN);

    // Read bytes
    while (len > 1)
    {
        while (!(TWCR & _BV(TWINT)))
            ;
        if ((TWSR & 0xF8) != TW_MR_DATA_ACK)
            return 0;
        data[--len] = TWDR;
        if (len)
            TWCR = _BV(TWINT) | _BV(TWEA) | _BV(TWEN);
    }

    // Read last byte
    TWCR = _BV(TWINT) | _BV(TWEN);
    while (!(TWCR & _BV(TWINT)))
        ;
    if ((TWSR & 0xF8) != TW_MR_DATA_NACK)
        return 0;
    data[0] = TWDR;
    TWCR = _BV(TWINT) | _BV(TWSTO);

    return 1;
}

uint8_t i2c_begin_write(uint8_t address)
{
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);
    while (!(TWCR & _BV(TWINT)))
        ;
    if ((TWSR & 0xF8) != TW_START)
        return 0;

    TWDR = address << 1;
    TWCR = _BV(TWINT) | _BV(TWEN);
    while (!(TWCR & _BV(TWINT)))
        ;
    if ((TWSR & 0xF8) != TW_MT_SLA_ACK)
        return 0;
    return 1;
}

uint8_t i2c_read_register(uint8_t address, uint8_t reg, uint8_t len, uint8_t *data)
{
    i2c_begin_write(address);
    i2c_write(reg);
    i2c_end();

    i2c_read(address, len, data);
}

uint8_t i2c_write_register(uint8_t address, uint8_t reg, uint8_t *data)
{
    i2c_begin_write(address);
    i2c_write(reg);
    i2c_write(data);
    i2c_end();
}
