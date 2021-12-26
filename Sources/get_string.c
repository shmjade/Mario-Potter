#include "../Headers/get_string.h"
#define MAX_NAME 20
#include <time.h>

///--------------Interface que pega uma string digitada pelo usuário e retorna para uma string de destino
void get_string(char titleWindow[30], char answer[], SRC media){
    const float fontSize = 100;
    float posx = (SCREEN_WIDTH-(MeasureTextEx(media.fonteHP, titleWindow, fontSize, 0).x))/2;
    Vector2 posTitle = {posx, 100};
    char name[MAX_NAME+1]="\0";
    int letterCount = 0;
    Rectangle textBox = {SCREEN_WIDTH*0.2, 300, SCREEN_WIDTH*0.6, 80};
    int timeCounter=0;
    PlayMusicStream(media.names);
    do{
        //-----Update-----
        UpdateMusicStream(media.names);
        timeCounter++;
        int key=GetCharPressed();
        if ((key >= 32) && (key <= 125) && (letterCount < MAX_NAME)){
            name[letterCount] = (char)key;
            name[letterCount+1] = '\0'; // Add null terminator at the end of the string.
            letterCount++;
            PlaySound(media.fast_spell);
        }
        if(IsKeyPressed(KEY_BACKSPACE)){
            letterCount--;
            if (letterCount < 0)
                letterCount = 0;
            name[letterCount] = '\0';
            PlaySound(media.fast_spell);
        }

        //------Draw-------
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextEx(media.fonteHP, titleWindow, posTitle, fontSize, 0, GOLD);
        DrawRectangleRec(textBox, RAYWHITE);
        DrawText(name, textBox.x + 5, textBox.y + 8, 60, MAROON);
        DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, MAX_NAME), 450, 400, 30, GRAY);
        if(timeCounter%5){
            DrawText("Press BACKSPACE to delete", 380, 460, 30, GRAY);
            DrawText("Press SPACE when done", 410, 500, 30, RED);
        }
        EndDrawing();
    }while(!IsKeyPressed(KEY_SPACE));
    strcpy(answer, name);
    PlaySound(media.heavy_spell);
}
/*
int get_house(SRC media){
    PlayMusicStream(media.names);
    do{
        //-----Update-----
        UpdateMusicStream(media.names);
        timeCounter++;
        int key=GetCharPressed();
        if ((key >= 32) && (key <= 125) && (letterCount < MAX_NAME)){
            name[letterCount] = (char)key;
            name[letterCount+1] = '\0'; // Add null terminator at the end of the string.
            letterCount++;
            PlaySound(media.fast_spell);
        }
        if(IsKeyPressed(KEY_BACKSPACE)){
            letterCount--;
            if (letterCount < 0)
                letterCount = 0;
            name[letterCount] = '\0';
            PlaySound(media.fast_spell);
        }

        //------Draw-------
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextEx(media.fonteHP, titleWindow, posTitle, fontSize, 0, GOLD);
        DrawRectangleRec(textBox, RAYWHITE);
        DrawText(name, textBox.x + 5, textBox.y + 8, 60, MAROON);
        DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, MAX_NAME), 450, 400, 30, GRAY);
        if(timeCounter%5){
            DrawText("Press BACKSPACE to delete", 380, 460, 30, GRAY);
            DrawText("Press SPACE when done", 410, 500, 30, RED);
        }
        EndDrawing();
    }while(!IsKeyPressed(KEY_SPACE));
}
*/
