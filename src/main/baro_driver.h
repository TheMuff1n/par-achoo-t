#ifndef BARO_DRIVER_H_
#define BARO_DRIVER_H_

#include <avr/io.h>

uint8_t baro_init(void);
uint8_t baro_read_int(uint8_t reg, int16_t *var);
uint8_t baro_read_uint(uint8_t reg, uint16_t *var);
uint8_t baro_raw_temp(uint16_t *temp);
uint8_t baro_temp(int32_t *temp);

#endif /* BARO_DRIVER_H_ */
