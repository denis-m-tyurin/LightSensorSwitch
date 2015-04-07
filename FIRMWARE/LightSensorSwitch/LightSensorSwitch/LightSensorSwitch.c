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
#include <avr/interrupt.h>
#include <util/delay.h>
#include "states.h"
#include "events.h"
#include "leds.h"
#include "light_sensor.h"
#include "watchdog.h"
#include "voltage_sensor.h"
#include "light_sensor.h"
#include "ext_load.h"

static void indicateThreshold();

int main(void)
{
	leds_init();

	/* Initialize ADC, which is used by light and voltage sensors
	 * Light sensor is connected to ADC3, voltage sensor to ADC2
	 * Voltage reference - AVcc */
	ADMUX = (1 << REFS0);
		
	ext_load_init();

	/* Set DAY mode on startup */
	states_set_state(STATE_DAY_MODE);
	
	/* Test LEDS */
	leds_red_blink(LEDS_LONG_BLINK);
	leds_blue_blink(LEDS_LONG_BLINK);
	
	sei();

    while(1)
    {
    	voltage_sensor_check_voltage();
    	light_sensor_check();

    	for(event_t event = get_event_from_queue(); EVENT_NONE != event; event = get_event_from_queue())
    	{
    		states_dispatch_event(event, NULL);
    	}

		states_set_state(states_get_scheduled_state());

    }
}

void indicateThreshold()
{
	uint16_t threshold = settings_manager_get_light_threshold();
	
	if (threshold < settings_manager_get_default_light_threshold())
	{
		while(threshold != settings_manager_get_default_light_threshold())
		{
			threshold+= settings_manager_get_light_threshold_step();
			_delay_ms(1000);
			leds_red_blink(1000);			
		}
	}
	
	if (threshold > settings_manager_get_default_light_threshold())
	{
		while(threshold != settings_manager_get_default_light_threshold())
		{
			threshold-= settings_manager_get_light_threshold_step();
			_delay_ms(1000);
			leds_blue_blink(1000);
		}
	}
	
}

ISR(WDT_vect)
{
	disableWatchdog();
}
