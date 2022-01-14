#include "../Headers/game_functions.h"


///----------Jogar um novo jogo---------
void play_new_game(SRC media){
    //Inicializa��es:
    GAME newGame;
    int level=0;
    PHASE journey[JOURNEY_SIZE];
    load_journey(journey, media);
    int flag_next=0, flag_win=0, flag_save=0;

    //Coletar o nome do jogador:
    get_string("What's your name?", newGame.namePlayer, media);
    int hero = choose_who(media);
    newGame.phase.hero.who = hero;
    catch_letter(newGame.phase.hero, media);
    letter(media);
    hat(media);
    newGame.house = sorting_hat(media);
    //Jogar:
    do{
        if(flag_save)
            save_phase(newGame.phase, newGame.namePlayer);
        newGame.phase = journey[level];
        newGame.phase.level=level+1;
        newGame.phase.hero.who = hero;
        printf("fase: %d", newGame.phase.level);
        flag_next=play_phase(&newGame.phase, media, &flag_save, newGame.namePlayer);
        if(flag_next && !flag_win){
            PlaySound(media.nextPhase);
            next_phase(media);
            level++;
            update_phase(&newGame, journey);
        }
        if(level==JOURNEY_SIZE-1)
            flag_win=1;

    }while(flag_next && !flag_win);
    update_ranking(newGame);
    if(flag_win)
       win_game(media);
}

///------------Jogar uma fase-------------
int play_phase(PHASE *phase, SRC media, int *flag_save, char namePlayer[]){
    int flag_die=0, flag_win=0;
    int timeEnemy=0, timeCoin=0, enemyType=0;
    srand(time(NULL));
    PlayMusicStream(media.HPthemeSong);
    while(!flag_die && !flag_win){
        UpdateMusicStream(media.HPthemeSong);
        //Se pressionar A, salvar o jogo:
        if(IsKeyPressed(KEY_A)) save_phase(*phase, namePlayer);
        if(phase->hero.afterDeath && IsKeyPressed(KEY_SPACE))
            phase->hero.afterDeath=0;
        if(phase->hero.afterDeath != 1)
            move_hero(phase, media);
        draw_phase(*phase, media);
        draw_hero(phase->hero, media);
        timeEnemy++;
        timeCoin++;
        if(timeEnemy==60*phase->delayEnemy && (phase->activeEnemies1+phase->activeEnemies2) < (phase->numEnem1+phase->numEnem2)){
            printf("Create enemy, time: %d", timeEnemy);
            if(enemyType%2 && phase->activeEnemies1<phase->numEnem1){
                create_enemy1(phase, media);
            }else if(phase->activeEnemies2<phase->numEnem2){
                create_enemy2(phase, media);
            }
            timeEnemy=0; //para criar mais inimigos futuramente
            enemyType++; //para alternar o tipo de inimigo
        }
        if(timeCoin==60*TIME_COIN){
            create_coin(phase);
            timeCoin=0;
        }

        move_enemies(phase);

        for(int i=0; i<phase->activeEnemies1; i++){
            draw_enm1(phase->enemies1[i], media);
            enter_return(&phase->enemies1[i].rec, phase, &phase->enemies1[i].direction);
        }

        for(int i=0; i<phase->activeEnemies2; i++){
            draw_enm2(phase->enemies2[i], media);
            enter_return(&phase->enemies2[i].rec, phase, &phase->enemies2[i].direction);
        }
        for(int i=0; i<phase->numCoins; i++){
            DrawRectangleRec(phase->coins[i].coinRec, GOLD);
        }
        collect_coin(phase);

        if(phase->hero.lives==0) flag_die=1;
        if(phase->deadEnemies == phase->numEnem1 + phase->numEnem2) flag_win=1;
    }
    return flag_win; //se venceu, retorna 1, sen�o, retorna 0
    printf("Flag win da play phase: %d", flag_win);
}


