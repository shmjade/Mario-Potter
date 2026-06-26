#ifndef MENU_FUNCTIONS_H
#define MENU_FUNCTIONS_H

#include "all_headers.h"
#define MENU_OPTIONS 7
#define LINES 10

void after_all(SRC *media, int *flagEnd); //verifica se realmente quer sair
int menu(SRC media); //pega a op��o do menu
void delay(int time); //faz uma pequena pausa
float align_text_x(char text[], Font aFont, int sizeFont); //centraliza o texto na horizontal
void about_us(SRC *media); //cr�ditos do jogo
void prompt_exit_dialog(SRC media); //ESC anywhere: shows after_all and exits process if confirmed


#endif // MENU_FUNCTIONS_H
