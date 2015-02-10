/*
 * LightSensorSwitch.c
 *
 * Created: 2/8/2015 5:14:58 PM
 *  Author: denis.tyurin
 */ 

#ifndef F_CPU
#define F_CPU 1000000UL // 1 MHz
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "states.h"
#include "leds.h"
#include "light_sensor.h"

int main(void)
{
	leds_init();
	light_sensor_init();

	/* Set DAY mode on startup */
	states_set_state(STATE_DAY_MODE);
	
	/* Test LEDS */
	leds_red_blink(LEDS_LONG_BLINK);
	leds_blue_blink(LEDS_LONG_BLINK);

    while(1)
    {
		states_dispatch_event(EVENT_IDLE, NULL);

    }
}