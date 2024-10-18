#pragma once
#include <iostream>
#include "SDLInc.h"

#define SAMPLE_RATE 4096

class Beep {
public:
    int amplitude = 28000;
    int freq = 440;
    float m1 = 1;
    float m2 = 1;
    float m3 = 1;
    SDL_AudioDeviceID dev;
    Beep() {}
    ~Beep() {
        stop();
    }
    static void audio_callback(void* userdata, Uint8* stream, int len) {
        Beep* beep = (Beep*)userdata;
        int sampleCount = len / 2;
        Sint16* stream16 = (Sint16*)stream;
        for (int i = 0; i < sampleCount; i++) {
            Sint16 sample1 = beep->amplitude * beep->m1 * sin(2 * M_PI * beep->freq * i / SAMPLE_RATE);
            Sint16 sample2 = beep->amplitude * beep->m2 * (sin(2 * M_PI * beep->freq / 8 * i / SAMPLE_RATE) > 0 ? 0.75f : -0.75f); 
            Sint16 sample3 = beep->amplitude * beep->m3 * sin(2 * M_PI * beep->freq * 8 * i / SAMPLE_RATE); 
            Sint16 mix = (sample1 + sample2 + sample3) / 3;
            // mono boost
            if (beep->m1 + beep->m2 + beep->m3 <= 1) {
                mix *= 2.5f;
            }
            // clip
            if (mix > beep->amplitude) mix = beep->amplitude;
            stream16[i] = mix;
        }
    }
    void setVolume(uint8_t v) {
        amplitude = 28000 * v / 100;
    }
    void play(bool once = true) {
        if (dev && once) return;
        // return;
        SDL_AudioSpec want, have;
        SDL_memset(&want, 0, sizeof(want));
        want.freq = SAMPLE_RATE;
        want.format = AUDIO_S16SYS;
        want.channels = 1;
        want.samples = 16;
        want.callback = audio_callback;
        want.userdata = this;
        dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
        SDL_PauseAudioDevice(dev, 0);
    }
    void stop() {
        // DBG("Stopping audio");
        SDL_PauseAudioDevice(dev, 1);
        SDL_CloseAudioDevice(dev);
        dev = 0;
    }
};