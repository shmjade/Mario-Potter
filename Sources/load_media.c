#include "../Headers/load_media.h"


PHASE load_phase(char fullTitle[MAX_CHAR_NAME], SRC media){
    PHASE thisPhase;
    thisPhase.numPlats=0;
    thisPhase.numTubes=0;
    thisPhase.numCoins=0;
    thisPhase.deadEnemies=0;
    thisPhase.activeEnemies1=0;
    thisPhase.activeEnemies2=0;
    thisPhase.hero.lives=MAX_LIVES;
    thisPhase.hero.points=0;
    thisPhase.level=1;
    thisPhase.powerHits=MAX_POWER;

    FILE *phase;
    char letter; //caracteres do arquivo
    int lin=0, col=0; //contador para linhas e colunas
    int p=0, t=0, r=0;


    ///Abertura do arquivo
    phase = fopen(fullTitle, "r"); //abertura para leitura
    if(phase==NULL){ //NULL indica erro na abertura do arquivo
        printf("Erro na abertura");
        thisPhase.level=-1;
        printf("%d", thisPhase.level);
    }
    else{//Abriu corretamente, coletaremos os dados do arquivo da fase:
        while(!feof(phase)){
            letter = fgetc(phase);
            switch(letter){
                case 'p' : thisPhase.numPlats++;
                            break;
                case 'P' : thisPhase.numPlats++;
                            break;
                case 'c' : thisPhase.numTubes++;
                            break;
                case 'C' : thisPhase.numTubes++;
                            break;
                case '\n' : col=0; lin++;
                            break;
            }
        }
    }
    printf("numPlats=%d, numTubes=%d", thisPhase.numPlats, thisPhase.numTubes);
    rewind(phase);


    ///------------------Encontrando os retornos--------------------
    char numReturn[9]="123456789";
    char collect;
    lin=0;
    col=0;
    while(!feof(phase) && lin!=28){
        collect = fgetc(phase);
        for(int j=0; j<9; j++){
            if(collect==numReturn[j]){
                thisPhase.returns[r].x = col*10;
                thisPhase.returns[r].y = lin*25;
                thisPhase.returns[r].height=media.recFireplace.height;
                thisPhase.returns[r].width=media.recFireplace.width;
                r++;
            }
        }
        col++;
        if(collect=='\n' || collect=='\r'){
            col=0;
            lin++;
        }
    }
    rewind(phase);

    ///--------Leitura de mais dados-------
    lin=0;
    col=0;
    while(!feof(phase) && lin!=28){
        letter = fgetc(phase);
        ///Procurando o Mario/Harry:
        if(letter=='m' || letter=='M'){
            thisPhase.hero.rec.x=col*10;
            thisPhase.hero.rec.y=lin*25;
            thisPhase.hero.rec.width = media.recFrameEnemies.width;
            thisPhase.hero.rec.height = media.recFrameEnemies.height;
        }
        ///Procurando o botão Power:
        if(letter=='b' || letter=='B'){
            thisPhase.recPower.x=col*10;
            thisPhase.recPower.y=lin*25;
            thisPhase.recPower.width=POWER_WIDTH;
            thisPhase.recPower.height=POWER_HEIGHT;
        }
        ///Procurando as plataformas:
        if(letter=='p' || letter=='P'){
            thisPhase.plats[p].rec.x=col*10;
            thisPhase.plats[p].rec.y=lin*25;
            thisPhase.plats[p].rec.height=25;
            thisPhase.plats[p].rec.width=10;
            p++;
        }
        ///Procurando os canos:
        if(letter=='c' || letter=='C'){
            thisPhase.tubes[t].x=col*10;
            thisPhase.tubes[t].y=lin*25;
            thisPhase.tubes[t].height=media.recFireplace.height;
            thisPhase.tubes[t].width=media.recFireplace.width;
            t++;
        }
        col++;
        if(letter=='\n' || letter=='\r'){
            col=0;
            lin++;
        }
    }

    ///--------Leitura da última linha-------
    fscanf(phase, "%d %d %d", &thisPhase.numEnem1, &thisPhase.numEnem2, &thisPhase.delayEnemy);

    ///--------Inicializando todos os inimigos de forma inativa----------
    ENEMY notBorn = {0, 0, 0, 0, 0, {2000, 2000, 0, 0}};
    for(int p=0; p<thisPhase.numEnem1; p++)
        thisPhase.enemies1[p] = notBorn;
    for(int p=0; p<thisPhase.numEnem2; p++)
        thisPhase.enemies2[p] = notBorn;
    printf("\nMario: posX = %.0f, posY = %.0f", thisPhase.hero.rec.x, thisPhase.hero.rec.y);
    printf("\nEnemy1: %d, enemy2: %d, delay: %d\n", thisPhase.numEnem1, thisPhase.numEnem2, thisPhase.delayEnemy);
    fclose(phase);

    return thisPhase;
}


