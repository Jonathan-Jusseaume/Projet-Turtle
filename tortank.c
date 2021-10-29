/**
 * Fichier crée par JUSSEAUME Jonathan le 21/10/2021
 */

/**
 * @todo Faire en sorte de se déplacer selon les informations que l'on a, TP si on a rien d'intéressant sur notre ligne, si on a des éléments intéressants, on paralyse quelqu'un qui a des bons éléments
 */


/**
 * Les librairies
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
/**
 * Les valeurs booléenes
 */
#define TRUE 1
#define FALSE 0

/**
 * Les valeurs possibles de blind
 */
#define BLINDED 1
#define NOT_BLINDED 0

/**
 * Les valeurs possibles de paralysie
 */
#define PARALYZED 1
#define NOT_PARALYZED 0

/**
 * Les valeurs que peuvent prendre une case
 */
#define WHITE 1
#define BLACK 0
#define UNKNOWN -1

/**
 * Les valeurs d'orientation de la tortue
 */
#define NOT_FOUND -1
#define LEFT 0
#define UP 1
#define RIGHT 2
#define DOWN 3

/**
 * Les modes pour demander les cases 
 */
#define MOST_LEFT 0
#define MOST_RIGHT 1
#define MOST_UP 1
#define MOST_DOWN 0

/**
 * Le nombre de lignes et de colonnes
 */
#define NUMBER_LINES 12
#define NUMBER_COLUMNS 12


/**
 * Structure qui définit ce qui constitue une position sur la grille
 * à savoir ue variable x qui correspond à la ligne à laquelle nous nous trouvons
 * et une variable y qui correspond à la colonne
 */
typedef struct Position {
    int x;
    int y;
} Position;

/**
 * Structure qui définit ce qui constitue la tortue de notre joueur.
 * Elle est définit par sa position sur le plateau, sa direction
 * ainsi que le fait que son crayon soit baissé ou non
 */
typedef struct Turtle {
    Position position;
    int direction;
    int penIsDown;
} Turtle;


/**
 * Structure qui définit ce qui constitue un joueur du jeu
 * Il se définit par un numéro, un nombre de points, un booléen
 * permettant de savoir si le joueur est retiré du jeu ou non
 * et il possède une tortue
 */
typedef struct Player {
    Turtle turtle;
    int number;
    int score;
    int isRemove;
} Player;


/**
 * Structure contenant toutes les informations essentielles de la partie.
 * Les informations que je possède sur la grille, par défaut toutes les cases valent UNKNOWN, une grille
 * qui mémorise si la case a déjà été cochée ou non par un joueur
 * la liste des joueurs, le numéro de notre joueur, notre tortue, ainsi que le nombre de joueurs
 */
typedef struct Game {
    int **grid;
    int **gridChecked;
    Player *players;
    int numberPlayers;
    int myNumero;
} Game;

/**
 * Structure dont le principal rôle va être de nous permettre de renvoyer plusieurs informations depuis une position
 * dans une direction. On va notamment renvoyer le nombre de points possiblement obtenables, ou encore le nombre de cases
 * inconnues
 */
typedef struct InformationFromPosition {
    Position position;
    int direction;
    int possibleScore;
    int numberUnknown;
    Position lastBlackCellUnchecked;
} InformationFromPosition;

/**
 * Renvoie une partie initialisée avec une grille 12x12 avec que des UNKNOWN,
 * le nombre de joueurs obtenu grâce à l'entrée standard qui nous permet d'instancier
 * une liste de joueurs à la bonne taille. Enfin, le numéro de notre joueur est également obtenu
 * @return la partie initialisée grâce à la première ligne d'information envoyé par le jeu
 */
Game initGame();

/**
 * Renvoie à partir d'une ligne envoyé par le jeu, une liste
 * de tous les entiers séparés par des espaces
 * @param line la ligne que l'on veut parser
 * @return une liste d'entiers (exemple: line= 1 2 3, return: [1, 2, 3])
 */
int *parseLine(char *line);

/**
 * Affiche la liste passée en paramètre dans la sortie d'erreur
 * @param list la liste que l'on va afficher
 * @param size le nombre d'éléments dans notre liste
 */
void displayArray(int *list, int size);

/**
 * Met à jour la partie qui a l'adresse passé en paramètre grâce aux informations
 * envoyées par le jeu en début de tour.
 * Rafraichit notamment le statut des différents Joueurs
 * @param game
 */
void updateGame(Game *game);

/**
 * Ajoute à la liste passé en paramètre, un entier
 * qui se trouve dans la chaine line entre l'indice indexStart et indexEnd - 1
 * @param list la liste à laquelle on veut ajouter un entier
 * @param size l'adresse de la taille de notre liste pour la modifier au moment de l'ajout
 * @param line la ligne dans laquelle on cherche notre entier
 * @param indexStart l'indice du début de notre nombre dans la chaine line
 * @param indexEnd l'indice + 1 de la fin de notre nombre dans la chaine line
 */
void addSubstringToIntList(int *list,
                           int *size,
                           char *line,
                           int indexStart,
                           int indexEnd);

