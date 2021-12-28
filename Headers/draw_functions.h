#ifndef _DRAW_FUNCTIONS_H_
#define _DRAW_FUNCTIONS_H_
#include "all_headers.h"

void draw_phase(PHASE phase, SRC media);
void draw_hero(HERO hero, SRC media);
void draw_enm1(ENEMY enm1, SRC media);
void draw_enm2(ENEMY enm2, SRC media);
void draw_patronum(Rectangle rec, SRC media);
void draw_fireplace(Rectangle fireplace, PHASE phase, SRC media);
void win_game(SRC media);
int sorting_hat(SRC media);
void letter(SRC media);
void hat(SRC media);
void draw_hat(SRC media);
void draw_owl(Vector2 pos_owl, SRC media);
void catch_letter(SRC media);
void next_phase(SRC media);
#endif // _DRAW_FUNCTIONS_H_
