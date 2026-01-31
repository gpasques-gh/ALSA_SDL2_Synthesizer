#include <ncurses.h>

#include "defs.h"
#include "synth.h"

void get_input(int input, note_t *note, sound_t *sound) {

    if (input == KEY_UP) {
        note->n_octave++;
        return;
    }

    if (input == KEY_DOWN) {
        note->n_octave--;
        return;
    }

    switch(input) {
        case kC:
            change_note(note, nC, note->n_octave, note->n_duration);
            note_to_sound(*note, sound);
            break;
        case kC_SHARP:
            change_note(note, nC_SHARP, note->n_octave, note->n_duration);
            note_to_sound(*note, sound);
            break;
        case kD:
            change_note(note, nD, note->n_octave, note->n_duration);
            note_to_sound(*note, sound);
            break;
        case kD_SHARP:
            change_note(note, nD_SHARP, note->n_octave, note->n_duration);
            note_to_sound(*note, sound);
            break;
        case kE:
            change_note(note, nE, note->n_octave, note->n_duration);
            note_to_sound(*note, sound);
            break;
        case kF:
            change_note(note, nF, note->n_octave, note->n_duration);
            note_to_sound(*note, sound);
            break;
        case kF_SHARP:
            change_note(note, nF_SHARP, note->n_octave, note->n_duration);
            note_to_sound(*note, sound);
            break;
        case kG:
            change_note(note, nG, note->n_octave, note->n_duration);
            note_to_sound(*note, sound);
            break;
        case kG_SHARP:
            change_note(note, nG_SHARP, note->n_octave, note->n_duration);
            note_to_sound(*note, sound);
            break;
        case kA:
            change_note(note, nA, note->n_octave, note->n_duration);
            note_to_sound(*note, sound);
            break;
        case kA_SHARP:
            change_note(note, nA_SHARP, note->n_octave, note->n_duration);
            note_to_sound(*note, sound);
            break;
        case kB:
            change_note(note, nB, note->n_octave, note->n_duration);
            note_to_sound(*note, sound);
            break;
        default:
            break;
    }
    

}