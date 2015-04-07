#ifndef STATE_DAY_H
#define STATE_DAY_H

#include "events.h"

void state_day_enter(void **pStateBuf);
void state_day_exit(void **pStateBuf);
void state_day_event_handler(event_t event, void **pStateBuf, void *data);
	
#endif
