#include <stdio.h>
#include <vector>
#include "daisysp.h"
#include "daisy_pod.h"
#include "mmm_common.h"
#include "pan.h"
#include "buffer.h"
#include "voice.h"

using namespace daisysp;
using namespace daisy;

float DSY_SDRAM_BSS buf[MAX_SIZE];
float DSY_SDRAM_BSS swap_buf[MAX_SIZE];
const int max = MAX_SIZE;
const int max_loop = MAX_SIZE_LOOP;

static DaisyPod pod;
static Pan pan;

int voice_amt;
int active_voice;

Buffer b;
//std::vector<Voice> Voices;
Voice Voices[2];
static void AudioCallback(float *in, float *out, size_t size);
void UpdateButtons();
void UpdateEncoder();
void Controls();

int main(void){
    pod.Init();
    pod.StartAdc();
    pod.StartAudio(AudioCallback);
    float samplerate = pod.AudioSampleRate();
    b.Init(buf, swap_buf, max);
    voice_amt = 2;

    for(int i=0;i<voice_amt;i++){
        //Voices.push_back(Voice());
        Voices[i].Init(samplerate, max_loop);
    }

    active_voice = 0;
    Voices[active_voice].active = true;

    while(1){}
}

static void AudioCallback(float *in, float *out, size_t size){
    Controls();
    float l1, l2, r1, r2;
    float outputL = 0, outputR = 0;

    for(size_t i = 0; i < size; i += 2){
        if(b.rec){
            b.Write(in[i]);
        }

        outputL = Voices[0].NextSample(b);
        outputR = Voices[1].NextSample(b);

        pan.Process(0.25, outputL, l1, r1);
        pan.Process(0.75, outputR, l2, r2);

        out[i] = tanh(l1+l2);
        out[i+1] = tanh(r1+r2);

        //out[i] = out[i+1] = in[i];
    }
}

void Controls(){
    pod.UpdateAnalogControls();
    pod.DebounceControls();

    //knob1 = rechts von den buttons
    float knob1 = pod.knob1.Process();
    float knob2 = pod.knob2.Process();

    int start, mod;
    start = clamp((int)scale(knob1, 0, 1, 0, MAX_SIZE_LOOP), 0, MAX_SIZE-1);
    mod = clamp((int)scale(knob2, 0, 1, 0, MAX_SIZE_LOOP/4), 0, MAX_SIZE-1);
    //mod1 = (int)scale(knob2, 0, 1, 0, MAX_SIZE_LOOP/4)+start;

    for(int i=0;i<voice_amt;i++){
        if(Voices[i].active){
            Voices[i].start = start;
            Voices[i].mod = mod;
        }
    }

    UpdateButtons();
    UpdateEncoder();

    pod.led1.Set(b.rec, b.play, 0);
    pod.UpdateLeds();
}

void UpdateButtons(){
    if(pod.button2.RisingEdge()){
        if(!b.rec){
            b.play = false;
            b.rec = true;
        }
    }
}

void UpdateEncoder(){
    if (pod.encoder.RisingEdge()){
        Voices[active_voice].active = false;
        active_voice++;
        active_voice %= voice_amt;
        Voices[active_voice].active = true;
    }

    int inc = pod.encoder.Increment();
    //this is dumb, either have active voice, or active flag
    //if we have active voice int we don't need to loop through
    //but can just access directly:
    //float temp = voices[active_voice].transpose + inc;
    //voices[active_voice].SetTransposition(CLAMP(temp, -12, 12));
    //but this is also dumb because we need the global variable active_voice
    //for this...

    Voices[0].SetTransposition(0);
    Voices[1].SetTransposition(-5);
    /*
    for(int i=0;i<voice_amt;i++){
        if(Voices[i].active){
            float temp = Voices[i].transpose + inc;
            Voices[i].SetTransposition(clamp(temp, -12, 12));
        }
    }
    */
}