///------------Atualiza os dados ao passar de fase-----------
//Pega os dados da fase anterior e transfere para esta:
void update_phase(GAME *game, PHASE journey[]){
    journey[game->phase.level].hero.points += game->phase.hero.points; //soma os pontos conquistados na fase anterior com os da nova (0)
    journey[game->phase.level].hero.lives = game->phase.hero.lives; //mant�m a quantidade de vidas do final da fase anterior
    journey[game->phase.level].powerHits = game->phase.powerHits; //mant�m a quantidade de vezes que pode usar o bot�o Power
}
/*
//Pega os dados da fase anterior e transfere para esta:
void update_phase(GAME *game, PHASE journey[]){
    journey[game->phase.level].hero.points += game->phase.hero.points; //soma os pontos conquistados na fase anterior com os da nova (0)
    journey[game->phase.level].hero.lives = game->phase.hero.lives; //mant�m a quantidade de vidas do final da fase anterior
    journey[game->phase.level].powerHits = game->phase.powerHits; //mant�m a quantidade de vezes que pode usar o bot�o Power
}
*/
int choose_who (SRC media){
    delay(20);
    int choice=HARRY; //Por defini��o: HERMIONE=0, RONY=1, HARRY=2, NEVILLE=3
    char title[17] = "POLYJUICE POTION";
    char subtitle[30] = "choose who you want to be";
    int fontSize=80;
    int fontDescr=40;
    char names[4][10] = {"HERMIONE", "RONY", "HARRY", "NEVILLE"};
    char descr[4][30] = {"The girl who knew", "The boy who doubted", "The boy who lived", "The boy who could have been"};
    Vector2 position={(SCREEN_WIDTH-media.recFrameWho.width)/2, 300};
    Vector2 posDescr={0,530};
    Vector2 posNames={0,230};
    Rectangle frameRec = media.recFrameWho;
    frameRec.y = media.recFrameWho.y*4;
    Sound voice[4];
    voice[HERMIONE] = media.imHermione;
    voice[HARRY] = media.imHarry;
    voice[RONY] = media.imRony;
    voice[NEVILLE] = media.imNeville;
    Vector2 posTitle = (Vector2) {(SCREEN_WIDTH - MeasureTextEx(media.fonteHP, title, fontSize, 5).x)/2, 50};
    Vector2 posSub = (Vector2) {(SCREEN_WIDTH - MeasureTextEx(media.fonteHP, subtitle, fontSize/2, 5).x)/2, 140};
    while(!IsKeyPressed(KEY_ENTER)){
        if(IsKeyPressed(KEY_LEFT)){
            if(choice==0)
                choice=3;
            else
                choice--;
            PlaySound(voice[choice]);
        }
        if(IsKeyPressed(KEY_RIGHT)){
            if(choice==3)
                choice=0;
            else
                choice++;
            PlaySound(voice[choice]);
        }
        frameRec.x=(2+choice*3)*media.recFrameWho.x;
        posDescr.x = (SCREEN_WIDTH-MeasureTextEx(media.fonteHP, descr[choice], fontDescr, 5).x)/2;
        posNames.x = (SCREEN_WIDTH-MeasureTextEx(media.fonteHP, names[choice], fontDescr, 5).x)/2;
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTextEx(media.fonteHP, title, posTitle, fontSize, 5, WHITE);
        DrawTextEx(media.fonteHP, names[choice], posNames, fontDescr, 5, WHITE);
        DrawTextEx(media.fonteHP, descr[choice], posDescr, fontDescr, 5, WHITE);
        DrawTextEx(media.fonteHP, subtitle, posSub, fontSize/2, 5, WHITE);
        DrawTextureRec(media.who, frameRec, position, WHITE);
        EndDrawing();
    }
    printf("\n\tChoice: %d", choice);
    return choice;
}


///----------------Carrega o jogo  de um arquivo txt----------------

