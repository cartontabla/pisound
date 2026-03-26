# PiSound S-Functions Parameter Guide

## Overview

The PiSound S-Functions have been redesigned for 24-bit audio at 48kHz with a simplified parameter structure.

## pisound_in (Audio/MIDI Input)

### Parameters (4 total):

1. **Sample Rate** (double, default: 48000)
   - Audio sample rate in Hz
   - Recommended: 48000 (native PiSound rate)
   - Also supported: 44100, 96000

2. **Buffer Size** (double, default: 256)
   - Number of audio samples per processing block
   - Common values: 128, 256, 512, 1024
   - Smaller = lower latency, higher CPU usage
   - Larger = higher latency, lower CPU usage
   - **Note**: If Latency parameter is > 0, buffer size is calculated automatically

3. **Latency** (double, in milliseconds, default: 5.33)
   - Desired latency in milliseconds
   - If > 0, buffer size is calculated: `buffer = latency * sample_rate / 1000`
   - Set to 0 to use explicit Buffer Size parameter
   - Recommended: 5-10ms for real-time performance

4. **Backend Selection** (double, default: 0)
   - 0 = Auto (try JACK first, fallback to ALSA)
   - 1 = ALSA only
   - 2 = JACK only
   - **Recommended for Patchbox OS**: 0 (Auto)

### Example Configuration:

**For low latency:**
```
Sample Rate: 48000
Buffer Size: 128
Latency: 0 (use explicit buffer size)
Backend: 0 (Auto)
```

**For general use:**
```
Sample Rate: 48000
Buffer Size: 256
Latency: 5.33
Backend: 0 (Auto)
```

### Outputs (4):

1. **Audio Left**: Vector of int32 (24-bit audio data)
2. **Audio Right**: Vector of int32 (24-bit audio data)
3. **MIDI Bytes**: Vector of uint8 [128]
4. **MIDI Count**: Scalar uint32

---

## pisound_out (Audio/MIDI Output)

### Parameters (1 total):

1. **Backend Selection** (double, default: 0)
   - 0 = Auto (try JACK first, fallback to ALSA)
   - 1 = ALSA only
   - 2 = JACK only
   - **Must match pisound_in backend**

### Inputs (4):

1. **Audio Left**: Vector of int32 (24-bit audio data) - DYNAMICALLY SIZED
2. **Audio Right**: Vector of int32 (24-bit audio data) - DYNAMICALLY SIZED
3. **MIDI Bytes**: Vector of uint8 [128]
4. **MIDI Count**: Scalar uint32

### Key Features:

- **No sample rate or buffer size parameters** - automatically matches hardware configuration
- **Dynamic input sizing** - adapts to connected signal size
- **Queries hardware** - reads configuration from JACK/ALSA
- **Simpler** - just select the backend

---

##  24-bit Audio Format

###  Data Type: int32

In MATLAB/Simulink, audio is represented as **int32** (32-bit signed integer):
- Range: -2,147,483,648 to +2,147,483,647
- **Upper 24 bits contain the actual audio data**
- Dynamic range: 144 dB (theoretical)

### Hardware Format:

- **ALSA**: `S32_LE` (32-bit little-endian, 24-bit significant)
- **JACK**: Float (-1.0 to +1.0), converted from/to int32

### Conversion Examples:

**Simulink to Hardware:**
```
int32 value: 2,147,483,647 (max) → Hardware: Full scale positive
int32 value: 0              → Hardware: Silence
int32 value: -2,147,483,648 (min) → Hardware: Full scale negative
```

**Processing in Simulink:**
- Use int32 operations
- Full 24-bit precision maintained
- 48 dB more dynamic range than 16-bit

---

## Configuration Workflow

### Correct Order:

1. **Add pisound_in** to your Simulink model first
2. Configure its parameters (sample rate, buffer, latency, backend)
3. **Add pisound_out** to your model
4. Configure only its backend parameter (must match pisound_in)
5. Connect audio signals (int32 vectors) between blocks

### Why This Order Matters:

- `pisound_in` initializes the hardware (JACK/ALSA)
- `pisound_out` queries the hardware configuration
- If `pisound_out` initializes first, it may not find valid hardware config

---

## Latency vs Buffer Size

### Understanding the Relationship:

```
Latency (ms) = (Buffer Size / Sample Rate) × 1000
```

### Examples at 48kHz:

| Buffer Size | Latency | Use Case |
|-------------|---------|----------|
| 64 | 1.33 ms | Ultra-low latency (high CPU) |
| 128 | 2.67 ms | Low latency performance |
| 256 | 5.33 ms | Balanced (recommended) |
| 512 | 10.67 ms | Standard use |
| 1024 | 21.33 ms | High latency (low CPU) |

### Choosing Buffer Size:

**Use smaller buffers (64-128) when:**
- Playing live instruments through effects
- Real-time monitoring is critical
- Your Raspberry Pi can handle the CPU load

**Use larger buffers (512-1024) when:**
- Processing complex algorithms
- CPU usage is high
- Slight latency is acceptable

---

## Backend Selection Guide

### JACK (Recommended for Patchbox OS)

**Advantages:**
- Professional audio routing
- Lower latency
- Better for complex audio setups
- Integration with other audio software
- Automatic connection management

**When to use:**
- Running Patchbox OS
-需要 multiple audio apps running simultaneously
- Using Pure Data, SuperCollider, etc.

### ALSA

**Advantages:**
- Direct hardware access
- Simpler setup
- Works without JACK server

**When to use:**
- Standalone application
- No JACK server available
- Simplest possible configuration

### Auto Mode (Default)

**Best of both worlds:**
- Tries JACK first
- Falls back to ALSA if JACK unavailable
- Recommended for maximum compatibility

---

## Troubleshooting

### "pisound_out failed to initialize"
- **Cause**: pisound_in not initialized first
- **Solution**: Ensure pisound_in appears before pisound_out in model execution order

### "Backend mismatch"
- **Cause**: pisound_in and pisound_out using different backends
- **Solution**: Set both to same backend value (0, 1, or 2)

### "Buffer size mismatch"
- **Cause**: Signal dimensions don't match
- **Solution**: Ensure buffer sizes match between pisound_in output and pisound_out input

### "JACK connection failed"
- **Cause**: JACK server not running
- **Solution**: Start JACK or use Auto mode (will fallback to ALSA)

### "24-bit audio sounds wrong"
- **Cause**: Processing as float instead of int32
- **Solution**: Ensure all processing blocks use int32 data type

---

## Best Practices

1. **Always use Auto mode (0)** unless you have specific backend requirements
2. **Start with 256 buffer size** and adjust based on CPU/latency needs
3. **Use latency parameter** for easier tuning instead of calculating buffer size
4. **Match sample rates** - use 48000 Hz for PiSound
5. **Initialize in correct order** - pisound_in before pisound_out
6. **Check data types** - ensure int32 throughout audio path
7. **Monitor CPU usage** - reduce buffer size only if CPU allows

---

## Quick Reference

### Minimal Configuration (Most Common):

**pisound_in:**
```
48000, 256, 5.33, 0
```

**pisound_out:**
```
0
```

### Low Latency Configuration:

**pisound_in:**
```
48000, 128, 2.67, 0
```

**pisound_out:**
```
0
```

### High Quality Configuration:

**pisound_in:**
```
96000, 512, 5.33, 0
```

**pisound_out:**
```
0
```
