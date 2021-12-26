#include "../Headers/movements.h"


///-------------------Movimentos----------------------

///-----------------Herói------------------
float speedJump;
void move_hero(PHASE *phase, SRC media){
    int cont;
    Rectangle recCollision;
    //Na horizontal:
    phase->hero.speedX = SPEED_HERO;
    if(IsKeyDown(KEY_LEFT)){
        phase->hero.direction=-1;
        if(phase->hero.rec.x < 0)
            phase->hero.rec.x = SCREEN_WIDTH - phase->hero.rec.width;
        else{
            phase->hero.rec.x -= phase->hero.speedX;
            if(collision(phase->hero.rec, phase->plats, phase->numPlats, &recCollision))
                phase->hero.rec.x += recCollision.width;
            if(collisionRec(phase->hero.rec, phase->tubes, phase->numTubes, &recCollision))
                phase->hero.rec.x += recCollision.width;
            if(collisionRec(phase->hero.rec, phase->returns, phase->numTubes, &recCollision))
                phase->hero.rec.x += recCollision.width;
            if(CheckCollisionRecs(phase->hero.rec, phase->recPower)){
                recCollision = GetCollisionRec(phase->hero.rec, phase->recPower);
                phase->hero.rec.x += recCollision.width;
            }
        }
    }
    if(IsKeyDown(KEY_RIGHT)){
        phase->hero.direction=1;
        if(phase->hero.rec.x > SCREEN_WIDTH)
            phase->hero.rec.x = 0;
        else{
            phase->hero.rec.x += phase->hero.speedX;
            if(collision(phase->hero.rec, phase->plats, phase->numPlats, &recCollision))
                phase->hero.rec.x -= recCollision.width;
            if(collisionRec(phase->hero.rec, phase->tubes, phase->numTubes, &recCollision))
                phase->hero.rec.x -= recCollision.width;
            if(collisionRec(phase->hero.rec, phase->returns, phase->numTubes, &recCollision))
                phase->hero.rec.x -= recCollision.width;
            if(CheckCollisionRecs(phase->hero.rec, phase->recPower)){
                recCollision = GetCollisionRec(phase->hero.rec, phase->recPower);
                phase->hero.rec.x -= recCollision.width;
            }
        }
    }
    ///--------------Na vertical-------------
    //Se não estiver caindo, nem descendo, começa a subir quando apertar D:
    if(IsKeyPressed(KEY_D) && !phase->hero.falling && !phase->hero.goingUp){
        cont=0;
        speedJump=30;
        phase->hero.goingUp=1;
        phase->hero.falling=0;
    }else
        phase->hero.falling=1;

    //Se ele pode pular:
    if(phase->hero.goingUp){
        phase->hero.falling=0;    //para de cair
        cont+=1;
        speedJump--;
        phase->hero.rec.y -= 8;
    ///--------Verificar se ele deve parar de pular----------
        //Se bate a cabeça na plataforma:
        if(collision(phase->hero.rec, phase->plats, phase->numPlats, &recCollision) ||
            collisionRec(phase->hero.rec, phase->tubes, phase->numTubes, &recCollision) ||
            collisionRec(phase->hero.rec, phase->returns, phase->numTubes, &recCollision)){
            phase->hero.rec.y += recCollision.height;
            if(knock_enemy1(recCollision, phase) || knock_enemy2(recCollision, phase))
                PlaySound(media.expelliarmus);
            phase->hero.goingUp=0; phase->hero.falling=1;
        }
        //Se bate a cabeça no power:
        if(CheckCollisionRecs(phase->hero.rec, phase->recPower)){
            recCollision = GetCollisionRec(phase->hero.rec, phase->recPower);
            phase->hero.rec.y += recCollision.height;
            phase->hero.goingUp=0; phase->hero.falling=1;
            if(hit_power(phase, media) && phase->powerHits>0)
                phase->powerHits--;
        }
        //Se atinge a altura máxima
        if(speedJump<0){
            phase->hero.goingUp=0;
            phase->hero.falling=1;
        }
    }


    ///------------Verificar se deve parar de cair:---------------

    //Se quiser ultrapassar o limite do chão:
    if(phase->hero.rec.y + phase->hero.rec.height >= SCREEN_HEIGHT-GROUND_HEIGHT){
        phase->hero.jump=1;
        phase->hero.falling=0; //para de cair
    }

    if(phase->hero.falling){
        phase->hero.rec.y+=GRAVITY;
        //Se colide com uma plataforma, um cano ou um retorno
        if(collision(phase->hero.rec, phase->plats, phase->numPlats, &recCollision) ||
            collisionRec(phase->hero.rec, phase->tubes, phase->numTubes, &recCollision) ||
            collisionRec(phase->hero.rec, phase->returns, phase->numTubes, &recCollision)){
                phase->hero.rec.y -= recCollision.height;
                phase->hero.falling=0;
        }
        //Se colide com o botão Power
        if(CheckCollisionRecs(phase->hero.rec, phase->recPower)){
            recCollision = GetCollisionRec(phase->hero.rec, phase->recPower);
            phase->hero.rec.y -= recCollision.height;
            phase->hero.falling=0;
        }
    }
    heroXenemy(phase, media); //Verifica se o herói morre/mata o inimigo
}