///-----------------Salva o jogo em um arquivo txt------------------
/*Para jogar um jogo antigo, precisamos salv�-lo de forma adequada,
armazenando todas as informa��es necess�rias, conforme:
- NOME DO JOGADOR: ser� o identificador para abrir o nosso jogo antigo
- FASE DO JOGO: guardar� as informa��es do jogo em si, contendo:
    - o n�vel da fase (level) para continuar a campanha;
    - As informa��es sobre o bot�o power:
        - Sua posi��o na tela, altura e largura (recPower);
        - Quantas vezes ele ainda pode ser usado (no m�ximo 3 ao longo do jogo);
    - As informa��es sobre o her�i/jogador (HERO):
        - sua velocidade horizontal (speedX);
        - sua dire��o de deslocamento (direction);
        - sua posi��o na tela, sua altura e largura (rec);
        - se pode pular (jump) - 1 se est� em cima do "ch�o" ou 0 se est�
          pulando ou caindo;
        - se est� subindo (jumping);
        - se est� caindo (falling);
        - se est� parado (stop);
        - se pode ir para a direita, ou seja, se n�o est� colidindo com o cen�rio
          est�tico (right);
        - se pode ir para a esquerda, ou seja, se n�o est� colidindo com o cen�rio
          est�tico (left);
        - a quantidade de vidas (lives);
        - a pontua��o do jogador (points);
        - se est� no per�odo ap�s ter morrido (afterDeath);
    - As informa��es sobre os inimigos:
        - delay na libera��o entre os inimigos (delayEnemy);
        - quantidade de inimigos j� derrotados na fase (deadEnemies);
        - Tipo 1:
            - quantidade de inimigos do tipo 1 na fase (numEnem1);
            - quantidade de inimigos do tipo 1 que j� foram liberados na fase (activeEnemies1);
            - vetor com inimigos do tipo 1, contendo:
                - se j� foi ativado ou n�o (alive);
                - seu tipo (level);
                - seu status (NORMAL, WEAK, DEAD);
                - sua velocidade de deslocamento na horizontal (speedX);
                - sua dire��o de deslocamento (direction);
                - seu tamanho e posi��o na tela (rec);
        - Tipo 2:
            - quantidade de inimigos do tipo 2 na fase (numEnem2);
            - quantidade de inimigos do tipo 2 que j� foram liberados na fase (activeEnemies2);
            - vetor com inimigos do tipo 2, contendo:
                - se j� foi ativado ou n�o (alive);
                - seu tipo (level);
                - seu status (NORMAL, WEAK, DEAD);
                - sua velocidade de deslocamento na horizontal (speedX);
                - sua dire��o de deslocamento (direction);
                - seu tamanho e posi��o na tela (rec);
    - As informa��es sobre as plataformas:
        - quantidade de plataformas na fase (cada p foi considerado uma plataforma);
        - arranjo de plataformas (PLATFORMS), contendo:
            - sua posi��o na tela;
            - sua largura e altura;
    - As informa��es sobre os canos/retornos:
        - quantidade de canos/retornos;
        - posi��o de cada cano/retorno;
    - As informa��es sobre as moedas:
        - quantidade de moedas j� liberadas na fase;
        - arranjo de moedas (COINS) contendo:
            - seu status (ativa ou inativa) para liberar ou n�o a movimenta��o;
            - sua velocidade de deslocamento na horizontal (speedX);
            - sua dire��o de deslocamento (direction);
            - sua posi��o na tela, sua altura e largura (coinRec).
Todas essas informa��es est�o armazenadas na nossa struct GAME, portanto, podemos
simplesmente armazenar as informa��es de acordo com a nossa struct. Para isso, faremos: */

