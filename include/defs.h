#ifndef DEFS_H
#define DEFS_H

#include <SDL2/SDL.h>

// NOTES SEMITONES
#define nC 1
#define nC_SHARP 2
#define nD 3
#define nD_SHARP 4
#define nE 5
#define nF 6
#define nF_SHARP 7
#define nG 8
#define nG_SHARP 9
#define nA 10
#define nA_SHARP 11
#define nB 12

// KEYBOARD INPUT
#define QWERTY 0
#define AZERTY 1

#define kC_QWERTY SDLK_a
#define kC_AZERTY SDLK_q
#define kC_SHARP_QWERTY SDLK_w
#define kC_SHARP_AZERTY SDLK_z
#define kD SDLK_s
#define kD_SHARP SDLK_e
#define kE SDLK_d
#define kF SDLK_f
#define kF_SHARP SDLK_t
#define kG SDLK_g
#define kG_SHARP SDLK_y
#define kA SDLK_h
#define kA_SHARP SDLK_u
#define kB SDLK_j

#define OSC_A_WAVE_INCREMENT SDLK_1
#define OSC_B_WAVE_INCREMENT SDLK_2
#define OSC_C_WAVE_INCREMENT SDLK_3

#define DETUNE_INCREMENT SDLK_4
#define CUTOFF_INCREMENT SDLK_5

#define ATTACK_INCREMENT_QWERTY SDLK_z
#define ATTACK_INCREMENT_AZERTY SDLK_w
#define DECAY_INCREMENT SDLK_x
#define SUSTAIN_INCREMENT SDLK_c
#define RELEASE_INCREMENT SDLK_v

// MIDI
#define MIDI_MAX_VALUE 127.0

#define PRESSED 0xF0
#define NOTE_ON 0x90
#define NOTE_OFF 0x80
#define KNOB_TURNED 0xB0

// ADSR
#define ARTURIA_ATT_KNOB 73
#define ARTURIA_DEC_KNOB 75
#define ARTURIA_SUS_KNOB 79
#define ARTURIA_REL_KNOB 72

// WAVEFORMS
#define ARTURIA_OSC_A_KNOB 80
#define ARTURIA_OSC_B_KNOB 81
#define ARTURIA_OSC_C_KNOB 82
#define ARTURIA_OSC_D_KNOB 93
#define ARTURIA_OSC_E_KNOB 18
#define ARTURIA_OSC_F_KNOB 19

// EFFECTS
#define ARTURIA_CUTOFF_KNOB 74
#define ARTURIA_DETUNE_KNOB 71
#define ARTURIA_AMPLITUDE_KNOB 85

// NOTE RELATED DEFS
#define VOICES 6
#define DEFAULT_OCTAVE 4
#define A_4 440
#define A4_POSITION 58 // Position of A4 starting from C0

// WAVEFORMS
#define SINE_WAVE 0
#define SQUARE_WAVE 1
#define TRIANGLE_WAVE 2
#define SAWTOOTH_WAVE 3

// SOUND RELATED DEFS
#define FRAMES 512 // Length of the frame buffer
#define DEFAULT_AMPLITUDE 6000
#define MAX_AMPLITUDE DEFAULT_AMPLITUDE * 2
#define RATE 44100
#define LATENCY 30000

// INTERFACE
#define WIDTH 1200
#define HEIGHT 800

#endif