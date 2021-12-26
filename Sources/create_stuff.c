#include "../Headers/create_stuff.h"


///---Retorna um vetor com as coordenadas de um cano aleatório----
Vector2 rand_tube(PHASE phase){
    int r = rand()% phase.numTubes;
    float x = phase.tubes[r].x;
    if(x>SCREEN_WIDTH/2)
        x -= phase.tubes[r].width;
    float y = phase.tubes[r].y + (phase.tubes[r].height)/2;
    Vector2 posTube = {x, y};
    printf("\nCano aleatorio: %d\nPosicao=(%f, %f)\n", r, x, y);
    return posTube;
}


///--------------Cria um inimigo do tipo 1--------------
void create_enemy1(PHASE *phase, SRC media){
    Vector2 posEnemy = rand_tube(*phase); //posiciona o novo inimigo em um tubo aleatório
    int dir;
    if(posEnemy.x>SCREEN_WIDTH/2) //se tiver na metade direita da tela
        dir=-1; //se move para a esquerda
    else //se estiver na metade da esquerda da tela
        dir=1; //se move para a direita
    ENEMY newEnemy =   {1, //indica que o inimigo está ativo
                        1, //Inimigo do tipo 1
                        2, //Status: normal (2), vulnerável (1) ou morto (0)
                        3, //módulo da velocidade do inimigo
                        dir, //direção de deslocamento
                        {posEnemy.x, posEnemy.y, media.recFrameEnemies.x, media.recFrameEnemies.y}}; //retângulo que representa o inimigo
    phase->enemies1[phase->activeEnemies1]= newEnemy; //coloca o inimigo inicial na posição activeEnemies1 do vetor de inimigos do tipo 1
    phase->activeEnemies1++;
    printf("\nInimigos 1: %d\n", phase->activeEnemies1);
}


///-------------Cria um inimigo do tipo 2--------------
void create_enemy2(PHASE *phase, SRC media){
    Vector2 posEnemy = rand_tube(*phase); //posiciona o novo inimigo em um tubo aleatório
    int dir;
    if(posEnemy.x>SCREEN_WIDTH/2) //se tiver na metade direita da tela
        dir=-1; //se move para a esquerda
    else //se estiver na metade da esquerda da tela
        dir=1; //se move para a direita
    ENEMY newEnemy =   {1, //indica que o inimigo está ativo
                        1, //Inimigo do tipo 1
                        3, //Status: normal (3), furioso (2), vulnerável (1) ou morto (0)
                        3, //módulo da velocidade do inimigo (inicia com a velocidade normal (3)
                        dir, //direção de deslocamento
                        {posEnemy.x, posEnemy.y, media.recFrameEnemies.x, media.recFrameEnemies.y}}; //retângulo que representa o inimigo
    phase->enemies2[phase->activeEnemies2]= newEnemy; //coloca o inimigo inicial na posição activeEnemies2 do vetor de inimigos do tipo 2
    phase->activeEnemies2++;
    printf("\nInimigos 2: %d\n", phase->activeEnemies2);
}


///-----------Cria uma moeda----------
void create_coin(PHASE *phase){
    Vector2 posCoin = rand_tube(*phase);
    int dir;
    if(posCoin.x>SCREEN_WIDTH/2)
        dir=-1;
    else
        dir=1;
    COIN newCoin = {1, 2, dir, {posCoin.x, posCoin.y, 20, 20}};
    phase->coins[phase->numCoins]=newCoin;
    phase->numCoins++;
    printf("\nMoedas: %d\n", phase->numCoins);
}
