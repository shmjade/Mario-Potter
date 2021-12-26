#include "../Headers/game_functions.h"


///----------Jogar um novo jogo---------
void play_new_game(SRC media){
    //Inicializações:
    GAME newGame;
    int level=0;
    PHASE journey[JOURNEY_SIZE];
    load_journey(journey, media);
    int flag_next=0, flag_win=0, flag_save=0;

    //Coletar o nome do jogador:
    get_string("What's your name?", newGame.namePlayer, media);
    letter(media);
    hat(media);
    newGame.house = sorting_hat(media);
    //Jogar:
    do{
        if(flag_save) save_phase(newGame.phase, newGame.namePlayer);
        newGame.phase = journey[level];
        newGame.phase.level=level+1;
        printf("fase: %d", newGame.phase.level);
        flag_next=play_phase(&newGame.phase, media, &flag_save, newGame.namePlayer);
        if(flag_next){
            DrawText("Next phase", 450, 300, 50, BLACK);
            level++;
            delay(60);
        }
        if(level==JOURNEY_SIZE)
            flag_win=1;
        update_phase(&newGame, journey);
    }while(flag_next && !flag_win);
    if(flag_win)
        win_game(media);
    update_ranking(newGame);
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
    return flag_win; //se venceu, retorna 1, senão, retorna 0
    printf("Flag win da play phase: %d", flag_win);
}


///------------Atualiza os dados ao passar de fase-----------
//Pega os dados da fase anterior e transfere para esta:
void update_phase(GAME *game, PHASE journey[]){
    journey[game->phase.level].hero.points += game->phase.hero.points; //soma os pontos conquistados na fase anterior com os da nova (0)
    journey[game->phase.level].hero.lives = game->phase.hero.lives; //mantém a quantidade de vidas do final da fase anterior
    journey[game->phase.level].powerHits = game->phase.powerHits; //mantém a quantidade de vezes que pode usar o botão Power
}

/*
///---------------Salva o jogo em um arquivo binário-------------
int save_game(GAME *game){
    FILE *arq; //declaração de um ponteiro para arquivo
    int success=1;

    if(!(arq = fopen(game->namePlayer,"wb"))){ // abre para escrita e testa a abertura
        success=0;
        printf("Erro na abertura\n");
    }
    else{
        if (fwrite(game, sizeof(GAME), 1, arq) != 1){
            success=0;
            printf("Erro na escrita!\n");
        }
    }
    fclose(arq);
    if(success) printf("Arquivo salvo com sucesso!");

    return success;
}
*/
/*
///--------------Carrega o jogo de um arquivo binário------------
GAME* load_game(char *nameArq){
    printf("Tentaremos abrir o arquivo");
    GAME *loaded = (GAME*) malloc(sizeof(GAME)); //cast para o ponteiro de jogo (não estava alocado na memória antes)
    FILE *arq;
    if(!(arq = fopen(nameArq, "rb"))){ //sinaliza se houve erro na abertura do arquivo
        printf("Erro na abertura do arquivo\n");
    }else{
        printf("\tJogo aberto: \n");
    //    while(!feof(arq)){//Enquanto não tiver chegado ao final do arquivo
            if(fread(loaded, sizeof(GAME), 1, arq)==1){
                printf("Nome do jogador: %s\n", loaded->namePlayer);
                printf("Fase: %d\n", loaded->phase.level);
                printf("Pontos: %d\n", loaded->phase.hero.points);
                printf("Vidas: %d\n", loaded->phase.hero.lives);
            }else{
                printf("Algo errado aqui");
            }
    //    }
        printf("\t-------fim-------\n\n");
        fclose(arq);
    }
    return loaded;
}
*/

///----------------Carrega o jogo  de um arquivo txt----------------