void save_phase(PHASE phase, char namePlayer[]){
    //Declara��o do ponteiro de arquivo:
    FILE *arq;
    printf("\nSave phase\n");
    int i;

    //Abriremos um arquivo com o mesmo nome do jogador para salvar as informa��es:
    if(!(arq = fopen(namePlayer, "w"))) // abre para escrita
        printf("Erro na abertura\n");
    else{
        fprintf(arq,"%d \n", phase.level);
        ///--------Informa��es do bot�o Power---------
        fprintf(arq,"%d \n", phase.powerHits);
        ///------------Informa��es do her�i-----------
        fprintf(arq,"%d \n", phase.hero.speedX);
        fprintf(arq,"%d \n", phase.hero.direction);
        fprintf(arq,"%f %f\n", phase.hero.rec.x, phase.hero.rec.y);
        fprintf(arq,"%d \n", phase.hero.jump);
        fprintf(arq,"%d \n", phase.hero.goingUp);
        fprintf(arq,"%d \n", phase.hero.falling);
        fprintf(arq,"%d \n", phase.hero.stop);
        fprintf(arq,"%d \n", phase.hero.left);
        fprintf(arq,"%d \n", phase.hero.right);
        fprintf(arq,"%d \n", phase.hero.lives);
        fprintf(arq,"%d \n", phase.hero.points);
        fprintf(arq,"%d \n", phase.hero.afterDeath);
        fprintf(arq,"%d \n", phase.hero.who);
        ///----------Informa��es dos inimigos------------
        fprintf(arq,"%d \n", phase.activeEnemies1);
        fprintf(arq,"%d \n", phase.activeEnemies2);
        fprintf(arq,"%d \n", phase.deadEnemies);
        for(i=0; i<phase.activeEnemies1; i++){
            fprintf(arq, "%d \n", phase.enemies1[i].alive);
            fprintf(arq, "%d \n", phase.enemies1[i].direction);
            fprintf(arq, "%d \n", phase.enemies1[i].level);
            fprintf(arq, "%f %f \n", phase.enemies1[i].rec.x, phase.enemies1[i].rec.y);
            fprintf(arq, "%d \n", phase.enemies1[i].speedX);
            fprintf(arq, "%d \n", phase.enemies1[i].status);
        }
        for(i=0; i<phase.activeEnemies1; i++){
            printf("alive: %d \n", phase.enemies1[i].alive);
            printf("direction: %d \n", phase.enemies1[i].direction);
            printf("level: %d \n", phase.enemies1[i].level);
            printf("x: %f y: %f \n", phase.enemies1[i].rec.x, phase.enemies1[i].rec.y);
            printf("speedX: %d \n", phase.enemies1[i].speedX);
            printf("Status: %d \n", phase.enemies1[i].status);
        }

        for(i=0; i<phase.activeEnemies2; i++){
            printf("alive: %d \n", phase.enemies2[i].alive);
            printf("direction: %d \n", phase.enemies2[i].direction);
            printf("level: %d \n", phase.enemies2[i].level);
            printf("x: %f y: %f \n", phase.enemies2[i].rec.x, phase.enemies2[i].rec.y);
            printf("speedX: %d \n", phase.enemies2[i].speedX);
            printf("Status: %d \n", phase.enemies2[i].status);
        }
        for(i=0; i<phase.activeEnemies2; i++){
            fprintf(arq, "%d \n", phase.enemies2[i].alive);
            fprintf(arq, "%d \n", phase.enemies2[i].direction);
            fprintf(arq, "%d \n", phase.enemies2[i].level);
            fprintf(arq, "%f %f \n", phase.enemies2[i].rec.x, phase.enemies2[i].rec.y);
            fprintf(arq, "%d \n", phase.enemies2[i].speedX);
            fprintf(arq, "%d \n", phase.enemies2[i].status);
        }
        ///----------Informa��es sobre as moedas---------
        fprintf(arq, "%d \n", phase.numCoins);
        for(i=0; i<phase.numCoins; i++){
            fprintf(arq, "%d \n", phase.coins[i].status);
            fprintf(arq, "%d \n", phase.coins[i].speedX);
            fprintf(arq, "%d \n", phase.coins[i].direction);
            fprintf(arq, "%f, %f \n", phase.coins[i].coinRec.x, phase.coins[i].coinRec.y);
        }
    }
    fclose(arq);
}


