#pragma once
#ifndef MMM_BUF_H
#define MMM_BUF_H
#ifdef __cplusplus

class Buffer{
    public:
        Buffer(){}
        ~Buffer(){}
        void Init(float *b_addr, float *sb_addr, int maxsize);
        void Reset();
        void Normalize();
        void CopyToSwap();
        void Write(float in);
        int reccount;
        int size;
        bool play;
        bool rec;
        float *buf;
        float *swap_buf;

    private:

};

#endif
#endif