/**
 * Permet de savoir si on est aveuglé pendant ce tour ou non
 * @param game la partie en cours qui va nous permettre de voir les différents joueurs
 * @return TRUE si on est aveuglé, FALSE si on est pas aveuglé
 */
int checkIfBlinded(Game game);

/**
 * Permet de savoir si on est paralysé pendant ce tour ou non
 * @param game la partie en cours qui va nous permettre de voir les différents joueurs
 * @return TRUE si on est paralysé, FALSE si on est pas paralysé
 */
int checkIfParalyzed(Game game);

/**
 * Met à jour la partie qui a l'adresse passé en paramètre et notamment sa grille
 * notamment en se basant sur le fait que le si sur une même ligne/colonne il y a deux cases noires
 * séparées alors tout ce qui est entre les deux sont des cases noires
 * @param game
 */
void updateGrid(Game *game);

/**
 * Renvoie les informations avec notamment la direction vers laquelle il faut aller pour marquer des points
 * @param game la situation de la partie actuelle
 * @return la direction de InformationFromPosition vaut -1 s'il n'y a aucune bonne direction pour marquer des points, 0 s'il faut aller à gauche,
 * 1 s'il faut aller en haut, 2 s'il faut aller à droite, 3 s'il faut aller en bas
 */
InformationFromPosition directionToScorePoints(Game game, Position position);

/**
 * Renvoie les informations par rapport à une position donnée en paramètre avec notamment la direction vers laquelle
 * il y a le plus de cases UNKNOWN et que ce nombre est supérieur à 2
 * @param la situation de la partie actuelle
 * @param la position par rapport à laquelle on veut regarder
 * @return la direction de InformationFromPosition vaut -1 s'il n'y a aucune bonne direction pour dévoiler des UNKNOWN, 0 s'il faut aller à gauche,
 * 1 s'il faut aller en haut, 2 s'il faut aller à droite, 3 s'il faut aller en bas
 */
InformationFromPosition directionToLookForward(Game game, Position position);

/**
 * Demande au jeu, l'information selon une case selon une direction et une position donnée puis met
 * à jour la partie dont a donné l'adresse en paramètre
 * @param game l'adresse de la partie dont on va modifier les informations
 * @param direction la direction dans laquelle on va demander l'information
 * @param position la position depuis laquelle on va demander l'information
 */
void askInformationInTheDirection(Game *game, int direction, Position position);

/**
 * Renvoie notre joueur
 * @param game la partie duquelle on récupère notre joueur
 * @return notre joueur avec notamment sa tortue
 */
Player getMyPlayer(Game game);

/**
 * Renvoie différentes informations à gauche de la position donnée en paramètre
 * @param game la situation de la partie actuelle
 * @param position notre position
 * @return différentes informations à gauche de la position
 */
InformationFromPosition informationLEFT(Game game, Position position);

/**
 * Renvoie différentes informations à droite de la position donnée en paramètre
 * @param game la situation de la partie actuelle
 * @param position notre position
 * @return différentes informations à droite de la position
 */
InformationFromPosition informationRIGHT(Game game, Position position);

/**
 * Renvoie différentes informations au-dessus de la position donnée en paramètre
 * @param game la situation de la partie actuelle
 * @param position notre position
 * @return différentes informations au-dessus de la position
 */
InformationFromPosition informationUP(Game game, Position position);

/**
 * Renvoie différentes informations en-dessous de la position donnée en paramètre
 * @param game la situation de la partie actuelle
 * @param position notre position
 * @return différentes informations en-dessous de la position
 */
InformationFromPosition informationDOWN(Game game, Position position);


/**
 * Main pour faire des tests de fonction
 * @return
 */
