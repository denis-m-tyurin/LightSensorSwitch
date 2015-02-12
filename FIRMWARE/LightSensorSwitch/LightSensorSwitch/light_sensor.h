#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

void light_sensor_init();

uint16_t light_sensor_get_data();

uint16_t get_light_treshold();

#endif