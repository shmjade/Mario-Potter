#include "../Headers/interactions.h"


///--------------------Interações--------------------------

///-----------Herói x Inimigo----------
//Se herói e inimigo colidem, verificar quem morre:
void heroXenemy(PHASE *phase, SRC media){
    int i;
    for(i=0; i<phase->activeEnemies1; i++){
        if(CheckCollisionRecs(phase->hero.rec, phase->enemies1[i].rec)){
            //Se o inimigo estiver vulnerável, matar o inimigo:
            if(phase->enemies1[i].status==1){
                phase->hero.points += KILL_ENEMY;
                phase->enemies1[i].rec.y=SCREEN_HEIGHT;
                phase->deadEnemies++;
            }else{//Se inimigo não estiver vulnerável, matar o herói:
                phase->hero.lives--;
                phase->hero.rec.x = (SCREEN_WIDTH-phase->hero.rec.width)/2;
                phase->hero.rec.y = 10;
                PlaySound(media.harryDied);
                phase->hero.afterDeath=1;
            }
        }
    }
    for(i=0; i<phase->activeEnemies2; i++){
        if(CheckCollisionRecs(phase->hero.rec, phase->enemies2[i].rec)){
            printf("Colisao");
            //Se o inimigo estiver vulnerável, matar o inimigo:
            if(phase->enemies2[i].status==1){
                phase->hero.points += KILL_ENEMY;
                phase->enemies2[i].rec.y=SCREEN_HEIGHT;
                phase->deadEnemies++;
            }else{//Se inimigo não estiver vulnerável, matar o herói:
                phase->hero.lives--;
                phase->hero.rec.x = (SCREEN_WIDTH-phase->hero.rec.width)/2;
                phase->hero.rec.y = 10;
                PlaySound(media.harryDied);
                phase->hero.afterDeath=1;
            }
        }
    }
}


///-----------Herói x Botão Power----------
//Ao atingir o botão Power, elimina os inimigos e as moedas que estão aparecendo na tela
int hit_power(PHASE *phase, SRC media){
    int hit=0;
    Rectangle screen = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    if(phase->powerHits>0){
        for(int i=0; i<phase->activeEnemies1; i++){
            if(CheckCollisionRecs(phase->enemies1[i].rec, screen)){
                draw_patronum(phase->enemies1[i].rec, media);
                phase->enemies1[i].rec.y = SCREEN_HEIGHT+100;
                phase->enemies1[i].status=0;
                phase->enemies1[i].alive=0;
                phase->hero.points += KILL_ENEMY;
                phase->deadEnemies++;
                hit=1;
            }
        }
        for(int i=0; i<phase->activeEnemies2; i++){
            if(CheckCollisionRecs(phase->enemies2[i].rec, screen)){
                draw_patronum(phase->enemies2[i].rec, media);
                phase->enemies2[i].rec.y = SCREEN_HEIGHT+100;
                phase->enemies2[i].status=0;
                phase->enemies2[i].alive=0;
                phase->hero.points += KILL_ENEMY;
                phase->deadEnemies++;
                hit=1;
            }
        }
        //Elimina as moedas
        for(int i=0; i<phase->numCoins; i++){
            if(CheckCollisionRecs(phase->coins[i].coinRec, screen)){
                phase->coins[i].coinRec.y = SCREEN_HEIGHT+100;
                phase->hero.points += GET_COIN;
                hit=1;
            }
        }
    }
    return hit;
}


///------------Herói x Inimigo do tipo 2---------------
int knock_enemy2(Rectangle recCollision, PHASE *phase){
    int i, knock=0;
    Rectangle recKnock = {recCollision.x-10, recCollision.y-50, recCollision.width+20, 40};
    for(i=0; i<phase->activeEnemies2; i++){
        if(CheckCollisionRecs(recKnock, phase->enemies2[i].rec)){
            phase->hero.points += KNOCK_ENEMY;
            phase->enemies2[i].rec.y -= 50;
            knock=1;
            switch(phase->enemies2[i].status){
                case 3: phase->enemies2[i].speedX *= 2; //aumenta o módulo da velocidade
                        phase->enemies2[i].status--;
                    break;
                case 2: phase->enemies2[i].speedX *= 0; //fica parado
                        phase->enemies2[i].status--;
                    break;
            }
        }
    }
    return knock;
}


///------------Herói x Inimigo do tipo 1---------------
int knock_enemy1(Rectangle recCollision, PHASE *phase){
    int i, knock=0;
    Rectangle recKnock = {recCollision.x-10, recCollision.y-50, recCollision.width+20, 40};
    for(i=0; i<phase->activeEnemies1; i++){
        if(CheckCollisionRecs(recKnock, phase->enemies1[i].rec)){
            phase->hero.points += KNOCK_ENEMY;
            phase->enemies1[i].rec.y -= 50;
            if(phase->enemies1[i].status!=1)
                phase->enemies1[i].status--;
            knock=1;
        }
    }
    return knock;
}


///-----------------Retornos com moedas e inimigos---------------
//Se um inimigo ou uma moeda encostarem em um retorno, posicioná-los
//nas coordenadas x e y do cano correspondente
void enter_return(Rectangle *someone, PHASE *phase, int *direction){
    for(int z=0; z<phase->numTubes; z++){
        if(CheckCollisionRecs(*someone, phase->returns[z])){
            someone->y = phase->tubes[z].y;
            someone->x = phase->tubes[z].x;
            if(someone->x > SCREEN_WIDTH/2)
                *direction=-1;
            else
                *direction=1;
        }
    }
}


///--------------------Plataformas--------------------
//Verifica a colisão de um retângulo dado com todas as plataformas da fase:
int collision(Rectangle someone, PLATFORM plats[], int numPlats, Rectangle *collided){
    int col=0;
    int i=0;
    for(i=0; i<numPlats; i++){
        if(CheckCollisionRecs(someone, plats[i].rec)){
            plats[i].status = 1;
            *collided = GetCollisionRec(someone, plats[i].rec);
            col=1;
        }
    }
    return col;
}

//Retorna o retângulo (collided) da colisão de um retângulo dado com um vetor de recs:
int collisionRec(Rectangle someone, Rectangle recs[], int numRecs, Rectangle *collided){
    int col=0;
    int i=0;
    for(i=0; i<numRecs; i++){
        if(CheckCollisionRecs(someone,recs[i])){
            *collided = GetCollisionRec(someone, recs[i]);
            col=1;
        }
    }
    return col;
}