int mainBrouillon(void) {
    fprintf(stderr, "CARABAFFE A L'ATTAQUE \n");
    Game game = initGame();
    int nbTurns = 0;
    while (nbTurns < 150) {
        updateGame(&game);
        fprintf(stdout, "NOBLIND\n");
        fflush(stdout);
        checkIfBlinded(game);
        fprintf(stdout, "NOREVEAL\n");
        fflush(stdout);
        for (int i = 0; i < NUMBER_LINES; i++) {
            displayArray(game.grid[i], NUMBER_COLUMNS);
        }
        fprintf(stdout, "NOPARALYZE\n");
        fflush(stdout);
        checkIfParalyzed(game);
        fprintf(stdout, "PASS\n");
        fflush(stdout);


        nbTurns++;
    }

    /*
     * Notre comportement sur tous les tours
     */
    while (nbTurns < 150) {
        /*
         * On récupère les informations de la partie
         */
        updateGame(&game);
        /*
         * On ne joue pas avec l'aveuglement
         */
        fprintf(stdout, "NOBLIND\n");
        fflush(stdout);
        int isBlinded = checkIfBlinded(game);
        /*
         * Si on est pas aveuglé alors on peut réveler une case
         */
        if (isBlinded == FALSE) {

        } else {
            fprintf(stdout, "NOREVEAL\n");
            fflush(stdout);
        }
        /*
         * On display la map actuelle
         */
        for (int i = 0; i < NUMBER_LINES; i++) {
            displayArray(game.grid[i], NUMBER_COLUMNS);
        }


        nbTurns++;
    }

    int number;
    if (checkIfBlinded(game) == FALSE) {
        fprintf(stdout, "REVEALL %d %d\n", 6, MOST_LEFT);
        fflush(stdout);
        char buffer[5];
        fgets(buffer, 5, stdin);
        fprintf(stderr, "Case=%s\n", buffer);
        number = atoi(buffer);
        if (number == -1) {
            for (int i = 0; i < NUMBER_COLUMNS; i++) {
                game.grid[6][i] = WHITE;
            }
        } else {
            for (int i = 0; i < number; i++) {
                game.grid[6][i] = WHITE;
            }
            game.grid[6][number] = BLACK;
        }
    }
    fprintf(stdout, "NOPARALYZE\n");
    fflush(stdout);
    if (checkIfParalyzed(game) == FALSE) {
        if (game.players[game.myNumero].turtle.direction == LEFT) {
            fprintf(stdout, "TELEPORT 6 %d; SWITCHPEN; ROTATE 1; ROTATE 1; PASS\n", number);
            fflush(stdout);
        } else if (game.players[game.myNumero].turtle.direction == RIGHT) {
            fprintf(stdout, "TELEPORT 6 %d; SWITCHPEN; PASS\n", number);
            fflush(stdout);
        } else if (game.players[game.myNumero].turtle.direction == UP) {
            fprintf(stdout, "TELEPORT 6 %d; SWITCHPEN; ROTATE 1; PASS\n", number);
            fflush(stdout);
        } else if (game.players[game.myNumero].turtle.direction == DOWN) {
            fprintf(stdout, "TELEPORT 6 %d; SWITCHPEN; ROTATE 0; PASS\n", number);
            fflush(stdout);
        }

    }
    nbTurns++;
    fflush(stdout);
    updateGame(&game);
    fprintf(stdout, "NOBLIND\n");
    fflush(stdout);
    fflush(stderr);
    int number2 = -1;
    if (checkIfBlinded(game) == FALSE) {
        fprintf(stdout, "REVEALL %d %d\n", 6, 1);
        fflush(stdout);
        char buffer[5];
        fflush(stdin);
        fgets(buffer, 5, stdin);
        number2 = atoi(buffer);
        if (number2 == -1) {
            for (int i = 0; i < NUMBER_COLUMNS; i++) {
                game.grid[6][i] = WHITE;
            }
        } else {
            for (int i = NUMBER_COLUMNS - 1; i > number2; i--) {
                game.grid[6][i] = WHITE;
            }
            game.grid[6][number2] = BLACK;
        }
    }
    fprintf(stdout, "NOPARALYZE\n");
    fflush(stdout);
    if (checkIfParalyzed(game) == FALSE) {
        fprintf(stdout, "MOVE %d\n", number2 - number);
        fflush(stdout);
    }
    nbTurns++;

    return 0;
}

/**
 * Main de l'application, il démarre par une initialisation de la partie,
 * puis notre IA va ensuite jouer les tours
 * @return 0 s'il n'y a pas eu de problèmes
 */
