/*
 * Fichier crée par JUSSEAUME Jonathan le 21/10/2021
 */


/*
 * Les librairies
 */
#include<stdio.h>

/*
 * Les constantes
 */

/*
 * Les valeurs booléenes
 */
#define TRUE 1
#define FALSE 0
/*
 * Les valeurs que peuvent prendre une case
 */
#define CHECKED 2
#define BLACK 1
#define WHITE 0
#define UNKNOWN -1
/*
 * Les valeurs d'orientation de la tortue
 */
#define LEFT 0
#define UP 1
#define RIGHT 2
#define DOWN 3

/*
 * Les modes pour demander les cases 
 */
#define MOST_LEFT 0
#define MOST_RIGHT 1
#define MOST_UP 1
#define MOST_DOWN 0

/*
 * Le nombre de lignes et de colonnes
 */
#define NUMBER_LINES 12
#define NUMBER_COLUMNS 12


/*
 * Les structures
 */

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
 * Structure qui définit ce qui constitue un joueur du jeu
 * Il se définit par un numéro, un nombre de points et un booléen
 * permettant de savoir si le joueur est retiré du jeu ou non
 */
typedef struct Player {
    int number;
    int score;
    int isRemove;
} Player;

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
 * Structure contenant toutes les informations essentielles de la partie.
 * Les informations que je possède sur la grille, par défaut toutes les cases valent UNKNOWN,
 * la liste des joueurs, le numéro de notre joueur, notre tortue
 */
typedef struct Game {
    int **grille;
    Player *players;
    int myNumero;
    Turtle turtle;
} Game;

/*
 * Les prototypes de fonction
 */
Game initGame();

/*
 * Le MAIN
 */
int main(void) {

}

/*
 * Les fonctions
 */

