#ifndef SERVO_DRIVER_H_
#define SERVO_DRIVER_H_

#include <avr/io.h>

void servo_init(void);
void servo_set_deg(uint8_t deg);

#endif /* SERVO_DRIVER_H_ */
