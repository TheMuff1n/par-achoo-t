#include "bt_driver.h"
#include "mpu_driver.h"
#include <avr/delay.h>

#define BT_SMSG_LEN 6

uint8_t msg[BT_RMSG_LEN > BT_SMSG_LEN ? BT_RMSG_LEN : BT_SMSG_LEN];
uint16_t ax, ay, az;

int main(void)
{
    /* Testprogramm */
    bt_init();
    mpu_init();
    DDRB |= _BV(PB5);
    PORTB &= ~_BV(PB5);
    sei();
    while (1)
    {
        mpu_get_accel(&ax, &ay, &az);
        int_to_str(ax, msg);
        while (!(bt_send(msg)))
            ;
        _delay_ms(50);
        // *msg = ay;
        // while (!(bt_send(msg)))
        //     ;
        // *msg = az;
        // while (!(bt_send(msg)))
        //     ;
    }
}

void int_to_str(uint16_t i, uint8_t *str)
{
    uint8_t idx;
    for (idx = 0; idx < 5; idx++)
        str[idx] = ' ';
    str[5] = '\n';
    idx = 4;
    do
    {
        str[idx--] = '0' + i % 10;
        i /= 10;
    } while (i);
}

// https://stackoverflow.com/a/9660930
char *itoa(int i, char b[])
{
    char const digit[] = "0123456789";
    char *p = b;
    if (i < 0)
    {
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do
    { //Move to where representation ends
        ++p;
        shifter = shifter / 10;
    } while (shifter);
    *p = '\0';
    do
    { //Move back, inserting digits as u go
        *--p = digit[i % 10];
        i = i / 10;
    } while (i);
    return b;
}
