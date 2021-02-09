#include "bt_driver.h"
#include "mpu_driver.h"
#include "baro_driver.h"
#include "test_utils.h"
#include "servo_driver.h"
#include "timer_driver.h"

uint8_t pres_idx, smsg[BT_SMSG_LEN], rmsg[BT_RMSG_LEN];
int16_t diff;
int32_t pres[2];

int main(void)
{
    // /* Testprogramm */
    timer_init();
    baro_init();
    servo_init();
    bt_init();
    DDRB |= _BV(PB5);
    PORTB &= ~_BV(PB5);

    servo_set_deg(0);

    sei();

    while (1)
    {
        if (timer_value(0))
            continue;

        if (!baro_pres(pres + pres_idx, 1))
            continue;

        if (pres_idx)
            diff = pres[1] - pres[0];
        else
            diff = pres[0] - pres[1];

        pres_idx ^= 1;

        BT_DEBUG((int32_t)diff, '\n');
    }
}
