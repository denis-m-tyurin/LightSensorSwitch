#ifndef F_CPU
#define F_CPU 1000000UL // 1 MHz
#endif

#include <stdlib.h>
#include <stdio.h>
#include <avr/eeprom.h>
#include "settings_manager.h"

/* EEPROM address list */
#define EEPROM_MAGIC_NUMBER_ADDR 2
#define EEPROM_SETTING_LIGHT_THRESHOLD_ADDR 4

/* Other defines */
#define EEPROM_MAGIC_NUMBER 0xABCD
#define LIGHT_TRESHOLD_DEFAULT_VALUE 800
#define LIGHT_TRESHOLD_DEFAULT_STEP 50

static uint8_t magic_written = 0;

static uint16_t light_threshold = 0;

void settings_manager_init()
{
	light_threshold = settings_manager_get_default_light_threshold();
	// Read all settings from EEPROM here
	if (eeprom_read_word((void *)EEPROM_MAGIC_NUMBER_ADDR) == EEPROM_MAGIC_NUMBER){
		// ok magic number matches accept values
		magic_written = 1;
		
		light_threshold = eeprom_read_word((void *)EEPROM_SETTING_LIGHT_THRESHOLD_ADDR);		
	}
	else
	{
		magic_written = 0;
	}
}

void settings_manager_write_data()
{
	if (0 == magic_written)
	{
		magic_written=1;
		eeprom_write_word((void *)EEPROM_MAGIC_NUMBER_ADDR, EEPROM_MAGIC_NUMBER);		
	}
	
	eeprom_write_word((void *) EEPROM_SETTING_LIGHT_THRESHOLD_ADDR, light_threshold);	
}

uint16_t settings_manager_get_default_light_threshold()
{
	return LIGHT_TRESHOLD_DEFAULT_VALUE;
}

uint16_t settings_manager_get_light_threshold()
{
	return light_threshold;
}

uint16_t settings_manager_get_light_threshold_step()
{
	return LIGHT_TRESHOLD_DEFAULT_STEP;
}

void settings_manager_set_light_threshold(uint16_t lightThreshold)
{
	light_threshold = lightThreshold;
	settings_manager_write_data();
}

