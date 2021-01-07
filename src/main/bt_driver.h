#ifndef BT_DRIVER_H_
#define BT_DRIVER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define FOSC 16000000
#define BAUD 9600
#define MYUBRR (FOSC / 16 / BAUD - 1)

#define BT_SMSG_LEN 12
#define BT_RMSG_LEN 1
#define BT_TIMEOUT 6

void bt_init(void);
uint8_t bt_send(uint8_t *msg);
uint8_t bt_get(uint8_t *msg);
uint8_t bt_get_last(void);

#endif /* BT_DRIVER_H_ */
