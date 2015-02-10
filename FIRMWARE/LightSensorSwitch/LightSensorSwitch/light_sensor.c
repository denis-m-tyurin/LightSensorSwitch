#ifndef F_CPU
#define F_CPU 1000000UL // 1 MHz
#endif

#include <avr/io.h>
#include "light_sensor.h"

void light_sensor_init()
{
	/* Setup ADC 
	 * Light sensor (resistor) is connected to ADC3 */
	
	/* Voltage reference - AVcc
	 * Channel - ADC3  */
	ADMUX = (1 << REFS0) | (1<<MUX1) | (1<<MUX0);
	
	/* Enable ADC */
	ADCSRA |= (1<<ADEN);
	
}

uint16_t light_sensor_get_data()
{
	uint16_t result;
	
	/* Start conversion */
	ADCSRA |= (1<<ADSC);
	
	/* Wait until its completion */
	while ((ADCSRA & (1<<ADSC)) != 0);
	
	/* Read ADC value (ADCL MUST BE READ FIRST) */
	result = ADCL;
	result |= (ADCH<<8);
	return result;
	
}