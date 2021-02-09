#ifndef TIMER_DRIVER_H_
#define TIMER_DRIVER_H_

#include <avr/io.h>

void timer_init(void);
void timer_start(uint8_t timer_idx, uint16_t millis);
uint16_t timer_value(uint8_t timer_idx);

#endif /* TIMER_DRIVER_H_ */
