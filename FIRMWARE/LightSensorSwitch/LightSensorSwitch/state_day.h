#ifndef STATE_DAY_H
#define STATE_DAY_H

void state_day_enter(void **pStateBuf);
void state_day_exit(void **pStateBuf);
void state_day_event_handler(uint8_t event, void **pStateBuf, void *data);
	
#endif