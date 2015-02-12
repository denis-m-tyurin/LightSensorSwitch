#ifndef STATE_NIGHT_H
#define STATE_NIGHT_H

void state_night_enter(void **pStateBuf);
void state_night_exit(void **pStateBuf);
void state_night_event_handler(uint8_t event, void **pStateBuf, void *data);
	
#endif