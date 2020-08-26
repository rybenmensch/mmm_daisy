#pragma once
#ifndef MMM_COMMON
#define MMM_COMMON
#ifdef __cplusplus

#include <math.h>
#include <cassert>

#define MAX_SIZE (48000 * 3)
#define MAX_SIZE_LOOP (48000 * 2)

namespace{
    inline float scale(float s, float a1, float a2, float b1, float b2){
        return b1 + ((s-a1)*(b2-b1)) / (a2-a1);
    }

    inline float atodb(float in) {
        return float((in <= 0.) ? -999. : (20. * log10(in)));
    }

    inline float dbtoa(float in) {
        return float(pow(10., in * 0.05));
    }
    //for some reason the template only works when we also define
    //a fct for floats only ???
    inline float clamp(float v, float lo, float hi){
        assert(!(hi<lo));
        return (v<lo) ? lo :(hi < v) ? hi : v;
    }

    template<class T>
    constexpr inline const T& clamp(const T& v, const T& lo, const T& hi){
        assert(!(hi<lo));
        return (v<lo) ? lo :(hi < v) ? hi : v;
    }
}
#endif
#endif /* end of include guard: MMM_COMMON */
