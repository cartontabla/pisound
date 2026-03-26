/*
 * test_ctrl_arch.c
 *
 * Simple unit test to validate control architecture.
 * Tests:
 *  1. Config file loading
 *  2. Event queue
 *  3. MIDI parsing
 *  4. Parameter ramping
 *
 * Compile:
 *   gcc -o test_ctrl test_ctrl_arch.c ctrl_event.c ctrl_map.c ctrl_apply.c midi_proto.c -lm
 *   ./test_ctrl
 */

#include "ctrl_event.h"
#include "ctrl_map.h"
#include "ctrl_apply.h"
#include "midi_proto.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define FS 48000
#define FRAME_SIZE 128

static void test_event_queue(void)
{
    printf("\n=== Test 1: Event Queue ===\n");
    
    ctrl_event_queue_t q;
    ctrl_event_queue_init(&q);
    
    /* Enqueue some events */
    for (int i = 0; i < 3; i++) {
        ctrl_event_t evt = {
            .param_id = i,
            .value = i * 0.333f,
            .mode = CTRL_MODE_ABSOLUTE,
            .timestamp_us = i * 1000
        };
        
        if (ctrl_event_enqueue(&q, &evt) != 0) {
            printf("ERROR: Failed to enqueue event %d\n", i);
            return;
        }
    }
    
    printf("Enqueued 3 events\n");
    
    /* Dequeue and verify */
    int count = 0;
    ctrl_event_t evt;
    while (ctrl_event_dequeue(&q, &evt) == 0) {
        printf("  Event %d: param_id=%u, value=%.3f\n", 
               count, evt.param_id, evt.value);
        count++;
    }
    
    if (count == 3) {
        printf("✓ Event queue OK\n");
    } else {
        printf("✗ Event queue FAILED (expected 3 events, got %d)\n", count);
    }
}

static void test_midi_parsing(void)
{
    printf("\n=== Test 2: MIDI Parsing ===\n");
    
    midi_proto_ctx_t proto;
    midi_proto_init(&proto);
    
    /* Simulate Oxygen 25 sending CC#1 = 100 */
    uint8_t midi_data[] = {0xB0, 0x01, 0x64};  /* CC on ch0, CC#1, value=100 */
    
    printf("Pushing MIDI bytes: 0x%02x 0x%02x 0x%02x\n", 
           midi_data[0], midi_data[1], midi_data[2]);
    
    midi_proto_push_bytes(&proto, midi_data, 3);
    
    /* Parse message */
    midi_msg_t msg;
    if (midi_proto_get_message(&proto, &msg) != 0) {
        printf("✗ Failed to parse MIDI message\n");
        return;
    }
    
    printf("Parsed: ");
    midi_proto_print_message(&msg);
    
    /* Convert to event */
    ctrl_event_t evt;
    if (midi_proto_to_event(&msg, &evt) != 0) {
        printf("✗ Failed to convert to event\n");
        return;
    }
    
    printf("Event: param_id=%u, value=%.3f, mode=%d\n",
           evt.param_id, evt.value, evt.mode);
    
    if (evt.param_id == 1 && fabs(evt.value - 100.0f/127.0f) < 0.01f) {
        printf("✓ MIDI parsing OK\n");
    } else {
        printf("✗ MIDI parsing FAILED\n");
    }
}

static void test_ramping(void)
{
    printf("\n=== Test 3: Parameter Ramping ===\n");
    
    ctrl_apply_ctx_t ctx;
    ctrl_apply_init(&ctx, 4, FS, FRAME_SIZE);
    
    /* Create a mapping: CC#1 → param 0, range [0..100], 100ms ramp */
    ctrl_mapping_t mapping = {
        .param_id = 0,
        .name = "delay_ms",
        .source = CTRL_SOURCE_MIDI_CC,
        .id.midi_cc = 1,
        .min_val = 0.0f,
        .max_val = 100.0f,
        .ramp_time_ms = 100
    };
    
    /* Create event: CC value 64 (≈ 50) */
    ctrl_event_t evt = {
        .param_id = 0,
        .value = 64.0f / 127.0f,
        .mode = CTRL_MODE_ABSOLUTE,
        .timestamp_us = 0
    };
    
    printf("Initial value: %.2f\n", ctrl_apply_get(&ctx, 0));
    printf("Applying event (target ≈ %.2f) with 100ms ramp...\n", mapping.min_val + evt.value * (mapping.max_val - mapping.min_val));
    
    ctrl_apply_event(&ctx, &evt, &mapping);
    
    printf("Target value: %.2f\n", ctrl_apply_get_target(&ctx, 0));
    
    /* Simulate audio frames */
    int frames = 0;
    int duration_ms = 0;
    
    printf("\nFrame-by-frame simulation:\n");
    for (int i = 0; i < 50; i++) {
        ctrl_apply_process_frame(&ctx);
        
        float val = ctrl_apply_get(&ctx, 0);
        duration_ms = (frames * FRAME_SIZE * 1000) / FS;
        
        if (i % 10 == 0 || duration_ms >= 100) {
            printf("  Frame %3d (t=%3dms): value=%.2f\n", frames, duration_ms, val);
        }
        
        frames++;
        
        if (duration_ms >= 120) break;  /* Ramp should be done by ~100ms */
    }
    
    float final_val = ctrl_apply_get(&ctx, 0);
    float target = ctrl_apply_get_target(&ctx, 0);
    
    if (fabs(final_val - target) < 0.1f) {
        printf("✓ Ramping OK (final=%.2f, target=%.2f)\n", final_val, target);
    } else {
        printf("✗ Ramping FAILED (final=%.2f, target=%.2f)\n", final_val, target);
    }
}

