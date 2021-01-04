#include "mpu_driver.h"
#include "i2c_driver.h"
#include <avr/interrupt.h>

#define MPU6050_ADR 0x68

uint8_t mpu_init(void)
{
    uint8_t id;
    i2c_read_register(MPU6050_ADR, 0x75, 1, &id);
    if (id != 0x68)
        return 0;

    // Set clk
    i2c_write_register(MPU6050_ADR, 0x6B, 0x01);
    i2c_write_register(MPU6050_ADR, 0x1C, 0x00);

    return 1;
}

uint8_t mpu_get_accel(int16_t *ax, int16_t *ay, int16_t *az)
{
    uint8_t data[6];
    i2c_read_register(MPU6050_ADR, 0x3B, 6, data);
    *ax = (((int16_t)data[5]) << 8) | data[4];
    *ay = (((int16_t)data[3]) << 8) | data[2];
    *az = (((int16_t)data[1]) << 8) | data[0];
}
