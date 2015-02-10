#ifndef F_CPU
#define F_CPU 1000000UL // 1 MHz
#endif

#include <stdio.h>
#include <stdlib.h>
#include "state_day.h"
#include "states.h"
#include "light_sensor.h"
#include "leds.h"
#include <util/delay.h>

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
			if (light_sensor_get_data() < 800)
			{
				leds_blue_blink(LEDS_LONG_BLINK);
			}
			else
			{
				leds_red_blink(LEDS_LONG_BLINK);
			}
			_delay_ms(1000);
			
			/* Read ADC value */
			
			
			break;
		default:
			break;				
	}	
}