/**
 * Fichier crée par JUSSEAUME Jonathan le 21/10/2021
 */

/**
 * @todo La méthode getMyPlayer doit renvoyer l'adresse de ce joueur et non pas le joueur en lui même
 * @todo Quand on se déplace notre tortue doit être déplacée aussi
 * @todo Rajouter un booléen hasUsePower sur la game pour vérifier qu'on a déjà utilisé une TP, Paralysie ou Aveuglement
 * @todo Quand on se tourne notre tortue doit être tournée aussi
 * @todo une fois que tout ça est validé, alors on pourra envisagé à faire 4 actions par tour
 * @todo pour Watel DOWN = UP !!!, revoir toute la gestion des directions
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
 * à savoir une variable x qui correspond à la ligne à laquelle nous nous trouvons
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
 * qui mémorise si la case a déjà été cochée ou non par un joueur,
 * la liste des joueurs, le numéro de notre joueur ainsi que le nombre de joueurs
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
Player *getMyPlayer(Game *game);

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
 * Renvoie le nombre de cases qui séparent la première position passée en paramètre de la
 * deuxième grâce à la direction passée en paramètre
 * @param from la position de départ
 * @param to la position visée
 * @param direction la direction dans laquelle on doit aller
 * @return
 */
int getNumberCellsBetweenTwoPositions(Game *game, Position to, int direction);


/**
 * Main de l'application, il démarre par une initialisation de la partie,
 * puis notre IA va ensuite jouer les tours
 * @return 0 s'il n'y a pas eu de problèmes
 */
