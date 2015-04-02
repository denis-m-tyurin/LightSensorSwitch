
#include "states.h"

/* include all the states */
#include "state_day.h"
#include "state_night.h"
#include "state_settings.h"

static void *pCurrentStateBuf = NULL;

static STATE_ID_T s_current_state = STATE_NO_STATE;
static STATE_ID_T s_scheduled_state = STATE_NO_STATE;
static STATE_ID_T s_past_state = STATE_NO_STATE;

STATE_T s_states[] = {
		{STATE_NO_STATE,
		NULL,
		NULL,
		NULL},
		{STATE_DAY_MODE,
		state_day_enter,
		state_day_exit,
		state_day_event_handler},
		{STATE_NIGHT_MODE,
		state_night_enter,
		state_night_exit,
		state_night_event_handler},
		{STATE_SETTINGS_MODE,
		state_settings_enter,
		state_settings_exit,
		state_settings_event_handler},
};

void states_set_state(STATE_ID_T state_id)
{
	/* Check if appropriate state ID exists */
	if ((state_id <= STATE_NO_STATE) || (state_id >= STATE_STATE_MAX))
		return;
	
	/* If it is the same state - no need to exit and then re-init */
	if (state_id != s_current_state)
	{
		/* Check whether we have an active state. Tear it down if necessary */
		if (STATE_NO_STATE != s_current_state)
		{
			s_states[s_current_state].state_exit(&pCurrentStateBuf);
		}
	
		/* Init the new state */
		s_past_state = s_current_state;
		s_current_state = state_id;
		s_states[s_current_state].state_enter(&pCurrentStateBuf);
	}
}

void states_schedule_state(STATE_ID_T state_id)
{
	s_scheduled_state = state_id;
}

STATE_ID_T states_get_next_state()
{
	STATE_ID_T next_state = s_current_state + 1;
	
	if (STATE_STATE_MAX == next_state)
	{
		/* Start from the beginning */
		next_state = STATE_NO_STATE + 1;
	}
	
	return next_state;
}

STATE_ID_T states_get_prev_state()
{
	STATE_ID_T prev_state = s_current_state - 1;
	
	if (STATE_NO_STATE == prev_state)
	{
		/* Jump to the last state  */
		prev_state = STATE_STATE_MAX - 1;
	}
	
	return prev_state;
}

STATE_ID_T states_get_scheduled_state()
{
	STATE_ID_T scheduled_state = s_scheduled_state;
	s_scheduled_state = STATE_NO_STATE;
	return scheduled_state;
}


STATE_ID_T states_get_past_state()
{
	return s_past_state;
}

void states_dispatch_event(uint8_t event, void *data)
{
	/* Check if any state is set */
	if (s_current_state == STATE_NO_STATE) 
		return;
	
	/* Dispatch event to current state */
	s_states[s_current_state].state_event_handler(event, &pCurrentStateBuf, data);
	
}
