#ifndef F_CPU
#define F_CPU 1000000UL // 1 MHz
#endif

#include "watchdog.h"
#include <avr/wdt.h>
#include <avr/interrupt.h>

void enableWatchdog()
{
	cli();
	MCUSR &= ~(1<<WDRF);
	wdt_reset();
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	WDTCSR = (~(1<<WDP2) & ~(1<<WDP1)) | ((1<<WDIE) | (1<<WDP3) | (1<<WDP0));
	sei();
}

void disableWatchdog()
{
	cli();
	wdt_reset();
	MCUSR &= ~(1<<WDRF);
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	WDTCSR = 0x00;
	sei();
}