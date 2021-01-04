#include "bt_driver.h"
#include "mpu_driver.h"
#include <util/delay.h>

void int_to_str(int16_t i, uint8_t *str);

uint8_t msg[BT_RMSG_LEN > BT_SMSG_LEN ? BT_RMSG_LEN : BT_SMSG_LEN];
int16_t ax, ay, az;

int main(void)
{
    /* Testprogramm */
    bt_init();
    mpu_init();

    sei();

    while (1)
    {
        mpu_get_accel(&ax, &ay, &az);

        int_to_str(ax, msg);
        while (!(bt_send(msg)))
            ;
        int_to_str(ay, msg);
        while (!(bt_send(msg)))
            ;
        int_to_str(az, msg);
        msg[5] = '\n';
        while (!(bt_send(msg)))
            ;
        _delay_ms(50);
    }
}

/**
 * Konvertiert einen vorzeichenbehafteten Integer in einen String
 * @param i Zu konvertierender Integer
 * @param str Ausgabearray der Laenge 7
 **/
void int_to_str(int16_t i, uint8_t *str)
{
    uint8_t idx, negative = 0;
    for (idx = 0; idx < 7; idx++)
        str[idx] = ' ';
    idx = 5;
    if (i < 0)
    {
        i *= -1;
        negative = 1;
    }
    do
    {
        str[idx--] = '0' + i % 10;
        i /= 10;
    } while (i);
    if (negative)
        str[idx] = '-';
}
