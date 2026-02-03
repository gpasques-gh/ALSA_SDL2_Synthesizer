#include <alsa/asoundlib.h>

#include "defs.h"
#include "synth.h"
#include "midi.h"

int get_midi(snd_rawmidi_t *midi_in, synth_t *synth, int *n_voices, double *attack, double *decay, double *sustain, double *release)
{
    unsigned char midi_buffer[1024];
    ssize_t ret = snd_rawmidi_read(midi_in, midi_buffer, sizeof(midi_buffer));

    if (ret < 0)
        return 1;
    
    for (int i = 0; i + 2 < ret; i += 3)
    {
        unsigned char status = midi_buffer[i];
        unsigned char data1 = midi_buffer[i + 1];
        unsigned char data2 = midi_buffer[i + 2];

        if ((status & PRESSED) == NOTE_ON && data2 > 0)
        {
            voice_t *free_voice = get_free_voice(synth);
            if (free_voice == NULL) 
                continue;
            if (*n_voices < VOICES)
                (*n_voices)++;
            change_freq(free_voice, data1, data2, synth->detune);
        }
        else if ((status & PRESSED) == NOTE_OFF ||
                ((status & PRESSED)) == NOTE_ON && data2 == 0)
        {
            if (*n_voices >= 1)
            {
                int found = 0;
                for (int v = 0; v < VOICES; v++)
                {
                    if (synth->voices[v].note == data1 && synth->voices[v].active)
                    {
                        synth->voices[v].adsr->state = ENV_RELEASE;
                        found = 1;
                        break;
                    }
                }
                if (found && *n_voices > 0)
                    (*n_voices)--;
            }
        }
        else if ((status & PRESSED) == KNOB_TURNED)
        {
            switch (data1)
            {
                case ARTURIA_ATT_KNOB:
                    *attack = (double)data2 / MIDI_MAX_VALUE;
                    break;
                case ARTURIA_DEC_KNOB:
                    *decay = (double)data2 / MIDI_MAX_VALUE;
                    break;
                case ARTURIA_SUS_KNOB:
                    *sustain = (double)data2 / MIDI_MAX_VALUE;
                    break;
                case ARTURIA_REL_KNOB:
                    *release = (double)data2 / MIDI_MAX_VALUE;
                    break;
                case ARTURIA_OSC_A_KNOB:
                    for (int v = 0; v < VOICES; v++)
                        synth->voices[v].oscillators[0].wave = (int)((data2 * 4) / (MIDI_MAX_VALUE + 1));
                    break;
                case ARTURIA_OSC_B_KNOB:
                    for (int v = 0; v < VOICES; v++)
                        synth->voices[v].oscillators[1].wave = (int)((data2 * 4) / (MIDI_MAX_VALUE + 1));
                    break;
                case ARTURIA_OSC_C_KNOB:
                    for (int v = 0; v < VOICES; v++)
                        synth->voices[v].oscillators[2].wave = (int)((data2 * 4) / (MIDI_MAX_VALUE + 1));
                    break;
                case ARTURIA_CUTOFF_KNOB: {
                    double cutoff = ((double)data2 / MIDI_MAX_VALUE) * (RATE / 2);
                    lp_init(synth->filter, cutoff);
                    break;
                }
                case ARTURIA_DETUNE_KNOB:
                    synth->detune = ((double)data2 / MIDI_MAX_VALUE);
                    break;
                case ARTURIA_AMPLITUDE_KNOB:
                    synth->amp = (int)((data2 * MAX_AMPLITUDE)) / (MIDI_MAX_VALUE + 1);
                    break;
                default:
                    break;
            }
        }
    }

    return 0;
}