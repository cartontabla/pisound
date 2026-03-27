# Control System Architecture (Chapter 15)

## Overview

This directory contains the unified control subsystem for real-time audio effect parameters. It decouples **transport** (MIDI/OSC/network) from **protocol** (MIDI/OSC) from **application logic** (parameter mapping, ramping, safety).

```
┌─────────────────────────────────────────────────────────────┐
│ TRANSPORTS (I/O)                                            │
│  ├─ midi_io    (ALSA, USB, DIN, BLE)                       │
│  ├─ udp_io     (Ethernet/WiFi)                             │
│  └─ serial_io  (future)                                    │
└─────────────────────────────────────────────────────────────┘
                           ↓ raw bytes
┌─────────────────────────────────────────────────────────────┐
│ PROTOCOLS (Parsing)                                         │
│  ├─ midi_proto  (MIDI CC/Note/PitchBend)                  │
│  └─ osc_proto   (OSC paths)                                │
└─────────────────────────────────────────────────────────────┘
                           ↓ unified events
┌─────────────────────────────────────────────────────────────┐
│ APPLICATION (Mapping + Ramping)                             │
│  ├─ ctrl_event     (event queue)                            │
│  ├─ ctrl_map       (parameter mapping table)               │
│  └─ ctrl_apply     (ramping, click-free transitions)       │
└─────────────────────────────────────────────────────────────┘
                           ↓ smooth parameters
              ┌──────────────────────────┐
              │  Audio Model Parameters  │
              └──────────────────────────┘
```

## Core Components

### 1. `ctrl_event.h/c` - Unified Event Format

All control sources (MIDI, OSC, etc.) translate to a common internal event:

```c
typedef struct {
    uint32_t    param_id;          // Parameter identifier
    float       value;              // Normalized [0.0, 1.0]
    ctrl_mode_t mode;               // ABSOLUTE, RELATIVE, TOGGLE
    uint64_t    timestamp_us;       // Optional: for sequencing
} ctrl_event_t;
```

- Simple FIFO queue (256 events, thread-safe for sw/sr)
- Non-blocking enqueue/dequeue

### 2. `ctrl_map.h/c` - Parameter Mapping Table

Maps external control identifiers to audio parameters with ramping info:

```c
typedef struct {
    uint32_t        param_id;
    char           *name;
    ctrl_source_t   source;         // MIDI_CC, MIDI_NOTE, OSC
    union {
        uint8_t     midi_cc;        // For MIDI: CC number 0..119
        char       *osc_path;       // For OSC: "/path/to/param"
    } id;
    float           min_val, max_val;
    uint16_t        ramp_time_ms;   // Smoothing time
} ctrl_mapping_t;
```

**Config File Format** (ASCII, simple):
```
# Comments start with #
transport|source|min|max|ramp_ms|name
midi|cc:1|0|100|50|delay_ms
midi|cc:2|0|1|100|depth
osc|/doubling/bypass|0|1|0|bypass
```

Loading:
```c
ctrl_map_table_t map;
ctrl_map_init(&map, 64);
ctrl_map_load(&map, "ctrl_config_doubling.txt");
```

### 3. `ctrl_apply.h/c` - Click-Free Ramping

Smoothly transitions parameters over specified time with linear interpolation:

```c
ctrl_apply_ctx_t ctx;
ctrl_apply_init(&ctx, 256 params, 48000 Fs, 128 frame_size);

// Apply an event with automatic ramping
ctrl_apply_event(&ctx, &event, &mapping);

// Per audio frame: advance ramps
ctrl_apply_process_frame(&ctx);

// Get current value (interpolated during ramp)
float current = ctrl_apply_get(&ctx, param_id);
```

**Why ramping?**
- Event arrives at t=1.5ms of a 2.67ms frame
- Parameter change would cause DC step → audible click
- Solution: ramp over ramp_time_ms (e.g., 50ms)
- No clicking, natural feel

### 4. `midi_proto.h/c` - MIDI Parser

Converts raw MIDI bytes to unified events:

**Supported Messages:**
- **Control Change (CC)**: Continuous parameters (knobs)
- **Note On/Off**: Toggle/binary controls
- **Pitch Bend**: High-res parameter change

Features:
- Running status support
- Non-blocking push/pull interface
- Converts to `ctrl_event_t`

Example:
```c
midi_proto_ctx_t proto;
midi_proto_init(&proto);
midi_proto_push_bytes(&proto, midi_data, len);

midi_msg_t msg;
if (midi_proto_get_message(&proto, &msg) == 0) {
    ctrl_event_t evt;
    midi_proto_to_event(&msg, &evt);
}
```

### 5. `midi_io.h/c` - MIDI I/O (Platform-Specific)

Platform abstraction for MIDI ports:
- **Linux (Pi)**: ALSA sequencer client
- **macOS/Windows**: Stubs (TODO: PortAudio, WinMM)

```c
midi_io_ctx_t *ctx;
midi_io_init(&ctx, "default");  // or "128:0" (client:port)
midi_io_recv(ctx, buffer, maxlen);
midi_io_send(ctx, data, len);
```

## Integration with Audio Model

The audio thread calls `ctrl_apply_process_frame()` **once per audio frame**:

```c
// In custom_main.c audio callback:
void audio_step(void) {
    // 1. Apply pending control events
    ctrl_event_t evt;
    while (ctrl_event_dequeue(&g_event_queue, &evt) == 0) {
        ctrl_mapping_t *map;
        ctrl_map_by_midi_cc(&g_map_table, evt.param_id, &map);
        if (map) {
            ctrl_apply_event(&g_apply_ctx, &evt, map);
        }
    }
    
    // 2. Advance ramps for this frame
    ctrl_apply_process_frame(&g_apply_ctx);
    
    // 3. Get current parameter values
    float delay = ctrl_apply_get(&g_apply_ctx, 0);  // param_id=0
    float depth = ctrl_apply_get(&g_apply_ctx, 1);  // param_id=1
    
    // 4. Update model with smooth values
    model.delay_ms = delay;
    model.depth = depth;
    
    // 5. Step the model
    model_step(&model, input, output);
}
```

## Data Flow Example

**User turns knob on Oxygen 25:**

1. **MIDI**: Oxygen sends CC#1 = 100
2. **midi_io_recv()**: Reads bytes from ALSA
3. **midi_proto_push_bytes()**: Buffers raw MIDI
4. **midi_proto_get_message()**: Extracts CC message
5. **midi_proto_to_event()**: Creates event (param_id=1, value=100/127)
6. **ctrl_event_enqueue()**: Adds to queue
7. **Audio thread runs:**
   - Dequeue event → lookup mapping (cc:1 → param_id=1, ramp=50ms)
   - **ctrl_apply_event()**: Schedule 50ms ramp from old → new value
   - **ctrl_apply_process_frame()**: Each frame, interpolate ramp
   - **ctrl_apply_get()**: Return smooth value to model
   - **No clicks!** Parameter smoothly transitions

## Latency Characteristics

| Stage | Latency | Notes |
|-------|---------|-------|
| MIDI hardware → driver | 0-1 ms | Deterministic |
| midi_io_recv() | ~1-5 ms | Buffering in ALSA |
| midi_proto parse | <0.1 ms | Simple parsing |
| Event queue | ~0 ms | FIFO in RAM |
| ctrl_apply ramp | User-defined | Mini 0, typical 50ms |
| **Total** | **1-10ms** + **ramping** | Depends on audio frame |

For **critical live control** (bypass, etc.), use ramp_ms=0.

## Testing

See `test_ctrl_arch.c` for:
- Loading config file
- Simulating MIDI input
- Verifying ramping behavior
- Checking parameter interpolation

Run:
```bash
gcc -o test_ctrl test_ctrl_arch.c ctrl_*.c midi_*.c -lm
./test_ctrl
```

## Implemented

| Transport | Feature | Status |
|-----------|---------|--------|
| USB MIDI | MIDI CC via ALSA sequencer (Oxygen 25) | ✓ done |
| UDP/WiFi | OSC float over UDP (RX) — TouchOSC verified | ✓ done |
| Both | Multi-source per parameter (MIDI+OSC same slot) | ✓ done |
| Both | Click-free ramping | ✓ done |
| BLE MIDI | CME WIDI Core bridge script (`ble_midi_bridge.py`) | ⚠ partial |
| Any | OSC TX (Pi → controller feedback) | pending |
| Any | Presets / scene recall | pending |

### BLE MIDI status (CME WIDI Core + H4MIDI WC)

Hardware confirmed working at radio level:
- WIDI Core (`F0:8C:F3:8B:63:04`) visible and advertising (verified with `hcitool lescan`)
- LE link establishes momentarily (`btmgmt` reports `connected`)
- Bridge script `ble_midi_bridge.py` written and ready (uses BlueZ D-Bus + `snd-virmidi`)

Blocking issue — BlueZ bonding incompatibility:
- Kernel 6.6.20 on Patchbox OS does **not** include `snd-seq-bluetooth` module
- BlueZ 5.66 initiates bonding on every `Connect()` call; WIDI Core rejects it (status 0x0a)
- Workaround attempts exhausted: `bondable off`, `JustWorksRepairing`, `gatttool`, raw `hcitool lecc`, `btmgmt pair`
- WirePlumber auto-connect succeeds briefly but disconnects (no A2DP profile)

Resolution path: either kernel upgrade with `snd-seq-bluetooth`, or a BlueZ version that supports GATT connection without forced bonding.

## Future Extensions

### BLE MIDI (complete)
1. Upgrade kernel to one that includes `snd-seq-bluetooth` module, OR
2. Update BlueZ to handle GATT without bonding for trusted devices

### Bidirectional TX
- `control_process_tx()` hook exists — connect `ctrl_out_get()` to `osc_send_float()`
- Enables TouchOSC displays, LED feedback on controllers

### Presets
- Save/load parameter snapshots
- Scene recall via MIDI Program Change

## Files

```
common/control/
├── ctrl_event.h/c           # Unified event queue
├── ctrl_map.h/c             # Mapping table (MIDI CC + OSC paths)
├── ctrl_apply.h/c           # Click-free ramping
├── midi_proto.h/c           # MIDI byte-stream parser
├── midi_io.h/c              # MIDI I/O via ALSA sequencer
├── osc_proto.h/c            # OSC packet parser + sender
├── udp_io.h/c               # UDP RX bind + TX sendto
├── control.h/c              # Integration: init/rx/tx/shutdown
├── ctrl_config_doubling.txt # Config example (doubling project)
├── ble_midi_bridge.py       # BLE MIDI → ALSA bridge (snd-virmidi)
├── test_ctrl_arch.c         # Unit test
└── README.md                # This file
```

---

**Design Principle:** Separate transport, protocol, and application logic to enable reuse across channels and protocols while keeping the audio thread deterministic and click-free.