int main(void) {
    fprintf(stderr, "TORTANK A L'ATTAQUE \n");

/*    int step = 0;
    for (int origine = 6; origine > 0; origine += step) {
        fprintf(stderr, "%d \n", origine);
        if (step < 0) {
            step--;
        } else {
            step++;
        }
        step *= -1;
    } */


    Game game = initGame();
    int nbTurns = 0;
    /*
     * Notre comportement sur tous les tours
     */
    while (nbTurns < 150) {
        /*
         * On récupère les informations de la partie
         */
        updateGame(&game);
        /*
         * On ne joue pas avec l'aveuglement
         */
        fprintf(stdout, "NOBLIND\n");
        fflush(stdout);
        int isBlinded = checkIfBlinded(game);
        /*
         * Si on est pas aveuglé alors on peut réveler une case
         */
        if (isBlinded == FALSE) {
            InformationFromPosition informationDirectionToGo = directionToLookForward(game, getMyPlayer(
                    game).turtle.position);
            /*
             * Si on a trouvé une direction intéressante depuis notre position, alors on va révélé l'information sur cette ligne
             * ou colonne dans la direction qui nous intéresse
             */
            if (informationDirectionToGo.direction != NOT_FOUND) {
                fprintf(stderr, "%d %d \n", informationDirectionToGo.direction, informationDirectionToGo.numberUnknown);
                for (int i = 0; i < NUMBER_LINES; i++) {
                    displayArray(game.grid[i], NUMBER_COLUMNS);
                }
                askInformationInTheDirection(&game, informationDirectionToGo.direction,
                                             getMyPlayer(game).turtle.position);
                for (int i = 0; i < NUMBER_LINES; i++) {
                    displayArray(game.grid[i], NUMBER_COLUMNS);
                }
            } else {
                /*
                 * Si on est au premier tour alors on a aucune information donc on fait aléatoirement
                 * selon l'orientation de notre tortue pour éviter de perdre des points.
                 * On utilise la méthode askInformationInTheDirection mais en lui donnant la direction
                 * inverse à notre tortue pour que l'on puisse se téléporter en début de ligne par rapport
                 * à notre sens et pas en fin de ligne pour cela il faut additionner notre direction par 2 et la faire %4
                 */
                if (nbTurns == 0) {
                    srand(time(NULL));
                    int nbColumnOrLine = rand() % ((8 + 1) - 4) + 4;
                    Position randomPosition = {nbColumnOrLine, nbColumnOrLine};
                    askInformationInTheDirection(&game, (getMyPlayer(game).turtle.direction + 2) % 4, randomPosition);
                }
                    /*
                    * Si on est pas au premier tour il faut faire par rapport à ce qui a déjà été dévoilé
                    */
                else {
                    /**
                     * FIXME
                     */
                    fprintf(stdout, "NOREVEAL\n");
                    fflush(stdout);
                }
            }
            /*
             * On ne joue pas avec la paralysie
             */
            fprintf(stdout, "NOPARALYZE\n");
            fflush(stdout);

            /**
             * FIXME
             *  - faire une méthode qui renvoie la chaine à envoyer au jeu
             *  - il faut d'abord s'informer dans la proximité, notamment si on est déjà sur une case noire mais qu'on connait pas où sont les autres cases noires de notre ligne/colonne
             *  - dans les cas où il n'y a vraiment rien à retirer depuis notre position, il faut s'intéresser aux positions centrales et faire par rapport aux informations qu'on a; regarder ligne/colonne 6 puis 7 puis 5 puis 8 puis 4 jusqu'a trouver quelque chose d'intéressant
             *  - c'est une des parties les plus complexes quand la grille est bien remplie et qu'on a déjà repéré des cases noires sur une ligne par exemple, il faut faire en fonction du nombre d'unknown dans la grille, 4 unknown dans une direction est excellent par exemple
             *  - ensuite on récupère le résultat obtenue et on met à jour la grille
             *  - faire une structure qui renvoit le nombre de cases noires, de cases blanches et unknown à gauche d'une position par exemple (fonction numberPointsAtLeft devient informationsLeft et renvoie un objet de type InformationFromPosition)
             *  - changer les fonctions directionToLookForward, bestScore pour qu'elle renvoie
             *  - une InformationFromPosition ou qu'elle renvoie une InformationFromPOsition avec une direction -1 quand il y a rien de satisfaisant
             */
        } else {
            fprintf(stdout, "NOREVEAL\n");
            fflush(stdout);
        }
        /*
         * On ne joue pas avec la paralysie
         */
        fprintf(stdout, "NOPARALYZE\n");
        fflush(stdout);
        /*
         * On regarde si on est paralysé afin de savoir si on peut bouger
         */
        int isParalyzed = checkIfParalyzed(game);
        if (isParalyzed == FALSE) {
            InformationFromPosition informationDirectionToScore = directionToScorePoints(game, getMyPlayer(game).turtle.position);
            /*
             * Si on trouve une direction pour marquer des points, alors on s'oriente vers elle et
             * on run
             */
            if (informationDirectionToScore != NOT_FOUND) {
                /*
                 * On regarde si l'orientation est mauvaise pour savoir si on doit faire
                 * rotate notre tortue
                 */
                if (informationDirectionToScore.direction != getMyPlayer(game).turtle.direction) {
                    /**
                     * FIXME
                     *  - code pour remettre notre tortue bien droite (fonction)
                     */
                }
                /**
                  * FIXME (le bon nombre de cases) donc rajouter la dernière case noire unchecked
                  * dans les InformationDirection,
                  * rajouter une fonction qui prenant notre position, celle visée et une direction renvoie le nombre de cases à
                  * parcourir
                 */

            }

        } else {
            fprintf(stdout, "PASS\n");
            fflush(stdout);
        }


        /*
         * On display la map actuelle
         */
        for (int i = 0; i < NUMBER_LINES; i++) {
            displayArray(game.grid[i], NUMBER_COLUMNS);
        }

        /**
         * FIXME
         *  - on place en mode déplacement, regarder autour de nous voir la meilleure position possible pour éviter de gacher une TP qui coute des points,
         *  - s'il y a une direction correcte on fait des tours sur nous même pour aller jusqu'a elle et courir pour marquer des points
         *  - si aucune direction correcte se dessine, tp sur une case noire où on vérifie qu'il y a beaucoup de cases noires possibles en un déplacement et on court pour marquer des points
         *  - si on a vraiment rien à porter même avec un TP alors on PASS notre tour
         */


        nbTurns++;
    }


























    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    updateGame(&game);
    fprintf(stdout, "NOBLIND\n");
    fflush(stdout);
    int number;
    if (checkIfBlinded(game) == FALSE) {
        fprintf(stdout, "REVEALL %d %d\n", 6, MOST_LEFT);
        fflush(stdout);
        char buffer[5];
        fgets(buffer, 5, stdin);
        number = atoi(buffer);
        if (number == -1) {
            for (int i = 0; i < NUMBER_COLUMNS; i++) {
                game.grid[6][i] = WHITE;
            }
        } else {
            for (int i = 0; i < number; i++) {
                game.grid[6][i] = WHITE;
            }
            game.grid[6][number] = BLACK;
        }
    }
    fprintf(stdout, "NOPARALYZE\n");
    fflush(stdout);
    if (checkIfParalyzed(game) == FALSE) {
        if (game.players[game.myNumero].turtle.direction == LEFT) {
            fprintf(stdout, "TELEPORT 6 %d; SWITCHPEN; ROTATE 1; ROTATE 1; PASS\n", number);
            fflush(stdout);
        } else if (game.players[game.myNumero].turtle.direction == RIGHT) {
            fprintf(stdout, "TELEPORT 6 %d; SWITCHPEN; PASS\n", number);
            fflush(stdout);
        } else if (game.players[game.myNumero].turtle.direction == UP) {
            fprintf(stdout, "TELEPORT 6 %d; SWITCHPEN; ROTATE 1; PASS\n", number);
            fflush(stdout);
        } else if (game.players[game.myNumero].turtle.direction == DOWN) {
            fprintf(stdout, "TELEPORT 6 %d; SWITCHPEN; ROTATE 0; PASS\n", number);
            fflush(stdout);
        }

    }
    nbTurns++;
    fflush(stdout);
    updateGame(&game);
    fprintf(stdout, "NOBLIND\n");
    fflush(stdout);
    fflush(stderr);
    int number2 = -1;
    if (checkIfBlinded(game) == FALSE) {
        fprintf(stdout, "REVEALL %d %d\n", 6, 1);
        fflush(stdout);
        char buffer[5];
        fflush(stdin);
        fgets(buffer, 5, stdin);
        number2 = atoi(buffer);
        if (number2 == -1) {
            for (int i = 0; i < NUMBER_COLUMNS; i++) {
                game.grid[6][i] = WHITE;
            }
        } else {
            for (int i = NUMBER_COLUMNS - 1; i > number2; i--) {
                game.grid[6][i] = WHITE;
            }
            game.grid[6][number2] = BLACK;
        }
    }
    int numberIA;
    for (int i = 0; i < game.numberPlayers; i++) {
        if (game.players[i].number != game.myNumero) {
            numberIA = game.players[i].number;
        }
    }
    fprintf(stdout, "PARALYZE %d\n", numberIA);
    fflush(stdout);
    if (checkIfParalyzed(game) == FALSE) {
        fprintf(stdout, "MOVE %d\n", number2 - number);
        fflush(stdout);
    }
    nbTurns++;


    while (nbTurns < 150) {
        fprintf(stdout, "NOBLIND\n");
        fflush(stdout);
        fprintf(stdout, "NOREVEAL\n");
        fflush(stdout);
        fprintf(stdout, "PARALYZE %d\n", numberIA);
        fflush(stdout);
        fprintf(stdout, "PASS\n");
        fflush(stdout);

        nbTurns++;
    }
    return 0;
}

