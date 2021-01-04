#include "baro_driver.h"
#include "i2c_driver.h"
#include <util/delay.h>

#define BMP180_ADR 0x77

int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD;
uint16_t AC4, AC5, AC6;

uint8_t buf[2];

uint8_t baro_init(void)
{
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

uint8_t baro_read_int(uint8_t reg, int16_t *var)
{
    if (!i2c_read_register(BMP180_ADR, reg, 2, buf))
        return 0;
    *var = (((int16_t)buf[1]) << 8) | buf[0];

    return 1;
}

uint8_t baro_read_uint(uint8_t reg, uint16_t *var)
{
    if (!i2c_read_register(BMP180_ADR, reg, 2, buf))
        return 0;
    *var = (((uint16_t)buf[1]) << 8) | buf[0];

    return 1;
}

uint8_t baro_raw_temp(uint16_t *temp)
{
    if (!i2c_write_register(BMP180_ADR, 0xF4, 0x2E))
        return 0;
    _delay_ms(5);

    return baro_read_uint(0xF6, temp);
}

uint8_t baro_temp(int32_t *temp)
{
    uint16_t raw;
    if (!baro_raw_temp(&raw))
        return 0;
    int32_t X1 = (((int32_t)raw - AC6) * AC5 / 32768);
    int32_t X2 = (((int32_t)MC) * 2048 / (X1 + MD));
    int32_t B5 = X1 + X2;
    *temp = (B5 + 8) / 16;
    return 1;
}
