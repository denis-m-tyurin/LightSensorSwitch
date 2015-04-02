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
#include "settings_manager.h"

typedef struct
{
	uint8_t light_history;
	uint8_t last_val; // used to enter settings mode
	uint8_t settings_seq; // used to enter settings mode
} day_state_data;

/* A number of night level light measurements that we get 
 * before switching to the night mode. This is to exclude 
 * ADC errors or impulses like flashlights, blinks, etc */
#define SWITCH_TO_NIGHT_MODE_CNT 8 

/* A number of special sequences used that we get
 * before switching to the settings mode */
#define SETTINGS_MODE_SWITCH_CNT 2

#define DAY_MODE 0
#define NIGHT_MODE 1

static void state_light_event(uint8_t current_mode, day_state_data *pData);

void state_day_enter(void **pStateBuf)
{
	day_state_data *pData;
		
	*pStateBuf = malloc(sizeof(day_state_data));
	pData = (day_state_data*) *pStateBuf;

	pData->light_history = 0;
	pData->last_val = DAY_MODE;
	pData->settings_seq = 0;
	
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
			if (light_sensor_get_data() < settings_manager_get_light_threshold())
			{
				leds_blue_blink(LEDS_SHORT_BLINK);
				pData->light_history++;
				
				if (pData->light_history > SWITCH_TO_NIGHT_MODE_CNT)
				{
					states_schedule_state(STATE_NIGHT_MODE);
				}
				else
				{
					state_light_event(NIGHT_MODE, pData);
				}
			}
			else
			{
				leds_red_blink(LEDS_SHORT_BLINK);
				pData->light_history = 0;
				state_light_event(DAY_MODE, pData);
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

static void state_light_event(uint8_t current_mode, day_state_data *pData)
{
	/* The sequence that switches to the settings mode:
	 * "DAY" level followed by "NIGHT" mode, repeated 
	 * SETTINGS_MODE_SWITCH_CNT times */
	
	/* New mode should differ previous one, otherwise reset sequence */
	if (current_mode == pData->last_val)
	{
		pData->settings_seq = 0;
		return;	
	}
	
	/* if this transition from NIGHT to DAY then it means that
	 * one step of sequence is passed
	 */
	if(DAY_MODE == current_mode)
	{
		pData->settings_seq++;
	}
	
	if (SETTINGS_MODE_SWITCH_CNT == pData->settings_seq)
	{
		/* Switch to settings mode when sequence is completed */
		states_schedule_state(STATE_SETTINGS_MODE);
	}
	
	/* And finally remember current mode */
	pData->last_val = current_mode;
}