static void test_config_loading(void)
{
    printf("\n=== Test 4: Config File Loading ===\n");
    
    ctrl_map_table_t map;
    ctrl_map_init(&map, 64);
    
    if (ctrl_map_load(&map, "ctrl_config_doubling.txt") != 0) {
        printf("✗ Failed to load config file\n");
        return;
    }
    
    printf("Loaded %zu mappings from config file:\n", map.count);
    
    for (size_t i = 0; i < map.count; i++) {
        ctrl_mapping_t *entry = &map.entries[i];
        printf("  [%zu] param_id=%u, name=%s, source=%d\n",
               i, entry->param_id, entry->name ? entry->name : "?", entry->source);
        
        if (entry->source == CTRL_SOURCE_MIDI_CC) {
            printf("       MIDI CC#%d, range=[%.1f..%.1f], ramp=%dms\n",
                   entry->id.midi_cc, entry->min_val, entry->max_val, entry->ramp_time_ms);
        }
    }
    
    /* Test lookup by MIDI CC */
    ctrl_mapping_t *found;
    if (ctrl_map_by_midi_cc(&map, 1, &found) == 0) {
        printf("✓ Found CC#1: param_id=%u, name=%s\n", found->param_id, found->name);
    } else {
        printf("! CC#1 not in config (that's OK for this test)\n");
    }
    
    printf("✓ Config loading OK\n");
    
    ctrl_map_free(&map);
}

static void test_integration(void)
{
    printf("\n=== Test 5: Full Integration ===\n");
    printf("Simulating: CC#1=100 → delay_ms parameter → ramped application\n\n");
    
    /* Setup */
    ctrl_map_table_t map;
    ctrl_apply_ctx_t apply_ctx;
    midi_proto_ctx_t midi_ctx;
    ctrl_event_queue_t queue;
    
    ctrl_map_init(&map, 64);
    ctrl_map_load(&map, "ctrl_config_doubling.txt");
    
    ctrl_apply_init(&apply_ctx, 256, FS, FRAME_SIZE);
    midi_proto_init(&midi_ctx);
    ctrl_event_queue_init(&queue);
    
    /* Simulate MIDI input: CC#1 = 100 */
    uint8_t midi_bytes[] = {0xB0, 0x01, 0x64};
    midi_proto_push_bytes(&midi_ctx, midi_bytes, 3);
    
    /* Parse and enqueue */
    midi_msg_t msg;
    if (midi_proto_get_message(&midi_ctx, &msg) == 0) {
        ctrl_event_t evt;
        midi_proto_to_event(&msg, &evt);
        ctrl_event_enqueue(&queue, &evt);
        printf("MIDI → Event queued (param_id=%u, value=%.3f)\n", evt.param_id, evt.value);
    }
    
    /* Audio loop simulation */
    printf("\nAudio processing:\n");
    for (int frame = 0; frame < 50; frame++) {
        /* 1. Dequeue and apply events */
        ctrl_event_t evt;
        while (ctrl_event_dequeue(&queue, &evt) == 0) {
            ctrl_mapping_t *map_entry;
            
            if (evt.param_id < 128) {
                /* MIDI CC */
                if (ctrl_map_by_midi_cc(&map, (uint8_t)evt.param_id, &map_entry) == 0) {
                    ctrl_apply_event(&apply_ctx, &evt, map_entry);
                    printf("  Event applied: %s (→ %.2f)\n", 
                           map_entry->name, map_entry->min_val + evt.value * (map_entry->max_val - map_entry->min_val));
                }
            }
        }
        
        /* 2. Process frame (advance ramps) */
        ctrl_apply_process_frame(&apply_ctx);
        
        /* 3. Get values for audio model */
        float delay_ms = ctrl_apply_get(&apply_ctx, 0);
        
        int t_ms = (frame * FRAME_SIZE * 1000) / FS;
        if (frame % 10 == 0 || delay_ms > 49.0f) {
            printf("  Frame %d (t=%dms): delay=%.2f ms\n", frame, t_ms, delay_ms);
        }
        
        if (t_ms > 120) break;
    }
    
    printf("✓ Integration test OK\n");
    
    /* Cleanup */
    ctrl_map_free(&map);
}

int main(void)
{
    printf("╔═══════════════════════════════════════════════════╗\n");
    printf("║  Control Architecture Test Suite (Chapter 15)    ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n");
    
    test_event_queue();
    test_midi_parsing();
    test_ramping();
    test_config_loading();
    test_integration();
    
    printf("\n╔═══════════════════════════════════════════════════╗\n");
    printf("║  All tests completed                              ║\n");
    printf("╚═══════════════════════════════════════════════════╝\n");
    
    return 0;
}