int *parseLine(char *line) {
    int sizeResult = 0;
    int *result = (int *) malloc(sizeResult * sizeof(int));
    int index = 0;
    int indexStartNumber = 0;
    /*
     * On parcourt la ligne
     */
    while (line[index] != '\0') {
        /*
         * Si on est sur un espace alors il faut ajouter le nombre
         * à notre liste de nombre
         */
        if (line[index] == ' ') {
            addSubstringToIntList(result, &sizeResult, line, indexStartNumber, index);
            indexStartNumber = index + 1;
        }
        index++;
    }
    addSubstringToIntList(result, &sizeResult, line, indexStartNumber, index);
    return result;
}

void addSubstringToIntList(int *list,
                           int *size,
                           char *line,
                           int indexStart,
                           int indexEnd) {
    char buffSubstring[indexEnd - indexStart + 1];
    memcpy(buffSubstring, &line[indexStart], indexEnd - indexStart);
    buffSubstring[indexEnd - indexStart] = '\0';
    *size = *size + 1;
    list = (int *) realloc(list, *size * sizeof(int));
    list[*size - 1] = atoi(buffSubstring);
}

void displayArray(int *list, int size) {
    fprintf(stderr, "[");
    for (int i = 0; i < size; i++) {
        fprintf(stderr, "%d", list[i]);
        if (size - 1 != i) {
            fprintf(stderr, ",");
        }
    }
    fprintf(stderr, "] \n");
}

