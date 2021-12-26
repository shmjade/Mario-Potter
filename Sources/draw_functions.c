#include "../Headers/draw_functions.h"

///----------------------------Funções de desenho-----------------------------


    ///---------------------------Fase-----------------------------
    void draw_phase(PHASE phase, SRC media){
        int i;
        Rectangle ground = {0, SCREEN_HEIGHT-GROUND_HEIGHT, SCREEN_WIDTH, GROUND_HEIGHT};

            BeginDrawing();
            ClearBackground(SKYBLUE);
            DrawRectangleRec(phase.recPower, GOLD);
            DrawText((TextFormat ("%i", phase.powerHits)), phase.recPower.x+10, phase.recPower.y+5, 30, BLACK);
            DrawRectangleRec(ground, BLACK);
            DrawText("Points: ", 40, 10, 30, BLACK);
            DrawText((TextFormat("%i", phase.hero.points)), 150, 10, 30, RED);
            DrawText("Lives: ", 260, 10, 30, BLACK);
            DrawText((TextFormat("%i", phase.hero.lives)), 350, 10, 30, RED);
            DrawText("Phase: ", 780, 10, 30, BLACK);
            DrawText((TextFormat("%i", phase.level)), 885, 10, 30, RED);
            DrawText("Enemies: ", 1000, 10, 30, BLACK);
            DrawText((TextFormat("%i", phase.numEnem1 + phase.numEnem2 - phase.deadEnemies)), 1130, 10, 30, RED);
            for(i=0; i<phase.numPlats; i++)
                DrawRectangleRec(phase.plats[i].rec, BLACK);
            for(i=0; i<phase.numTubes; i++){
                DrawTexture(media.fireplace, phase.returns[i].x, phase.returns[i].y, WHITE);
                DrawTexture(media.fireplace, phase.tubes[i].x, phase.tubes[i].y, WHITE);
                //draw_fireplace(phase.plats[i].rec, phase, media);
                //draw_fireplace(phase.returns[i], phase, media);
            }
            EndDrawing();
    }

    ///---------Herói----------
    int framesCounter=0, currentFrame=6, framesSpeed=30;
    void draw_hero(HERO hero, SRC media){
        Texture2D sprite;
        Vector2 position={hero.rec.x, hero.rec.y};
        sprite=media.enemies;
        Rectangle frameRec = media.recFrameEnemies;
        int initialFrame=6;
        frameRec.x = (float)currentFrame*(float)sprite.width/12*6;

        if(hero.afterDeath){
            frameRec.y = (float)sprite.height/8*4;
            frameRec.x = (float)sprite.width/12*7;
            DrawText("Press SPACE to release", 510, 80, 15, BLACK);
        }else{
            //Ajustando a direção:
            if(hero.direction==1) //Direita
                frameRec.y = (float)sprite.height/8*6;
            else //Esquerda
                frameRec.y = (float)sprite.height/8*5;

            //Só se mexe se estiver apertando a tecla
            if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT)){
                framesCounter++;
                if (framesCounter >= (60/framesSpeed)){
                    framesCounter = 0;
                    currentFrame++;
                    if(currentFrame > initialFrame+2)
                        currentFrame = initialFrame;
                }
           }
           frameRec.x = (float)currentFrame*(float)sprite.width/12;
        }
        DrawTextureRec(sprite, frameRec, position, WHITE);  // Draw part of the texture
    }

    ///---------Inimigo 1----------
    int framesCounter2=0, currentFrame2=0, framesSpeed2=2;
    void draw_enm1(ENEMY enm1, SRC media){
        Texture2D sprite;
        Vector2 position={enm1.rec.x, enm1.rec.y};
        sprite=media.enemies;
        Rectangle frameRec = media.recFrameEnemies;

        //Ajustando a direção:
        if(enm1.direction==1) //Direita
            frameRec.y = (float)sprite.height/8*2;
        else //Esquerda
            frameRec.y = (float)sprite.height/8;

        //Só se mexe se não estiver no modo vulnerável
        if(enm1.status!=1){
            framesCounter2++;
            if (framesCounter2 >= (60/framesSpeed2)){
                framesCounter2 = 0;
                currentFrame2++;
                if(currentFrame2 >2)
                    currentFrame2 = 0;
            }
            frameRec.x = (float)currentFrame2*(float)sprite.width/12;
       }else{
            frameRec.x = (float)sprite.width/12;
            frameRec.y = 0;
       }
        DrawTextureRec(sprite, frameRec, position, WHITE);  // Draw part of the texture
    }

    ///---------Inimigo 2----------
    int framesCounter1=0, currentFrame1=0, framesSpeed1=2;
    void draw_enm2(ENEMY enm2, SRC media){
        Texture2D sprite;
        Vector2 position={enm2.rec.x, enm2.rec.y};
        sprite=media.enemies;
        Rectangle frameRec = media.recFrameEnemies;
        int initialFrame;

        //De acordo com o status:
        if(enm2.status==3)
            initialFrame=3;
        if(enm2.status==2)
            initialFrame=9;


        //Ajustando a direção:
        if(enm2.direction==1) //Direita
            frameRec.y = (float)sprite.height/8*2;
        else //Esquerda
            frameRec.y = (float)sprite.height/8;

        //Ajustando a velocidade:

        //Só se mexe se não estiver no modo vulnerável
        currentFrame1=initialFrame;
        if(enm2.status!=1){
            framesCounter1++;
            if (framesCounter1 >= (60/framesSpeed1)){
                framesCounter1 = 0;
                currentFrame1++;
                if(currentFrame1 >initialFrame+2)
                    currentFrame1 = initialFrame;
            }
            frameRec.x = (float)currentFrame1*(float)sprite.width/12;
       }else{
            frameRec.x = (float)sprite.width/12*4;
            frameRec.y = 0;
       }
        DrawTextureRec(sprite, frameRec, position, WHITE);  // Draw part of the texture
    }


    ///---------------------Canos/Retornos--------------------
    void draw_fireplace(Rectangle fireplace, PHASE phase, SRC media){
        int fire=0;
        //Checa colisão com um inimigo do tipo 1:
        for(int e=0; e<phase.numEnem1; e++){
            if(CheckCollisionRecs(fireplace, phase.enemies1[e].rec))
                fire=1;
        }
        //Checa colisão com um inimigo do tipo 2:
        for(int e=0; e<phase.numEnem2; e++){
            if(CheckCollisionRecs(fireplace, phase.enemies2[e].rec))
                fire=1;
        }
        //Checa colisão com as moedas:
        for(int e=0; e<phase.numEnem2; e++){
            if(CheckCollisionRecs(fireplace, phase.enemies2[e].rec))
                fire=1;
        }
    //    Se colidiram, desenhar a lareira com fogo. Senão, desenhar sem:
        if(fire)
            DrawTexture(media.fireplaceGreen, fireplace.x, fireplace.y, WHITE);
        else
            DrawTexture(media.fireplace, fireplace.x, fireplace.y, WHITE);
    }


    ///-------------Efeito visual do patrono-----------
    int time_patronum, framesCounterP=0, currentFrameP=5, framesSpeedP=1;
    void draw_patronum(Rectangle rec, SRC media){
        time_patronum=0;
        Vector2 posText = {200, 300};
        PlaySound(media.expectro);
        Texture2D sprite;
        sprite=media.patronumEffect;
        Vector2 position = {rec.x-rec.width, rec.y-rec.height}; //centralizando o efeito
        Rectangle frameRec = media.recFramePatronum;
        framesCounterP++;
        if (framesCounterP >= 1200){
            framesCounterP = 0;
            currentFrameP--;
            if(currentFrameP<1)
                currentFrameP = 5;
        }
        frameRec.x = (float)currentFrameP*(float)sprite.width/12;
        DrawTextureRec(sprite, frameRec, position, WHITE);  // Draw part of the texture
        DrawTextEx(media.fonteHP, "EXPECTRO PATRONUM", posText, 80, 10, PURPLE);
        delay(140);
    }

    ///------------------Ganhou o jogo-----------------