///-------------Carregar uma fase antiga----------------
PHASE load_old_phase(PHASE staticPhase, char namePlayer[], SRC media){
    PHASE loaded;
    int i;
    ///----------informa��es est�ticas---------
    loaded.numEnem1=staticPhase.numEnem1;
    loaded.numEnem2=staticPhase.numEnem2;
    loaded.delayEnemy=staticPhase.delayEnemy;
    loaded.numPlats=staticPhase.numPlats;
    loaded.numTubes=staticPhase.numTubes;
    loaded.recPower.x=staticPhase.recPower.x;
    loaded.recPower.y=staticPhase.recPower.y;
    loaded.recPower.width=staticPhase.recPower.width;
    loaded.recPower.height=staticPhase.recPower.height;

    for(i=0; i<staticPhase.numTubes; i++){
        loaded.tubes[i].x = staticPhase.tubes[i].x;
        loaded.tubes[i].y = staticPhase.tubes[i].y;
        loaded.tubes[i].width = staticPhase.tubes[i].width;
        loaded.tubes[i].height = staticPhase.tubes[i].height;
    }
    for(i=0; i<staticPhase.numTubes; i++){
        loaded.returns[i].x = staticPhase.returns[i].x;
        loaded.returns[i].y = staticPhase.returns[i].y;
        loaded.returns[i].width = staticPhase.returns[i].width;
        loaded.returns[i].height = staticPhase.returns[i].height;
    }
    for(i=0; i<staticPhase.numPlats; i++){
        loaded.plats[i].rec.x = staticPhase.plats[i].rec.x;
        loaded.plats[i].rec.y = staticPhase.plats[i].rec.y;
        loaded.plats[i].rec.width = staticPhase.plats[i].rec.width;
        loaded.plats[i].rec.height = staticPhase.plats[i].rec.height;
    }
    loaded.hero.rec.height = staticPhase.hero.rec.height;
    loaded.hero.rec.width = staticPhase.hero.rec.width;

    ///----------Atualizando informa��es m�veis-------------
    //Declara��o do ponteiro de arquivo:
    FILE *arq;
    printf("\nLoad game\n");
    ///Abertura do arquivo
    arq = fopen(namePlayer, "r"); //abertura para leitura
    if(arq==NULL){ //NULL indica erro na abertura do arquivo
        printf("Erro na abertura");
    }else{//Abriu corretamente, coletaremos os dados do arquivo da fase a ser carregada:
            printf("Loading");
            fscanf(arq, "%d %d", &loaded.level, &loaded.powerHits);
            ///---------Informa��es do her�is--------------
            fscanf(arq, "%d %d", &loaded.hero.speedX, &loaded.hero.direction);
            fscanf(arq, "%f %f", &loaded.hero.rec.x, &loaded.hero.rec.y);
            fscanf(arq, "%d", &loaded.hero.jump);
            fscanf(arq, "%d", &loaded.hero.goingUp);
            fscanf(arq, "%d", &loaded.hero.falling);
            fscanf(arq, "%d", &loaded.hero.stop);
            fscanf(arq, "%d", &loaded.hero.left);
            fscanf(arq, "%d", &loaded.hero.right);
            fscanf(arq, "%d", &loaded.hero.lives);
            fscanf(arq, "%d", &loaded.hero.points);
            fscanf(arq, "%d", &loaded.hero.afterDeath);
            fscanf(arq, "%d", &loaded.hero.who);

            ///---------Informa��es dos inimigos-----------
            fscanf(arq, "%d", &loaded.activeEnemies1);
            fscanf(arq, "%d", &loaded.activeEnemies2);
            fscanf(arq, "%d", &loaded.deadEnemies);

            ///---------Inicializando os inimigos como inativos---------
            ENEMY notBorn = {0, 0, 0, 0, 0, {2000, 2000, 0, 0}};
            for(int p=0; p<loaded.numEnem1; p++)
                loaded.enemies1[p] = notBorn;
            for(int p=0; p<loaded.numEnem2; p++)
                loaded.enemies2[p] = notBorn;
            ///------------Atualizando os dados dos inimigos------------
            for(i=0; i<loaded.activeEnemies1; i++){
                loaded.enemies1[i].rec.height = media.recFrameEnemies.height;
                loaded.enemies1[i].rec.width = media.recFrameEnemies.width;
                fscanf(arq, "%d", &loaded.enemies1[i].alive);
                fscanf(arq, "%d", &loaded.enemies1[i].direction);
                fscanf(arq, "%d", &loaded.enemies1[i].level);
                fscanf(arq, "%f", &loaded.enemies1[i].rec.x);
                fscanf(arq, "%f", &loaded.enemies1[i].rec.y);
                fscanf(arq, "%d", &loaded.enemies1[i].speedX);
                fscanf(arq, "%d", &loaded.enemies1[i].status);
            }
            for(i=0; i<loaded.activeEnemies2; i++){
                loaded.enemies2[i].rec.height = media.recFrameEnemies.height;
                loaded.enemies2[i].rec.width = media.recFrameEnemies.width;
                fscanf(arq, "%d", &loaded.enemies2[i].alive);
                fscanf(arq, "%d", &loaded.enemies2[i].direction);
                fscanf(arq, "%d", &loaded.enemies2[i].level);
                fscanf(arq, "%f", &loaded.enemies2[i].rec.x);
                fscanf(arq, "%f", &loaded.enemies2[i].rec.y);
                fscanf(arq, "%d", &loaded.enemies2[i].speedX);
                fscanf(arq, "%d", &loaded.enemies2[i].status);
            }

            ///------------Informa��es das moedas--------------
            fscanf(arq, "%d", &loaded.numCoins);
            for(i=0; i<loaded.numCoins; i++){
                fscanf(arq, "%d", &loaded.coins[i].status);
                fscanf(arq, "%d", &loaded.coins[i].speedX);
                fscanf(arq, "%d", &loaded.coins[i].direction);
                fscanf(arq, "%f %f", &loaded.coins[i].coinRec.x, &loaded.coins[i].coinRec.y);
            }
    }
    fclose(arq);
    printf("Carregando: \n");
        for(i=0; i<loaded.activeEnemies1; i++){
            printf("alive: %d \n", loaded.enemies1[i].alive);
            printf("direction: %d \n", loaded.enemies1[i].direction);
            printf("level: %d \n", loaded.enemies1[i].level);
            printf("x: %f y: %f \n", loaded.enemies1[i].rec.x, loaded.enemies1[i].rec.y);
            printf("speedX: %d \n", loaded.enemies1[i].speedX);
            printf("Status: %d \n", loaded.enemies1[i].status);
            printf("Altura: %f \n", loaded.enemies1[i].rec.height);
        }
        for(i=0; i<loaded.activeEnemies2; i++){
            printf("alive: %d \n", loaded.enemies2[i].alive);
            printf("direction: %d \n", loaded.enemies2[i].direction);
            printf("level: %d \n", loaded.enemies2[i].level);
            printf("x: %f y: %f \n", loaded.enemies2[i].rec.x, loaded.enemies2[i].rec.y);
            printf("speedX: %d \n", loaded.enemies2[i].speedX);
            printf("Status: %d \n", loaded.enemies2[i].status);
            printf("Altura: %f \n", loaded.enemies2[i].rec.height);
        }
    printf("\nActive1: %d", loaded.activeEnemies1);
    printf("\nActive2: %d", loaded.activeEnemies2);
    printf("\ndelay enemy: %d", loaded.delayEnemy);
    printf("\nNum1: %d", loaded.numEnem1);
    printf("\nNum2: %d", loaded.numEnem2);
    return loaded;
}





