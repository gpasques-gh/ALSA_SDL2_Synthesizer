#ifndef DEFS_H
#define DEFS_H

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

// NOTE RELATED DEFS
#define DEFAULT_OCTAVE 4
#define A_4 440
#define A4_POSITION 58 // Position of A4 starting from C0

// KEYBOARD (AZERTY, change q to a and z to w for QWERTY)
#define kC 'q'
#define kC_SHARP 'z'
#define kD 's'
#define kD_SHARP 'e'
#define kE 'd'
#define kF 'f'
#define kF_SHARP 't'
#define kG 'g'
#define kG_SHARP 'y'
#define kA 'h'
#define kA_SHARP 'u'
#define kB 'j'

// SOUND RELATED DEFS
#define FRAMES 512 // Length of the frame buffer
#define AMPLITUDE 1500
#define RATE 44100

// SOUND WAVES
#define SINE_WAVE 0
#define SQUARE_WAVE 1
#define TRIANGLE_WAVE 2
#define SAWTOOTH_WAVE 3

// TEXT USER INTERFACE
#define WIDTH 300
#define HEIGHT 300
#define FILE_INTERFACE "interface/interface.txt"

#endif 