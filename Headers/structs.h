#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include "all_headers.h"
#include "definitions.h"

//Herói:
typedef struct{
    int who; //HERMIONE, HARRY, RONY, NEVILLE
    int speedX; //Podemos escolher
    int direction; //Direção: LEFT ou RIGHT
    Rectangle rec; //guarda o tamanho e a posição
    int jump; //Se pode pular ou não: TRUE ou FALSE
    int goingUp; //Se pode pular ou não: TRUE ou FALSE
    int falling; //Se está caindo ou não: TRUE ou FALSE
    int stop; //Se está parado ou não: TRUE ou FALSE
    int left; //se o herói pode ir para a esquerda
    int right; //se o herói pode ir para a direita
    int lives; //vidas
    int points; //pontuação
    int afterDeath; //indica se o herói está no período após morrer
}HERO;


//Inimigos:
typedef struct{
    int alive; //se está vivo (1) ou morto (0)
    int level; //DRACO ou VOLDEMORT
    int status; //FURIOUS, NORMAL, WEAK ou DEAD
    int speedX; //Podemos escolher
    int direction; //Direção: LEFT ou RIGHT
    Rectangle rec; //guarda o tamanho e a posição
}ENEMY;



//Recursos a serem carregados
typedef struct{
    // Fontes
    Font fonteHP; //Fonte personalizada de Harry Potter

    // Som
    Sound sound_start; //Som ao iniciar jogo
    Sound sound_menu; //Som de seleção do menu
    Sound sound_jump; //Som ao pular
    Sound sound_turn; //Som ao trocar de direção
    Sound sound_coin; //Som ao coletar moeda
    Sound sound_kill; //Som ao eliminar inimigo
    Sound heavy_spell; //som para apagar
    Sound fast_spell; //som
    Sound expelliarmus; //som
    Sound harryDied; //som
    Sound expectro; //feitiço patrono
    Sound imHermione; //voz Hermione
    Sound imHarry; //voz Harry
    Sound imRony; //voz Rony
    Sound imNeville; //dizendo Neville
    Sound mischief; //dizendo Neville

    // Música de fundo:
    Music HPthemeSong; //Música tema de Harry Potter
    Music afterAllthisTime; //Som Dumbledore e Snape
    Music aboutUs; //Sobre nós
    Music names; //Personagens falando o nome

    //Textures:
    Texture2D harryRight; //sprite Harry
    Texture2D harryLeft;  //sprite Harry
    Texture2D harryFront; //sprite Harry
    Texture2D dracoRight; //sprite Draco
    Texture2D dracoFront; //sprite Draco
    Texture2D fireplace;  //cano/retorno
    Texture2D fireplaceGreen;  //cano/retorno
    Texture2D enemies;  //inimigos
    Texture2D who;
    Rectangle recFrameWho;
    Texture2D broom;  //inimigos
    Rectangle recFrameEnemies;
    Texture2D ground;     //chão
    Texture2D patronumEffect;     //feitiço
    Rectangle recFramePatronum;
    Rectangle recFireplace;
    Texture2D houses;
    Rectangle recFrameHouses;
    Texture2D hermione;
    Rectangle recFrameHermione;
    Texture2D harry;
    Rectangle recFrameHarry;
    Texture2D rony;
    Rectangle recFrameRony;
    Texture2D neville;
    Rectangle recFrameNeville;
    Texture2D podium;
    Texture2D letter;
    Texture2D gryffindor;
    Texture2D ravenclaw;
    Texture2D slytherin;
    Texture2D hufflepuff;
    Texture2D sortingHat;
    Texture2D owl;
    Rectangle recFrameOwl;
    Rectangle recFrameHat;
    Texture2D envelope;
    Texture2D express;
    Texture2D marauders;
    Music hat_song;
    Sound nextPhase;
}SRC;



//Plataformas:
typedef struct{
    Rectangle rec; //posição, tamanho e altura
    int status; //Atingida (1) ou normal (0)
}PLATFORM;

//Moedas:
typedef struct{
    int status; //1 ativo ou 0 inativo
    int speedX; //Velocidade
    int direction; //LEFT ou RIGHT
    Rectangle coinRec;
}COIN;

//Fase
typedef struct{
    int level; //Nível do jogo (aumenta ao passar de fase)
    Rectangle recPower; //posição do botão Power
    HERO hero; //herói
    int numEnem1; //quantidade de inimigos do tipo 1
    int numEnem2; //quantidade de inimigos do tipo 2
    int delayEnemy; //tempo de espera entre a liberação dos inimigos
    int numPlats; //quantidade de plataformas da fase
    int numTubes; //quantidade de canos da fase
    PLATFORM plats[MAX_PLATS]; //vetor de plataformas
    Rectangle tubes[MAX_TUBES]; //vetor de canos
    Rectangle returns[MAX_TUBES]; //vetor de retornos
    ENEMY enemies1[MAX_ENEMIES]; //vetor de inimigos do tipo 1
    ENEMY enemies2[MAX_ENEMIES]; //vetor de inimigos do tipo 2
    int activeEnemies1; //quantidade de inimigos do tipo 1 ativados
    int activeEnemies2; //quantidade de inimigos do tipo 2 ativados
    COIN coins[MAX_COINS]; //vetor de moedas
    int numCoins; //quantidade de moedas criadas
    int deadEnemies; //quantidade de inimigos derrotados
    int powerHits; //quantidade de vezes que pode usar o botão power
}PHASE;

//Jogo:
typedef struct{
    PHASE phase; //Fase do jogo (será salva ao apertar A)
    char namePlayer[MAX_CHAR_NAME]; //Nome do jogador
    int house;
}GAME;

//Ranking:
typedef struct{
    char name[MAX_CHAR_NAME]; //jogador de pontuação mais alta
    int points;
    int pointsSly;
    int pointsGryf;
    int pointsHuf;
    int pointsRav;
}RANKING;


#endif // STRUCTS_H_INCLUDED
