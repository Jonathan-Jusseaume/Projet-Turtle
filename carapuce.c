/**
 * Fichier crée par JUSSEAUME Jonathan le 21/10/2021
 */


/**
 * Les librairies
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
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
#define BLACK 1
#define WHITE 0
#define UNKNOWN -1

/**
 * Les valeurs d'orientation de la tortue
 */
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
 * Main de l'application, il démarre par une initialisation de la partie,
 * puis notre IA va ensuite jouer les tours
 * @return 0 s'il n'y a pas eu de problèmes
 */
int main(void) {
    fprintf(stderr, "CARAPUCE A L'ATTAQUE \n");
    /*
     * On initialise notre partie
     */
    Game game = initGame();
    int nbTurns = 0;

    /*
     * Cette variable correspond à la ligne ou la colonne que l'on va
     * cibler, au début c'est la numéro 6 car elle est presque au milieu donc on est sur de voir
     * des cases noires
     */
    int target = 6;
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
    nbTours++;
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
    nbTours++;


    while (nbTours < 150) {
        fprintf(stdout, "NOBLIND\n");
        fflush(stdout);
        fprintf(stdout, "NOREVEAL\n");
        fflush(stdout);
        fprintf(stdout, "NOPARALYZE\n");
        fflush(stdout);
        fprintf(stdout, "PASS\n");
        fflush(stdout);

        nbTours++;
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
            game.gridChecked[i] = FALSE;
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
        fprintf(stderr, "Cell %d: l=%d, c=%d \n", i, cellInfo[1], cellInfo[2]);
        game->grid[cellInfo[1]][cellInfo[2]] = cellInfo[3];
        game->gridChecked[cellInfo[1]][cellInfo[2]] = TRUE;
        free(cellInfo);
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

}