///-----------------Salva o jogo em um arquivo txt------------------
/*Para jogar um jogo antigo, precisamos salvá-lo de forma adequada,
armazenando todas as informações necessárias, conforme:
- NOME DO JOGADOR: será o identificador para abrir o nosso jogo antigo
- FASE DO JOGO: guardará as informações do jogo em si, contendo:
    - o nível da fase (level) para continuar a campanha;
    - As informações sobre o botão power:
        - Sua posição na tela, altura e largura (recPower);
        - Quantas vezes ele ainda pode ser usado (no máximo 3 ao longo do jogo);
    - As informações sobre o herói/jogador (HERO):
        - sua velocidade horizontal (speedX);
        - sua direção de deslocamento (direction);
        - sua posição na tela, sua altura e largura (rec);
        - se pode pular (jump) - 1 se está em cima do "chão" ou 0 se está
          pulando ou caindo;
        - se está subindo (jumping);
        - se está caindo (falling);
        - se está parado (stop);
        - se pode ir para a direita, ou seja, se não está colidindo com o cenário
          estático (right);
        - se pode ir para a esquerda, ou seja, se não está colidindo com o cenário
          estático (left);
        - a quantidade de vidas (lives);
        - a pontuação do jogador (points);
        - se está no período após ter morrido (afterDeath);
    - As informações sobre os inimigos:
        - delay na liberação entre os inimigos (delayEnemy);
        - quantidade de inimigos já derrotados na fase (deadEnemies);
        - Tipo 1:
            - quantidade de inimigos do tipo 1 na fase (numEnem1);
            - quantidade de inimigos do tipo 1 que já foram liberados na fase (activeEnemies1);
            - vetor com inimigos do tipo 1, contendo:
                - se já foi ativado ou não (alive);
                - seu tipo (level);
                - seu status (NORMAL, WEAK, DEAD);
                - sua velocidade de deslocamento na horizontal (speedX);
                - sua direção de deslocamento (direction);
                - seu tamanho e posição na tela (rec);
        - Tipo 2:
            - quantidade de inimigos do tipo 2 na fase (numEnem2);
            - quantidade de inimigos do tipo 2 que já foram liberados na fase (activeEnemies2);
            - vetor com inimigos do tipo 2, contendo:
                - se já foi ativado ou não (alive);
                - seu tipo (level);
                - seu status (NORMAL, WEAK, DEAD);
                - sua velocidade de deslocamento na horizontal (speedX);
                - sua direção de deslocamento (direction);
                - seu tamanho e posição na tela (rec);
    - As informações sobre as plataformas:
        - quantidade de plataformas na fase (cada p foi considerado uma plataforma);
        - arranjo de plataformas (PLATFORMS), contendo:
            - sua posição na tela;
            - sua largura e altura;
    - As informações sobre os canos/retornos:
        - quantidade de canos/retornos;
        - posição de cada cano/retorno;
    - As informações sobre as moedas:
        - quantidade de moedas já liberadas na fase;
        - arranjo de moedas (COINS) contendo:
            - seu status (ativa ou inativa) para liberar ou não a movimentação;
            - sua velocidade de deslocamento na horizontal (speedX);
            - sua direção de deslocamento (direction);
            - sua posição na tela, sua altura e largura (coinRec).
Todas essas informações estão armazenadas na nossa struct GAME, portanto, podemos
simplesmente armazenar as informações de acordo com a nossa struct. Para isso, faremos: */