///----------------------Inimigos-----------------------

void move_enemies(PHASE *phase){
    Rectangle recCollision;
    ///------------------Mover os inimigos do tipo 1------------------
    for(int i=0; i<phase->numEnem1; i++){
        if(phase->enemies1[i].alive==1){
            ///-------Na horizontal:
            if(phase->enemies1[i].status >1) //Se não estiver vulnerável
                phase->enemies1[i].rec.x += (phase->enemies1[i].speedX)*(phase->enemies1[i].direction); //anda
            //Se colidir com uma plataforma, troca a direção
            if(collision(phase->enemies1[i].rec, phase->plats, phase->numPlats, &recCollision)){
                phase->enemies1[i].rec.x -= (recCollision.width)*(phase->enemies1[i].direction);
                phase->enemies1[i].direction *= -1;
            }
            //Se colidir com o botão power, troca de direção:
            if(CheckCollisionRecs(phase->recPower, phase->enemies1[i].rec)){
                recCollision=GetCollisionRec(phase->recPower, phase->enemies1[i].rec);
                phase->enemies1[i].rec.x -= (recCollision.width)*(phase->enemies1[i].direction);
                phase->enemies1[i].direction *= -1;
            }
            //Deve estar dentro dos limites laterais da tela:
            if(phase->enemies1[i].rec.x > SCREEN_WIDTH)
                phase->enemies1[i].rec.x=0;
            if(phase->enemies1[i].rec.x + phase->enemies1[i].rec.width < 0)
                phase->enemies1[i].rec.x=SCREEN_WIDTH-phase->enemies1[i].rec.width;

            //Checa colisão com outros inimigos (tipo 1):
            for(int j=0; j<phase->numEnem1; j++){
                if(i!=j){
                    if(CheckCollisionRecs(phase->enemies1[i].rec, phase->enemies1[j].rec)){
                        recCollision=GetCollisionRec(phase->enemies1[i].rec, phase->enemies1[j].rec);
                        phase->enemies1[i].rec.x -= (recCollision.width)*(phase->enemies1[i].direction);
                        phase->enemies1[i].direction *= -1;
                    }
                }
            }
            //Checa colisão com outros inimigos (tipo 2):
            for(int j=0; j<phase->numEnem2; j++){
                if(CheckCollisionRecs(phase->enemies1[i].rec, phase->enemies2[j].rec)){
                    if(phase->enemies1[i].rec.x < phase->enemies2[j].rec.x){
                        phase->enemies1[i].direction=-1;
                        phase->enemies2[j].direction=1;
                    }else{
                        phase->enemies1[i].direction=1;
                        phase->enemies2[j].direction=-1;
                    }
                }
            }
            //Na vertical:
            if(phase->enemies1[i].rec.y + phase->enemies1[i].rec.height < SCREEN_HEIGHT - GROUND_HEIGHT){
                phase->enemies1[i].rec.y += GRAVITY;
                //Checa colisão com as plataformas:
                if(collision(phase->enemies1[i].rec, phase->plats, phase->numPlats, &recCollision))
                    phase->enemies1[i].rec.y -= recCollision.height;
                //Checa colisão com outros inimigos (tipo 1):
                for(int j=0; j<phase->numEnem1; j++){
                    if(i!=j){
                        if(CheckCollisionRecs(phase->enemies1[i].rec, phase->enemies1[j].rec)){
                            recCollision=GetCollisionRec(phase->enemies1[i].rec, phase->enemies1[j].rec);
                            phase->enemies1[i].rec.y -= recCollision.height;
                        }
                    }
                }
                //Checa colisão com outros inimigos (tipo 2):
                for(int j=0; j<phase->numEnem2; j++){
                    if(CheckCollisionRecs(phase->enemies1[i].rec, phase->enemies2[j].rec)&& phase->enemies1[i].status != 1  && phase->enemies1[i].rec.y - GRAVITY > phase->enemies2[j].rec.y){
                        recCollision=GetCollisionRec(phase->enemies1[i].rec, phase->enemies2[j].rec);
                        phase->enemies1[i].rec.y -= recCollision.height;
                    }
                }
            }
        }
    }
    ///------------------Mover os inimigos do tipo 2------------------
    for(int i=0; i<phase->numEnem2; i++){
        if(phase->enemies2[i].alive==1){
            ///-------Na horizontal:
            if(phase->enemies2[i].status >1) //Se não estiver vulnerável
                phase->enemies2[i].rec.x += (phase->enemies2[i].speedX)*(phase->enemies2[i].direction); //anda
            //Se colidir com uma plataforma, troca a direção
            if(collision(phase->enemies2[i].rec, phase->plats, phase->numPlats, &recCollision)){
                phase->enemies2[i].rec.x -= (recCollision.width)*(phase->enemies2[i].direction);
                phase->enemies2[i].direction *= -1;
            }

            //Se colidir com o botão power, troca de direção:
            if(CheckCollisionRecs(phase->recPower, phase->enemies2[i].rec)){
                recCollision=GetCollisionRec(phase->recPower, phase->enemies2[i].rec);
                phase->enemies2[i].rec.x -= (recCollision.width)*(phase->enemies2[i].direction);
                phase->enemies2[i].direction *= -1;
            }

            //Deve estar dentro dos limites laterais da tela:
            if(phase->enemies2[i].rec.x > SCREEN_WIDTH)
                phase->enemies2[i].rec.x=phase->enemies2[i].rec.width;
            if(phase->enemies2[i].rec.x + phase->enemies2[i].rec.width < 0)
                phase->enemies2[i].rec.x=SCREEN_WIDTH-phase->enemies2[i].rec.width;
            //Checa colisão com outros inimigos (tipo 2):
            for(int j=0; j<phase->numEnem2; j++){
                if(i!=j){
                    if(CheckCollisionRecs(phase->enemies2[i].rec, phase->enemies2[j].rec)){
                        recCollision=GetCollisionRec(phase->enemies2[i].rec, phase->enemies2[j].rec);
                        phase->enemies2[i].rec.x -= (recCollision.width)*(phase->enemies2[i].direction);
                        phase->enemies2[i].direction *= -1;
                    }
                }
            }
            //Checa colisão com outros inimigos (tipo 1):
            for(int j=0; j<phase->numEnem1; j++){
                if(CheckCollisionRecs(phase->enemies2[i].rec, phase->enemies1[j].rec)){
                    if(phase->enemies2[i].rec.x < phase->enemies1[j].rec.x){
                        phase->enemies2[i].direction=-1;
                    }else{
                        phase->enemies2[i].direction=1;
                        phase->enemies1[j].direction=-1;
                    }
                }
            }
            //Na vertical:
            if(phase->enemies2[i].rec.y + phase->enemies2[i].rec.height < SCREEN_HEIGHT - GROUND_HEIGHT){
                phase->enemies2[i].rec.y += GRAVITY;
                if(collision(phase->enemies2[i].rec, phase->plats, phase->numPlats, &recCollision))
                    phase->enemies2[i].rec.y -= recCollision.height;
                //Checa colisão com outros inimigos (tipo 2):
                for(int j=0; j<phase->numEnem2; j++){
                    if(i!=j){
                        if(CheckCollisionRecs(phase->enemies2[i].rec, phase->enemies2[j].rec)){
                            recCollision=GetCollisionRec(phase->enemies2[i].rec, phase->enemies2[j].rec);
                            phase->enemies2[i].rec.y -= recCollision.height;
                        }
                    }
                }
                //Checa colisão com outros inimigos (tipo 1):
                for(int j=0; j<phase->numEnem1; j++){
                    if(CheckCollisionRecs(phase->enemies2[i].rec, phase->enemies1[j].rec) && phase->enemies2[i].status != 1 && phase->enemies2[i].rec.y - GRAVITY > phase->enemies1[j].rec.y){
                        recCollision=GetCollisionRec(phase->enemies2[i].rec, phase->enemies1[j].rec);
                        phase->enemies2[i].rec.y -= recCollision.height;
                    }
                }
            }
        }
    }
}