int time_win;
void win_game(SRC media){
    int time_text=0;
//    char text[8] = "YOU WON";
    time_win=0;
    while(time_win<60*200){
        if(time_win%60)
            time_text++;
        BeginDrawing();
        ClearBackground(GREEN);
//        for(int i=0; i<8; i++){
//            DrawText(text[i], 100+i*10, 50, BLACK);
//            DrawText(text[i], 100+i*10, 50, BLACK);
//        }
        EndDrawing();
        time_win++;
    }
}

    ///------------------Perdeu o jogo-----------------



    ///---------------Escolher a casa----------------
const int maxString = 30;
int two_answers(SRC media, char question[maxString], char opt1[maxString], char opt2[maxString]){
    Vector2 coordTitle = (Vector2){400,55}; //Coordenada do título
    int selected=0;
    int fontSize=40;
    Color color1=RED, color2=BLACK;
    //---------- Atualizações ------------
    while(!IsKeyPressed(KEY_ENTER)){
        if(IsKeyPressed(KEY_UP)){
            PlaySound(media.fast_spell);
            selected=0;
            color1=RED;
            color2=BLACK;
        }
        if(IsKeyPressed(KEY_DOWN)){
            PlaySound(media.fast_spell);
            selected=1;
            color2=RED;
            color1=BLACK;
        }
        //Desenhos:
        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawTextEx(media.fonteHP, "SORTING HAT", coordTitle, 80, 5, BLACK);
        DrawText(question, (SCREEN_WIDTH-MeasureText(question, fontSize))/2, 200, fontSize, BLACK);
        DrawText(opt1, (SCREEN_WIDTH-MeasureText(opt1, fontSize))/2, 300, fontSize, color1);
        DrawText(opt2, (SCREEN_WIDTH-MeasureText(opt2, fontSize))/2, 400, fontSize, color2);
        EndDrawing();
    }
    PlaySound(media.heavy_spell);
    delay(20);
    return selected;
}

