#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>
#include <math.h>

#define NUM_SAMPLES (SAMPLE_RATE) // 0.1 seconds of samples
#define A_REF 1.0 // reference amplitude for calculating decibel value

int main() {
    int SAMPLE_RATE = 44100;
    int rc;
    int16_t buffer[NUM_SAMPLES];
    double rms = 0.0;
    double db = 0.0;
    snd_pcm_t *handle;
    snd_pcm_hw_params_t *params;
    snd_pcm_uframes_t frames;

    // Open default ALSA device for recording
    rc = snd_pcm_open(&handle, "default", SND_PCM_STREAM_CAPTURE, 0);
    if (rc < 0) {
        fprintf(stderr, "unable to open PCM device: %s\n", snd_strerror(rc));
        exit(1);
    }

    // Set hardware parameters for recording
    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(handle, params);
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(handle, params, 1);
    snd_pcm_hw_params_set_rate_near(handle, params, &SAMPLE_RATE, 0);
    snd_pcm_hw_params_set_period_size_near(handle, params, &frames, 0);
    rc = snd_pcm_hw_params(handle, params);
    if (rc < 0) {
        fprintf(stderr, "unable to set HW parameters: %s\n", snd_strerror(rc));
        exit(1);
    }

    // Start recording
    rc = snd_pcm_prepare(handle);
    if (rc < 0) {
        fprintf(stderr, "unable to prepare PCM device: %s\n", snd_strerror(rc));
        exit(1);
    }
    rc = snd_pcm_start(handle);
    if (rc < 0) {
        fprintf(stderr, "unable to start PCM device: %s\n", snd_strerror(rc));
        exit(1);
    }

    // Read samples from microphone and calculate decibel value
    rc = snd_pcm_readi(handle, buffer, NUM_SAMPLES);
    if (rc != NUM_SAMPLES) {
        fprintf(stderr, "short read from PCM device: %d\n", rc);
        exit(1);
    }
    for (int i = 0; i < NUM_SAMPLES; i++) {
        rms += pow(buffer[i], 2);
    }
    rms /= NUM_SAMPLES;
    rms = sqrt(rms);
    db = 20 * log10(rms / A_REF);

    printf("Decibel value: %f dB\n", db);

    // Cleanup
    snd_pcm_close(handle);
    return 0;
}
