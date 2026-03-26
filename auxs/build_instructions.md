# Build Instructions for PiSound S-Functions (24-bit Audio)

## Prerequisites

When compiling the generated code on the Raspberry Pi with Patchbox OS:

### 1. Simulink Headers
The file `simstruc.h` and related Simulink headers must be available. These are typically generated/packaged by Simulink Coder. If not, copy them from your MATLAB installation (`<matlabroot>/simulink/include`) to the Pi.

### 2. Install Required Libraries

#### For JACK Support (Recommended for Patchbox OS)
```bash
sudo apt-get update
sudo apt-get install libjack-jackd2-dev libasound2-dev build-essential
```

#### For ALSA-Only Mode
```bash
sudo apt-get install libasound2-dev build-essential
```

## Compilation Options

### Option 1: JACK + ALSA Support (Default, Recommended for Patchbox OS)
Compile with both JACK and ALSA support. JACK will be tried first, with automatic fallback to ALSA:

```bash
gcc -o my_model my_model.c pisound_in.c pisound_out.c \
    -I. -I./include -ljack -lasound -lpthread -lm -lrt -O2
```

### Option 2: ALSA-Only Mode
Force ALSA-only compilation (no JACK support):

```bash
gcc -o my_model my_model.c pisound_in.c pisound_out.c \
    -I. -I./include -lasound -lpthread -lm -lrt -O2 -DUSE_ALSA_ONLY
```

### Compiling Individual S-Functions

If compiling S-Functions separately (for testing):

**With JACK support:**
```bash
gcc -c pisound_in.c -ljack -lasound -lpthread
gcc -c pisound_out.c -ljack -lasound -lpthread
```

**ALSA-only:**
```bash
gcc -c pisound_in.c -lasound -lpthread -DUSE_ALSA_ONLY
gcc -c pisound_out.c -lasound -lpthread -DUSE_ALSA_ONLY
```

## Usage

### S-Function Parameters (NEW - 24-bit version)

#### pisound_in (4 parameters):

1.  **Sample Rate** (double): Audio sample rate in Hz (default: 48000)
2.  **Buffer Size** (double): Number of samples per frame (default: 256)
3.  **Latency** (double): Desired latency in ms (default: 5.33)
    -   If > 0, buffer size is calculated automatically
    -   Set to 0 to use explicit Buffer Size parameter
4.  **Backend Selection** (double):
    -   `0` = Auto (Try JACK first, fallback to ALSA) - **Recommended for Patchbox OS**
    -   `1` = ALSA only
    -   `2` = JACK only

**Example in Simulink:**
-   Sample Rate: `48000`
-   Buffer Size: `256`
-   Latency: `5.33` (or `0` to use explicit buffer size)
-   Backend: `0` (Auto - JACK preferred)

#### pisound_out (1 parameter):

1.  **Backend Selection** (double):
    -   `0` = Auto (must match pisound_in)
    -   `1` = ALSA only
    -   `2` = JACK only

**Note:** pisound_out automatically matches the sample rate and buffer size from the hardware configuration set by pisound_in.

### 24-bit Audio Format

-   **Simulink Data Type**: `int32` (32-bit signed integer)
-   **Hardware Format**: 
    -   ALSA: S32_LE (24-bit significant data in 32-bit container)
    -   JACK: float (internally), converted from/to int32
-   **Dynamic Range**: 144 dB (theoretical)
-   **48 dB improvement** over 16-bit audio

### Important Order:

**pisound_in MUST be initialized BEFORE pisound_out** in your Simulink model. This ensures the hardware is configured before pisound_out attempts to use it.

### JACK Configuration on Patchbox OS

If using JACK, make sure the JACK server is running:

```bash
# Check if JACK is running
jack_wait -c

# Start JACK manually (if needed)
jackd -d alsa -dhw:pisound -r48000 -p256 -n2 &
```

On Patchbox OS, JACK is usually already running and managed by the system.

### Connecting JACK Ports

After starting your Simulink model with JACK backend, the following ports will be available:

**Input S-Function (`pisound_in`):**
-   `pisound_simulink_in:input_L`
-   `pisound_simulink_in:input_R`
-   `pisound_simulink_in:midi_in`

**Output S-Function (`pisound_out`):**
-   `pisound_simulink_out:output_L`
-   `pisound_simulink_out:output_R`
-   `pisound_simulink_out:midi_out`

Connect these ports to your PiSound hardware using a JACK connection manager:

```bash
# List all JACK ports
jack_lsp

# Connect to PiSound capture (example)
jack_connect system:capture_1 pisound_simulink_in:input_L
jack_connect system:capture_2 pisound_simulink_in:input_R

# Connect to PiSound playback (example)
jack_connect pisound_simulink_out:output_L system:playback_1
jack_connect pisound_simulink_out:output_R system:playback_2
```

Or use a GUI tool like `qjackctl` for easier connection management.

## Troubleshooting

### "Failed to open JACK client"
-   Ensure JACK server is running: `jack_wait -c`
-   Check JACK is using the correct device: `jack_lsp -c`
-   If JACK fails, the S-Function will automatically fall back to ALSA (if backend=0)

### "ALSA Error PCM Open"
-   Check if PiSound is detected: `aplay -l` and `arecord -l`
-   Verify device name: Should show `pisound` in the list
-   Ensure no other application is using the audio device

### "pisound_out initialization failed"
-   **Cause**: pisound_in not initialized before pisound_out
-   **Solution**: Ensure pisound_in appears first in Simulink execution order

### "Buffer size mismatch"
-   **Cause**: Signal dimensions don't match between pisound_in and pisound_out
-   **Solution**: Verify buffer sizes match (pisound_out uses dynamic sizing, should adapt automatically)

### "24-bit audio sounds distorted"
-   **Cause**: Processing blocks expecting different data type
-   **Solution**: Ensure all audio processing uses int32 data type throughout

### JACK and ALSA conflict
-   Don't run ALSA and JACK backends simultaneously
-   If JACK is running, it controls the hardware
-   Use backend parameter `2` to force JACK only, or `1` to force ALSA only

### Runtime Messages
The S-Functions print diagnostic messages to stderr:
-   `"JACK client opened successfully (24-bit mode)"` - JACK connection established
-   `"Using ALSA backend (24-bit S32_LE format)"` - Fell back to ALSA
-   `"ERROR: Failed to initialize any audio backend"` - Both backends failed
-   `"Calculated buffer size from latency X.XXms: XXX samples"` - Latency-based buffer calculation

### Best Practices for Patchbox OS
1.  Use backend parameter `0` (Auto) for maximum flexibility
2.  Set sample rate to 48000 Hz (native PiSound rate)
3.  Use latency parameter (5-10ms) instead of manual buffer size selection
4.  Ensure JACK is properly configured with matching sample rate
5.  Use `qjackctl` or command-line tools to verify connections
6.  Monitor CPU usage - JACK is generally more efficient than ALSA for low-latency audio
7.  **Always initialize pisound_in before pisound_out** in your Simulink model

## Quick Configuration Examples

### Recommended (Balanced):
```
pisound_in: 48000, 256, 5.33, 0
pisound_out: 0
```

### Low Latency:
```
pisound_in: 48000, 128, 2.67, 0
pisound_out: 0
```

### High Quality (96kHz):
```
pisound_in: 96000, 512, 5.33, 0
pisound_out: 0
```