#ifndef FUNC_LINMATHS
#define FUNC_LINMATHS


void trans_base(float *M, int scalex, int scaley, float posx, float posy);
void trans_translate(float *M, float changex, float changey);
void trans_scale(float *M, float scalex, float scaley);
void trans_rotate(float *M, float angle);


#endif
