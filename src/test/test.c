#include "bt_driver.h"
#include "mpu_driver.h"
#include "baro_driver.h"
#include "test_utils.h"
#include <util/delay.h>

uint8_t msg[BT_RMSG_LEN > BT_SMSG_LEN ? BT_RMSG_LEN : BT_SMSG_LEN];
int16_t ax, ay, az;
int32_t temp, pres;

int main(void)
{
    /* Testprogramm */
    bt_init();
    // mpu_init();
    baro_init();

    DDRB |= _BV(PB5);
    PORTB &= ~_BV(PB5);

    sei();

    while (1)
    {
        if (!baro_temp(&temp))
            PORTB |= _BV(PB5);

        long_to_str(temp, msg);
        msg[BT_SMSG_LEN - 1] = ' ';
        while (!(bt_send(msg)))
            ;

        if (!baro_pres(&pres, 1))
            PORTB |= _BV(PB5);

        long_to_str(pres, msg);
        msg[BT_SMSG_LEN - 1] = '\n';
        while (!bt_send(msg))
            ;

        // mpu_get_accel(&ax, &ay, &az);

        // int_to_str(ax, msg);
        // while (!(bt_send(msg)))
        //     ;
        // int_to_str(ay, msg);
        // while (!(bt_send(msg)))
        //     ;
        // int_to_str(az, msg);
        // msg[5] = '\n';
        // while (!(bt_send(msg)))
        //     ;

        _delay_ms(500);
    }
}