Game initGame() {
    Game game;
    char buffer[30];
    fgets(buffer, 30, stdin);
    int *array = parseLine(buffer);
    game.myNumero = array[1];
    game.numberPlayers = array[0];
    game.players = (Player *) malloc(game.numberPlayers * sizeof(Player));
    game.grid = (int **) malloc(NUMBER_LINES * sizeof(int *));
    game.gridChecked = (int **) malloc(NUMBER_LINES * sizeof(int *));
    for (int i = 0; i < NUMBER_LINES; i++) {
        game.grid[i] = (int *) malloc(NUMBER_COLUMNS * sizeof(int));
        game.gridChecked[i] = (int *) malloc(NUMBER_COLUMNS * sizeof(int));
        for (int j = 0; j < NUMBER_COLUMNS; j++) {
            if (i == 0 || i == NUMBER_LINES - 1 || j == 0 || j == NUMBER_COLUMNS - 1) {
                game.grid[i][j] = WHITE;
            } else {
                game.grid[i][j] = UNKNOWN;
            }
            game.gridChecked[i][j] = FALSE;
        }
    }
    free(array);
    return game;
}

void updateGame(Game *game) {
    char buffer[15];
    for (int i = 0; i < game->numberPlayers; i++) {
        fgets(buffer, 15, stdin);
        int *playerInfo = parseLine(buffer);
        fprintf(stderr, "Joueur=%d, score=%d, isRemove=%d \n", playerInfo[0], playerInfo[1], playerInfo[2]);
        game->players[i].number = playerInfo[0];
        game->players[i].score = playerInfo[1];
        game->players[i].isRemove = playerInfo[2];
        free(playerInfo);
    }
    fgets(buffer, 15, stdin);
    int nbCells = atoi(buffer);
    fprintf(stderr, "Nombre cellules=%d \n", nbCells);
    for (int i = 0; i < nbCells; i++) {
        fgets(buffer, 15, stdin);
        int *cellInfo = parseLine(buffer);
        fprintf(stderr, "Cell %d: l=%d, c=%d, color=%d \n", i, cellInfo[1], cellInfo[2], cellInfo[3]);
        fprintf(stderr, "STEP 1\n");
        game->grid[cellInfo[1]][cellInfo[2]] = cellInfo[3];
        fprintf(stderr, "STEP 2\n");
        game->gridChecked[cellInfo[1]][cellInfo[2]] = TRUE;
        fprintf(stderr, "STEP 3\n");
        free(cellInfo);
        fprintf(stderr, "STEP 4\n");
    }
    for (int i = 0; i < game->numberPlayers; i++) {
        fgets(buffer, 15, stdin);
        int *playerTurtleInfo = parseLine(buffer);
        game->players[i].turtle.position.x = playerTurtleInfo[1];
        game->players[i].turtle.position.y = playerTurtleInfo[2];
        game->players[i].turtle.direction = playerTurtleInfo[3];
        game->players[i].turtle.penIsDown = playerTurtleInfo[4];
        fprintf(stderr, "Player: %d, Turtle: x=%d y=%d direction = %d, penIsDown=%d \n", playerTurtleInfo[0],
                playerTurtleInfo[1],
                playerTurtleInfo[2],
                playerTurtleInfo[3],
                playerTurtleInfo[4]);
        free(playerTurtleInfo);
    }
    updateGrid(game);
}


int checkIfBlinded(Game game) {
    int isBlinded = FALSE;
    fprintf(stderr, "nombreJoueurs: %d \n", game.numberPlayers);
    for (int i = 0; i < game.numberPlayers; i++) {
        char buffer[15];
        fgets(buffer, 15, stdin);
        int *playerBlind = parseLine(buffer);
        fprintf(stderr, "joueur: %d, blind:%d \n", playerBlind[0], playerBlind[1]);
        if (playerBlind[0] == game.myNumero &&
            playerBlind[1] == BLINDED) {
            isBlinded = TRUE;
        }
        free(playerBlind);
    }
    return isBlinded;
}

int checkIfParalyzed(Game game) {
    int isParalyzed = FALSE;
    fprintf(stderr, "nombreJoueurs: %d \n", game.numberPlayers);
    for (int i = 0; i < game.numberPlayers; i++) {
        char buffer[15];
        fgets(buffer, 15, stdin);
        int *playerParalyzed = parseLine(buffer);
        fprintf(stderr, "joueur: %d, paralyzed:%d \n", playerParalyzed[0], playerParalyzed[1]);
        if (playerParalyzed[0] == game.myNumero &&
            playerParalyzed[1] == PARALYZED) {
            isParalyzed = TRUE;
        }
        free(playerParalyzed);
    }
    return isParalyzed;
}

