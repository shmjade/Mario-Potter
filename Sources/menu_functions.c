#include "../Headers/menu_functions.h"
#include "../Headers/all_headers.h"

void prompt_exit_dialog(SRC media){
    int flagEnd = 0;
    after_all(&media, &flagEnd);
    if(flagEnd){
        CloseAudioDevice();
        CloseWindow();
        exit(0);
    }
}

void after_all(SRC *media, int *flagEnd){
    fflush(stdin);
    Vector2 coordTitle = (Vector2){150,75}; //Coordenada do t?tulo
    int timeCounter=0;

    //Tocar m?sica de fundo:
    SetMusicVolume(media->afterAllthisTime, 1.0);
    PlayMusicStream(media->afterAllthisTime);

    //Ret?ngulo Yes:
    Vector2 recYes = {380, 380}; //Coordenadas do ret?ngulo Yes
    Color colorYes = RED; //Come?a vermelho
    //Ret?ngulo No:
    Vector2 recNo = {630, 380}; //Coordenadas do ret?ngulo No
    Color colorNo = RAYWHITE; //Come?a branco


    int input=0; //Escolha do usu?rio, ? o que a fun??o retorna

    //Loop principal (da tecla ENTER):
    while(!IsKeyPressed(KEY_ENTER)){ //Enquanto n?o apertar ENTER
        UpdateMusicStream(media->afterAllthisTime);
        timeCounter++;
        //Atualiza??es:
        if(IsKeyDown(KEY_LEFT)){
            colorYes = MAROON;
            colorNo = RAYWHITE;
            input = 0; //Se input for 0, a resposta ? "Yes"
            PlaySound(media->fast_spell);
        }else if(IsKeyDown(KEY_RIGHT)){
            colorYes = RAYWHITE;
            colorNo = MAROON;
            input = 1; //Se input for 1, a resposta ? "No"
            PlaySound(media->fast_spell);
        }

        //Desenhos:
        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawTextEx(media->fonteHP, "MARIO POTTER", coordTitle, 150, 5, BLACK);
        DrawText("AFTER ALL THIS TIME?", 300, 310, 50, BLACK);
        if(timeCounter%5)
            DrawText("Press ENTER to select", 410, 470, 30, GRAY);
        //Ret?ngulo Yes:
        DrawRectangle(recYes.x, recYes.y, 180, 60, colorYes);
        DrawText("ALWAYS", 410, recYes.y+17, 30, BLACK);
        DrawRectangleLines(recYes.x, recYes.y, 180, 60, BLACK);
        //Ret?ngulo No:
        DrawRectangle(recNo.x, recNo.y, 180, 60, colorNo);
        DrawText("EXIT", 685, recNo.y+17, 30, BLACK);
        DrawRectangleLines(recNo.x, recNo.y, 180, 60, BLACK);
        EndDrawing();
    }
   // UnloadMusicStream(media->afterAllthisTime);
    PlaySound(media->heavy_spell);
    *flagEnd = input;
}


