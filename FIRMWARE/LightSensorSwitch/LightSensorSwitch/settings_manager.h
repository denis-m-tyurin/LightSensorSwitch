#ifndef SETTINGS_MANAGER_H
#define SETTINGS_MANAGER_H

void settings_manager_init();
uint16_t settings_manager_get_light_threshold();
uint16_t settings_manager_get_default_light_threshold();
uint16_t settings_manager_get_light_threshold_step();
void settings_manager_set_light_threshold(uint16_t lightThreshold);

#endif