int main(void) {
    fprintf(stderr, "TORTANK A L'ATTAQUE \n \n");
    Game game = initGame();
    int nbTurns = 0;

    // Notre comportement à tous les tours
    while (nbTurns < 150) {
        // On récupère les informations de la partie
        updateGame(&game);

        // On se passe de l'aveuglement pour l'instant
        fprintf(stdout, "NOBLIND\n");
        fflush(stdout);

        int isBlinded = checkIfBlinded(game);

        // Si on est pas aveuglé alors on peut réveler une case
        if (isBlinded == FALSE) {
            InformationFromPosition informationDirectionToGo = directionToLookForward(game, getMyPlayer(
                    &game)->turtle.position);
            /*
             * Si on a trouvé une direction intéressante depuis notre position, alors on va révélé l'information sur cette ligne
             * ou colonne dans la direction qui nous intéresse
             */
            if (informationDirectionToGo.direction != NOT_FOUND) {
                fprintf(stderr, "TORTANK DEMANDE DE L'INFORMATION PROCHE \n \n");
                askInformationInTheDirection(&game, informationDirectionToGo.direction,
                                             getMyPlayer(&game)->turtle.position);
            } else {
                // Comportement au premier tour
                if (nbTurns == 0) {
                    // On tire au sort notre ligne ou notre colonne entre 4 et 8
                    srand(time(NULL));
                    int nbColumnOrLine = rand() % ((8 + 1) - 4) + 4;

                    // On demande des informations à l'opposé de notre position
                    Position randomPosition = {nbColumnOrLine, nbColumnOrLine};
                    askInformationInTheDirection(&game, (getMyPlayer(&game)->turtle.direction + 2) % 4, randomPosition);

                    fprintf(stderr, "TORTANK DEMANDE UNE INFORMATION AU HASARD \n \n");
                }
                    // Pas de révélation pour l'instant si on ne voit rien d'intéressant
                else {
                    InformationFromPosition bestPosition;
                    bestPosition.numberUnknown = 0;
                    bestPosition.direction = NOT_FOUND;
                    for (int origine = 2; origine < NUMBER_LINES - 2; origine++) {
                        InformationFromPosition informationFromPosition = directionToLookForward(game,
                                                                                                 (Position) {origine,
                                                                                                             6});

                        if (informationFromPosition.direction != NOT_FOUND &&
                            informationFromPosition.numberUnknown >= bestPosition.numberUnknown) {
                            bestPosition = informationFromPosition;
                        }
                    }
                    if (bestPosition.direction != UNKNOWN) {
                        askInformationInTheDirection(&game, bestPosition.direction,
                                                     bestPosition.position);
                        fprintf(stderr, "TORTANK REVELE DE L'INFORMATION DISTANTE \n \n");
                    } else {
                        fprintf(stderr, "TORTANK N'EST PAS ASSEZ INTELLIGENT POUR REVELER DE L'INFORMATION \n \n");
                        fprintf(stdout, "NOREVEAL\n");
                        fflush(stdout);
                    }
                }
            }
        } else {
            fprintf(stderr, "TORTANK EST AVEUGLE \n \n");
            fprintf(stdout, "NOREVEAL\n");
            fflush(stdout);
        }

        // On ne joue pas avec la paralysie
        fprintf(stdout, "NOPARALYZE\n");
        fflush(stdout);

        // On regarde si on est paralysé afin de savoir si on peut bouger
        int isParalyzed = checkIfParalyzed(game);
        if (isParalyzed == FALSE) {
            int nbPlays = 0;
            while (nbPlays < 4) {
                InformationFromPosition informationDirectionToScore = directionToScorePoints(game, getMyPlayer(
                        &game)->turtle.position);
                fprintf(stderr, "INFORMATION TO SCORE: points:%d, direction:%d \n",
                        informationDirectionToScore.possibleScore,
                        informationDirectionToScore.direction
                );

                // Si on trouve une direction pour marquer des points, alors on s'oriente puis on déplace vers
                if (informationDirectionToScore.direction != NOT_FOUND) {

                    fprintf(stderr, "TORTANK A TROUVE DES CASES NOIRES A PROXIMITE\n \n");


                    // Si on a pas la bonne orientation alors on doit tourner
                    if (informationDirectionToScore.direction != getMyPlayer(&game)->turtle.direction) {
                        // On se va pas faire une rotation pour rien, on ne fait une rotation que si on peut faire un mouvement après
                        if (nbPlays == 3) {
                            fprintf(stdout, "\n");
                            fflush(stdout);
                            break;
                        }
                        fprintf(stderr, "TORTANK DOIT SE TOURNER \n \n");
                        if (getMyPlayer(&game)->turtle.direction == RIGHT &&
                            informationDirectionToScore.direction == DOWN) {
                            fprintf(stdout, "ROTATE %d;", MOST_RIGHT);
                        } else if (getMyPlayer(&game)->turtle.direction == RIGHT &&
                                   informationDirectionToScore.direction == UP) {
                            fprintf(stdout, "ROTATE %d;", MOST_LEFT);
                        } else if (getMyPlayer(&game)->turtle.direction == LEFT &&
                                   informationDirectionToScore.direction == DOWN) {
                            fprintf(stdout, "ROTATE %d;", MOST_LEFT);
                        } else if (getMyPlayer(&game)->turtle.direction == LEFT &&
                                   informationDirectionToScore.direction == UP) {
                            fprintf(stdout, "ROTATE %d;", MOST_RIGHT);
                        } else if (getMyPlayer(&game)->turtle.direction == UP &&
                                   informationDirectionToScore.direction == RIGHT) {
                            fprintf(stdout, "ROTATE %d;", MOST_RIGHT);
                        } else if (getMyPlayer(&game)->turtle.direction == UP &&
                                   informationDirectionToScore.direction == LEFT) {
                            fprintf(stdout, "ROTATE %d;", MOST_LEFT);
                        } else if (getMyPlayer(&game)->turtle.direction == DOWN &&
                                   informationDirectionToScore.direction == RIGHT) {
                            fprintf(stdout, "ROTATE %d;", MOST_LEFT);
                        } else if (getMyPlayer(&game)->turtle.direction == DOWN &&
                                   informationDirectionToScore.direction == LEFT) {
                            fprintf(stdout, "ROTATE %d;", MOST_RIGHT);
                        }
                        getMyPlayer(&game)->turtle.direction = informationDirectionToScore.direction;
                        nbPlays++;
                    }


                    fprintf(stdout, "MOVE %d;", getNumberCellsBetweenTwoPositions(&game,
                                                                                  informationDirectionToScore.lastBlackCellUnchecked,
                                                                                  informationDirectionToScore.direction));
                    fflush(stdout);

                    nbPlays++;
                    if (nbPlays == 4) {
                        fprintf(stdout, "\n");
                        fflush(stdout);
                        break;
                    }

                } else {
                    InformationFromPosition informationPositionToTeleport;
                    informationPositionToTeleport.possibleScore = 0;
                    informationPositionToTeleport.position.x = -1;
                    informationPositionToTeleport.position.y = -1;
                    for (int i = 0; i < NUMBER_LINES; i++) {
                        for (int j = 0; j < NUMBER_COLUMNS; j++) {
                            if (game.grid[i][j] == BLACK && game.gridChecked[i][j] == FALSE) {
                                Position positionToLook = {i, j};
                                InformationFromPosition interestingInformation = directionToScorePoints(game,
                                                                                                        positionToLook);
                                if (interestingInformation.possibleScore >=
                                    informationPositionToTeleport.possibleScore) {
                                    informationPositionToTeleport = interestingInformation;
                                }
                            }
                        }
                    }

                    if (informationPositionToTeleport.position.x != -1 &&
                        informationPositionToTeleport.position.y != -1) {
                        fprintf(stderr, "TORTANK VA SE TELEPORTER\n \n");
                        fprintf(stdout, "TELEPORT %d %d;", informationPositionToTeleport.position.x,
                                informationPositionToTeleport.position.y);
                        getMyPlayer(&game)->turtle.position.x = informationPositionToTeleport.position.x;
                        getMyPlayer(&game)->turtle.position.y = informationPositionToTeleport.position.y;
                        fflush(stdout);
                        nbPlays++;
                        if (nbPlays == 4) {
                            fprintf(stdout, "\n");
                            break;
                        }

                        if (getMyPlayer(&game)->turtle.penIsDown == FALSE) {
                            fprintf(stdout, "SWITCHPEN\n");
                            fflush(stdout);
                            break;
                        } else {
                            fprintf(stdout, "PASS\n");
                            fflush(stdout);
                            break;
                        }
                    } else {
                        fprintf(stdout, "PASS\n");
                        fflush(stdout);
                        break;
                    }
                }
            }
        } else {
            fprintf(stdout, "PASS\n");
            fflush(stdout);
            break;
        }
        nbTurns++;
    }
    return 0;
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
    fscanf(stdin, "%d %d", &game.numberPlayers, &game.myNumero);
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
    return game;
}

