#include "../Headers/menu_functions.h"
#include "../Headers/all_headers.h"

void after_all(SRC *media, int *flagEnd){
    fflush(stdin);
    Vector2 coordTitle = (Vector2){150,75}; //Coordenada do título
    int timeCounter=0;

    //Tocar música de fundo:
    SetMusicVolume(media->afterAllthisTime, 1.0);
    PlayMusicStream(media->afterAllthisTime);

    //Retângulo Yes:
    Vector2 recYes = {380, 380}; //Coordenadas do retângulo Yes
    Color colorYes = RED; //Começa vermelho
    //Retângulo No:
    Vector2 recNo = {630, 380}; //Coordenadas do retângulo No
    Color colorNo = RAYWHITE; //Começa branco


    int input=0; //Escolha do usuário, é o que a função retorna

    //Loop principal (da tecla ENTER):
    while(!IsKeyDown(KEY_SPACE)){ //Enquanto não apertar ENTER
        UpdateMusicStream(media->afterAllthisTime);
        timeCounter++;
        //Atualizações:
        if(IsKeyDown(KEY_LEFT)){
            colorYes = MAROON;
            colorNo = RAYWHITE;
            input = 0; //Se input for 0, a resposta é "Yes"
            PlaySound(media->fast_spell);
        }else if(IsKeyDown(KEY_RIGHT)){
            colorYes = RAYWHITE;
            colorNo = MAROON;
            input = 1; //Se input for 1, a resposta é "No"
            PlaySound(media->fast_spell);
        }

        //Desenhos:
        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawTextEx(media->fonteHP, "MARIO POTTER", coordTitle, 150, 5, BLACK);
        DrawText("AFTER ALL THIS TIME?", 300, 310, 50, BLACK);
        if(timeCounter%5)
            DrawText("Press SPACE to select", 410, 470, 30, GRAY);
        //Retângulo Yes:
        DrawRectangle(recYes.x, recYes.y, 180, 60, colorYes);
        DrawText("ALWAYS", 410, recYes.y+17, 30, BLACK);
        DrawRectangleLines(recYes.x, recYes.y, 180, 60, BLACK);
        //Retângulo No:
        DrawRectangle(recNo.x, recNo.y, 180, 60, colorNo);
        DrawText("EXIT", 685, recNo.y+17, 30, BLACK);
        DrawRectangleLines(recNo.x, recNo.y, 180, 60, BLACK);
        EndDrawing();
    }
   // UnloadMusicStream(media->afterAllthisTime);
    PlaySound(media->heavy_spell);
    *flagEnd = input;
}