void updateGrid(Game *game) {
    fprintf(stderr, "J'update la grille \n");
    int hasChangedSomething = FALSE;
    /*
     * On regarde les lignes d'abord
     */
    for (int i = 0; i < NUMBER_LINES; i++) {
        int indexBlackBlock = -1;
        for (int j = 0; j < NUMBER_COLUMNS; j++) {
            if (game->grid[i][j] == BLACK) {
                /*
                 * Cas où on détecte pour la première fois une case noire
                 */
                if (indexBlackBlock == -1) {
                    indexBlackBlock = j;
                } else {
                    /*
                     * S'il y a plus d'une case d'écart entre deux cases noires
                     * alors il faut combler le trou par des cases noires, on a donc
                     * modifié des cases
                     */
                    if (j - indexBlackBlock > 1) {
                        for (int k = indexBlackBlock + 1; k < j; k++) {
                            game->grid[i][k] = BLACK;
                        }
                        hasChangedSomething = TRUE;
                    }
                    indexBlackBlock = j;
                }
            }
        }
    }

    /*
     * On fait la même chose mais pour les colonnes
     */
    for (int j = 0; j < NUMBER_COLUMNS; j++) {
        int indexBlackBlock = -1;
        for (int i = 0; i < NUMBER_LINES; i++) {
            if (game->grid[i][j] == BLACK) {
                /*
                 * Cas où on détecte pour la première fois une case noire
                 */
                if (indexBlackBlock == -1) {
                    indexBlackBlock = i;
                } else {
                    /*
                     * S'il y a plus d'une case d'écart entre deux cases noires
                     * alors il faut combler le trou par des cases noires, on a donc
                     * modifié des cases
                     */
                    if (i - indexBlackBlock > 1) {
                        for (int k = indexBlackBlock + 1; k < i; k++) {
                            game->grid[k][j] = BLACK;
                        }
                        hasChangedSomething = TRUE;
                    }
                    indexBlackBlock = i;
                }
            }
        }
    }

    /*
     * Si on a changé quelque chose peut être que l'on a trouvé des informations
     * intéressantes sur une autre colonne ou ligne, on lance donc une récursion jusqu'à ce qu'on ne
     * détecte plus aucune modification
     */
    if (hasChangedSomething == TRUE) {
        updateGrid(game);
    }
}

/**
 * FIXME
 * - faire en sorte que l'affecation soit direct au niveau de InformationFromPosition
 */
InformationFromPosition directionToScorePoints(Game game, Position position) {
    InformationFromPosition returnedInformation;
    returnedInformation.direction = NOT_FOUND;
    returnedInformation.possibleScore = 0;
    returnedInformation.position = position;
    /*
     * On regarde dans toutes les directions le score possible
     * et on retient le meilleur score, on va dans la direction du meilleure score
     * obtenue, si aucune ligne ne rapporte de points on renvoie -1
     */
    int leftScore = informationLEFT(game, position).possibleScore;
    if (leftScore > returnedInformation.possibleScore) {
        returnedInformation.possibleScore = leftScore;
        returnedInformation.direction = LEFT;
    }
    int rightScore = informationRIGHT(game, position).possibleScore;
    if (rightScore > returnedInformation.possibleScore) {
        returnedInformation.possibleScore = rightScore;
        returnedInformation.direction = RIGHT;
    }
    int upScore = informationUP(game, position).possibleScore;
    if (upScore > returnedInformation.possibleScore) {
        returnedInformation.possibleScore = upScore;
        returnedInformation.direction = UP;
    }
    int downScore = informationDOWN(game, position).possibleScore;
    if (downScore > returnedInformation.possibleScore) {
        returnedInformation.possibleScore = downScore;
        returnedInformation.direction = DOWN;
    }
    return returnedInformation;
}

InformationFromPosition directionToLookForward(Game game, Position position) {
    InformationFromPosition returnedInformation;
    returnedInformation.direction = NOT_FOUND;
    returnedInformation.numberUnknown = 0;
    returnedInformation.position = position;
    /*
     * On regarde dans toutes les directions le score possible
     * et on retient la direction dans laquelle il y a le plus de cases inconnues lorsque
     * ce nombre est supérieur à 2
     * Si aucune direction est satisfaisante on renvoie -1
     */
    int leftScore = informationLEFT(game, position).numberUnknown;
    if (leftScore > returnedInformation.numberUnknown && leftScore > 2) {
        returnedInformation.numberUnknown = leftScore;
        returnedInformation.direction = LEFT;
    }
    int rightScore = informationRIGHT(game, position).numberUnknown;
    if (rightScore > returnedInformation.numberUnknown && rightScore > 2) {
        returnedInformation.numberUnknown = rightScore;
        returnedInformation.direction = RIGHT;
    }
    int upScore = informationUP(game, position).numberUnknown;
    if (upScore > returnedInformation.numberUnknown && upScore > 2) {
        returnedInformation.numberUnknown = upScore;
        returnedInformation.direction = UP;
    }
    int downScore = informationDOWN(game, position).numberUnknown;
    if (downScore > returnedInformation.numberUnknown && downScore > 2) {
        returnedInformation.numberUnknown = downScore;
        returnedInformation.direction = DOWN;
    }
    return returnedInformation;
}