void updateGame(Game *game) {
    for (int i = 0; i < game->numberPlayers; i++) {
        fscanf(stdin, "%d %d %d", &game->players[i].number,
               &game->players[i].score,
               &game->players[i].isRemove);
    }
    int nbCells = 0;
    fscanf(stdin, "%d", &nbCells);
    for (int i = 0; i < nbCells; i++) {
        int row, col, color, idPlayer;
        fscanf(stdin, "%d %d %d %d",
               &idPlayer,
               &row,
               &col,
               &color);
        game->grid[row][col] = color;
        game->gridChecked[row][col] = TRUE;
    }
    for (int i = 0; i < game->numberPlayers; i++) {
        int idPlayer;
        fscanf(stdin, "%d %d %d %d %d",
               &idPlayer,
               &game->players[i].turtle.position.x,
               &game->players[i].turtle.position.y,
               &game->players[i].turtle.direction,
               &game->players[i].turtle.penIsDown);
    }
    updateGrid(game);
}


int checkIfBlinded(Game game) {
    int isBlinded = FALSE;
    for (int i = 0; i < game.numberPlayers; i++) {
        int playerId, blind;
        fscanf(stdin, "%d %d",
               &playerId,
               &blind
        );
        if (playerId == game.myNumero &&
            blind == BLINDED) {
            isBlinded = TRUE;
        }
    }
    return isBlinded;
}

int checkIfParalyzed(Game game) {
    int isParalyzed = FALSE;
    for (int i = 0; i < game.numberPlayers; i++) {
        int playerId, para;
        fscanf(stdin, "%d %d",
               &playerId,
               &para
        );
        if (playerId == game.myNumero &&
            para == PARALYZED) {
            isParalyzed = TRUE;
        }
    }
    return isParalyzed;
}

