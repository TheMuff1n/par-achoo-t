/**
 * Treiber fuer Bluetooth mit UART
 * 
 * Getestet mit HC-05 (ZS-040)
 * Angeschlossen an Pin 0 und 1 am Arduino UNO (RX, TX)
 **/

#include "bt_driver.h"

// Globale private Daten
uint8_t smsg[BT_SMSG_LEN];
uint8_t rmsg[BT_RMSG_LEN];
uint8_t sidx, ridx, last_byte;
uint8_t timeout;

// Initialisiert den Treiber
void bt_init(void)
{
    // Baudrate setzen
    UBRR0H = (uint8_t)(MYUBRR >> 8);
    UBRR0L = (uint8_t)MYUBRR;
    // Format setzen: 8 data, 1 stop
    UCSR0C = (3 << UCSZ00);
    // Sende-, Empfangspuffer leer
    sidx = BT_SMSG_LEN;
    ridx = 0;
    last_byte = ' ';
    // Interrupts, Empfaenger ein
    UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);

    // Timeout-Timer zuruecksetzen
    TCNT0 = 0;
    timeout = 0;
    // Modus Normal, Zaehltakt aus, Overflow-Interrupt ein
    TCCR0A = 0;
    TCCR0B = 0;
    TIMSK0 = (1 << TOIE0);
}

// Kopiert msg in Sendepuffer, bei Erfolg Ruecksprung mit 1, sonst 0
uint8_t bt_send(uint8_t *msg)
{
    // Interrupt deaktivieren
    uint8_t tmp = UCSR0B;
    UCSR0B &= ~(1 << UDRIE0);
    // Wenn Sendepuffer nicht leer
    if (sidx < BT_SMSG_LEN)
    {
        // Interrupt wiederherstellen, Ruecksprung mit falsch
        UCSR0B = tmp;
        return 0;
    }
    // Nachricht in Sendepuffer kopieren
    for (sidx = 0; sidx < BT_SMSG_LEN; sidx++)
        smsg[sidx] = msg[sidx];
    // Sendepuffer voll, Interrupt aktivieren
    sidx = 0;
    UCSR0B |= (1 << UDRIE0);
    return 1;
}

// Kopiert Empfangspuffer in msg, bei Erfolg ruecksprung mit 1, sonst 0
uint8_t bt_get(uint8_t *msg)
{
    // Interrupt deaktivieren
    uint8_t tmp = UCSR0B;
    UCSR0B &= ~(1 << RXCIE0);
    // Wenn Empfangspuffer nicht voll
    if (ridx < BT_RMSG_LEN)
    {
        // Interrupt wiederherstellen, Ruecksprung mit falsch
        UCSR0B = tmp;
        return 0;
    }
    // Empfangspuffer in Nachricht kopieren
    for (ridx = 0; ridx < BT_RMSG_LEN; ridx++)
        msg[ridx] = rmsg[ridx];
    // Empfangspuffer leer, Interrupt wiederherstellen
    ridx = 0;
    UCSR0B = tmp;
    return 1;
}

// Letztes empfangenes Byte zurueckgeben
uint8_t bt_get_last(void)
{
    return last_byte;
}

// UDR-frei Interrupt
ISR(USART_UDRE_vect)
{
    // Wenn Empfangspuffer nicht voll
    if (sidx < BT_SMSG_LEN)
        UDR0 = smsg[sidx++];
    // Sonst Interrupt deaktivieren
    else
        UCSR0B &= ~(1 << UDRIE0);
}

// Empfangsinterrupt
ISR(USART_RX_vect)
{
    last_byte = UDR0;
    // Wenn Sendepuffer nicht voll
    if (ridx < BT_RMSG_LEN)
    {
        // Empfangenes Byte kopieren, Zaehler erhoehen
        rmsg[ridx++] = last_byte;

        // Timeout-Timer starten, clk/1024
        TCNT0 = 0;
        TCCR0B = (0b101 << CS00);
    }
}

// Timer-Overflow Interrupt
ISR(TIMER2_OVF_vect)
{
    // Wenn timeout erreicht (pro Interrupt ca. 16ms)
    if (timeout++ > (BT_TIMEOUT - 2))
    {
        // Empfangspuffer leeren
        if ((ridx > 0) && (ridx < BT_RMSG_LEN))
            ridx = 0;
        // Timer stoppen
        TCCR0B = 0;
    }
}
