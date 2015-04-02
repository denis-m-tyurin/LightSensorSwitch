#ifndef STATE_SETTINGS_H
#define STATE_SETTINGS_H

void state_settings_enter(void **pStateBuf);
void state_settings_exit(void **pStateBuf);
void state_settings_event_handler(uint8_t event, void **pStateBuf, void *data);
	
#endif