void load_media(SRC *media){
    media->fonteHP = LoadFont("Fonts/fonteHP.TTF");
    media->afterAllthisTime = LoadMusicStream("Audio/afterAllThisTime.mp3");
    media->aboutUs = LoadMusicStream("Audio/about.mp3");
    media->HPthemeSong = LoadMusicStream("Audio/HPthemeSong.mp3");
    media->names = LoadMusicStream("Audio/names.mp3");
    media->fast_spell = LoadSound("Audio/fastSpell.wav");
    media->heavy_spell = LoadSound("Audio/heavySpell.wav");
    media->expelliarmus = LoadSound("Audio/expelliarmus.wav");
    media->expectro = LoadSound("Audio/patronum.wav");
    media->harryDied = LoadSound("Audio/harryDied.wav");
    media->harryRight = LoadTexture("Images/harryRight.png");
    media->harryLeft = LoadTexture("Images/harryLeft.png");
    media->harryFront = LoadTexture("Images/harryFront.png");
    media->dracoRight = LoadTexture("Images/dracoRight.png");
    media->dracoFront = LoadTexture("Images/dracoFront.png");
    media->fireplace = LoadTexture("Images/fireplace.png");
    media->fireplaceGreen = LoadTexture("Images/fireplaceGreen.png");
    media->ground = LoadTexture("Images/ground.png");
    media->broom = LoadTexture("Images/broom.png");
    media->enemies = LoadTexture("Images/enemies.png");
    media->recFrameEnemies.width = (float)media->enemies.width/12;
    media->recFrameEnemies.height = (float)media->enemies.height/8;
    media->recFrameEnemies.x = (float)media->enemies.width/12;
    media->recFrameEnemies.y = (float)media->enemies.height/8;
    media->patronumEffect = LoadTexture("Images/patronum.png");
    media->recFramePatronum.width = (float)media->patronumEffect.width/5;
    media->recFramePatronum.height = (float)media->patronumEffect.height/3;
    media->recFramePatronum.x = (float)media->patronumEffect.width/5;
    media->recFramePatronum.y = (float)media->patronumEffect.height/3;    media->recFramePatronum.width = (float)media->patronumEffect.width/5;
    media->recFireplace.height = (float)media->fireplace.height;
    media->recFireplace.width = (float)media->fireplace.width;
    media->houses = LoadTexture("Images/house_cup.png");
    media->recFrameHouses.width = (float)media->houses.width/2;
    media->recFrameHouses.height = (float)media->houses.height/2;
    media->recFrameHouses.x = (float)media->houses.width/2;
    media->recFrameHouses.y = (float)media->houses.height/2;
    media->podium = LoadTexture("Images/podium.png");
    media->letter = LoadTexture("Images/letter.png");
    media->gryffindor = LoadTexture("Images/gryffindor.png");
    media->hufflepuff = LoadTexture("Images/hufflepuff.png");
    media->ravenclaw = LoadTexture("Images/ravenclaw.png");
    media->slytherin = LoadTexture("Images/slytherin.png");
}


void load_map(SRC media){
    //Coleta o nome do arquivo a ser aberto:
    char title[MAX_CHAR_NAME];
    get_string("Type the file's name", title, media); //pega o nome com a interface da função get_name
    char fullTitle[MAX_CHAR_NAME] = "Phases/";
    strcat(fullTitle, title); //concatena para colocar a localização correta do arquivo
    puts(fullTitle);
    PHASE phase = load_phase(fullTitle, media);
    phase.activeEnemies1=0;
    phase.activeEnemies2=0;
    phase.numCoins=0;
    phase.hero.points=0;
    phase.hero.lives=3;
    if(phase.level==-1)
        printf("File not found");
    else
        play_phase(&phase, media, 0, "jogolivre");
}




void load_journey(PHASE journey[JOURNEY_SIZE], SRC media){
    journey[0] = load_phase("Phases/fase1.txt", media);
    journey[1] = load_phase("Phases/fase2.txt", media);
    journey[2] = load_phase("Phases/fase3.txt", media);
}

