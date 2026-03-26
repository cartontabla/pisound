#include "ctrl_event.h"
#include <string.h>

/*
 * ctrl_event.c
 *
 * Simple FIFO queue for control events.
 * Thread-safe for single-writer/single-reader (the common audio case).
 */

void ctrl_event_queue_init(ctrl_event_queue_t *q)
{
    memset(q, 0, sizeof(*q));
}

int ctrl_event_enqueue(ctrl_event_queue_t *q, const ctrl_event_t *evt)
{
    if (q->count >= CTRL_EVENT_QUEUE_SIZE) {
        return -1; /* Queue full */
    }

    memcpy(&q->events[q->head], evt, sizeof(*evt));
    q->head = (q->head + 1) % CTRL_EVENT_QUEUE_SIZE;
    q->count++;

    return 0;
}

int ctrl_event_dequeue(ctrl_event_queue_t *q, ctrl_event_t *evt)
{
    if (q->count == 0) {
        return -1; /* Queue empty */
    }

    memcpy(evt, &q->events[q->tail], sizeof(*evt));
    q->tail = (q->tail + 1) % CTRL_EVENT_QUEUE_SIZE;
    q->count--;

    return 0;
}

int ctrl_event_queue_empty(const ctrl_event_queue_t *q)
{
    return q->count == 0;
}
