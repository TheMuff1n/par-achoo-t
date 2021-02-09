#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t timers[4];
uint8_t idx;

void timer_init(void)
{
    for (idx = 0; idx < 4; idx++)
        timers[idx] = 0;
    TCCR2A = _BV(WGM21);
    TCCR2B = 0;
    OCR2A = 249;
    TCNT2 = 0;
}

void timer_start(uint8_t timer_idx, uint16_t millis)
{
    timers[timer_idx & 0x03] = millis;
    TCCR2B = _BV(CS22);
    TIMSK2 = _BV(OCIE2A);
}

uint16_t timer_value(uint8_t timer_idx)
{
    return timers[timer_idx];
}

ISR(TIMER2_COMPA_vect)
{
    for (idx = 0; idx < 4; idx++)
        if (timers[idx])
            timers[idx]--;
}
