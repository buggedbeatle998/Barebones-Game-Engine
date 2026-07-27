#include "linmaths.h"

#include <math.h>

#include "graphics.h"


void trans_base(float *M, int scalex, int scaley, float posx, float posy) {
        M[ 0] = 1.f / tex_w * scalex;
        M[ 1] = 0.f;
        M[ 2] = 0.f;
        M[ 3] = 0.f;
        M[ 4] = 0.f;
        M[ 5] = -1.f / tex_h * scaley;
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
