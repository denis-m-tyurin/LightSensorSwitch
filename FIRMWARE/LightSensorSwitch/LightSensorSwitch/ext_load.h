#ifndef EXT_LOAD_H
#define EXT_LOAD_H

void ext_load_init();
void ext_load_on();
void ext_load_off();

void ext_load_overvoltage_enter();
void ext_load_overvoltage_exit();

void ext_load_low_power_enter();
void ext_load_low_power_exit();

#endif
