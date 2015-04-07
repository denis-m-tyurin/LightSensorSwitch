#ifndef F_CPU
#define F_CPU 1000000UL // 1 MHz
#endif

#include <avr/io.h>
#include "voltage_sensor.h"
#include "events.h"

#define VOLTAGE_TRESHOLD_DEFAULT_VALUE_OFF 430 // 2.1V
#define VOLTAGE_TRESHOLD_DEFAULT_VALUE_ON 492 // 2.4V
#define VOLTAGE_OVERVOLTAGE_VALUE 1010 //4.93V

#define CYCLES_TO_SWITCH_STATE 5 // Used for debouncing

static volatile uint8_t count_to_switch = 0;

typedef enum
{
	LOW_LEVEL_STATE = 0,
	NORMAL_LEVEL_STATE,
	OVERVOLTAGE_STATE
} voltage_state;

static volatile voltage_state current_state = NORMAL_LEVEL_STATE;

uint16_t voltage_sensor_get_data()
{
	uint16_t result;

	/* Enable ADC */
	ADCSRA |= (1<<ADEN);

	/* Select ADC2 channel */
	ADMUX |= (1<<MUX1);
	ADMUX &= ~((1<<MUX2)|(1<<MUX3)|(1<<MUX0));

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

void voltage_sensor_check_voltage()
{
	uint16_t current_voltage = voltage_sensor_get_data();

	switch(current_state)
	{
		case NORMAL_LEVEL_STATE:
			/* This means that an external load is active */
			if ( current_voltage <= VOLTAGE_TRESHOLD_DEFAULT_VALUE_OFF )
			{
				count_to_switch++;
				if (CYCLES_TO_SWITCH_STATE == count_to_switch)
				{
								count_to_switch = 0;
								current_state = LOW_LEVEL_STATE;
								post_event(EVENT_LOW_POWER);
				}
			}
			else if (current_voltage > VOLTAGE_OVERVOLTAGE_VALUE)
			{
				count_to_switch = 0;
				current_state = OVERVOLTAGE_STATE;
				post_event(EVENT_OVERVOLTAGE);
			}
			else
			{
				count_to_switch = 0;
			}
		break;
		case LOW_LEVEL_STATE:

			if (current_voltage > VOLTAGE_OVERVOLTAGE_VALUE)
			{
				count_to_switch = 0;
				current_state = OVERVOLTAGE_STATE;
				post_event(EVENT_OVERVOLTAGE);
			}
			else if ( current_voltage >= VOLTAGE_TRESHOLD_DEFAULT_VALUE_ON )
			{
				count_to_switch++;
				if (CYCLES_TO_SWITCH_STATE == count_to_switch)
				{
					count_to_switch = 0;
					current_state = NORMAL_LEVEL_STATE;
					post_event(EVENT_RESTORED_FROM_LOW_POWER);
				}
			}
			else
			{
				count_to_switch = 0;

				/* keep posting emergency event */
				post_event(EVENT_LOW_POWER);
			}


		break;
		case OVERVOLTAGE_STATE:
			if (current_voltage < VOLTAGE_OVERVOLTAGE_VALUE)
			{
				// voltage is back to normal
				count_to_switch = 0;
				current_state = NORMAL_LEVEL_STATE;
				post_event(EVENT_RESTORED_FROM_OVERVOLTAGE);
			}
			else
			{
				// continue posting emergency events
				post_event(EVENT_OVERVOLTAGE);
			}
		break;
	}



}
