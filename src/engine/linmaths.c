#include "linmaths.h"

#include <stdint.h>
#include <math.h>


static float _rsqrt(float num);


void trans_base(float *M, float scalex, float scaley, float posx, float posy) {
        M[ 0] = scalex;
        M[ 1] = 0.f;
        M[ 2] = 0.f;
        M[ 3] = 0.f;
        M[ 4] = 0.f;
        M[ 5] = scaley;
        M[ 6] = 0.f;
        M[ 7] = 0.f;
        M[ 8] = 0.f;
        M[ 9] = 0.f;
        M[10] = 0.f;
        M[11] = 0.f;
        M[12] = posx;
        M[13] = posy;
        M[14] = 0.f;
        M[15] = 1.f;
}


void trans_set_pos(float *M, float posx, float posy) {
    M[12] = posx;
    M[13] = posy;
}


static float _rsqrt(float num) {
    union{float f; uint32_t u;} temp = {num};
    temp.u = 0x5F375A86 - (temp.u >> 1);
    temp.f = temp.f * (1.5f - 0.5f * num * temp.f * temp.f);
    temp.f = temp.f * (1.5f - 0.5f * num * temp.f * temp.f);
    return temp.f * (1.5f - 0.5f * num * temp.f * temp.f);
}

void trans_set_scale(float *M, float scalex, float scaley) {
    scalex *= _rsqrt(M[0] * M[0] + M[1] * M[1]);
    scaley *= _rsqrt(M[4] * M[4] + M[5] * M[5]);
    M[0] *= scalex;
    M[1] *= scalex;
    M[4] *= scaley;
    M[5] *= scaley;
}


void trans_set_angle(float *M, float angle) {
    float scalex = _rsqrt(M[0] * M[0] + M[1] * M[1]);
    float scaley = _rsqrt(M[4] * M[4] + M[5] * M[5]);
    float Ca = cos(angle);
    float Sa = sin(angle);
    M[0] = scalex * Ca;
    M[1] = scalex * Sa;
    M[4] = -scaley * Sa;
    M[5] = scaley * Ca;
}


int trans_get_posx(float *M) {
    return M[12];
}


int trans_get_posy(float *M) {
    return M[13];
}


int trans_get_scalex(float *M) {
    return _rsqrt(M[0] * M[0] + M[1] * M[1]);
}


int trans_get_scaley(float *M) {
    return _rsqrt(M[4] * M[4] + M[5] * M[5]);
}


int trans_get_angle(float *M) {
    return atan2(M[1], M[0]);
}


void trans_translate(float *M, float changex, float changey) {
    M[12] += changex;
    M[13] += changey;
}


void trans_scale(float *M, float scalex, float scaley) {
    M[0] *= scalex;
    M[1] *= scalex;
    M[4] *= scaley;
    M[5] *= scaley;
}


void trans_rotate(float *M, float angle) {
    float Ca = cos(angle);
    float Sa = sin(angle);
    float temp = M[0];
    M[0] = temp * Ca + M[4] * Sa;
    M[4] = M[4] * Ca - temp * Sa;
    temp = M[1];
    M[1] = temp * Ca + M[5] * Sa;
    M[5] = M[5] * Ca - temp * Sa;
}
