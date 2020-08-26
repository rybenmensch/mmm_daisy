#include "Buffer.h"

void Buffer::Init(float *b_addr, float *sb_addr, int maxsize){
    buf = b_addr;
    swap_buf = sb_addr;
    reccount = 0;
    play = false;
    rec = false;
    size = maxsize;
    Reset();
}

void Buffer::Reset(){
    for(int i=0;i<size;i++){
        buf[i] = swap_buf[i] = 0;
    }
}

void Buffer::Normalize(){
    float biggest = 0;
    for(int i=0;i<size;i++){
        float temp = (biggest>buf[i])?biggest:buf[i];
        biggest = temp;
    }

    float gainf = 1/biggest;
    for(int i=0;i<size;i++){
        buf[i] *= gainf;
    }
}

void Buffer::CopyToSwap(){
    for(int i=0;i<size;i++){
        swap_buf[i] = buf[i];
    }
}

void Buffer::Write(float in){
    if(reccount<size){
        buf[reccount++] = in;
    }else{
        rec = false;
        play = true;
        reccount = 0;
        Normalize();
        CopyToSwap();
    }
}
