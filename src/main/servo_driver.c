/**
 * Treiber fuer Servo mit PWM
 * 
 * Getestet mit Micro Servo SG90
 * Angeschlossen an Pin 10 am Arduino Nano (OC1B)
 **/
#include "servo_driver.h"
#include <avr/interrupt.h>

uint16_t val;

/**
 * Initialisiert den Servo
 * Verwendet Fast PWM mit Timer 1
 **/
void servo_init(void)
{
    // Pin 10 als Output setzen
    DDRB |= _BV(PB2);
    // Periodendauer 20ms
    OCR1A = 40000;
    // Pin 10 LOW bei Compare Match
    // Modus Fast PWM mit OCR1A als TOP
    TCCR1A = _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
    TCCR1B = _BV(WGM13) | _BV(WGM12);
}

/**
 * Stellt den Winkel fuer den Servomotor ein
 * @param deg Winkel von 0 bis 180 Grad
 **/
void servo_set_deg(uint8_t deg)
{
    // Winkel auf [0, 180] limitieren
    if (deg > 180)
        deg = 180;
    // Wert fuer OCR1B berechnen
    val = (uint16_t)((uint32_t)deg * 1994 / 100 + 1108);
    // Timer ausschalten
    TCCR1B &= ~_BV(CS11);
    // Wert von val in Register schreiben
    OCR1BH = (uint8_t)(val >> 8);
    OCR1BL = (uint8_t)(val);
    // Timer aktivieren
    TCNT1 = 0;
    TCCR1B |= _BV(CS11);
}
