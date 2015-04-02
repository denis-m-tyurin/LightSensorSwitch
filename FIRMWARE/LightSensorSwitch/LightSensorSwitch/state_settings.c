#ifndef F_CPU
#define F_CPU 1000000UL // 1 MHz
#endif

#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "state_settings.h"
#include "states.h"
#include "light_sensor.h"
#include "leds.h"
#include "settings_manager.h"

#define IDLE_CYCLES_TO_EXIT 20
#define LED_SWITCH_DELAY 4000

typedef struct
{
	uint8_t idle_cycles; //Used to exit the settings mode
} settings_state_data;


void state_settings_enter(void **pStateBuf)
{
	settings_state_data *pData;
		
	*pStateBuf = malloc(sizeof(settings_state_data));
	pData = (settings_state_data*) *pStateBuf;
	
	pData->idle_cycles = 0;

}

void state_settings_exit(void **pStateBuf)
{
		if (NULL != *pStateBuf)
		{
			free(*pStateBuf);
		}
}

void state_settings_event_handler(uint8_t event, void **pStateBuf, void *data)
{
	settings_state_data *pData = (settings_state_data*) *pStateBuf;
		
	switch (event)
	{
		uint16_t red_val;
		uint16_t blue_val;
		
		case EVENT_IDLE:
			
			leds_set_red(LED_STATE_ON);
			_delay_ms(LED_SWITCH_DELAY);
			leds_set_red(LED_STATE_OFF);
			_delay_ms(100);
			leds_set_red(LED_STATE_ON);
			red_val = light_sensor_get_data();
			_delay_ms(LED_SWITCH_DELAY);
			leds_set_red(LED_STATE_OFF);
			leds_set_blue(LED_STATE_ON);
			_delay_ms(LED_SWITCH_DELAY);
			leds_set_blue(LED_STATE_OFF);
			_delay_ms(100);
			leds_set_blue(LED_STATE_ON);
			blue_val = light_sensor_get_data();
			_delay_ms(LED_SWITCH_DELAY);
			leds_set_blue(LED_STATE_OFF);
			pData->idle_cycles++;
			
			if ((blue_val > settings_manager_get_light_threshold()) && (red_val <= settings_manager_get_light_threshold()))
			{
				/* This means that DAY should lasts longer and we need to lower threshold */
				leds_red_flash(5000);
				settings_manager_set_light_threshold(settings_manager_get_light_threshold() - settings_manager_get_light_threshold_step());
				pData->idle_cycles = 0;
			}
			
			if ((blue_val < settings_manager_get_light_threshold()) && (red_val >= settings_manager_get_light_threshold()))
			{
				/* This means that NIGHT should lasts longer and we need to higher threshold */
				leds_blue_flash(5000);
				settings_manager_set_light_threshold(settings_manager_get_light_threshold() + settings_manager_get_light_threshold_step());
				pData->idle_cycles = 0;
			}
						
			if (IDLE_CYCLES_TO_EXIT == pData->idle_cycles)
			{
				states_schedule_state(STATE_NIGHT_MODE);
			}
			break;
		default:
			break;				
	}	
}

