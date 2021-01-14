#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer_driver.h"
#include "baro_driver.h"
#include "servo_driver.h"
#include "bt_driver.h"

#define ERROR()

#define BARO_DELAY 200

#define THRESHOLD_ASCENT 20
#define THRESHOLD_APEX 25
#define THRESHOLD_LANDED 20

#define SERVO_CLOSE 0
#define SERVO_RELEASE 180

enum State
{
	staging,
	ready,
	ascent,
	apex,
	descent,
	landed
} state;

uint8_t pres_idx;
int16_t diff;
int32_t pres[2];

void init_drivers(void)
{
	timer_init();
	baro_init();
	servo_init();
	bt_init();
}

void handle_baro(void)
{
	if (timer_value(0))
		return;

	if (!baro_pres(pres + pres_idx, 1))
		ERROR();

	if (pres_idx)
		diff = pres[1] - pres[0];
	else
		diff = pres[0] - pres[1];

	pres_idx ^= 1;

	switch (state)
	{
	case ready:
		if (diff < -THRESHOLD_ASCENT)
			state = ascent;
		break;
	case ascent:
		if (diff > -THRESHOLD_APEX)
			state = apex;
		break;
	case descent:
		if (diff < THRESHOLD_LANDED)
			state = landed;
		break;
	default:;
	}

	timer_start(0, BARO_DELAY);
}

void release_parachute(void)
{
	servo_set_deg(SERVO_RELEASE);
}

int main(void)
{
	/* Hauptprogramm */
	init_drivers();
	servo_set_deg(SERVO_CLOSE);
	state = staging;

	sei();

	while (1)
	{
		switch (state)
		{
		case staging:
			state = ready;
			break;
		case ready:
			handle_baro();
			break;
		case ascent:
			handle_baro();
			break;
		case apex:
			release_parachute();
			state = descent;
			break;
		case descent:
			handle_baro();
			break;
		case landed:
			break;
		}
	}
}
