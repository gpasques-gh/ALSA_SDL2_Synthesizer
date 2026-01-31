#include <ncurses.h>
#include <math.h>
#include <unistd.h>
#include <alsa/asoundlib.h>

#include "defs.h"
#include "synth.h"
#include "input.h"

int main() {

    initscr();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    int input = 0;

    int octave = DEFAULT_OCTAVE;
    note_t note = {.n_semitone = nC, .n_octave = octave, .n_duration = 5};
    
    adsr_t adsr = {
        .att = 0.0,   
        .dec = 0.3,    
        .sus = 0.0,    
        .rel = 0.1     
    };
    
    sound_t sound = {
        .s_adsr = adsr,
        .s_active = 0,
        .s_phase = 0.0,
        .s_frames_left = 0,
        .s_frames_total = 0,
        .s_wave = 1
    };

    snd_pcm_t *handle;
        
    if (snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0) {
        perror("snd_pcm_open");
        return 1;
    }

    int params_err = snd_pcm_set_params(handle, 
        SND_PCM_FORMAT_S16_LE,
        SND_PCM_ACCESS_RW_INTERLEAVED,
        1, RATE, 1, 500000);

    if (params_err < 0) {
        fprintf(stderr, "snd_pcm_set_params error: %s\n", snd_strerror(params_err));
        return 1;
    }

    snd_pcm_prepare(handle);

    short buffer[FRAMES];
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < FRAMES; j++) {
            buffer[j] = 0;
 
        }
        snd_pcm_writei(handle, buffer, FRAMES);
    }

    
    while (input != 'x') {
        clear();

        mvprintw(0, 0, "Semitone: %d", note.n_semitone);
        mvprintw(1, 0, "Octave: %d", note.n_octave);
        mvprintw(2, 0, "Frequency: %f", sound.s_freq);

        refresh();

        render_sound(&sound, buffer);
        
        int err = snd_pcm_writei(handle, buffer, FRAMES);
        if (err == -EPIPE) {
            snd_pcm_prepare(handle);
        } else if (err < 0) {
            snd_pcm_prepare(handle);
        }

        input = getch();
        get_input(input, &note, &sound);

        
    }

    snd_pcm_drain(handle);
    snd_pcm_close(handle);

    endwin();

    return 0;
}