#pragma once
#ifndef MMM_VOICE_H
#define MMM_VOICE_H
#include "daisysp.h"
#include "buffer.h"
#include "mmm_common.h"
#ifdef __cplusplus

namespace daisysp{
class Voice{
    public:
        Voice(){};
        void Init(int sr, int m_l);
        void SetTransposition(float t);
        float NextSample(Buffer& bufref);
        int NextIndex();
        int mod;
        int pos;
        int start;
        int len;
        float gain;
        bool active;
        float transpose;
    private:
        PitchShifter ps;
};
}
#endif
#endif
