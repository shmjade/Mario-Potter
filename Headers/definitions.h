#ifndef DEFINITIONS_H_INCLUDED
#define DEFINITIONS_H_INCLUDED

#include "all_headers.h"

///---- Defini��es do enunciado ----
#define OUR_FPS 60 //Velocidade de reprodu��o
#define SCREEN_WIDTH 1200 //Largura da tela
#define SCREEN_HEIGHT 700 //Altura da tela
#define GROUND_HEIGHT 65 //Altura do ch�o
#define MAX_POWER 3 //Quantidade m�xima de vezes para usar o bot�o Power durante o jogo

//Arquivo texto da fase:
#define TXT_LIN 29 //quantidade de linhas do arquivo texto
#define TXT_COL 120 //quantidade de colunas do arquivo texto
#define TXT_HEIGHT 25 //cada linha representa 25 pixels na vertical
#define TXT_WIDTH 10 //cada coluna representa 10 pixels na horizontal

//Pontua��o:
#define KNOCK_ENEMY 10 //Ao atingir o inimigo com um golpe inferior
#define KILL_ENEMY 800 //Ao destruir o inimigo
#define GET_COIN 800 //Ao coletar a moeda


///------- Nossas defini��es -------
//Casas de Hogwarts:
#define GRYFFINDOR 0
#define HUFFLEPUFF 1
#define RAVENCLAW 2
#define SLYTHERIN 3

//Quantidades m�ximas:
#define MAX_PLAYERS 5 //Quantidade m�xima de jogadores cadastrados:
#define MENU_OPTIONS 7
#define MAX_PLATS 500
#define MAX_TUBES 9
#define MAX_COINS 60
#define MAX_ENEMIES 10
#define JOURNEY_SIZE 3
#define GRAVITY 5
#define RANKING_SIZE 5

//Dire��es:
#define LEFT -1 //Sentido de deslocamento para a esquerda
#define RIGHT 1 //Sentido de deslocamento para a direita

//Jogador:
#define MAX_LIVES 5 //Limite m�ximo de vidas
#define MAX_CHAR_NAME 20 //Limite m�ximo de caracteres no nome
#define SPEED_HERO 8 //Velocidade do her�i

//Tempos para aparecer na tela:
#define TIME_COIN 20



//Teclas:
#define TECLA_SALVAR KEY_A //tecla que salva o jogo





//Inimigos:
/*
//tipos:
#define VOLD 1 //para o inimigo mais forte
#define DRACO 0 //para o inimigo mais fraco */
//status:
#define STRONG 1 //furioso
#define NORMAL 0 //invulner�vel
#define WEAK -1 //vulner�vel
//velocidade:
#define BOLT 15 //velocidade m�xima
#define RUN 10 //velocidade r�pida
#define WALK 5 //velocidade devagar


//Menu:
#define ABOUT_LINES 10


//Tamanhos:
#define HERO_HEIGHT 60
#define HERO_WIDTH 30
#define POWER_HEIGHT 40
#define POWER_WIDTH 40
#define COIN_HEIGHT 20


//Auxiliares:
#define TRUE 1
#define FALSE 0

#endif // DEFINITIONS_H_INCLUDED
