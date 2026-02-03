#define _GNU_SOURCE
#include <math.h>
#include "defs.h"
#include "synth.h"

double adsr_process(adsr_t *adsr)
{
    switch(adsr->state)
    {
        case ENV_IDLE:
            break;
        case ENV_ATTACK:
            adsr->output = adsr->output * *adsr->attack;
            if (adsr->output >= 1.0)
            {
                adsr->output = 1.0;
                adsr->state = ENV_DECAY;
            }
            break;
        case ENV_DECAY:
            adsr->output = adsr->output * *adsr->decay;
            if (adsr->output <= *adsr->sustain)
            {
                adsr->output = *adsr->sustain;
                adsr->state = ENV_SUSTAIN;
            }
            break;
        case ENV_SUSTAIN:
            break;
        case ENV_RELEASE:
            adsr->output = adsr->output * *adsr->release;
            if (adsr->output <= 0.0)
            {
                adsr->output = 0.0;
                adsr->state = ENV_IDLE;
            }
    }
    return adsr->output;
}

void render_sine(osc_t *osc, short *buffer, int amp)
{
    double phase_inc = osc->freq / RATE;
    for (int i = 0; i < FRAMES; i++)
    {
        double sample = sin(2.0 * M_PI * osc->phase);
        buffer[i] = (short)(sample * amp);
        osc->phase += phase_inc;
        if (osc->phase >= 1.0) osc->phase -= 1.0;
    }
}

void render_square(osc_t *osc, short *buffer, int amp)
{
    double phase_inc = osc->freq / RATE;
    for (int i = 0; i < FRAMES; i++)
    {
        double sample = (osc->phase < 0.5) ? 1.0 : -1.0;
        buffer[i] = (short)(sample * amp);
        osc->phase += phase_inc;
        if (osc->phase >= 1.0) osc->phase -= 1.0;
    }
}

void render_triangle(osc_t *osc, short *buffer, int amp)
{
    double phase_inc = osc->freq / RATE;
    for (int i = 0; i < FRAMES; i++)
    {
        double sample = 1.0 - 4.0 * fabs(osc->phase - 0.5);
        buffer[i] = (short)(sample * amp);
        osc->phase += phase_inc;
        if (osc->phase >= 1.0) osc->phase -= 1.0;
    }
}

void render_sawtooth(osc_t *osc, short *buffer, int amp)
{
    double phase_inc = osc->freq / RATE;
    for (int i = 0; i < FRAMES; i++)
    {
        double sample = 2.0 * osc->phase - 1.0;
        buffer[i] = (short)(sample * amp);
        osc->phase += phase_inc;
        if (osc->phase >= 1.0) osc->phase -= 1.0;
    }
}

void render_osc(osc_t *osc, short *buffer, int amp)
{
    switch(osc->wave)
    {
        case SINE_WAVE:
            render_sine(osc, buffer, amp);
            break;
        case SQUARE_WAVE:
            render_square(osc, buffer, amp);
            break;
        case TRIANGLE_WAVE:
            render_triangle(osc, buffer, amp);
            break;
        case SAWTOOTH_WAVE:
            render_sawtooth(osc, buffer, amp);
            break;
        default:
            memset(buffer, 0, FRAMES * sizeof(short));
            break;
    }
}

void render_voice(voice_t *voice, short *buffer, int amp)
{
    if (!voice->active)
    {
        memset(buffer, 0, FRAMES * sizeof(short));
        return;
    }

    short osc_buffers[3][FRAMES];
    for (int i = 0; i < 3; i++)
        render_osc(&voice->oscillators[i], osc_buffers[i], amp);

    for (int i = 0; i < FRAMES; i++)
    {
        double envelope = adsr_process(voice->adsr);
        double mixed = (double)osc_buffers[0][i] +
                       (double)osc_buffers[1][i] +
                       (double)osc_buffers[2][i];
        mixed /= 3.0;
        mixed *= envelope;
        mixed *= voice->velocity_amp;

        if (mixed > 32767.0) mixed = 32767.0;
        if (mixed < -32768.0) mixed = -32768.0;
        buffer[i] = (short)mixed;
    }
}

void render_synth(synth_t *synth, short *buffer, int n_voices)
{
    if (n_voices == 0)
    {
        memset(buffer, 0, FRAMES * sizeof(short));
        return;
    }

    short voices_buffers[VOICES][FRAMES];
    for (int i = 0; i < VOICES; i++)
        render_voice(&synth->voices[i], voices_buffers[i], synth->amp);

    for (int i = 0; i < FRAMES; i++)
    {
        double mixed = 0.0;
        for (int v = 0; v < VOICES; v++)
            mixed += voices_buffers[v][i];
        mixed /= 6;
        mixed = lp_process(synth->filter, mixed);
        if (mixed > 32767.0) mixed = 32767.0;
        if (mixed < -32768.0) mixed = -32768.0;
        buffer[i] = (short)mixed;
    }
}

void change_freq(voice_t *voice, int note, int velocity, double detune)
{
    int a4_diff = note - A4_POSITION;

    voice->note = note;
    voice->active = 1;
    voice->adsr->output = 0.001;
    voice->adsr->state = ENV_ATTACK;
    voice->velocity_amp = velocity / MIDI_MAX_VALUE;

    voice->oscillators[0].freq = A_4 * pow(2, a4_diff / 12.0);
    voice->oscillators[0].phase = 0.0;
    voice->oscillators[1].freq = A_4 * pow(2, a4_diff / 12.0);
    voice->oscillators[1].phase = 0.0;
    voice->oscillators[2].freq = A_4 * pow(2, a4_diff / 12.0);
    voice->oscillators[2].phase = 0.0;
}

const char *get_wave_name(int wave)
{
    switch(wave) 
    {
        case SINE_WAVE:
            return "Sine wave";
        case SQUARE_WAVE:
            return "Square wave";
        case TRIANGLE_WAVE:
            return "Triangle wave";
        case SAWTOOTH_WAVE:
            return "Sawtooth wave";
        default:
            return "Unknown wave";
    }
}

void lp_init(lp_filter_t *filter, float cutoff)
{
    float rc = 1.0f / (2.0f * M_PI * cutoff);
    float dt = 1.0f / RATE;
    filter->alpha = dt / (rc + dt);
    filter->prev = 0.0f;
    filter->cutoff = cutoff;
}

short lp_process(lp_filter_t *filter, short input)
{
    float x = (float)input;
    filter->prev = filter->prev + filter->alpha * (x - filter->prev);
    return (short)filter->prev;
}


voice_t *get_free_voice(synth_t *synth)
{
    for (int i = 0; i < VOICES; i++) 
        if (!synth->voices[i].active)
            return &synth->voices[i];
    return NULL;
}