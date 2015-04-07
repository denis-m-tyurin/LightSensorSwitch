#ifndef STATE_NIGHT_H
#define STATE_NIGHT_H

#include "events.h"

void state_night_enter(void **pStateBuf);
void state_night_exit(void **pStateBuf);
void state_night_event_handler(event_t event, void **pStateBuf, void *data);
	
#endif
