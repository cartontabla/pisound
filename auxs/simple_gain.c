/*
 * simple_gain.c - Simple gain audio effect for PiSound
 *
 * This is a standalone program that reads audio from PiSound input,
 * applies a simple gain (volume) effect, and sends it to PiSound output.
 *
 * Features:
 * - 24-bit audio processing (int32)
 * - Stereo input/output
 * - Adjustable gain parameter
 * - Real-time processing with JACK or ALSA
 *
 * Build on Raspberry Pi:
 *   gcc -o simple_gain simple_gain.c pisound_in.c pisound_out.c \
 *       -ljack -lasound -lpthread -lm -lrt -O2
 *
 * Usage:
 *   ./simple_gain [gain_db]
 *   Example: ./simple_gain 6.0    (amplify by +6dB)
 *            ./simple_gain -3.0   (attenuate by -3dB)
 */

#include <math.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Audio parameters */
#define SAMPLE_RATE 48000
#define BUFFER_SIZE 256
#define LATENCY_MS 5.33
#define BACKEND_AUTO 0

/* MIDI buffer (not used in this effect) */
#define MIDI_BUF_SIZE 128

/* Global flag for graceful shutdown */
static volatile int running = 1;

void signal_handler(int sig) {
  printf("\nShutting down...\n");
  running = 0;
}

/* Convert dB to linear gain */
float db_to_linear(float db) { return powf(10.0f, db / 20.0f); }

/* Apply gain to audio buffer with clipping protection */
void apply_gain(int32_t *buffer_l, int32_t *buffer_r, int size,
                float gain_linear) {
  for (int i = 0; i < size; i++) {
    /* Convert to float for processing */
    float sample_l = (float)buffer_l[i];
    float sample_r = (float)buffer_r[i];

    /* Apply gain */
    sample_l *= gain_linear;
    sample_r *= gain_linear;

    /* Clip to int32 range to prevent overflow */
    if (sample_l > 2147483647.0f)
      sample_l = 2147483647.0f;
    if (sample_l < -2147483648.0f)
      sample_l = -2147483648.0f;
    if (sample_r > 2147483647.0f)
      sample_r = 2147483647.0f;
    if (sample_r < -2147483648.0f)
      sample_r = -2147483648.0f;

    /* Convert back to int32 */
    buffer_l[i] = (int32_t)sample_l;
    buffer_r[i] = (int32_t)sample_r;
  }
}

int main(int argc, char *argv[]) {
  /* Default gain: 0 dB (unity gain, no change) */
  float gain_db = 0.0f;

  /* Parse command line argument */
  if (argc > 1) {
    gain_db = atof(argv[1]);
    printf("Gain: %.2f dB\n", gain_db);
  } else {
    printf("Gain: 0.0 dB (unity gain)\n");
    printf("Usage: %s [gain_db]\n", argv[0]);
  }

  /* Convert to linear gain */
  float gain_linear = db_to_linear(gain_db);
  printf("Linear gain: %.4f\n", gain_linear);

  /* Audio buffers */
  int32_t *audio_in_l = (int32_t *)malloc(BUFFER_SIZE * sizeof(int32_t));
  int32_t *audio_in_r = (int32_t *)malloc(BUFFER_SIZE * sizeof(int32_t));
  int32_t *audio_out_l = (int32_t *)malloc(BUFFER_SIZE * sizeof(int32_t));
  int32_t *audio_out_r = (int32_t *)malloc(BUFFER_SIZE * sizeof(int32_t));

  /* MIDI buffers (pass-through, not processed) */
  uint8_t midi_in[MIDI_BUF_SIZE];
  uint8_t midi_out[MIDI_BUF_SIZE];
  uint32_t midi_count = 0;

  if (!audio_in_l || !audio_in_r || !audio_out_l || !audio_out_r) {
    fprintf(stderr, "Error: Failed to allocate audio buffers\n");
    return 1;
  }

  /* Initialize buffers */
  memset(audio_in_l, 0, BUFFER_SIZE * sizeof(int32_t));
  memset(audio_in_r, 0, BUFFER_SIZE * sizeof(int32_t));
  memset(midi_in, 0, MIDI_BUF_SIZE);

  /* Setup signal handler for graceful shutdown */
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  printf("\n=== Simple Gain Effect ===\n");
  printf("Sample Rate: %d Hz\n", SAMPLE_RATE);
  printf("Buffer Size: %d samples\n", BUFFER_SIZE);
  printf("Latency: %.2f ms\n", LATENCY_MS);
  printf("\nPress Ctrl+C to stop\n\n");

  /* NOTE: This is a simplified example
   * In actual implementation with PiSound S-Functions:
   * 1. Initialize pisound_in and pisound_out
   * 2. Call their mdlOutputs() functions each iteration
   * 3. Process audio between input and output
   *
   * For now, this demonstrates the DSP processing logic
   */

  int frame_count = 0;

  while (running) {
    /*
     * IN REAL IMPLEMENTATION:
     *
     * 1. Call pisound_in mdlOutputs to get audio:
     *    - audio_in_l = pisound_in output port 0
     *    - audio_in_r = pisound_in output port 1
     *    - midi_in = pisound_in output port 2
     *    - midi_count = pisound_in output port 3
     */

    /* Simulate input (would come from pisound_in) */
    /* In real version, this is replaced by actual audio input */

    /* 2. PROCESS AUDIO - Apply gain effect */
    memcpy(audio_out_l, audio_in_l, BUFFER_SIZE * sizeof(int32_t));
    memcpy(audio_out_r, audio_in_r, BUFFER_SIZE * sizeof(int32_t));
    apply_gain(audio_out_l, audio_out_r, BUFFER_SIZE, gain_linear);

    /* 3. Pass through MIDI unchanged */
    memcpy(midi_out, midi_in, MIDI_BUF_SIZE);

    /*
     * 4. Call pisound_out mdlOutputs to send audio:
     *    - pisound_out input port 0 = audio_out_l
     *    - pisound_out input port 1 = audio_out_r
     *    - pisound_out input port 2 = midi_out
     *    - pisound_out input port 3 = midi_count
     */

    /* Status update every 1000 frames */
    frame_count++;
    if (frame_count % 1000 == 0) {
      printf("Processing... %d frames\r", frame_count);
      fflush(stdout);
    }

    /* Sleep to simulate real-time (remove when using actual S-Functions) */
    usleep((BUFFER_SIZE * 1000000) / SAMPLE_RATE);
  }

  printf("\nProcessed %d frames total\n", frame_count);

  /* Cleanup */
  free(audio_in_l);
  free(audio_in_r);
  free(audio_out_l);
  free(audio_out_r);

  printf("Shutdown complete\n");
  return 0;
}
