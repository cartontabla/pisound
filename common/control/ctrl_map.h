#ifndef CTRL_MAP_H
#define CTRL_MAP_H

#include <stdint.h>
#include <stddef.h>

/*
 * ctrl_map.h
 *
 * Mapping table: translates MIDI CC, notes, or OSC addresses
 * to parameter IDs with scaling and ramping information.
 */

typedef enum {
    CTRL_SOURCE_MIDI_CC = 0,
    CTRL_SOURCE_MIDI_NOTE,
    CTRL_SOURCE_MIDI_PITCHBEND,
    CTRL_SOURCE_OSC,
} ctrl_source_t;

typedef struct {
    uint32_t        param_id;       /* Parameter ID (app-defined) */
    char           *name;           /* Parameter name (for logging) */
    ctrl_source_t   source;         /* MIDI CC, OSC, etc. */
    
    /* Source-specific identifier */
    union {
        uint8_t     midi_cc;        /* For MIDI CC: 0..119 */
        uint8_t     midi_note;      /* For MIDI Note: 0..127 */
        char       *osc_path;       /* For OSC: e.g., "/doubling/delay" */
    } id;
    
    /* Value range and ramping */
    float           min_val;        /* Minimum mapped value */
    float           max_val;        /* Maximum mapped value */
    uint16_t        ramp_time_ms;   /* Smoothing time (0 = no ramp) */
} ctrl_mapping_t;

typedef struct {
    ctrl_mapping_t *entries;
    size_t         count;
    size_t         capacity;
} ctrl_map_table_t;

/* Public API */
int  ctrl_map_init(ctrl_map_table_t *map, size_t capacity);
void ctrl_map_free(ctrl_map_table_t *map);

int  ctrl_map_add(ctrl_map_table_t *map, const ctrl_mapping_t *entry);
int  ctrl_map_by_midi_cc(const ctrl_map_table_t *map, uint8_t cc, 
                         ctrl_mapping_t **out);
int  ctrl_map_by_osc_path(const ctrl_map_table_t *map, const char *path,
                          ctrl_mapping_t **out);

/* Load from config file */
int  ctrl_map_load(ctrl_map_table_t *map, const char *filepath);

#endif /* CTRL_MAP_H */
