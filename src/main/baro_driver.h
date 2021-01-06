#ifndef BARO_DRIVER_H_
#define BARO_DRIVER_H_

#include <avr/io.h>

uint8_t baro_init(void);
uint8_t baro_read_int(uint8_t reg, int16_t *var);
uint8_t baro_read_uint(uint8_t reg, uint16_t *var);
uint8_t baro_temp_raw(int32_t *temp);
uint8_t baro_temp(int32_t *temp);
uint8_t baro_pres_raw(int32_t *pres, uint8_t oss);
uint8_t baro_pres(int32_t *pres, uint8_t oss);

#endif /* BARO_DRIVER_H_ */
