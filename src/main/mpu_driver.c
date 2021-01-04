/**
 * Treiber fuer Beschleunigungssensor mit I2C
 * 
 * Getestet mit MPU-6050 (GY-521)
 * Angeschlossen an Pin 4 und 5 am Arduino Nano (SDA, SCL)
 **/

#include "mpu_driver.h"
#include "i2c_driver.h"

#define MPU6050_ADR 0x68
#define mpu_read_register(reg, len, data) \
    i2c_read_register((MPU6050_ADR), (reg), (len), (data))
#define mpu_write_register(reg, data) \
    i2c_write_register((MPU6050_ADR), (reg), (data))

/**
 * Initialisiert den Sensor
 * @return Bei Erfolg 1, sonst 0
 **/
uint8_t mpu_init(void)
{
    // Erreichbarkeit pruefen
    uint8_t id;
    mpu_read_register(0x75, 1, &id);
    if (id != 0x68)
        return 0;

    // clk-Quelle setzen
    if (!mpu_write_register(0x6B, 0x01))
        return 0;

    // Sensor aufwecken
    if (!mpu_write_register(0x1C, 0x00))
        return 0;

    return 1;
}

/**
 * Liest Beschleunigungssensordaten aus
 * @param ax Pointer fuer x-Wert
 * @param ay Pointer fuer y-Wert
 * @param az Pointer fuer z-Wert
 * @return Bei Erfolg 1, sonst 0
 **/
uint8_t mpu_get_accel(int16_t *ax, int16_t *ay, int16_t *az)
{
    // Beschleunigungssensordaten in Puffer lesen
    uint8_t data[6];
    if (!mpu_read_register(0x3B, 6, data))
        return 0;

    // Daten sind vorzeichenbehaftete 16-Bit Integer
    *ax = (((int16_t)data[5]) << 8) | data[4];
    *ay = (((int16_t)data[3]) << 8) | data[2];
    *az = (((int16_t)data[1]) << 8) | data[0];

    return 1;
}