int time_letter=0;
void letter(SRC media){
    while(time_letter<60*5){ //esperar 10 segundos
        time_letter++;
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(media.letter,(SCREEN_WIDTH-569)/2, 0, WHITE);
        EndDrawing();
    }
}

int time_hat=0;
void hat(SRC media){
    Vector2 coord1 = {(SCREEN_WIDTH-MeasureTextEx(media.fonteHP, "THE SORTING HAT WILL", 80, 5).x)/2, 250};
    Vector2 coord2 = {(SCREEN_WIDTH-MeasureTextEx(media.fonteHP, "PUT YOU AT THE", 80, 5).x)/2, 350};
    Vector2 coord3 = {(SCREEN_WIDTH-MeasureTextEx(media.fonteHP, "RIGHT HOUSE", 80, 5).x)/2, 450};
    Vector2 coord4 = {(SCREEN_WIDTH-MeasureTextEx(media.fonteHP, "Answer to enter a house", 60, 5).x)/2, 550};
    while(time_hat<60*10){ //esperar 10 segundos
        time_hat++;
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextEx(media.fonteHP, "THE SORTING HAT WILL", coord1, 80, 5, WHITE);
        DrawTextEx(media.fonteHP, "PUT YOU AT THE", coord2, 80, 5, WHITE);
        DrawTextEx(media.fonteHP, "RIGHT HOUSE", coord3, 80, 5, WHITE);
        if(time(NULL)%3)
            DrawTextEx(media.fonteHP, "Answer to enter a house", coord4, 60, 5, GRAY);
        DrawTexture(media.slytherin, 5, 0, WHITE);
        DrawTexture(media.hufflepuff, 5, 470, WHITE);
        DrawTexture(media.gryffindor, 1020, 0, WHITE);
        DrawTexture(media.ravenclaw, 1020, 470, WHITE);
        EndDrawing();
    }
}


