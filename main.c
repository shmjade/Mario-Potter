#include "Headers/all_headers.h"

int main(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MARIO POTTER");
    SetTargetFPS(60);
    InitAudioDevice();
    SRC media;
    load_media(&media);
    int flagEnd=FALSE;
    int choiceMenu;
    while(!flagEnd){
        choiceMenu = menu(media);
        switch(choiceMenu){
            case 0 : play_new_game(media);
                break;
            case 1 : continue_game(media);
                break;
            case 2 : load_map(media);
                break;
            case 3 : ranking_screen(media);
                break;
            case 4 : sorting_hat(media);
                break;
            case 5 : about_us(&media);
                break;
            case 6 : after_all(&media, &flagEnd);
                break;
        }
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}


