#pragma once
#ifndef DSY_PAN_H
#define DSY_PAN_H

#include <stdint.h>
#ifdef __cplusplus

namespace daisysp{
/** pan module
Original author(s) : Manolo Müller
Year : 2020
*/

class Pan{
  public:
    Pan() {}
    ~Pan() {}
    void Process(float pos, float in, float &outL, float &outR);
private:
    inline float tableval(int index);
};
} // namespace daisysp
#endif
#endif