void askInformationInTheDirection(Game *game, int direction, Position position) {
    char buffer[5];
    int cellRevealed;
    if (direction == LEFT) {
        fprintf(stdout, "REVEALL %d %d\n", position.x, MOST_LEFT);
        fflush(stdout);
        fgets(buffer, 5, stdin);
        cellRevealed = atoi(buffer);
        for (int i = 0; i < cellRevealed; i++) {
            game->grid[position.x][i] = WHITE;
        }
        game->grid[position.x][cellRevealed] = BLACK;
    } else if (direction == DOWN) {
        fprintf(stdout, "REVEALC %d %d\n", position.y, MOST_DOWN);
        fflush(stdout);
        fgets(buffer, 5, stdin);
        cellRevealed = atoi(buffer);
        for (int i = cellRevealed + 1; i < NUMBER_LINES; i++) {
            game->grid[i][position.y] = WHITE;
        }
        game->grid[cellRevealed][position.y] = BLACK;
    } else if (direction == UP) {
        fprintf(stdout, "REVEALC %d %d\n", position.y, MOST_UP);
        fflush(stdout);
        fgets(buffer, 5, stdin);
        cellRevealed = atoi(buffer);
        for (int i = 0; i < cellRevealed; i++) {
            game->grid[i][position.y] = WHITE;
        }
        game->grid[cellRevealed][position.y] = BLACK;
    } else if (direction == RIGHT) {
        fprintf(stdout, "REVEALL %d %d\n", position.x, MOST_RIGHT);
        fflush(stdout);
        fgets(buffer, 5, stdin);
        cellRevealed = atoi(buffer);
        for (int i = cellRevealed + 1; i < NUMBER_COLUMNS; i++) {
            game->grid[position.x][i] = WHITE;
        }
        game->grid[position.x][cellRevealed] = BLACK;
    }
    updateGrid(game);
}

Player getMyPlayer(Game game) {
    Player myPlayer;
    for (int i = 0; i < game.numberPlayers; i++) {
        if (game.players[i].number == game.myNumero) {
            myPlayer = game.players[i];
        }
    }
    return myPlayer;
}

InformationFromPosition informationLEFT(Game game, Position position) {
    InformationFromPosition informationLEFT;
    informationLEFT.position = position;
    informationLEFT.direction = LEFT;
    informationLEFT.possibleScore = 0;
    informationLEFT.numberUnknown = 0;
    for (int i = position.y; i > 0; i--) {
        /*
         * Si à notre gauche, on a une case noire qui n'est pas déjà cochée
         * alors on augmente le score de gauche de 1
         */
        if (game.grid[position.x][i] == BLACK
            && game.gridChecked[position.x][i] == FALSE) {
            informationLEFT.possibleScore++;
            informationLEFT.lastBlackCellUnchecked.x = position.x;
            informationLEFT.lastBlackCellUnchecked.y = i;
        }
        if (game.grid[position.x][i] == UNKNOWN) {
            informationLEFT.numberUnknown++;
        }
    }
    return informationLEFT;
}

InformationFromPosition informationRIGHT(Game game, Position position) {
    InformationFromPosition informationRIGHT;
    informationRIGHT.position = position;
    informationRIGHT.direction = RIGHT;
    informationRIGHT.possibleScore = 0;
    informationRIGHT.numberUnknown = 0;
    for (int i = position.y; i < NUMBER_COLUMNS; i++) {
        /*
         * Si à notre droite, on a une case noire qui n'est pas déjà cochée
         * alors on augmente le score de gauche de 1
         */
        if (game.grid[position.x][i] == BLACK
            && game.gridChecked[position.x][i] == FALSE) {
            informationRIGHT.possibleScore++;
            informationRIGHT.lastBlackCellUnchecked.x = position.x;
            informationRIGHT.lastBlackCellUnchecked.y = i;
        }
        if (game.grid[position.x][i] == UNKNOWN) {
            informationRIGHT.numberUnknown++;
        }
    }
    return informationRIGHT;
}

InformationFromPosition informationUP(Game game, Position position) {
    InformationFromPosition informationUP;
    informationUP.position = position;
    informationUP.direction = UP;
    informationUP.possibleScore = 0;
    informationUP.numberUnknown = 0;
    for (int i = position.x; i > 0; i--) {
        /*
         * Si au-dessus de nous, on a une case noire qui n'est pas déjà cochée
         * alors on augmente le score de gauche de 1
         */
        if (game.grid[i][position.y] == BLACK
            && game.gridChecked[i][position.y] == FALSE) {
            informationUP.possibleScore++;
            informationUP.lastBlackCellUnchecked.x = i;
            informationUP.lastBlackCellUnchecked.y = position.y;
        }
        if (game.grid[i][position.y] == UNKNOWN) {
            informationUP.numberUnknown++;
        }
    }
    return informationUP;
}

InformationFromPosition informationDOWN(Game game, Position position) {
    InformationFromPosition informationDOWN;
    informationDOWN.position = position;
    informationDOWN.direction = UP;
    informationDOWN.possibleScore = 0;
    informationDOWN.numberUnknown = 0;
    for (int i = position.x; i < NUMBER_LINES; i++) {
        /*
         * Si en-dessous de nous, on a une case noire qui n'est pas déjà cochée
         * alors on augmente le score de gauche de 1
         */
        if (game.grid[i][position.y] == BLACK
            && game.gridChecked[i][position.y] == FALSE) {
            informationDOWN.possibleScore++;
            informationDOWN.lastBlackCellUnchecked.x = i;
            informationDOWN.lastBlackCellUnchecked.y = position.y;
        }
        if (game.grid[i][position.y] == UNKNOWN) {
            informationDOWN.numberUnknown++;
        }
    }
    return informationDOWN;
}
