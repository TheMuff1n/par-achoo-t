#include "bt_driver.h"

uint8_t msg[BT_RMSG_LEN > BT_SMSG_LEN ? BT_RMSG_LEN : BT_SMSG_LEN];

int main(void)
{
    /* Testprogramm */
    bt_init();
    sei();
    while (1)
    {
        if (bt_get(msg))
            bt_send(msg);
    }
}
