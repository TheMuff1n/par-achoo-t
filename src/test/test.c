#include "bt_driver.h"
#include "mpu_driver.h"
#include "baro_driver.h"
#include "test_utils.h"
#include "servo_driver.h"
#include "timer_driver.h"

uint8_t smsg[BT_SMSG_LEN];
uint8_t rmsg[BT_RMSG_LEN];
int16_t ax, ay, az;
int32_t temp, pres;

int main(void)
{
    // /* Testprogramm */
    // bt_init();
    // // mpu_init();
    // baro_init();
    // servo_init();
    // timer_init();

    // servo_set_deg(0);

    // DDRB |= _BV(PB5);
    // PORTB &= ~_BV(PB5);

    // sei();

    // while (1)
    // {
    //     if (!timer_value(0))
    //     {
    //         // if (bt_get(rmsg))
    //         //     servo_set_deg(rmsg[0]);

    //         // if (!baro_temp(&temp))
    //         //     PORTB |= _BV(PB5);

    //         // long_to_str(temp, smsg);
    //         // smsg[BT_SMSG_LEN - 1] = ' ';
    //         // while (!(bt_send(smsg)))
    //         //     ;

    //         // if (!baro_pres(&pres, 1))
    //         //     PORTB |= _BV(PB5);

    //         // long_to_str(pres, smsg);
    //         // smsg[BT_SMSG_LEN - 1] = '\n';
    //         // while (!bt_send(smsg))
    //         //     ;

    //         PORTB ^= _BV(PB5);

    //         timer_start(0, 1000);
    //     }
    // }
    DDRB = _BV(PB5);
    PORTB = _BV(PB5);
    while (1)
        ;
}