///-----------------Moedas-----------------------
//Essa função, em específico, faz a movimentação das moedas e a interação com
//os demais elementos do jogo simultaneamente
void collect_coin(PHASE *phase){
    Rectangle recCollision;
    int i;
    for(i=0; i<= phase->numCoins; i++){
        ///---------Movimento da moeda----------
        if(phase->coins[i].status==1){
            //Na horizontal:
            phase->coins[i].coinRec.x += (phase->coins[i].speedX*phase->coins[i].direction);
            if(collision(phase->coins[i].coinRec, phase->plats, phase->numPlats, &recCollision)){
                phase->coins[i].coinRec.x -= recCollision.width;
                phase->coins[i].direction *= -1;
            }
            //Colisão com inimigos:
            for(int j =0; j<phase->activeEnemies1; j++){
                if(CheckCollisionRecs(phase->coins[i].coinRec, phase->enemies1[j].rec)){
                    if(phase->coins[i].coinRec.x < phase->enemies1[j].rec.x)
                        phase->coins[i].direction=-1;
                    else
                        phase->coins[i].direction=1;
                }
            }
            for(int j =0; j<phase->activeEnemies2; j++){
                if(CheckCollisionRecs(phase->coins[i].coinRec, phase->enemies2[j].rec)){
                    if(phase->coins[i].coinRec.x < phase->enemies2[j].rec.x)
                        phase->coins[i].direction=-1;
                    else
                        phase->coins[i].direction=1;
                }
            }

            //Deve estar dentro dos limites da tela:
            if(phase->coins[i].coinRec.x > SCREEN_WIDTH)
                phase->coins[i].coinRec.x=0;
            if(phase->coins[i].coinRec.x < 0)
                phase->coins[i].coinRec.x=SCREEN_WIDTH;
            //Na vertical:
            if(phase->coins[i].coinRec.y + phase->coins[i].coinRec.height < SCREEN_HEIGHT - GROUND_HEIGHT){
                phase->coins[i].coinRec.y += 5;
            if(collision(phase->coins[i].coinRec, phase->plats, phase->numPlats, &recCollision))
                phase->coins[i].coinRec.y -= recCollision.height;
            }
        }
        ///--------------Coletar a moeda---------------
        if(CheckCollisionRecs(phase->coins[i].coinRec, phase->hero.rec)){
            phase->coins[i].coinRec.y=-10000;
            phase->coins[i].status=0;
            phase->hero.points+=30;
        }
        enter_return(&phase->coins[i].coinRec, phase, &phase->coins[i].direction);

    }
}



