#ifndef KEYBOARD_INPUT_H
#define KEYBOARD_INPUT_H

#include <SDL2/SDL.h>
#include "defs.h"
#include "synth.h"

// NOTE KEYS
#define kC SDLK_q
#define kC_SHARP SDLK_z
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

void handle_input(SDL_Event *event, note_t *note, synth_3osc_t *synth);

#endif