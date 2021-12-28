#ifndef _GAME_FUNCTIONS_H_
#define _GAME_FUNCTIONS_H_
#include "all_headers.h"

void play_new_game(SRC media);
int play_phase(PHASE *phase, SRC media, int *flag_save, char namePlayer[]);
void save_phase(PHASE phase, char namePlayer[]);
void update_phase(GAME *game, PHASE journey[]);
void save_game(int *flag_save, GAME game);
GAME* load_game(char *nameArq);
void checa_leitura();
void continue_game(SRC media);
void update_ranking(GAME game);
int is_between(RANKING ranking[RANKING_SIZE], GAME game);
void ranking_screen(SRC media);
int choose_who (SRC media);

#endif // _GAME_FUNCTIONS_H_
