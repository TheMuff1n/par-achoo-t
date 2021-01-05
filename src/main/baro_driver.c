#include "baro_driver.h"
#include "i2c_driver.h"
#include <util/delay.h>

#include "bt_driver.h"

#define BMP180_ADR 0x77

int16_t AC1, AC2, AC3, B1, B2, MB, MC, MD;
uint16_t AC4, AC5, AC6;

int32_t X1, X2, X3, B3, B5, B6, T, p, UT, UP;
uint32_t B4, B7;

uint8_t buf[3];

uint8_t dmsg[BT_SMSG_LEN];

void long_to_str(int32_t i, uint8_t *str)
{
    uint8_t idx, negative = 0;
    for (idx = 0; idx < BT_SMSG_LEN; idx++)
        str[idx] = ' ';
    idx = BT_SMSG_LEN - 2;
    if (i < 0)
    {
        i *= -1;
        negative = 1;
    }
    do
    {
        str[idx--] = '0' + i % 10;
        i /= 10;
    } while (i);
    if (negative)
        str[idx] = '-';
}

void bt_debug(int32_t val, uint8_t dchar)
{
    long_to_str(val, dmsg);
    dmsg[BT_SMSG_LEN - 1] = dchar;
    while (!bt_send(dmsg))
        ;
}

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

uint8_t baro_raw_temp(int32_t *temp)
{
    if (!i2c_write_register(BMP180_ADR, 0xF4, 0x2E))
        return 0;
    _delay_ms(5);

    if (!i2c_read_register(BMP180_ADR, 0xF6, 2, buf))
        return 0;

    UT = (((int32_t)buf[1]) << 8) | buf[0];
    *temp = UT;

    return 1;
}

uint8_t baro_temp(int32_t *temp)
{
    if (!baro_raw_temp(&UT))
        return 0;
    // bt_debug(UT, '-');

    X1 = (UT - AC6) * AC5 / 32768;
    // bt_debug(X1, 'b');
    X2 = (int32_t)MC * 2048 / (X1 + MD);
    // bt_debug(X2, 'c');
    B5 = X1 + X2;
    // bt_debug(B5, 'd');

    T = (B5 + 8) / 16;
    // bt_debug(T, 't');
    *temp = T;

    return 1;
}

uint8_t baro_pres_raw(int32_t *pres, uint8_t oss)
{
    oss &= 0x03;
    if (!i2c_write_register(BMP180_ADR, 0xF4, 0x34 | (oss << 6)))
        return 0;
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
    case 3:
        _delay_ms(26);
    }

    if (!i2c_read_register(BMP180_ADR, 0xF6, 3, buf))
        return 0;
    *pres = ((((int32_t)buf[2]) << 16) | ((int32_t)buf[1] << 8) | buf[0]) >> (8 - oss);

    return 1;
}

uint8_t baro_pres(int32_t *pres, uint8_t oss)
{
    // bt_debug(AC1, 'a');
    // bt_debug(AC2, 'b');
    // bt_debug(AC3, 'c');
    // bt_debug((int32_t)AC4, 'd');
    // bt_debug((int32_t)AC5, 'e');
    // bt_debug((int32_t)AC6, 'f');
    // bt_debug(B1, 'g');
    // bt_debug(B2, 'h');
    // bt_debug(MB, 'i');
    // bt_debug(MC, 'j');
    // bt_debug(MD, 'k');

    if (!baro_temp(&T))
        return 0;
    if (!baro_pres_raw(&UP, oss))
        return 0;
    // bt_debug(UP, 'i');

    B6 = B5 - 4000;
    // bt_debug(B6, 'z');
    X1 = (B2 * (B6 * B6 / 4096)) / 2048;
    // bt_debug(X1, 'y');
    X2 = AC2 * B6 / 2048;
    // bt_debug(X2, 'x');
    X3 = X1 + X2;
    // bt_debug(X3, 'w');
    B3 = ((((int32_t)AC1 * 4 + X3) << oss) + 2) / 4;
    // bt_debug(B3, 'v');
    X1 = AC3 * B6 / 8192;
    // bt_debug(X1, 'u');
    X2 = (B1 * (B6 * B6 / 4096)) / 65536;
    // bt_debug(X2, 't');
    X3 = ((X1 + X2) + 2) / 4;
    // bt_debug(X3, 's');
    B4 = AC4 * (uint32_t)(X3 + 32768) / 32768;
    // bt_debug(B4, 'r');
    B7 = ((uint32_t)UP - B3) * (50000 >> oss);
    // bt_debug(B6, 'q');
    if (B7 < 0x80000000)
        p = (B7 * 2) / B4;
    else
        p = (B7 / B4) * 2;
    // bt_debug(p, 'p');
    X1 = (p / 256) * (p / 256);
    // bt_debug(X1, 'o');
    X1 = (X1 * 3038) / 65536;
    // bt_debug(X1, 'n');
    X2 = (-7357 * p) / 65536;
    // bt_debug(X2, 'm');
    p = p + (X1 + X2 + 3791) / 16;
    // bt_debug(p, 'l');

    *pres = p;

    return 1;
}
