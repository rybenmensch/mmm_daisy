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

//make a scrubber only with offsets for the knobs instead of absolute positions?
int voice_amt;
int active_voice;
float prevs[2] = {0};
float prevm[2] = {0};
bool use_diff = false;

Buffer b;
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

    float diffs[2] = {0}, diffm[2] = {0};
    //difference stuff makes it less predictable but more prone to drop outs

    if(use_diff){
        for(int i=0;i<voice_amt;i++){
            diffs[i] = start - prevs[i];
            diffm[i] = mod - prevm[i];
            prevs[i] = start;
            prevm[i] = mod;

            if(Voices[i].active){
                Voices[i].start = start;
                Voices[i].mod = mod;
            }else{
                /*
                Voices[i].start = clamp(Voices[i].start+diffs[voice_amt-1], 0, MAX_SIZE-1);
                Voices[i].mod = clamp(Voices[i].mod+diffm[voice_amt-1], 0, MAX_SIZE-1);
                */
                Voices[i].start = clamp(Voices[i].start+diffs[voice_amt-1-i], 0, MAX_SIZE-1);
                Voices[i].mod = clamp(Voices[i].mod+diffm[voice_amt-1-i], 0, MAX_SIZE-1);
            }
        }
    }else{
        Voices[active_voice].start = start;
        Voices[active_voice].mod = mod;
    }

    /*
    Voices[active_voice].start = start;
    Voices[active_voice].mod = mod;
    */

    UpdateButtons();
    UpdateEncoder();

    pod.led1.Set(b.rec, b.play, 0);
    pod.led2.Set(0, 0, use_diff);
    pod.UpdateLeds();
}

void UpdateButtons(){
    if(pod.button2.RisingEdge()){
        if(!b.rec){
            b.play = false;
            b.rec = true;
        }
    }
    if(pod.button1.RisingEdge()){
        use_diff = !use_diff;
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

    float temp = Voices[active_voice].transpose + inc;
    Voices[0].SetTransposition(temp);
    Voices[1].SetTransposition(temp);

    //Voices[active_voice].SetTransposition(clamp(temp, -12, 12));
}
