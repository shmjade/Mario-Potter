#ifndef _INTERACTIONS_H_
#define _INTERACTIONS_H_
#include "all_headers.h"

void heroXenemy(PHASE *phase, SRC media);
int hit_power(PHASE *phase, SRC media);
int knock_enemy1(Rectangle recCollision, PHASE *phase);
int knock_enemy2(Rectangle recCollision, PHASE *phase);
void enter_return(Rectangle *someone, PHASE *phase, int *direction);
int collision(Rectangle someone, PLATFORM plats[], int numPlats, Rectangle *collided);
int collisionRec(Rectangle someone, Rectangle recs[], int numRecs, Rectangle *collided);

#endif // _INTERACTIONS_H_
