#ifndef STATES_H
#define STATES_H

#include <avr/io.h>
#include <stddef.h>
#include "events.h"

/*	States description
 *
 *	< Day mode   								>
 *				|
 *	< Night mode								>
 *				| 
 *	< Settings									>
 *				
 */

typedef enum 
{
	STATE_NO_STATE = 0,
	STATE_DAY_MODE,
	STATE_NIGHT_MODE,
		
	STATE_STATE_MAX
} STATE_ID_T;

typedef struct  
{
	int state_id;
	void (*state_enter)(void **);
	void (*state_exit)(void **);
	void (*state_event_handler)(uint8_t, void **, void*);	
} STATE_T;

void states_set_state(STATE_ID_T state_id);
void states_schedule_state(STATE_ID_T state_id);

STATE_ID_T states_get_next_state();
STATE_ID_T states_get_prev_state();
STATE_ID_T states_get_scheduled_state();
STATE_ID_T states_get_past_state();

void states_dispatch_event(event_t event, void *data);
#endif