///-----------------Continuar um jogo antigo-----------------------
void continue_game(SRC media){
    GAME game;
    PHASE journey[JOURNEY_SIZE];
    load_journey(journey, media);
    get_string("What's your name?", game.namePlayer, media);
    int level;
    int flag_save=0, flag_win=0, flag_next=0;
    FILE *arq;

    ///Abertura do arquivo
    arq = fopen(game.namePlayer, "r"); //abertura para leitura
    if(arq==NULL){ //NULL indica erro na abertura do arquivo
        printf("Erro na abertura");
    }else
        fscanf(arq, "%d", &level);
    fclose(arq);
    level--; //Se a fase � 1, ser� a posi��o 0 do vetor journey;

    game.phase = load_old_phase(journey[level], game.namePlayer, media);

    do{
        printf("\n------Fase: %d", game.phase.level);
        flag_next = play_phase(&game.phase, media, &flag_save, game.namePlayer);
        if(flag_next)
            game.phase.level++;
        if(game.phase.level==7)
            flag_win=1;
    }while(flag_next && !flag_win);
    update_ranking(game);
    if(flag_win)
        win_game(media);
}

///-------------------RANKING-------------------
//Atualizar os dados do ranking:
void update_ranking(GAME game){
    RANKING ranking[1];
    FILE *arq;
    if(!(arq = fopen("highscores.bin", "rb"))) //sinaliza se houve erro na abertura do arquivo
        printf("Erro na abertura do arquivo\n");
    else
        fread(&ranking, sizeof(RANKING), 1, arq); //os dados j� est�o na ordem correta
    fclose(arq);
    printf("Sly: %d", ranking[0].pointsSly);
    if(game.phase.hero.points>ranking->points){
        ranking->points = game.phase.hero.points;
        strcpy(ranking[0].name, game.namePlayer);
        printf("Pontuacao maior");
    }
    switch(game.house){
        case SLYTHERIN: ranking[0].pointsSly += game.phase.hero.points;
            break;
        case GRYFFINDOR: ranking[0].pointsGryf += game.phase.hero.points;
            break;
        case RAVENCLAW: ranking[0].pointsRav += game.phase.hero.points;
            break;
        case HUFFLEPUFF: ranking[0].pointsHuf += game.phase.hero.points;
            break;
        default: printf("Erro");
    }
    if(!(arq = fopen("highscores.bin", "wb"))) //sinaliza se houve erro na abertura do arquivo
        printf("Erro na abertura do arquivo\n");
    else if (fwrite(&ranking, sizeof(RANKING), 1, arq) != 1)
        printf("Erro na escrita!\n");
    else
        printf("Ranking atualizado");
    printf("%s", ranking[0].name);
    printf("Casa 1: %d", ranking[0].pointsSly);
    fclose(arq);
}
/*

void ranking_screen(SRC media){
    RANKING ranking[RANKING_SIZE];
    FILE *arq;
    if(!(arq = fopen("highscores.bin", "rb+"))){ //sinaliza se houve erro na abertura do arquivo
        printf("Erro na abertura do arquivo\n");
    }else{
        ///-------------Leitura dos dados--------------
        fread(ranking, sizeof(RANKING), RANKING_SIZE, arq); //os dados j� est�o na ordem correta
    }
    int places[4] = {GRYFFINDOR, HUFFLEPUFF, RAVENCLAW, SLYTHERIN};
    printf("Gryf: %d", ranking->pointsGryf);
    int houses[4] = {ranking->pointsGryf, ranking->pointsHuf, ranking->pointsRav, ranking->pointsSly};
    Texture2D housesIMG[4] = {media.gryffindor, media.hufflepuff, media.ravenclaw, media.slytherin}, a_img;
    int i, j, aux, temp;
    for(i=0;i<4;i++){
        for(j=i+1;j<4;j++){
            if(houses[i] >= houses[j]){
                aux=houses[i];
                temp=places[i];
                a_img=housesIMG[i];
                houses[i]=houses[j];
                houses[j]=aux;
                housesIMG[i]=housesIMG[j];
                housesIMG[j]=a_img;
                places[i]=places[j];
                places[j]=temp;
            }
        }
    }
    Vector2 position[3];
    position[0] = (Vector2) {501, 230};
    position[1] = (Vector2) {320, 310};
    position[2] = (Vector2) {700, 310};

    char champion[50] = "Hogwarts champion: ";
    strcat(champion, ranking->name);
    position[0].x=501;
    printf("x= %f, y=%f", position[0].x, position[0].y);
    do{
        BeginDrawing();
        DrawTextEx(media.fonteHP, "House Cup Results", (Vector2){340,35}, 100, 2, WHITE);
        DrawText(champion, (SCREEN_WIDTH-MeasureText(champion, 40))/2, 140, 40, RAYWHITE);
        ClearBackground(BLACK);
        DrawTexture(media.podium, 300, 300, WHITE);
        char temp[10];
        for(i=0; i<3; i++){
            DrawTexture(housesIMG[i], position[i].x, position[i].y, WHITE);
            DrawText(itoa(houses[i], temp, 10), position[i].x+30, position[i].y-30, 30, WHITE);
        }
        EndDrawing();
    }while(!IsKeyDown(KEY_ENTER));
}
*/
void ranking_screen(SRC media){
    RANKING ranking[1];
    FILE *arq;
    if(!(arq = fopen("highscores.bin", "r"))){ //sinaliza se houve erro na abertura do arquivo
        printf("Erro na abertura do arquivo\n");
    }else{
        ///-------------Leitura dos dados--------------
        fread(ranking, sizeof(RANKING), 1, arq); //os dados j� est�o na ordem correta
    }
    int places[4] = {GRYFFINDOR, HUFFLEPUFF, RAVENCLAW, SLYTHERIN};
    printf("Gryf: %d", ranking->pointsGryf);
    int houses[4] = {ranking->pointsGryf, ranking->pointsHuf, ranking->pointsRav, ranking->pointsSly};
    Texture2D housesIMG[4] = {media.gryffindor, media.hufflepuff, media.ravenclaw, media.slytherin}, a_img;
    int i, j, aux, temp;
    for(i=0;i<4;i++){
        for(j=i+1;j<4;j++){
            if(houses[i] >= houses[j]){
                aux=houses[i];
                temp=places[i];
                a_img=housesIMG[i];
                houses[i]=houses[j];
                houses[j]=aux;
                housesIMG[i]=housesIMG[j];
                housesIMG[j]=a_img;
                places[i]=places[j];
                places[j]=temp;
            }
        }
    }
    Vector2 position[3];
    position[0] = (Vector2) {501, 230};
    position[1] = (Vector2) {320, 310};
    position[2] = (Vector2) {700, 310};

    char champion[50] = "Hogwarts champion: ";
    strcat(champion, ranking->name);
    position[0].x=501;
    printf("x= %f, y=%f", position[0].x, position[0].y);
    do{
        BeginDrawing();
        DrawTextEx(media.fonteHP, "House Cup Results", (Vector2){340,35}, 100, 2, WHITE);
        DrawText(champion, (SCREEN_WIDTH-MeasureText(champion, 40))/2, 140, 40, RAYWHITE);
        ClearBackground(BLACK);
        DrawTexture(media.podium, 300, 300, WHITE);
        char temp[10];
        for(i=0; i<3; i++){
            DrawTexture(housesIMG[i], position[i].x, position[i].y, WHITE);
            DrawText(itoa(houses[i], temp, 10), position[i].x+30, position[i].y-30, 30, WHITE);
        }
        EndDrawing();
    }while(!IsKeyDown(KEY_ENTER));
}
