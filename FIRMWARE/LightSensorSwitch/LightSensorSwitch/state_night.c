#ifndef F_CPU
#define F_CPU 1000000UL // 1 MHz
#endif

#include <stdio.h>
#include <stdlib.h>
#include <avr/sleep.h>
#include "state_night.h"
#include "states.h"
#include "light_sensor.h"
#include "leds.h"
#include "watchdog.h"
#include "settings_manager.h"

typedef struct
{
	uint8_t light_history;	
} night_state_data;

/* A number of day level light measurements that we get 
 * before switching to the day mode. This is to exclude 
 * ADC errors or impulses like flashlights, blinks, etc */
#define SWITCH_TO_DAY_MODE_CNT 8 

void state_night_enter(void **pStateBuf)
{
	night_state_data *pData;
		
	*pStateBuf = malloc(sizeof(night_state_data));
	pData = (night_state_data*) *pStateBuf;

	pData->light_history = 0;
	PORTD |= (1<<PD5);
}

void state_night_exit(void **pStateBuf)
{
		if (NULL != *pStateBuf)
		{
			free(*pStateBuf);
		}
		
		PORTD &= ~(1<<PD5);
}

void state_night_event_handler(uint8_t event, void **pStateBuf, void *data)
{
	night_state_data *pData = (night_state_data*) *pStateBuf;
		
	switch (event)
	{
		case EVENT_IDLE:
			/* Run ADC and wait for completion */
			if (light_sensor_get_data() < settings_manager_get_light_threshold())
			{
				leds_blue_blink(LEDS_SHORT_BLINK);
				pData->light_history = 0;
			}
			else
			{
				leds_red_blink(LEDS_SHORT_BLINK);
				pData->light_history++;
				
				if (pData->light_history > SWITCH_TO_DAY_MODE_CNT)
				{
					states_schedule_state(STATE_DAY_MODE);
				}
			}
					
			/* Turn ON watchdog and sleep */
			disableWatchdog();
			enableWatchdog();
			
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);
			sleep_mode();
			
			break;
		default:
			break;				
	}	
}

