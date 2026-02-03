#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SDL2/SDL.h>
#include "synth.h"

void handle_input(SDL_Keycode key, synth_t *synth, int layout, int *n_voices, int *octave);
void handle_release(SDL_Keycode key, synth_t *synth, int layout, int *n_voices, int octave);
int key_to_note(SDL_Keycode key, int layout, int octave);

#endif