int sorting_hat(SRC media){
    char sly[3][20] = {"Pride", "Ambitious", "Cunning"};
    char gryf[3][20] = {"Brave", "Chivalrous", "Valiant"};
    char rav[3][20] = {"Wise", "Clever", "Logical"};
    char huf[3][20] = {"Loyal", "Patient", "Friendly"};
    char house[3][20];

    Vector2 coordTitle = (Vector2){150,75}; //Coordenada do título
    Vector2 pos;
    Color background;
    //Tocar música de fundo:
    SetMusicVolume(media.afterAllthisTime, 1.0);
    PlayMusicStream(media.afterAllthisTime);

    int gryffindor=0, ravenclaw=0, slytherin=0, hufflepuff=0, chosen;
    if(two_answers(media, "Dusk or dawn?", "Dusk", "Dawn")==0){
        slytherin++;
        hufflepuff++;
    }else{
        gryffindor++;
        ravenclaw++;
    }
    if(two_answers(media, "Forest or river?", "Forest", "River")==1){
        slytherin++;
        hufflepuff++;
    }else{
        gryffindor++;
        ravenclaw++;
    }
    if(two_answers(media, "Moon or stars?", "Moon", "Stars")==0){
        ravenclaw++;
        slytherin++;
    }else{
        hufflepuff++;
        gryffindor++;
    }
    if(two_answers(media, "Black or white?", "Black", "White")==0){
        gryffindor++;
        slytherin++;
    }else{
        ravenclaw++;
        hufflepuff++;
    }
    if(two_answers(media, "Heads or tails?", "Heads", "Tails")==1){
        gryffindor++;
        slytherin++;
    }else{
        ravenclaw++;
        hufflepuff++;
    }
    if(two_answers(media, "Left or right?", "Left", "Right")==0){
        ravenclaw++;
        slytherin++;
    }else{
        gryffindor++;
        hufflepuff++;
    }
    if(ravenclaw>=gryffindor && ravenclaw>=slytherin && ravenclaw>=hufflepuff){
        chosen = RAVENCLAW;
        pos = (Vector2) {1,0};
        background=BLUE;
        for(int i=0; i<3; i++)
            strcpy(house[i], rav[i]);
    }else if(gryffindor>=ravenclaw && gryffindor>=slytherin && gryffindor>=hufflepuff){
        chosen = GRYFFINDOR;
        pos = (Vector2) {0,0};
        background=RED;
        for(int i=0; i<3; i++)
            strcpy(house[i], gryf[i]);
    }
    else if(slytherin>=hufflepuff && slytherin>=gryffindor && slytherin>=ravenclaw){
        chosen = SLYTHERIN;
        pos = (Vector2) {1,1};
        background=GREEN;
        for(int i=0; i<3; i++)
            strcpy(house[i], sly[i]);
    }else{
        chosen = HUFFLEPUFF;
        pos = (Vector2) {0,1};
        background=YELLOW;
        for(int i=0; i<3; i++)
            strcpy(house[i], huf[i]);
    }
    Rectangle rec = {media.recFrameHouses.x*pos.x, media.recFrameHouses.y*pos.y, media.recFrameHouses.width, media.recFrameHouses.height};
    while(!IsKeyDown(KEY_SPACE)){
        BeginDrawing();
        ClearBackground(background);
        DrawTextEx(media.fonteHP, "You've been selected to:", (Vector2) {350, 100}, 80, 2, BLACK);
        DrawTextureRec(media.houses, rec, (Vector2) {550, 200}, WHITE);
        for(int i=0; i<3; i++){
            DrawText(house[i], 25+(SCREEN_WIDTH-MeasureText(house[i], 30))/2, 400+i*40, 30, BLACK);
        }
        if(time(NULL)%2)
            DrawText("Press SPACE to continue", 410, 525, 30, RAYWHITE);
        EndDrawing();
    }
    switch(chosen){
    case RAVENCLAW:
        break;
    case GRYFFINDOR:
        break;
    case SLYTHERIN:
        break;
    case HUFFLEPUFF:
        break;
    }
    return chosen;
}

