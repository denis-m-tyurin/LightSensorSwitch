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

typedef struct
{
	uint8_t light_history;	
} day_state_data;

/* A number of night level light measurements that we get 
 * before switching to the night mode. This is to exclude 
 * ADC errors or impulses like flashlights, blinks, etc */
#define SWITCH_TO_NIGHT_MODE_CNT 8 

void state_day_enter(void **pStateBuf)
{
	day_state_data *pData;
		
	*pStateBuf = malloc(sizeof(day_state_data));
	pData = (day_state_data*) *pStateBuf;

	pData->light_history = 0;
	
	PORTD &= ~(1<<PD5);
}

void state_day_exit(void **pStateBuf)
{
		if (NULL != *pStateBuf)
		{
			free(*pStateBuf);
		}
}


void state_day_event_handler(uint8_t event, void **pStateBuf, void *data)
{
	day_state_data *pData = (day_state_data*) *pStateBuf;
		
	switch (event)
	{
		case EVENT_IDLE:
			/* Run ADC and wait for completion */
			if (light_sensor_get_data() < get_light_treshold())
			{
				leds_blue_blink(LEDS_SHORT_BLINK);
				pData->light_history++;
				
				if (pData->light_history > SWITCH_TO_NIGHT_MODE_CNT)
				{
					states_schedule_state(STATE_NIGHT_MODE);
				}
			}
			else
			{
				leds_red_blink(LEDS_SHORT_BLINK);
				pData->light_history = 0;
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

