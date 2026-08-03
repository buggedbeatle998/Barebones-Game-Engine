#ifndef FUNC_LINMATHS
#define FUNC_LINMATHS


void trans_base(float *M, float scalex, float scaley, float posx, float posy);
void trans_set_pos(float *M, float posx, float posy);
void trans_set_scale(float *M, float scalex, float scaley);
void trans_set_angle(float *M, float angle);

int  trans_get_posx(float *M);
int  trans_get_posy(float *M);
int  trans_get_scalex(float *M);
int  trans_get_scaley(float *M);
int  trans_get_angle(float *M);

void trans_translate(float *M, float changex, float changey);
void trans_scale(float *M, float scalex, float scaley);
void trans_rotate(float *M, float angle);


#endif