/*
int menu(SRC *media){
    Vector2 coordTitle = (Vector2){140,45}; //Coordenada do t?tulo

    //Inicializa??o
    SetMusicVolume(media->HPthemeSong, 1.0); //Escolhendo o volume da m?sica
    PlayMusicStream(media->HPthemeSong); //Tocando a m?sica tema

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

    //Defini??es:
    SetTargetFPS(60); //Velocidade de reprodu??o do jogo (frames per second)
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
    int selected=0; //posi??o do jogador (inicia na primeira op??o)
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

    //Mensagem espa?ada no eixo y:
    posy[0]=300; //tamanho definido arbitrariamente
    for(i=1; i<MENU_OPTIONS; i++){
        posy[i]=posy[i-1]+fontSize+10; //dist?ncia de 10 entre as mensagens
    }

    //Loop principal (da tecla ENTER):
    do{ //Enquanto n?o apertar enter
        UpdateMusicStream(media->HPthemeSong);
        //Atualiza??es:
        if(IsKeyPressed(KEY_UP)){ //Se apertar tecla para cima
            if(selected==0)    //Se for o primeiro da lista, ir para o ?ltimo
                selected = MENU_OPTIONS-1;
            else
                selected--;    //Sen?o for o primeiro da lista, diminuir o ?ndice (subir uma posi??o)
            PlaySound(media->fast_spell);
        }else if(IsKeyPressed(KEY_DOWN)){ //Se apertar tecla para baixo
            if(selected==MENU_OPTIONS-1) //Se for o ?ltimo da lista, ir para o primeiro
                selected = 0;
            else
                selected++;    //Sen?o for o ?ltimo da lista, aumentar o ?ndice (descer uma posi??o)
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

int menu(SRC media){
    Vector2 coordTitle = (Vector2){140,45}; //Coordenada do t?tulo

    //Inicializa??o
    SetMusicVolume(media.HPthemeSong, 1.0); //Escolhendo o volume da m?sica
    PlayMusicStream(media.HPthemeSong); //Tocando a m?sica tema

    Vector2 posFront = {900, 400};
    Vector2 posFrontHP = {200, 400};
    Vector2 posDraco = {-100, 637};
    Vector2 posHarry = {100, 637};
    Rectangle frameDraco = { 0.0f, 0.0f, (float)media.dracoRight.width/3, (float)media.dracoRight.height };
    Rectangle frameHarry = { 0.0f, 0.0f, (float)media.harryRight.width/6, (float)media.harryRight.height };
    Rectangle frameFront = { 0.0f, 0.0f, (float)media.dracoFront.width/3, (float)media.dracoFront.height };
    Rectangle frameFrontHP = { 0.0f, 0.0f, (float)media.harryFront.width/4, (float)media.harryFront.height };
    int currentFrame = 0, currentFrameD = 0, currentFrameH = 0;
    int framesCounter = 0;
    int framesSpeed = 6;

    //Defini??es:
    SetTargetFPS(60); //Velocidade de reprodu??o do jogo (frames per second)
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
    int selected=0; //posi??o do jogador (inicia na primeira op??o)
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

    //Mensagem espa?ada no eixo y:
    posy[0]=300; //tamanho definido arbitrariamente
    for(i=1; i<MENU_OPTIONS; i++){
        posy[i]=posy[i-1]+fontSize+10; //dist?ncia de 10 entre as mensagens
    }

    //Loop principal (da tecla ENTER):
    do{ //Enquanto n?o apertar enter
        UpdateMusicStream(media.HPthemeSong);
        //ESC: mostrar tela de sa?da
        if(IsKeyPressed(KEY_ESCAPE)) prompt_exit_dialog(media);
        //Atualiza??es:
        if(IsKeyPressed(KEY_UP)){ //Se apertar tecla para cima
            if(selected==0)    //Se for o primeiro da lista, ir para o ?ltimo
                selected = MENU_OPTIONS-1;
            else
                selected--;    //Sen?o for o primeiro da lista, diminuir o ?ndice (subir uma posi??o)
            PlaySound(media.fast_spell);
        }else if(IsKeyPressed(KEY_DOWN)){ //Se apertar tecla para baixo
            if(selected==MENU_OPTIONS-1) //Se for o ?ltimo da lista, ir para o primeiro
                selected = 0;
            else
                selected++;    //Sen?o for o ?ltimo da lista, aumentar o ?ndice (descer uma posi??o)
            PlaySound(media.fast_spell);
        }
        if(posDraco.x>SCREEN_WIDTH)
            posDraco.x=0;
        else
            posDraco.x+=3;
        if(posHarry.x>SCREEN_WIDTH)
            posHarry.x=0;
        else
            posHarry.x+=3;
        framesCounter++;
        if (framesCounter >= (60/framesSpeed)){
            framesCounter = 0;
            currentFrame++;
            currentFrameD++;
            currentFrameH++;
        }
        if(currentFrame>2)
            currentFrame = 0;
        if(currentFrameD>2)
            currentFrameD = 0;
        if(currentFrameH>5)
            currentFrameH = 0;

        frameDraco.x = (float)currentFrameD*(float)media.dracoRight.width/3;
        frameHarry.x = (float)currentFrameH*(float)media.harryRight.width/6;
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

        DrawTextureRec(media.dracoRight, frameDraco, posDraco, WHITE);
        DrawTextureRec(media.harryRight, frameHarry, posHarry, WHITE);
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
    char goBack[30]="Press ENTER to return";
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

    PlayMusicStream(audio); //Tocando a m?sica tema
    SetMusicVolume(audio, 1.0); //Escolhendo o volume da m?sica

    while(!IsKeyPressed(KEY_ENTER) && !IsKeyPressed(KEY_M)){
        timeCounter++;
        UpdateMusicStream(audio);
        if(IsKeyPressed(KEY_ESCAPE)) prompt_exit_dialog(*media);
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


















