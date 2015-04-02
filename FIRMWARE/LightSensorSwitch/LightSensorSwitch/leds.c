
#ifndef F_CPU
#define F_CPU 1000000UL // 1 MHz
#endif

#include <avr/io.h>
#include <util/delay.h>
#include "leds.h"

void leds_init()
{
	/* PB1 - RED; PB2 - BLUE
	 * Set PB1&PB2 as Outputs */
	DDRB = (1 << PB1) | (1 << PB2);
}

void leds_red_blink(long duration_us)
{
	long counter;
	leds_set_red(LED_STATE_ON);
	for (counter=0; counter<duration_us; counter++)
	{
		_delay_us(1);
	}
	
	leds_set_red(LED_STATE_OFF);
}

void leds_blue_blink(long duration_us)
{
		long counter;
		leds_set_blue(LED_STATE_ON);
		for (counter=0; counter<duration_us; counter++)
		{
			_delay_us(1);
		}
		
		leds_set_blue(LED_STATE_OFF);
}

void leds_red_flash(long duration_ms)
{
	long counter;
		
	for (counter = duration_ms / 100; counter>0; counter--)
	{
		leds_set_red(LED_STATE_ON);
		_delay_ms(50);
		leds_set_red(LED_STATE_OFF);
		_delay_ms(50);
	}
}

void leds_blue_flash(long duration_ms)
{
	long counter;
		
	for (counter = duration_ms / 100; counter>0; counter--)
	{
		leds_set_blue(LED_STATE_ON);
		_delay_ms(50);
		leds_set_blue(LED_STATE_OFF);
		_delay_ms(50);
	}
}

void leds_set_red(int led_state)
{
	if (LED_STATE_ON == led_state)
	{
		PORTB |= (1<<PB1);
	}
	else if (LED_STATE_OFF == led_state)
	{
		PORTB &= ~(1<<PB1);
	}
}

void leds_set_blue(int led_state)
{
	if (LED_STATE_ON == led_state)
	{
		PORTB |= (1<<PB2);
	}
	else if (LED_STATE_OFF == led_state)
	{
		PORTB &= ~(1<<PB2);
	}	
}