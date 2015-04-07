#ifndef F_CPU
#define F_CPU 1000000UL // 1 MHz
#endif

#include <stdio.h>
#include <stdlib.h>
#include <avr/sleep.h>
#include "state_day.h"
#include "states.h"
#include "light_sensor.h"
#include "leds.h"
#include "watchdog.h"
#include "ext_load.h"

static void mcu_power_down()
{
	/* Turn ON watchdog and sleep */
	disableWatchdog();
	enableWatchdog();

	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_mode();
}

void state_day_enter(void **pStateBuf)
{
	ext_load_off();
}

void state_day_exit(void **pStateBuf)
{

}


void state_day_event_handler(event_t event, void **pStateBuf, void *data)
{
	switch (event)
	{
		case EVENT_IDLE:
			/* Do nothing, just go power down */
			mcu_power_down();
			break;
		case EVENT_LIGHT_SENSOR_BLINK_BLUE:
			leds_blue_blink(LEDS_SHORT_BLINK);
			mcu_power_down();
			break;
		case EVENT_LIGHT_SENSOR_BLINK_RED:
			leds_red_blink(LEDS_SHORT_BLINK);
			mcu_power_down();
			break;
		case EVENT_LIGHT_SENSOR_OK_TO_ON:
			states_schedule_state(STATE_NIGHT_MODE);
			break;
		case EVENT_OVERVOLTAGE:
			leds_red_blink(LEDS_LONG_BLINK);
			leds_red_blink(LEDS_LONG_BLINK);
			leds_red_blink(LEDS_LONG_BLINK);
			ext_load_overvoltage_enter();
			break;
		case EVENT_RESTORED_FROM_OVERVOLTAGE:
			leds_blue_blink(LEDS_LONG_BLINK);
			ext_load_overvoltage_exit();
			break;
		case EVENT_LOW_POWER:
			ext_load_low_power_enter();
			break;
		case EVENT_RESTORED_FROM_LOW_POWER:
			ext_load_low_power_exit();
			break;
		default:
			break;				
	}	
}
