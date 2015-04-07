#ifndef F_CPU
#define F_CPU 1000000UL // 1 MHz
#endif

#include <avr/io.h>
#include "light_sensor.h"
#include "events.h"

#define LIGHT_TRESHOLD_DEFAULT_VALUE_ON 800
#define LIGHT_TRESHOLD_DEFAULT_VALUE_OFF 850

#define CYCLES_TO_SWITCH_STATE 8 // Used for debouncing

typedef enum
{
	LIGHT_SENSOR_OK_TO_ON = 0,
	LIGHT_SENSOR_OK_TO_OFF
} light_sensor_state;

static volatile light_sensor_state current_state = LIGHT_SENSOR_OK_TO_OFF;

static volatile uint8_t count_to_switch = 0;



light_sensor_data light_sensor_get_data()
{
	light_sensor_data result;
	
	/* Enable ADC */
	ADCSRA |= (1<<ADEN);

	/* Select ADC3 channel */
	ADMUX |= (1<<MUX0)|(1<<MUX1);
	ADMUX &= ~((1<<MUX2)|(1<<MUX3));

	/* Start conversion */
	ADCSRA |= (1<<ADSC);
	
	/* Wait until its completion */
	while ((ADCSRA & (1<<ADSC)) != 0);
	
	/* Read ADC value (ADCL MUST BE READ FIRST) */
	result = ADCL;
	result |= (ADCH<<8);
	
	ADCSRA &= ~(1<<ADEN);
	
	return result;
}

void light_sensor_check()
{
	light_sensor_data new_data = light_sensor_get_data();

	switch(current_state)
	{
		case LIGHT_SENSOR_OK_TO_ON:
			/* We are in night mode */
			if ( new_data >= LIGHT_TRESHOLD_DEFAULT_VALUE_OFF )
			{
				count_to_switch++;
				if (CYCLES_TO_SWITCH_STATE == count_to_switch)
				{
					count_to_switch = 0;
					current_state = LIGHT_SENSOR_OK_TO_OFF;
					post_event(EVENT_LIGHT_SENSOR_OK_TO_OFF);
				}
				else
				{
					post_event(EVENT_LIGHT_SENSOR_BLINK_RED);
				}
			}
			else
			{
				post_event(EVENT_LIGHT_SENSOR_BLINK_BLUE);
				count_to_switch = 0;
			}
		break;
		case LIGHT_SENSOR_OK_TO_OFF:
			/* We are in day mode */
			if ( new_data <= LIGHT_TRESHOLD_DEFAULT_VALUE_ON )
			{
				count_to_switch++;
				if (CYCLES_TO_SWITCH_STATE == count_to_switch)
				{
					count_to_switch = 0;
					current_state = LIGHT_SENSOR_OK_TO_ON;
					post_event(EVENT_LIGHT_SENSOR_OK_TO_ON);
				}
				else
				{
					post_event(EVENT_LIGHT_SENSOR_BLINK_BLUE);
				}
			}
			else
			{
				post_event(EVENT_LIGHT_SENSOR_BLINK_RED);
				count_to_switch = 0;
			}
		break;
	}
}