/*
int house_choice(SRC media){
    Vector2 coordTitle = (Vector2){140,45}; //Coordenada do título

    //Inicialização
    SetMusicVolume(media->HPthemeSong, 1.0); //Escolhendo o volume da música
    PlayMusicStream(media->HPthemeSong); //Tocando a música tema

    Vector2 posFront = {900, 400};
    Vector2 posFrontHP = {200, 400};
    Vector2 posDraco = { -100, 637};
    Vector2 position = { 0, 637.0f };
    Rectangle frameRec = { 0.0f, 0.0f, (float)media->harryRight.width/6, (float)media->harryRight.height };
    Rectangle frameDraco = { 0.0f, 0.0f, (float)media->dracoRight.width/3, (float)media->dracoRight.height };
    Rectangle frameFront = { 0.0f, 0.0f, (float)media->dracoFront.width/3, (float)media->dracoFront.height };
    Rectangle frameFrontHP = { 0.0f, 0.0f, (float)media->harryFront.width/5, (float)media->harryFront.height };
    int currentFrame = 0;
    int framesCounter = 0;
    int framesSpeed = 4;

    //Definições:
    SetTargetFPS(60); //Velocidade de reprodução do jogo (frames per second)
    char messages[MENU_OPTIONS][20]={ //Mensagens
        "New game",
        "Continue game",
        "Load map",
        "Ranking",
        "Help",
        "About us",
        "Exit"
    };
    int posx[MENU_OPTIONS]; //coordenada x da mensagem
    int posy[MENU_OPTIONS]; //coordenada y da mensagem
    int selected=0; //posição do jogador (inicia na primeira opção)
    int i; //contador
    const int fontSize = 30; //tamanho da fonte
    Color color;
    const Color colorSelected = RED; //cor do item selecionado
    const Color colorDefault = BLACK; //cor dos demais itens

    //Mensagem centralizada no eixo x
    posx[0]=535; //New game
    posx[1]=495; //Continue game
    posx[2]=537; //Load map
    posx[3]=546; //Ranking
    posx[4]=570; //Help
    posx[5]=536; //About us
    posx[6]=570; //Exit

    //Mensagem espaçada no eixo y:
    posy[0]=300; //tamanho definido arbitrariamente
    for(i=1; i<MENU_OPTIONS; i++){
        posy[i]=posy[i-1]+fontSize+10; //distância de 10 entre as mensagens
    }

    //Loop principal (da tecla ENTER):
    do{ //Enquanto não apertar enter
        UpdateMusicStream(media->HPthemeSong);
        //Atualizações:
        if(IsKeyPressed(KEY_UP)){ //Se apertar tecla para cima
            if(selected==0)    //Se for o primeiro da lista, ir para o último
                selected = MENU_OPTIONS-1;
            else
                selected--;    //Senão for o primeiro da lista, diminuir o índice (subir uma posição)
            PlaySound(media->fast_spell);
        }else if(IsKeyPressed(KEY_DOWN)){ //Se apertar tecla para baixo
            if(selected==MENU_OPTIONS-1) //Se for o último da lista, ir para o primeiro
                selected = 0;
            else
                selected++;    //Senão for o último da lista, aumentar o índice (descer uma posição)
            PlaySound(media->fast_spell);
        }
        if(position.x>SCREEN_WIDTH)
            position.x=0;
        else
            position.x+=3;
        if(posDraco.x>SCREEN_WIDTH)
            posDraco.x=0;
        else
            posDraco.x+=3;
        framesCounter++;
        if (framesCounter >= (60/framesSpeed)){
            framesCounter = 0;
            currentFrame++;

            if(currentFrame >4)
                currentFrame = 0;

            frameRec.x = (float)currentFrame*(float)media->harryRight.width/6;
            frameDraco.x = (float)currentFrame*(float)media->dracoRight.width/3;
            frameFront.x = (float)currentFrame*(float)media->dracoFront.width/3;
            frameFrontHP.x = (float)currentFrame*(float)media->harryFront.width/4;
        }


        //Desenhos:
        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawTextEx(media->fonteHP, "MARIO POTTER", coordTitle, 150, 5, BLACK);
        for(i=0; i<MENU_OPTIONS; i++){
            if(i==selected){
                color=colorSelected;
            }else{
                color=colorDefault;
            }
            DrawText(messages[i], posx[i], posy[i], fontSize, color);
        }
        if(time(NULL)%2)
            DrawText("Press ENTER to select", 400, 240, 35, BLACK);

        DrawTextureRec(media->harryRight, frameRec, position, WHITE);  // Draw part of the texture
        DrawTextureRec(media->dracoRight, frameDraco, posDraco, WHITE);
        DrawTextureRec(media->dracoFront, frameFront, posFront, WHITE);
        DrawTextureRec(media->harryFront, frameFrontHP, posFrontHP, WHITE);
        EndDrawing();
    }while(!IsKeyDown(KEY_ENTER));
    delay(20);
    PlaySound(media->heavy_spell);
    return selected;
}
*/























