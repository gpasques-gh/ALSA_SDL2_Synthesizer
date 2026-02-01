#ifndef MIDI_H
#define MIDI_H

#include <alsa/asoundlib.h>
#include "synth.h"

#define ARTURIA_ATT_KNOB 73
#define ARTURIA_DEC_KNOB 75
#define ARTURIA_REL_KNOB 79
#define ARTURIA_SUS_KNOB 72

#define PRESSED 0xF0
#define NOTE_PRESSED 0x90
#define KNOB_TURNED 0xB0

int get_midi(snd_rawmidi_t *midi_in, note_t *pnote, synth_3osc_t *synth);

#endif