void save_phase(PHASE phase, char namePlayer[]){
    //Declaração do ponteiro de arquivo:
    FILE *arq;
    printf("\nSave phase\n");
    int i;

    //Abriremos um arquivo com o mesmo nome do jogador para salvar as informações:
    if(!(arq = fopen(namePlayer, "w"))) // abre para escrita
        printf("Erro na abertura\n");
    else{
        fprintf(arq,"%d \n", phase.level);
        ///--------Informações do botão Power---------
        fprintf(arq,"%d \n", phase.powerHits);
        ///------------Informações do herói-----------
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
        ///----------Informações dos inimigos------------
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
        ///----------Informações sobre as moedas---------
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
    ///----------informações estáticas---------
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

    ///----------Atualizando informações móveis-------------
    //Declaração do ponteiro de arquivo:
    FILE *arq;
    printf("\nLoad game\n");
    ///Abertura do arquivo
    arq = fopen(namePlayer, "r"); //abertura para leitura
    if(arq==NULL){ //NULL indica erro na abertura do arquivo
        printf("Erro na abertura");
    }else{//Abriu corretamente, coletaremos os dados do arquivo da fase a ser carregada:
            printf("Loading");
            fscanf(arq, "%d %d", &loaded.level, &loaded.powerHits);
            ///---------Informações do heróis--------------
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

            ///---------Informações dos inimigos-----------
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

            ///------------Informações das moedas--------------
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
    int flag_save=0;
    FILE *arq;

    ///Abertura do arquivo
    arq = fopen(game.namePlayer, "r"); //abertura para leitura
    if(arq==NULL){ //NULL indica erro na abertura do arquivo
        printf("Erro na abertura");
    }else
        fscanf(arq, "%d", &level);
    fclose(arq);
    level--; //Se a fase é 1, será a posição 0 do vetor journey;

    game.phase = load_old_phase(journey[level], game.namePlayer, media);
    printf("Carregou a phase");
    printf("Antes de jogar");

    play_phase(&game.phase, media, &flag_save, game.namePlayer);
    update_ranking(game);
}

///-------------------RANKING-------------------
//Atualizar os dados do ranking:
void update_ranking(GAME game){
    RANKING ranking[1];
    FILE *arq;
    if(!(arq = fopen("highscores.bin", "rb"))) //sinaliza se houve erro na abertura do arquivo
        printf("Erro na abertura do arquivo\n");
    else
        fread(&ranking, sizeof(RANKING), 1, arq); //os dados já estão na ordem correta
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
//Atualizar os dados do ranking:
void update_ranking(GAME game){
    RANKING ranking[RANKING_SIZE];
    FILE *arq;
    if(!(arq = fopen("highscores.bin", "rb+"))){ //sinaliza se houve erro na abertura do arquivo
        printf("Erro na abertura do arquivo\n");
    }else{
        ///-------------Leitura dos dados--------------
        for(int i=0; i<RANKING_SIZE; i++)
            fread(&ranking[i], sizeof(RANKING), 1, arq); //os dados já estão na ordem correta
    }
    int position=is_between(ranking, game);
    if(position!=-2 && position !=5){
        for(int i=RANKING_SIZE; i>position; i--){
            ranking[i].points = ranking[i-1].points;
            strcpy(ranking[i].name, ranking[i-1].name);
        }
        ranking[position].points = game.phase.hero.points;
    }
    if (fwrite(ranking, sizeof(RANKING), 1, arq) != 1)
        printf("Erro na escrita!\n");
    fclose(arq);
}
*/
/*
int is_between(RANKING ranking[RANKING_SIZE], GAME game){
    int position=-2;
    if(game.phase.hero.points<ranking[4].points) //Se for menor do que a menor das pontuações do ranking
        position=5;
    else{
        if(game.phase.hero.points>ranking[4].points && game.phase.hero.points<ranking[3].points)
            position=4;
        if(game.phase.hero.points>ranking[3].points && game.phase.hero.points<ranking[2].points)
            position=3;
        if(game.phase.hero.points>ranking[2].points && game.phase.hero.points<ranking[1].points)
            position=2;
        if(game.phase.hero.points>ranking[1].points && game.phase.hero.points<ranking[0].points)
            position=1;
        if(game.phase.hero.points>ranking[0].points)
            position=0;
    }
    return position;
}
*/


void ranking_screen(SRC media){
    RANKING ranking[RANKING_SIZE];
    FILE *arq;
    if(!(arq = fopen("highscores.bin", "rb+"))){ //sinaliza se houve erro na abertura do arquivo
        printf("Erro na abertura do arquivo\n");
    }else{
        ///-------------Leitura dos dados--------------
        for(int i=0; i<RANKING_SIZE; i++)
            fread(&ranking[i], sizeof(RANKING), 1, arq); //os dados já estão na ordem correta
    }
    int places[4] = {GRYFFINDOR, HUFFLEPUFF, RAVENCLAW, SLYTHERIN};
    int houses[4] = {ranking->pointsGryf, ranking->pointsHuf, ranking->pointsRav, ranking->pointsSly};
    int i, j, aux, temp;
    for(i=0;i<4;i++){
        for(j=i+1;j<4;j++){
            if(houses[i] >= houses[j]){
                aux=houses[i];
                temp=places[i];
                houses[i]=houses[j];
                houses[j]=aux;
                places[i]=places[j];
                places[j]=temp;
            }
        }
    }
    Vector2 pos[3], position[3];
    Vector2 posTitle = {325, 60};
    position[0] = (Vector2) {520, 270};
    position[1] = (Vector2) {330, 350};
    position[2] = (Vector2) {710, 350};
    Vector2 posGrif = {0,0};
    Vector2 posRav = {1,0};
    Vector2 posHuf = {0,1};
    Vector2 posSly = {1,1};
    for(i=0; i<3; i++){
        switch(places[i]){
            case SLYTHERIN: pos[i].x = position[i].x*posSly.x;
                            pos[i].y = position[i].y*posSly.y;
                break;
            case GRYFFINDOR: pos[i].x = position[i].x*posGrif.x;
                             pos[i].y = position[i].y*posGrif.y;
                break;
            case RAVENCLAW: pos[i].x = position[i].x*posRav.x;
                            pos[i].y = position[i].y*posRav.y;
                break;
            case HUFFLEPUFF: pos[i].x = position[i].x*posHuf.x;
                             pos[i].y = position[i].y*posHuf.y;
                break;
        }
    }
    Rectangle recFirst = {media.recFrameHouses.x*pos[0].x, media.recFrameHouses.y*pos[0].y, media.recFrameHouses.width, media.recFrameHouses.height};
    Rectangle recSecond = {media.recFrameHouses.x*pos[1].x, media.recFrameHouses.y*pos[1].y, media.recFrameHouses.width, media.recFrameHouses.height};
    Rectangle recThird = {media.recFrameHouses.x*pos[2].x, media.recFrameHouses.y*pos[2].y, media.recFrameHouses.width, media.recFrameHouses.height};
    char champion[] = "Hogwarts champion: ";
    strcat(champion, ranking->name);
    do{
        BeginDrawing();
        DrawTextEx(media.fonteHP, "House Cup Results", posTitle, 100, 2, WHITE);
        DrawText(champion, (SCREEN_WIDTH-MeasureText(champion, 40))/2, 180, 40, RAYWHITE);
        ClearBackground(BLACK);
        DrawTexture(media.podium, 300, 300, WHITE);
        DrawTextureRec(media.houses, recFirst, position[0], WHITE);
        DrawTextureRec(media.houses, recSecond, position[1], WHITE);
        DrawTextureRec(media.houses, recThird, position[2], WHITE);
        EndDrawing();
    }while(!IsKeyDown(KEY_ENTER));
}








