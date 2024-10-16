#pragma once
#include <iostream>
#include "SDLInc.h"

#define SAMPLE_RATE 4096

class Beep {
public:
    int amplitude = 28000;
    int freq = 440;
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
           stream16[i] = beep->amplitude * sin(2 * M_PI * beep->freq * i / SAMPLE_RATE);
        }
        // std::cout << beep->freq << std::endl;
    }
    void setVolume(uint8_t v) {
        amplitude = 28000 * v / 100;
    }
    void play() {
        // return;
        SDL_AudioSpec want, have;
        SDL_memset(&want, 0, sizeof(want));
        want.freq = SAMPLE_RATE;
        want.format = AUDIO_S16SYS;
        want.channels = 1;
        // want.samples = 44000 / 16;
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
    }
};