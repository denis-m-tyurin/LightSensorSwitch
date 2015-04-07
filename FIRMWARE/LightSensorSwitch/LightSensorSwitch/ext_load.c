#ifndef F_CPU
#define F_CPU 1000000UL // 1 MHz
#endif

#include <avr/io.h>
#include "ext_load.h"

#define USE_PWM

#ifdef USE_PWM
	#define PWM_RESOLUTION 1024
#endif

#define FLAG_LOW_POWER 0x1
#define FLAG_OVERVOLTAGE 0x2
#define FLAG_LOAD_ON 0x4

static volatile uint8_t flags = 0;

#define LOAD_ON 1
#define LOAD_OFF 0
static volatile uint8_t load_state = LOAD_OFF;

static void check_conditions();
static void load_on();
static void load_off();

void ext_load_init()
{
	DDRD |= (1<<PD5);
	PORTD &= ~(1<<PD5);
}

void ext_load_on()
{
	flags |= FLAG_LOAD_ON;
	check_conditions();
}

void ext_load_off()
{
	flags &= ~FLAG_LOAD_ON;
	check_conditions();
}

void ext_load_overvoltage_enter()
{
	flags |= FLAG_OVERVOLTAGE;
	check_conditions();
}

void ext_load_overvoltage_exit()
{
	flags &= ~FLAG_OVERVOLTAGE;
	check_conditions();
}

void ext_load_low_power_enter()
{
	flags |= FLAG_LOW_POWER;
	check_conditions();
}

void ext_load_low_power_exit()
{
	flags &= ~FLAG_LOW_POWER;
	check_conditions();
}

void check_conditions()
{
	// While in overvoltage state - keep load always ON
	if ((flags & FLAG_OVERVOLTAGE) == FLAG_OVERVOLTAGE)
	{
		load_on();
		return;
	}

	// While in lowpower state - keep load always OFF
	if ((flags & FLAG_LOW_POWER) == FLAG_LOW_POWER)
	{
		load_off();
		return;
	}

	// in normal mode - just enable or disable load
	if ((flags & FLAG_LOAD_ON) == FLAG_LOAD_ON)
	{
		load_on();
	}
	else
	{
		load_off();
	}
}

void load_on()
{
	if (LOAD_ON == load_state)
		return;

#ifdef USE_PWM
	/* PD5 = OC0B, i.e. it is possible to use h/w PWM. But let's use
	 * software PWM here since it is just a single-shoot action which
	 * can be easily done by s/w */
	{
		uint16_t pwm_counter, pwm_top;

		for (pwm_top = 0; pwm_top < PWM_RESOLUTION; pwm_top++)
		{
			PORTD |= (1<<PD5);
			for(pwm_counter = 0; pwm_counter < PWM_RESOLUTION; pwm_counter ++)
			{
				if (pwm_counter == pwm_top)
					PORTD &= ~(1<<PD5);
			}
		}
		PORTD |= (1<<PD5);
	}

#else
	PORTD |= (1<<PD5);
#endif
	load_state = LOAD_ON;
}

void load_off()
{
	if (LOAD_OFF == load_state)
		return;

#ifdef USE_PWM
	{
		uint16_t pwm_counter, pwm_top;

		for (pwm_top = PWM_RESOLUTION; pwm_top > 0; pwm_top--)
		{
			PORTD |= (1<<PD5);
			for(pwm_counter = 0; pwm_counter < PWM_RESOLUTION; pwm_counter ++)
			{
				if (pwm_counter == pwm_top)
					PORTD &= ~(1<<PD5);
			}
		}
	}
#else
	PORTD &= ~(1<<PD5);
#endif
	load_state = LOAD_OFF;
}
