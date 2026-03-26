#ifndef CTRL_EVENT_H
#define CTRL_EVENT_H

#include <stdint.h>
#include <stddef.h>

/*
 * ctrl_event.h
 *
 * Defines unified internal control event representation.
 * All transports (MIDI/OSC) and protocols translate to this common format.
 */

typedef enum {
    CTRL_MODE_ABSOLUTE,   /* Direct value; replaces current */
    CTRL_MODE_RELATIVE,   /* Increment/decrement from current */
    CTRL_MODE_TOGGLE,     /* Binary: press = toggle */
} ctrl_mode_t;

typedef struct {
    uint32_t    param_id;          /* Parameter identifier (0..255) */
    float       value;              /* Normalized value [0.0, 1.0] */
    ctrl_mode_t mode;               /* Interpretation of value */
    uint64_t    timestamp_us;       /* Microseconds since start (for sequencing) */
} ctrl_event_t;

/* Event queue macros */
#define CTRL_EVENT_QUEUE_SIZE 256

typedef struct {
    ctrl_event_t events[CTRL_EVENT_QUEUE_SIZE];
    size_t       head;    /* Write position */
    size_t       tail;    /* Read position */
    size_t       count;   /* Number of events in queue */
} ctrl_event_queue_t;

/* Public API */
void ctrl_event_queue_init(ctrl_event_queue_t *q);
int  ctrl_event_enqueue(ctrl_event_queue_t *q, const ctrl_event_t *evt);
int  ctrl_event_dequeue(ctrl_event_queue_t *q, ctrl_event_t *evt);
int  ctrl_event_queue_empty(const ctrl_event_queue_t *q);

#endif /* CTRL_EVENT_H */