void updateGrid(Game *game) {
    int hasChangedSomething = FALSE;
    /*
     * On regarde les lignes d'abord
     */
    for (int i = 0; i < NUMBER_LINES; i++) {
        int indexBlackBlock = -1;
        for (int j = 0; j < NUMBER_COLUMNS; j++) {
            if (game->grid[i][j] == BLACK) {
                // Cas où on détecte pour la première fois une case noire
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

    // On fait la même chose mais pour les colonnes
    for (int j = 0; j < NUMBER_COLUMNS; j++) {
        int indexBlackBlock = -1;
        for (int i = 0; i < NUMBER_LINES; i++) {
            if (game->grid[i][j] == BLACK) {
                // Cas où on détecte pour la première fois une case noire
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
        fprintf(stderr, "TORTANK EST AKINATOR\n \n");
        updateGrid(game);
    }
}

InformationFromPosition directionToScorePoints(Game game, Position position) {
    InformationFromPosition returnedInformation;
    returnedInformation.direction = NOT_FOUND;
    returnedInformation.possibleScore = 0;
    returnedInformation.position = position;
    int myDirection = getMyPlayer(&game)->turtle.direction;
    /*
     * On regarde dans toutes les directions le score possible
     * et on retient le meilleur score, on va dans la direction du meilleure score
     * obtenue, si aucune ligne ne rapporte de points on renvoie -1
     */
    InformationFromPosition informationLeft = informationLEFT(game, position);
    if (informationLeft.possibleScore > returnedInformation.possibleScore && myDirection != RIGHT) {
        returnedInformation = informationLeft;
    }
    InformationFromPosition informationRight = informationRIGHT(game, position);
    if (informationRight.possibleScore > returnedInformation.possibleScore && myDirection != LEFT) {
        returnedInformation = informationRight;
    }
    InformationFromPosition informationUp = informationUP(game, position);
    if (informationUp.possibleScore > returnedInformation.possibleScore && myDirection != DOWN) {
        returnedInformation = informationUp;
    }
    InformationFromPosition informationDown = informationDOWN(game, position);
    if (informationDown.possibleScore > returnedInformation.possibleScore && myDirection != UP) {
        returnedInformation = informationDown;
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
    InformationFromPosition informationLeft = informationLEFT(game, position);
    if (informationLeft.numberUnknown > returnedInformation.numberUnknown) {
        returnedInformation = informationLeft;
    }
    InformationFromPosition informationRight = informationRIGHT(game, position);
    if (informationRight.numberUnknown > returnedInformation.numberUnknown) {
        returnedInformation = informationRight;
    }
    InformationFromPosition informationUp = informationUP(game, position);
    if (informationUp.numberUnknown > returnedInformation.numberUnknown) {
        returnedInformation = informationUp;
    }
    InformationFromPosition informationDown = informationDOWN(game, position);
    if (informationDown.numberUnknown > returnedInformation.numberUnknown) {
        returnedInformation = informationDown;
    }
    return returnedInformation;
}

void askInformationInTheDirection(Game *game, int direction, Position position) {
    int cellRevealed;
    if (direction == LEFT) {
        fprintf(stdout, "REVEALL %d %d\n", position.x, MOST_LEFT);
        fflush(stdout);
        fscanf(stdin, "%d", &cellRevealed);
        for (int i = 0; i < cellRevealed; i++) {
            game->grid[position.x][i] = WHITE;
        }
        game->grid[position.x][cellRevealed] = BLACK;
    } else if (direction == DOWN) {
        fprintf(stdout, "REVEALC %d %d\n", position.y, MOST_UP);
        fflush(stdout);
        fscanf(stdin, "%d", &cellRevealed);
        for (int i = cellRevealed + 1; i < NUMBER_LINES; i++) {
            game->grid[i][position.y] = WHITE;
        }
        game->grid[cellRevealed][position.y] = BLACK;
    } else if (direction == UP) {
        fprintf(stdout, "REVEALC %d %d\n", position.y, MOST_DOWN);
        fflush(stdout);
        fscanf(stdin, "%d", &cellRevealed);
        for (int i = 0; i < cellRevealed; i++) {
            game->grid[i][position.y] = WHITE;
        }
        game->grid[cellRevealed][position.y] = BLACK;
    } else if (direction == RIGHT) {
        fprintf(stdout, "REVEALL %d %d\n", position.x, MOST_RIGHT);
        fflush(stdout);
        fscanf(stdin, "%d", &cellRevealed);
        for (int i = cellRevealed + 1; i < NUMBER_COLUMNS; i++) {
            game->grid[position.x][i] = WHITE;
        }
        game->grid[position.x][cellRevealed] = BLACK;
    }
    updateGrid(game);
}

Player *getMyPlayer(Game *game) {
    Player *myPlayer;
    for (int i = 0; i < game->numberPlayers; i++) {
        if (game->players[i].number == game->myNumero) {
            myPlayer = &game->players[i];
            break;
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

InformationFromPosition informationDOWN(Game game, Position position) {
    InformationFromPosition informationDOWN;
    informationDOWN.position = position;
    informationDOWN.direction = DOWN;
    informationDOWN.possibleScore = 0;
    informationDOWN.numberUnknown = 0;
    for (int i = position.x + 1; i < NUMBER_LINES; i++) {
        /*
         * Si au-dessus de nous, on a une case noire qui n'est pas déjà cochée
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

InformationFromPosition informationUP(Game game, Position position) {
    InformationFromPosition informationUP;
    informationUP.position = position;
    informationUP.direction = UP;
    informationUP.possibleScore = 0;
    informationUP.numberUnknown = 0;
    for (int i = position.x; i >= 0; i--) {
        /*
         * Si en-dessous de nous, on a une case noire qui n'est pas déjà cochée
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

int getNumberCellsBetweenTwoPositions(Game *game, Position to, int direction) {
    int number = 0;
    Position *from = &getMyPlayer(game)->turtle.position;
    switch (direction) {
        case LEFT:
            number = from->y - to.y;
            for (int i = to.y; i <= from->y; i++) {
                game->gridChecked[to.x][i] = TRUE;
            }
            from->y -= number;
            break;
        case RIGHT:
            number = to.y - from->y;
            for (int i = from->y; i <= to.y; i++) {
                game->gridChecked[to.x][i] = TRUE;
            }
            from->y += number;
            break;
        case UP:
            number = from->x - to.x;
            for (int i = to.x; i <= from->x; i++) {
                game->gridChecked[i][to.y] = TRUE;
            }
            from->x -= number;
            break;
        case DOWN:
            number = to.x - from->x;
            for (int i = from->x; i <= to.x; i++) {
                game->gridChecked[i][to.y] = TRUE;
            }
            from->x += number;
            break;
    }
    return number;
}
