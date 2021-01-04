#ifndef MPU_DRIVER_H_
#define MPU_DRIVER_H_

#include <avr/io.h>

uint8_t mpu_init(void);
uint8_t mpu_get_accel(int16_t *ax, int16_t *ay, int16_t *az);

#endif
