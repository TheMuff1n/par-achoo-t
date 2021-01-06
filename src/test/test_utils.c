#include "test_utils.h"
#include "bt_driver.h"

uint8_t dmsg[BT_SMSG_LEN];

/**
 * Konvertiert einen vorzeichenbehafteten 32-Bit-Integer in einen String
 * @param i Zu konvertierender 32-Bit-Integer
 * @param str Ausgabearray der Laenge 7
 **/
void long_to_str(int32_t i, uint8_t *str)
{
    uint8_t idx, negative = 0;
    // String leeren
    for (idx = 0; idx < BT_SMSG_LEN; idx++)
        str[idx] = ' ';
    idx = BT_SMSG_LEN - 2;
    // Betrag und Vorzeichen der Eingabe bestimmen
    if (i < 0)
    {
        i *= -1;
        negative = 1;
    }
    // Ziffern in Buchstaben umwandeln
    do
    {
        str[idx--] = '0' + i % 10;
        i /= 10;
    } while (i);
    // Vorzeichen hinzufügen falls Zahl negativ
    if (negative)
        str[idx] = '-';
}

/**
 * Konvertiert einen vorzeichenbehafteten 16-Bit-Integer in einen String
 * @param i Zu konvertierender 16-Bit-Integer
 * @param str Ausgabearray der Laenge 7
 **/
void int_to_str(int16_t i, uint8_t *str)
{
    long_to_str((int16_t)i, str);
}

/**
 * Sendet einen vorzeichenbehafteten 32-Bit-Integer lesbar in ASCII per Bluetooth. 
 * Die Debugnachricht wird mit einem gegeben Buchstaben zur Wiedererkennung markiert
 * @param val Zu sendender 32-Bit-Integer
 * @param dchar Markierungsbuchstabe
 **/
void bt_debug(int32_t val, uint8_t dchar)
{
    long_to_str(val, dmsg);
    dmsg[BT_SMSG_LEN - 1] = dchar;
    while (!bt_send(dmsg))
        ;
}
