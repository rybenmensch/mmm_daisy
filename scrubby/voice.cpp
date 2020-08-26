#include "voice.h"

using namespace daisysp;

void Voice::Init(int sr, int m_l){
    mod         = m_l;
    pos         = 0;
    start       = 0;
    len         = m_l/4;
    gain        = 1.0f;
    active      = false;
    transpose   = 0.0f;
    ps.Init(sr);
    ps.SetTransposition(0.f);
}

void Voice::SetTransposition(float t){
    transpose = t;
    ps.SetTransposition(transpose);
}

float Voice::NextSample(Buffer& bufref){
    //int index = CLAMP(NextIndex(), 0, MAX_SIZE);
    int index = clamp(NextIndex(), 0, MAX_SIZE);
    float temp = 0.f;
    if(bufref.rec){
        temp = bufref.swap_buf[index];
    }else{
        temp = bufref.buf[index];
    }

    return ps.Process(temp) * gain;
}

int Voice::NextIndex(){
    ++pos %= mod;
    pos = (pos)?pos:start;
    return pos;
}
