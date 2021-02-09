#include <avr/io.h>
#include <avr/interrupt.h>

#include "timer_driver.h"
#include "baro_driver.h"
#include "servo_driver.h"
#include "bt_driver.h"

#define ERROR()

enum State
{
	staging,
	ready,
	ascent,
	apex,
	descent,
	landed
} state;
enum Action
{
	tr_ascent,
	tr_apex,
	tr_landed,
	sv_close,
	sv_release,
	ts_release,
	ts_zero,
	br_oss,
	br_delay,
	verify,
	complete
} action;

uint8_t pres_idx, rmsg[BT_RMSG_LEN], smsg[BT_SMSG_LEN];
int16_t diff;
uint16_t sdata;
int32_t pres[2];

uint8_t servo_close = 0, servo_release = 180, baro_oss = 1;
uint16_t threshold_ascent = 20, threshold_apex = 25, threshold_landed = 20,
		 baro_delay = 200;

void init_drivers(void)
{
	timer_init();
	baro_init();
	servo_init();
	bt_init();
}

void test_release(void)
{
	servo_set_deg(servo_release);

	timer_start(1, 1000);
	while (timer_value(1))
		;

	servo_set_deg(servo_close);
}

void baro_calc_diff(void)
{
	if (!baro_pres(pres + pres_idx, baro_oss))
		ERROR();

	if (pres_idx)
		diff = pres[1] - pres[0];
	else
		diff = pres[0] - pres[1];

	pres_idx ^= 1;
}

void bt_send_block(uint16_t val)
{
	uint8_t bmsg[2];
	bmsg[0] = (uint8_t)val;
	bmsg[1] = (uint8_t)(val >> 8);

	while (!(bt_send(bmsg)))
		;
}

void test_zero(void)
{
	uint8_t cnt = 20;
	int16_t zero = 0;

	while (cnt)
	{
		if (timer_value(1))
			continue;

		baro_calc_diff();

		if (diff < 1000)
		{
			zero += diff;
			cnt--;
		}

		timer_start(1, baro_delay);
	}

	bt_send_block((uint16_t)zero);
}

void verify_staging(void)
{
	bt_send_block(threshold_ascent << 4 | tr_ascent);
	bt_send_block(threshold_apex << 4 | tr_apex);
	bt_send_block(threshold_landed << 4 | tr_landed);
	bt_send_block(servo_close << 4 | sv_close);
	bt_send_block(servo_release << 4 | sv_release);
	bt_send_block(baro_oss << 4 | br_oss);
	bt_send_block(baro_delay << 4 | br_delay);
}

void handle_staging(void)
{
	if (!bt_get(rmsg))
		return;

	action = rmsg[0] & 0x0f;
	sdata = ((uint16_t)rmsg[1]) << 4 | rmsg[0] >> 4;

	switch (action)
	{
	case tr_ascent:
		threshold_ascent = sdata;
		break;
	case tr_apex:
		threshold_apex = sdata;
		break;
	case tr_landed:
		threshold_landed = sdata;
		break;
	case sv_close:
		servo_close = (uint8_t)sdata;
		break;
	case sv_release:
		servo_release = (uint8_t)sdata;
		break;
	case ts_release:
		test_release();
		break;
	case ts_zero:
		test_zero();
		break;
	case br_oss:
		baro_oss = (uint8_t)sdata;
		break;
	case br_delay:
		baro_delay = sdata;
		break;
	case verify:
		verify_staging();
		break;
	case complete:
		state = ready;
		PORTB |= _BV(PB5);
		break;
	}
}

void handle_baro(void)
{
	if (timer_value(0))
		return;

	baro_calc_diff();

	switch (state)
	{
	case ready:
		if (diff < -(int16_t)threshold_ascent)
			state = ascent;
		break;
	case ascent:
		if (diff > -(int16_t)threshold_apex)
			state = apex;
		break;
	case descent:
		if (diff < threshold_landed)
			state = landed;
		break;
	default:;
	}

	timer_start(0, baro_delay);
}

void release_parachute(void)
{
	servo_set_deg(servo_release);
}

int main(void)
{
	/* Hauptprogramm */
	init_drivers();
	servo_set_deg(servo_close);
	state = staging;

	sei();

	while (1)
	{
		switch (state)
		{
		case staging:
			handle_staging();
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
