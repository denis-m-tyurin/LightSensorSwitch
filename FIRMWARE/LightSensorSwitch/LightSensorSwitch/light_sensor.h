#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

typedef uint16_t light_sensor_data;

light_sensor_data light_sensor_get_data();
void light_sensor_check();

#endif
