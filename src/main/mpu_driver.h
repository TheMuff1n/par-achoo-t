#ifndef MPU_DRIVER_H_
#define MPU_DRIVER_H_

#include <avr/io.h>

uint8_t mpu_init(void);
uint8_t mpu_get_accel(uint16_t *ax, uint16_t *ay, uint16_t *az);

#endif