int menu(SRC media){
    Vector2 coordTitle = (Vector2){140,45}; //Coordenada do título

    //Inicialização
    SetMusicVolume(media.HPthemeSong, 1.0); //Escolhendo o volume da música
    PlayMusicStream(media.HPthemeSong); //Tocando a música tema

    Vector2 posFront = {900, 400};
    Vector2 posFrontHP = {200, 400};
    Vector2 posDraco = {-100, 637};
    Vector2 posUmbridge = {-200, 637};
    Vector2 position = {0, 637.0f};
    Vector2 posRony = {100, 637};
    Vector2 posHermione = {200, 637};
    Vector2 posHarry = {300, 637};
    Rectangle frameRec = (Rectangle) media.recFrameEnemies;
    Rectangle frameDraco = (Rectangle) media.recFrameEnemies;
    Rectangle frameHarry = (Rectangle) media.recFrameEnemies;
    Rectangle frameHermione = (Rectangle) media.recFrameEnemies;
    Rectangle frameRony = (Rectangle) media.recFrameEnemies;
    Rectangle frameFront = { 0.0f, 0.0f, (float)media.dracoFront.width/3, (float)media.dracoFront.height };
    Rectangle frameFrontHP = { 0.0f, 0.0f, (float)media.harryFront.width/4, (float)media.harryFront.height };
    int currentFrame = 0, currentFrameD=3, currentFrameH=6, currentFrameM=0, currentFrameR=3;
    int framesCounter = 0;
    int framesSpeed = 6;

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
        UpdateMusicStream(media.HPthemeSong);
        //Atualizações:
        if(IsKeyPressed(KEY_UP)){ //Se apertar tecla para cima
            if(selected==0)    //Se for o primeiro da lista, ir para o último
                selected = MENU_OPTIONS-1;
            else
                selected--;    //Senão for o primeiro da lista, diminuir o índice (subir uma posição)
            PlaySound(media.fast_spell);
        }else if(IsKeyPressed(KEY_DOWN)){ //Se apertar tecla para baixo
            if(selected==MENU_OPTIONS-1) //Se for o último da lista, ir para o primeiro
                selected = 0;
            else
                selected++;    //Senão for o último da lista, aumentar o índice (descer uma posição)
            PlaySound(media.fast_spell);
        }
        if(position.x>SCREEN_WIDTH)
            position.x=0;
        else
            position.x+=3;
        if(posDraco.x>SCREEN_WIDTH)
            posDraco.x=0;
        else
            posDraco.x+=3;
        if(posHarry.x>SCREEN_WIDTH)
            posHarry.x=0;
        else
            posHarry.x+=3;
        if(posRony.x>SCREEN_WIDTH)
            posRony.x=0;
        else
            posRony.x+=3;
        if(posHermione.x>SCREEN_WIDTH)
            posHermione.x=0;
        else
            posHermione.x+=3;
        framesCounter++;
        if (framesCounter >= (60/framesSpeed)){
            framesCounter = 0;
            currentFrame++;
            currentFrameD++;
            currentFrameH++;
            currentFrameM++;
            currentFrameR++;
        }
        if(currentFrame>2)
            currentFrame = 0;
        if(currentFrameD>5)
            currentFrameD = 3;
        if(currentFrameH>8)
            currentFrameH = 6;
        if(currentFrameM>2)
            currentFrameM = 0;
        if(currentFrameR>5)
            currentFrameR = 3;

        frameRec.x = (float)currentFrame*(float)media.recFrameEnemies.x;
        frameRec.y = (float)media.recFrameEnemies.y*2;
        frameDraco.x = (float)currentFrameD*(float)media.recFrameEnemies.x;
        frameDraco.y = (float)media.recFrameEnemies.y*2;
        frameHarry.x = (float)currentFrameH*(float)media.recFrameEnemies.x;
        frameHarry.y = (float)media.recFrameEnemies.y*6;
        frameHermione.x = (float)currentFrameM*(float)media.recFrameEnemies.x;
        frameHermione.y = (float)media.recFrameEnemies.y*6;
        frameRony.x = (float)currentFrameR*(float)media.recFrameEnemies.x;
        frameRony.y = (float)media.recFrameEnemies.y*6;
        frameFront.x = (float)currentFrame*(float)media.dracoFront.width/3;
        frameFrontHP.x = (float)currentFrame*(float)media.harryFront.width/4;


        //Desenhos:
        BeginDrawing();
        ClearBackground(SKYBLUE);
        DrawTextEx(media.fonteHP, "MARIO POTTER", coordTitle, 150, 5, BLACK);
        for(i=0; i<MENU_OPTIONS; i++){
            if(i==selected)
                color=colorSelected;
            else
                color=colorDefault;
            DrawText(messages[i], posx[i], posy[i], fontSize, color);
        }
        if(time(NULL)%2)
            DrawText("Press ENTER to select", 400, 240, 35, BLACK);

        DrawTextureRec(media.enemies, frameRec, position, WHITE);  // Draw part of the texture
        DrawTextureRec(media.enemies, frameDraco, posDraco, WHITE);
        DrawTextureRec(media.enemies, frameHarry, posHarry, WHITE);
        DrawTextureRec(media.enemies, frameRony, posRony, WHITE);
        DrawTextureRec(media.enemies, frameHermione, posHermione, WHITE);
        DrawTextureRec(media.dracoFront, frameFront, posFront, WHITE);
        DrawTextureRec(media.harryFront, frameFrontHP, posFrontHP, WHITE);
        EndDrawing();
    }while(!IsKeyDown(KEY_ENTER));
    PlaySound(media.heavy_spell);
    delay(20);
    return selected;
}

void delay(int time){
    for(int i=0; i<time; i++){
        BeginDrawing();
        EndDrawing();
    }
}

float align_text_x(char text[], Font aFont, int sizeFont){
    return (SCREEN_WIDTH - MeasureTextEx(aFont, text, sizeFont, 0).x)/2;
}

void about_us(SRC *media){
    int timeCounter=0;
    Vector2 pos[LINES];
    Vector2 posTitle;
    Vector2 posBack;
    Music audio = media->aboutUs;
    int i;
    char title[10]="ABOUT";
    char goBack[30]="Press SPACE to return";
    posTitle.x=align_text_x(title, media->fonteHP, 80);
    posTitle.y=200;
    posBack.x=align_text_x(goBack, media->fonteHP, 40);
    posBack.y=800;
    const int delay=50;
    char messages[LINES][100]={ //Mensagens
        " ",
        "Jade Hatanaka",
        "From Ravenclaw's house",
        "Student of UFRGS",
        "1st semester of Computer Science",
        " ",
        "What's not mine:",
        "Sprite images",
        "Audio",
        "Characters"
    };
    //Alinhamento horizontal:
    for(i=0; i<LINES; i++)
        pos[i].x=align_text_x(messages[i], media->fonteHP, 35);
    //Alinhamento vertical:
    pos[0].y=300;
    for(i=1; i<LINES; i++)
        pos[i].y=pos[i-1].y+45;

    PlayMusicStream(audio); //Tocando a música tema
    SetMusicVolume(audio, 1.0); //Escolhendo o volume da música

    while(!IsKeyDown(KEY_SPACE)){
        timeCounter++;
        UpdateMusicStream(audio);
        if(posTitle.y < -delay) posTitle.y=SCREEN_HEIGHT+delay;
            posTitle.y--;
        for(i=0; i<LINES; i++){
            if(pos[i].y < -delay) pos[i].y=SCREEN_HEIGHT+delay;
            pos[i].y--;
        }
        if(posBack.y < -delay) posBack.y=SCREEN_HEIGHT+delay;
            posBack.y--;

        BeginDrawing();
        ClearBackground(BLACK);
        for(i=0; i<LINES; i++){
            DrawTextEx(media->fonteHP, title, posTitle, 80, 0, RED);
            DrawTextEx(media->fonteHP, messages[i], pos[i], 35, 0, WHITE);
        }
        if(timeCounter%7)
            DrawTextEx(media->fonteHP, goBack, posBack, 40, 0, GRAY);
        EndDrawing();
    }
    PlaySound(media->heavy_spell);
}


















