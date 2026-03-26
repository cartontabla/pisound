#include "ctrl_map.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/*
 * ctrl_map.c
 *
 * Mapping table implementation.
 * Supports loading MIDI and OSC mappings from ASCII config files.
 */

int ctrl_map_init(ctrl_map_table_t *map, size_t capacity)
{
    if (capacity == 0) capacity = 64;
    
    map->entries = (ctrl_mapping_t *)malloc(capacity * sizeof(ctrl_mapping_t));
    if (!map->entries) return -1;
    
    memset(map->entries, 0, capacity * sizeof(ctrl_mapping_t));
    map->count = 0;
    map->capacity = capacity;
    
    return 0;
}

void ctrl_map_free(ctrl_map_table_t *map)
{
    if (!map) return;
    
    for (size_t i = 0; i < map->count; i++) {
        if (map->entries[i].name) {
            free(map->entries[i].name);
        }
        if (map->entries[i].source == CTRL_SOURCE_OSC && map->entries[i].id.osc_path) {
            free(map->entries[i].id.osc_path);
        }
    }
    
    if (map->entries) {
        free(map->entries);
    }
    
    memset(map, 0, sizeof(*map));
}

int ctrl_map_add(ctrl_map_table_t *map, const ctrl_mapping_t *entry)
{
    if (!map || !entry) return -1;
    if (map->count >= map->capacity) return -2; /* Capacity exceeded */
    
    ctrl_mapping_t *dst = &map->entries[map->count];
    
    /* Copy basic fields */
    dst->param_id       = entry->param_id;
    dst->source         = entry->source;
    dst->min_val        = entry->min_val;
    dst->max_val        = entry->max_val;
    dst->ramp_time_ms   = entry->ramp_time_ms;
    
    /* Copy name */
    if (entry->name) {
        dst->name = (char *)malloc(strlen(entry->name) + 1);
        if (!dst->name) return -3;
        strcpy(dst->name, entry->name);
    } else {
        dst->name = NULL;
    }
    
    /* Copy source-specific data */
    if (entry->source == CTRL_SOURCE_OSC) {
        if (entry->id.osc_path) {
            dst->id.osc_path = (char *)malloc(strlen(entry->id.osc_path) + 1);
            if (!dst->id.osc_path) {
                free(dst->name);
                return -3;
            }
            strcpy(dst->id.osc_path, entry->id.osc_path);
        }
    } else {
        dst->id.midi_cc = entry->id.midi_cc;
    }
    
    map->count++;
    return 0;
}

int ctrl_map_by_midi_cc(const ctrl_map_table_t *map, uint8_t cc, 
                        ctrl_mapping_t **out)
{
    if (!map || !out) return -1;
    
    for (size_t i = 0; i < map->count; i++) {
        if (map->entries[i].source == CTRL_SOURCE_MIDI_CC && 
            map->entries[i].id.midi_cc == cc) {
            *out = &map->entries[i];
            return 0;
        }
    }
    
    return -2; /* Not found */
}

int ctrl_map_by_osc_path(const ctrl_map_table_t *map, const char *path,
                         ctrl_mapping_t **out)
{
    if (!map || !path || !out) return -1;
    
    for (size_t i = 0; i < map->count; i++) {
        if (map->entries[i].source == CTRL_SOURCE_OSC && 
            map->entries[i].id.osc_path &&
            strcmp(map->entries[i].id.osc_path, path) == 0) {
            *out = &map->entries[i];
            return 0;
        }
    }
    
    return -2; /* Not found */
}

/*
 * config file format (ASCII):
 * Lines starting with '#' are comments.
 * Format: transport|source|min|max|ramp_ms|param_name
 * 
 * Example:
 *   midi|cc:44|0|100|50|delay_ms
 *   midi|cc:45|0|1|100|depth
 *   osc|/doubling/bypass|0|1|0|bypass
 */

static int _parse_line(char *line, ctrl_mapping_t *out, uint32_t *param_id)
{
    char *saveptr;
    char *transport, *source_str, *min_str, *max_str, *ramp_str, *name_str;
    
    /* Tokenize */
    transport = strtok_r(line, "|", &saveptr);
    source_str = strtok_r(NULL, "|", &saveptr);
    min_str = strtok_r(NULL, "|", &saveptr);
    max_str = strtok_r(NULL, "|", &saveptr);
    ramp_str = strtok_r(NULL, "|", &saveptr);
    name_str = strtok_r(NULL, "|", &saveptr);
    
    if (!transport || !source_str || !min_str || !max_str || !ramp_str) {
        return -1;
    }
    
    /* Parse values */
    out->min_val = (float)atof(min_str);
    out->max_val = (float)atof(max_str);
    out->ramp_time_ms = (uint16_t)atoi(ramp_str);
    
    /* Assign param_id (auto-increment) */
    out->param_id = (*param_id)++;
    
    /* Parse name */
    if (name_str) {
        out->name = (char *)malloc(strlen(name_str) + 1);
        if (out->name) strcpy(out->name, name_str);
    }
    
    /* Parse transport and source */
    if (strcmp(transport, "midi") == 0) {
        if (strncmp(source_str, "cc:", 3) == 0) {
            out->source = CTRL_SOURCE_MIDI_CC;
            out->id.midi_cc = (uint8_t)atoi(source_str + 3);
        } else {
            return -1;
        }
    } else if (strcmp(transport, "osc") == 0) {
        out->source = CTRL_SOURCE_OSC;
        out->id.osc_path = (char *)malloc(strlen(source_str) + 1);
        if (!out->id.osc_path) return -1;
        strcpy(out->id.osc_path, source_str);
    } else {
        return -1;
    }
    
    return 0;
}

int ctrl_map_load(ctrl_map_table_t *map, const char *filepath)
{
    FILE *fp;
    char line[256];
    int lineno = 0;
    uint32_t param_id = 0;
    
    if (!map || !filepath) return -1;
    
    fp = fopen(filepath, "r");
    if (!fp) return -2; /* File not found */
    
    while (fgets(line, sizeof(line), fp)) {
        lineno++;
        
        /* Remove trailing newline */
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') line[len-1] = '\0';
        
        /* Skip empty lines and comments */
        if (line[0] == '\0' || line[0] == '#') continue;
        
        /* Skip leading whitespace */
        char *start = line;
        while (*start && isspace(*start)) start++;
        if (!*start) continue;
        
        /* Parse line */
        ctrl_mapping_t entry = {0};
        if (_parse_line(start, &entry, &param_id) != 0) {
            fprintf(stderr, "Warning: ctrl_map_load() error at line %d\n", lineno);
            continue;
        }
        
        /* If another entry with the same param_name already exists, share
         * its param_id so MIDI and OSC control the same parameter slot. */
        if (entry.name) {
            for (size_t j = 0; j < map->count; j++) {
                if (map->entries[j].name &&
                    strcmp(map->entries[j].name, entry.name) == 0) {
                    entry.param_id = map->entries[j].param_id;
                    param_id--;   /* undo the auto-increment */
                    break;
                }
            }
        }

        /* Add to map */
        if (ctrl_map_add(map, &entry) != 0) {
            fprintf(stderr, "Warning: ctrl_map_load() failed to add entry at line %d\n", lineno);
        }
    }
    
    fclose(fp);
    return 0;
}
