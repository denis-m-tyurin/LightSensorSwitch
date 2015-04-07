#include "events.h"
#include <stdio.h>

typedef struct event_list_item event_list_item;

struct event_list_item
{
	event_t event;
	event_list_item *next_event;
};

static volatile event_list_item *event_queue = NULL;

void post_event(event_t event)
{
	event_list_item *new_event = (event_list_item*) malloc(sizeof(event_list_item));
	event_list_item *queue_last = event_queue;

	new_event->event = event;
	new_event->next_event = NULL;

	if (NULL == event_queue)
	{
		// No events in queue, this will be the first one
		event_queue = new_event;
	}
	else
	{
		/* find last item in the queue */
		while (NULL != queue_last->next_event)
		{
			queue_last = queue_last->next_event;
		}

		queue_last->next_event = new_event;
	}
}

event_t get_event_from_queue()
{
	event_t result = EVENT_NONE;
	event_list_item *event_from_queue = event_queue;

	if (NULL == event_from_queue)
		return EVENT_NONE;

	result = event_from_queue->event;
	event_queue = event_from_queue->next_event;

	free(event_from_queue);

	return result;
}
