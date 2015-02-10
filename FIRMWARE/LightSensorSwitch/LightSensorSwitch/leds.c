
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
	PORTB |= (1<<PB1);
	for (counter=0; counter<duration_us; counter++)
	{
		_delay_us(1);
	}
	
	PORTB &= ~(1<<PB1);
}

void leds_blue_blink(long duration_us)
{
		long counter;
		PORTB |= (1<<PB2);
		for (counter=0; counter<duration_us; counter++)
		{
			_delay_us(1);
		}
		
		PORTB &= ~(1<<PB2);
}