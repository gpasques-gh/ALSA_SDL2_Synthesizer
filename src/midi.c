#include <alsa/asoundlib.h>

#include "synth.h"
#include "defs.h"
#include "midi.h"

/**
 * Getting the midi input from the given RawMidi input
 * Change the given 3osc synth oscillators frequency
 */
int get_midi(snd_rawmidi_t *midi_in, note_t *pnote, synth_3osc_t *synth) {

    unsigned char midi_buffer[1024];
    ssize_t ret = snd_rawmidi_read(midi_in, midi_buffer, sizeof(midi_buffer));
    
    if (ret < 0) {
        fprintf(stderr, "Bad return value while reading from midi input.\n");
        return 1;
    }

    for (int i = 0; i + 2 < ret; i += 3) {
        unsigned char status = midi_buffer[i];
        unsigned char data1 = midi_buffer[i+1]; // Note if key pressed, CC value if knob turned
        unsigned char data2 = midi_buffer[i+2]; // Velocity if key pressed, Knob turn value if knob turned

        if ((status & PRESSED) == NOTE_PRESSED && data2 > 0) {
            pnote->n_semitone = (data1 % 12);
            pnote->n_octave = (data1 / 12) - 1;
            note_to_sound(*pnote, synth->osc_a);
            note_to_sound(*pnote, synth->osc_b);
            note_to_sound(*pnote, synth->osc_c);
            synth->osc_b->s_freq += 1;
            synth->osc_c->s_freq -= 1;
        }

        if ((status & PRESSED) == KNOB_TURNED) {
            switch(data1) {
                case ARTURIA_ATT_KNOB:
                    synth->osc_a->s_adsr->att = (data2 / 127.0);
                    break;
                case ARTURIA_DEC_KNOB:
                    synth->osc_a->s_adsr->dec = (data2 / 127.0);
                    break;
                case ARTURIA_SUS_KNOB:
                    synth->osc_a->s_adsr->sus = (data2 / 127.0);
                    break;
                case ARTURIA_REL_KNOB:
                    synth->osc_a->s_adsr->dec = (data2 / 127.0);
                    break;
                default:
                    break;
            }
        }
    }

    return 0;
    
}