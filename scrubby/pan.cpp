#include "pan.h"
#include <math.h>
#include <algorithm>

using namespace daisysp;

void Pan::Process(float pos, float in, float &outL, float &outR){
    int index = pos * 1023;
    int inv_index = 1023 - index;
    outL = Pan::tableval(index) * in;
    outR = Pan::tableval(inv_index) * in;
}

inline float Pan::tableval(int index){
    return cos((M_PI/2) * (float)index/1023);
}
