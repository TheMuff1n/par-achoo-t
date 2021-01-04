#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

#include <avr/io.h>

uint8_t i2c_begin_write(uint8_t address);
uint8_t i2c_read(uint8_t address, uint8_t len, uint8_t *data);
uint8_t i2c_write(uint8_t data);
void i2c_end(void);
uint8_t i2c_write_register(uint8_t address, uint8_t reg, uint8_t data);
uint8_t i2c_read_register(uint8_t address, uint8_t reg, uint8_t len, uint8_t *data);

#endif /* I2C_DRIVER_H */
