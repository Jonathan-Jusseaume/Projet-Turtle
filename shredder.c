/*
 * Fichier crée par JUSSEAUME Jonathan le 21/10/2021
 * IA Shredder permettant de jouer à TURTLE PICROSS
 */

// Les librairies
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

// Les constantes
#define NUMBER_LINES 12
#define NUMBER_COLUMNS 12

// Les énumérations

/*
 * Enumération contenant les valeurs booléenes TRUE et FALSE valant respectivement 1 et 0
 */
typedef enum boolean {
    FALSE = 0, TRUE = 1
} boolean;

/*
 * Enumération contenant les valeurs possibles concernant l'aveuglement à savoir 1 si on est aveuglé et 0 sinon
 */
typedef enum blind_status {
    NOT_BLINDED = 0, BLINDED = 1
} blind_status;

/*
 * Enumération contenant les valeurs possibles concernant la paralysie à savoir 1 si on est paralysé et 0 sinon
 */
typedef enum paralyze_status {
    NOT_PARALYZED = 0, PARALYZED = 1
} paralyze_status;

/*
 * Enumération qui contient les valeurs des informations que l'on peut avoir sur une case, 1 si elle est blanche
 * 0 si elle est noire et -1 si on se sait pas sa couleur
*/
typedef enum cell_information {
    UNKNOWN = -1, BLACK = 0, WHITE = 1,
} cell_information;

/*
 * Enumération qui contient les directions possibles dans lesquelles peut aller la tortue
 */
typedef enum direction {
    NOT_FOUND = -1, LEFT = 0, UP = 1, RIGHT = 2, DOWN = 3
} direction;

/*
 * Enumération qui contient les valeurs pour demander de l'information sur les lignes
 */
typedef enum horizontal_ask {
    MOST_LEFT = 0, MOST_RIGHT = 1
} horizontal_ask;

/*
 * Enumération qui contient les valeurs pour demander de l'information sur les lignes
 */
typedef enum vertical_ask {
    MOST_DOWN = 0, MOST_UP = 1
} vertical_ask;

// Les structures

/*
 * Structure qui définit ce qui constitue une position sur la grille
 * à savoir une variable x qui correspond à la ligne à laquelle nous nous trouvons
 * et une variable y qui correspond à la colonne
 */
typedef struct position {
    int x;
    int y;
} position;

/*
 * Structure qui définit ce qui constitue la tortue de notre joueur.
 * Elle est définit par sa position sur le plateau, sa direction
 * ainsi que le fait que son crayon soit baissé ou non
 */
typedef struct turtle {
    position position;
    direction direction;
    boolean pen_is_down;
} turtle;


/*
 * Structure qui définit ce qui constitue un joueur du jeu
 * Il se définit par un numéro, un nombre de points, un booléen
 * permettant de savoir si le joueur est retiré du jeu ou non
 * et il possède une tortue
 */
typedef struct player {
    turtle turtle;
    int number;
    int score;
    boolean is_remove;
} player;


/*
 * Structure contenant toutes les informations essentielles de la partie.
 * Les informations que je possède sur la grille, par défaut toutes les cases valent UNKNOWN, une grille
 * qui mémorise si la case a déjà été cochée ou non par un joueur,
 * la liste des joueurs, le numéro de notre joueur ainsi que le nombre de joueurs
 */
typedef struct game {
    cell_information **grid;
    boolean **grid_checked;
    player *players;
    int number_players;
    int my_numero;
} game;

/*
 * Structure dont le principal rôle va être de nous permettre de renvoyer plusieurs informations depuis une position
 * dans une direction. On va notamment renvoyer le nombre de points possiblement obtenables, ou encore le nombre de cases
 * inconnues
 */
typedef struct information_from_position {
    position position;
    direction direction;
    int possible_score;
    int number_unknown;
    position position_last_black_cell_unchecked;
} information_from_position;

/*
 * Renvoie une partie initialisée avec une grille 12x12 avec que des UNKNOWN,
 * le nombre de joueurs obtenu grâce à l'entrée standard qui nous permet d'instancier
 * une liste de joueurs à la bonne taille. Enfin, le numéro de notre joueur est également obtenu
 */
game init_game();

/*
 * Affiche dans la sortie d'erreur les size éléments contenues ou non dans le pointeur list
 */
void display_array(int *list, int size);

/*
 * Met à jour la partie qui a l'adresse passé en paramètre grâce aux informations
 * envoyées par le jeu en début de tour.
 * Rafraichit notamment le statut des différents Joueurs
 */
void update_game(game *game);

/*
 * Renvoie TRUE si notre joueur est aveuglé durant ce tour, FALSE sinon
 */
boolean check_if_blinded(game game);

/*
 * Renvoie TRUE si notre joueur est aveuglé durant ce tour, FALSE sinon
 */
boolean check_if_paralyzed(game game);

/*
 * Met à jour la partie qui a l'adresse passé en paramètre et notamment sa grille
 * notamment en se basant sur le fait que le si sur une même ligne/colonne il y a deux cases noires
 * séparées alors tout ce qui est entre les deux sont des cases noires
 */
void update_grid(game *game);

/*
 * Renvoie les informations depuis une position passée en paramètre pour marquer un maximum de points durant ce tour.
 * La direction de information_from_position vaut -1 si nous n'avons pas trouvé de direction pour marquer des points sinon elle vaut
 * la valeur de la direction dans laquelle nous devons aller. Elle ne peut pas aller dans le sens contraire que la direction passée en paramètre.
 */
information_from_position direction_to_score_points(game game, position position, direction current_direction);

/*
 * Renvoie les informations par rapport à une position donnée en paramètre avec notamment la direction vers laquelle
 * il y a le plus de cases inconnues.
 * La direction de information_from_position vaut -1 si nous n'avons pas trouvé de direction où il y a des cases inconnues sinon elle vaut
 * la valeur de la direction dans laquelle nous devons regarder
 */
information_from_position direction_to_look_forward(game game, position position);

/*
 * Demande au jeu, l'information selon une case selon une direction et une position donnée puis met
 * à jour la partie dont a donné l'adresse en paramètre
 */
void ask_information_in_the_direction(game *game, int direction, position position);

/*
 * Renvoie différentes informations à gauche de la position donnée en paramètre
 */
information_from_position information_left(game game, position position);

/*
 * Renvoie différentes informations à droite de la position donnée en paramètre
 */
information_from_position information_right(game game, position position);

/*
 * Renvoie différentes informations en_dessous de la position donnée en paramètre
 */
information_from_position information_down(game game, position position);

/*
 * Renvoie différentes informations au-dessus de la position donnée en paramètre
 */
information_from_position information_up(game game, position position);

/*
 * Renvoie le nombre de cases qui séparent la première position passée en paramètre de la
 * deuxième grâce à la direction passée en paramètre
 */
int get_number_cells_between_two_positions(position from, position to, int direction);


/*
 * Main de l'application, il démarre par une initialisation de la partie,
 * puis notre IA va ensuite jouer les tours
 * @return 0 s'il n'y a pas eu de problèmes
 */
int main(void) {

    fprintf(stderr, "SHREDDER EST PRET AU COMBAT \n \n");

    game game = init_game();

    // On crée un pointeur pour accéder facilement à notre joueur
    player *my_player;

    int nb_turns = 0;

    // Notre comportement à tous les tours
    while (nb_turns < 150) {

        // On récupère les informations de la partie
        update_game(&game);

        // Au premier tour on met dans notre pointeur my_player, l'adresse de notre joueur
        if (nb_turns == 0) {
            for (int i = 0; i < game.number_players; i++) {
                if (game.players[i].number == game.my_numero) {
                    my_player = &game.players[i];
                    break;
                }
            }
        }

        // On se passe de l'aveuglement pour l'instant
        fprintf(stdout, "NOBLIND\n");
        fflush(stdout);

        boolean is_blinded = check_if_blinded(game);

        for (int i = 0; i < NUMBER_LINES; i++) {
            display_array(game.grid[i], NUMBER_COLUMNS);
        }

        // Si on est pas aveuglé alors on peut réveler une case
        if (is_blinded == FALSE) {
            information_from_position information_direction_to_go = direction_to_look_forward(game,
                                                                                              my_player->turtle.position);
            /*
             * Si on a trouvé une direction intéressante depuis notre position, alors on va révélé l'information sur cette ligne
             * ou colonne dans la direction qui nous intéresse
             */
            if (information_direction_to_go.direction != NOT_FOUND) {
                fprintf(stderr, "SHREDDER DEMANDE DE L'INFORMATION PROCHE \n \n");
                ask_information_in_the_direction(&game,
                                                 information_direction_to_go.direction,
                                                 my_player->turtle.position);
            } else {
                // Comportement au premier tour
                if (nb_turns == 0) {
                    // On tire au sort notre ligne ou notre colonne entre 4 et 6
                    srand(time(NULL));
                    int nb_column_or_row = rand() % ((6 + 1) - 4) + 4;

                    // On demande des informations à l'opposé de notre position
                    position random_position = {nb_column_or_row, nb_column_or_row};
                    ask_information_in_the_direction(&game,
                                                     (my_player->turtle.direction + 2) % 4,
                                                     random_position);

                    fprintf(stderr, "SHREDDER DEMANDE UNE INFORMATION AU HASARD \n \n");
                } else {
                    information_from_position best_position_to_reveal;
                    best_position_to_reveal.number_unknown = 0;
                    best_position_to_reveal.direction = NOT_FOUND;
                    for (int origine = 1; origine < NUMBER_LINES - 1; origine++) {
                        information_from_position information = direction_to_look_forward(game,
                                                                                          (position) {
                                                                                                  origine,
                                                                                                  6});

                        if (information.direction != NOT_FOUND &&
                            information.number_unknown >= best_position_to_reveal.number_unknown) {
                            best_position_to_reveal = information;
                        }
                    }
                    if (best_position_to_reveal.direction != NOT_FOUND) {
                        ask_information_in_the_direction(&game,
                                                         best_position_to_reveal.direction,
                                                         best_position_to_reveal.position);
                        fprintf(stderr, "SHREDDER REVELE DE L'INFORMATION DISTANTE \n \n");
                    } else {
                        fprintf(stderr, "SHREDDER N'EST PAS ASSEZ INTELLIGENT POUR REVELER DE L'INFORMATION \n \n");
                        fprintf(stdout, "NOREVEAL\n");
                        fflush(stdout);
                    }
                }
            }
        } else {
            fprintf(stderr, "SHREDDER EST AVEUGLE \n \n");
            fprintf(stdout, "NOREVEAL\n");
            fflush(stdout);
        }

        // On ne joue pas avec la paralysie
        fprintf(stdout, "NOPARALYZE\n");
        fflush(stdout);

        // On regarde si on est paralysé afin de savoir si on peut bouger
        int isParalyzed = check_if_paralyzed(game);
        for (int i = 0; i < NUMBER_LINES; i++) {
            display_array(game.grid[i], NUMBER_COLUMNS);
        }


        if (isParalyzed == FALSE) {

            /*
            * On cherche une position depuis laquelle partir pour marquer des points
            */
            information_from_position best_position_to_score;
            best_position_to_score.possible_score = 0;
            best_position_to_score.position.x = -1;
            best_position_to_score.position.y = -1;
            for (int i = 0; i < NUMBER_LINES; i++) {
                for (int j = 0; j < NUMBER_COLUMNS; j++) {
                    if (game.grid[i][j] == BLACK) {
                        position position_to_look = {i, j};
                        information_from_position information = direction_to_score_points(game,
                                                                                          position_to_look,
                                                                                          my_player->turtle.direction);
                        /*
                         * Si on est sur notre position on supprime 3 points car on va se téléporter
                         */
                        if (position_to_look.x == my_player->turtle.position.x &&
                            position_to_look.y == my_player->turtle.position.y) {
                            information.possible_score -= 3;
                        }

                        /*
                         * Si on est déjà dans la bonne direction, on retire 1 point car notre ROTATE va nous couter cela
                         */
                        if (information.direction == my_player->turtle.direction) {
                            information.possible_score -= 1;
                        }

                        if (information.possible_score >= best_position_to_score.possible_score) {
                            best_position_to_score = information;
                        }
                    }
                }
            }
            /*
             * On trouve notre position
             */
            if (best_position_to_score.direction != NOT_FOUND) {
                // Si la meilleure position n'est pas la notre alors on se téléporte
                if (best_position_to_score.position.x != my_player->turtle.position.x
                    || best_position_to_score.position.y != my_player->turtle.position.y) {
                    fprintf(stderr, "SHREDDER VA SE TELEPORTER\n \n");
                    fprintf(stdout, "TELEPORT %d %d;", best_position_to_score.position.x,
                            best_position_to_score.position.y);
                    fflush(stdout);
                }
                if (my_player->turtle.pen_is_down == FALSE) {
                    fprintf(stdout, "SWITCHPEN;");
                    fflush(stdout);
                }
                // On fait une rotation si on est pas dans la bonne direction
                if (best_position_to_score.direction != my_player->turtle.direction) {
                    fprintf(stderr, "SHREDDER DOIT SE TOURNER \n \n");
                    if (my_player->turtle.direction == RIGHT && best_position_to_score.direction == DOWN) {
                        fprintf(stdout, "ROTATE %d;", MOST_RIGHT);
                    } else if (my_player->turtle.direction == RIGHT &&
                               best_position_to_score.direction == UP) {
                        fprintf(stdout, "ROTATE %d;", MOST_LEFT);
                    } else if (my_player->turtle.direction == LEFT &&
                               best_position_to_score.direction == DOWN) {
                        fprintf(stdout, "ROTATE %d;", MOST_LEFT);
                    } else if (my_player->turtle.direction == LEFT &&
                               best_position_to_score.direction == UP) {
                        fprintf(stdout, "ROTATE %d;", MOST_RIGHT);
                    } else if (my_player->turtle.direction == UP &&
                               best_position_to_score.direction == RIGHT) {
                        fprintf(stdout, "ROTATE %d;", MOST_RIGHT);
                    } else if (my_player->turtle.direction == UP &&
                               best_position_to_score.direction == LEFT) {
                        fprintf(stdout, "ROTATE %d;", MOST_LEFT);
                    } else if (my_player->turtle.direction == DOWN &&
                               best_position_to_score.direction == RIGHT) {
                        fprintf(stdout, "ROTATE %d;", MOST_LEFT);
                    } else if (my_player->turtle.direction == DOWN &&
                               best_position_to_score.direction == LEFT) {
                        fprintf(stdout, "ROTATE %d;", MOST_RIGHT);
                    }
                    fflush(stdout);
                }
                int number_cells = get_number_cells_between_two_positions(best_position_to_score.position,
                                                                          best_position_to_score.position_last_black_cell_unchecked,
                                                                          best_position_to_score.direction);
                if (number_cells > 0) {
                    fprintf(stdout, "MOVE %d\n", number_cells);
                    fflush(stdout);
                } else {
                    fprintf(stdout, "PASS\n");
                    fflush(stdout);
                }
            } else {
                fprintf(stdout, "PASS\n");
                fflush(stdout);
            }
        } else {
            fprintf(stderr, "SHREDDER EST PARALYSE \n");
            fprintf(stdout, "PASS\n");
            fflush(stdout);
        }
        nb_turns++;
    }
    return 0;
}

void display_array(int *list, int size) {
    fprintf(stderr, "[");
    for (int i = 0; i < size; i++) {
        fprintf(stderr, "%d", list[i]);
        if (size - 1 != i) {
            fprintf(stderr, ",");
        }
    }
    fprintf(stderr, "] \n");
}

game init_game() {
    game game;
    fscanf(stdin, "%d %d", &game.number_players, &game.my_numero);
    game.players = (player *) malloc(game.number_players * sizeof(player));
    game.grid = (cell_information **) malloc(NUMBER_LINES * sizeof(int *));
    game.grid_checked = (boolean **) malloc(NUMBER_LINES * sizeof(int *));
    for (int i = 0; i < NUMBER_LINES; i++) {
        game.grid[i] = (cell_information *) malloc(NUMBER_COLUMNS * sizeof(int));
        game.grid_checked[i] = (boolean *) malloc(NUMBER_COLUMNS * sizeof(int));
        for (int j = 0; j < NUMBER_COLUMNS; j++) {
            if (i == 0 || i == NUMBER_LINES - 1 || j == 0 || j == NUMBER_COLUMNS - 1) {
                game.grid[i][j] = WHITE;
            } else {
                game.grid[i][j] = UNKNOWN;
            }
            game.grid_checked[i][j] = FALSE;
        }
    }
    return game;
}

void update_game(game *game) {
    for (int i = 0; i < game->number_players; i++) {
        fscanf(stdin, "%d %d %d", &game->players[i].number,
               &game->players[i].score,
               (int *) &game->players[i].is_remove);
    }
    int nbCells = 0;
    fscanf(stdin, "%d", &nbCells);
    for (int i = 0; i < nbCells; i++) {
        int row, col, color, id_player;
        fscanf(stdin, "%d %d %d %d",
               &id_player,
               &row,
               &col,
               &color);
        game->grid[row][col] = color;
        game->grid_checked[row][col] = TRUE;
    }
    for (int i = 0; i < game->number_players; i++) {
        int id_player;
        fscanf(stdin, "%d %d %d %d %d",
               &id_player,
               &game->players[i].turtle.position.x,
               &game->players[i].turtle.position.y,
               (int *) &game->players[i].turtle.direction,
               (int *) &game->players[i].turtle.pen_is_down);
    }
    update_grid(game);
}

boolean check_if_blinded(game game) {
    boolean is_blinded = FALSE;
    for (int i = 0; i < game.number_players; i++) {
        int player_id, blind;
        fscanf(stdin, "%d %d",
               &player_id,
               &blind
        );
        if (player_id == game.my_numero &&
            blind == BLINDED) {
            is_blinded = TRUE;
        }
    }
    return is_blinded;
}

boolean check_if_paralyzed(game game) {
    int is_paralyzed = FALSE;
    for (int i = 0; i < game.number_players; i++) {
        int player_id, para;
        fscanf(stdin, "%d %d",
               &player_id,
               &para
        );
        if (player_id == game.my_numero &&
            para == PARALYZED) {
            is_paralyzed = TRUE;
        }
    }
    return is_paralyzed;
}

void update_grid(game *game) {
    int has_changed_something = FALSE;
    /*
     * On regarde les lignes d'abord
     */
    for (int i = 0; i < NUMBER_LINES; i++) {
        int index_black_cell = -1;
        for (int j = 0; j < NUMBER_COLUMNS; j++) {
            if (game->grid[i][j] == BLACK) {
                // Cas où on détecte pour la première fois une case noire
                if (index_black_cell == -1) {
                    index_black_cell = j;
                } else {
                    /*
                     * S'il y a plus d'une case d'écart entre deux cases noires
                     * alors il faut combler le trou par des cases noires, on a donc
                     * modifié des cases
                     */
                    if (j - index_black_cell > 1) {
                        for (int k = index_black_cell + 1; k < j; k++) {
                            game->grid[i][k] = BLACK;
                        }
                        has_changed_something = TRUE;
                    }
                    index_black_cell = j;
                }
            }
        }
    }

    // On fait la même chose mais pour les colonnes
    for (int j = 0; j < NUMBER_COLUMNS; j++) {
        int index_black_cell = -1;
        for (int i = 0; i < NUMBER_LINES; i++) {
            if (game->grid[i][j] == BLACK) {
                // Cas où on détecte pour la première fois une case noire
                if (index_black_cell == -1) {
                    index_black_cell = i;
                } else {
                    /*
                     * S'il y a plus d'une case d'écart entre deux cases noires
                     * alors il faut combler le trou par des cases noires, on a donc
                     * modifié des cases
                     */
                    if (i - index_black_cell > 1) {
                        for (int k = index_black_cell + 1; k < i; k++) {
                            game->grid[k][j] = BLACK;
                        }
                        has_changed_something = TRUE;
                    }
                    index_black_cell = i;
                }
            }
        }
    }

    /*
     * Si on a changé quelque chose peut être que l'on a trouvé des informations
     * intéressantes sur une autre colonne ou ligne, on lance donc une récursion jusqu'à ce qu'on ne
     * détecte plus aucune modification
     */
    if (has_changed_something == TRUE) {
        fprintf(stderr, "SHREDDER EST AKINATOR\n \n");
        update_grid(game);
    }
}

information_from_position direction_to_score_points(game game, position position, direction current_direction) {
    information_from_position returned_information;
    returned_information.direction = NOT_FOUND;
    returned_information.possible_score = 0;
    returned_information.position = position;
    /*
     * On regarde dans toutes les directions le score possible
     * et on retient le meilleur score, on va dans la direction du meilleure score
     * obtenue, si aucune ligne ne rapporte de points on renvoie -1
     */
    information_from_position information_l = information_left(game, position);
    if (information_l.possible_score > returned_information.possible_score && current_direction != RIGHT) {
        returned_information = information_l;
    }
    information_from_position information_r = information_right(game, position);
    if (information_r.possible_score > returned_information.possible_score && current_direction != LEFT) {
        returned_information = information_r;
    }
    information_from_position information_u = information_up(game, position);
    if (information_u.possible_score > returned_information.possible_score && current_direction != DOWN) {
        returned_information = information_u;
    }
    information_from_position information_d = information_down(game, position);
    if (information_d.possible_score > returned_information.possible_score && current_direction != UP) {
        returned_information = information_d;
    }
    return returned_information;
}

information_from_position direction_to_look_forward(game game, position position) {
    information_from_position returned_information;
    returned_information.direction = NOT_FOUND;
    returned_information.number_unknown = 0;
    returned_information.position = position;
    /*
     * On regarde dans toutes les directions le score possible
     * et on retient la direction dans laquelle il y a le plus de cases inconnues
     * Si aucune direction est satisfaisante on renvoie -1
     */
    information_from_position information_l = information_left(game, position);
    if (information_l.number_unknown > returned_information.number_unknown) {
        returned_information = information_l;
    }
    information_from_position information_r = information_right(game, position);
    if (information_r.number_unknown > returned_information.number_unknown) {
        returned_information = information_r;
    }
    information_from_position information_u = information_up(game, position);
    if (information_u.number_unknown > returned_information.number_unknown) {
        returned_information = information_u;
    }
    information_from_position information_d = information_down(game, position);
    if (information_d.number_unknown > returned_information.number_unknown) {
        returned_information = information_d;
    }
    return returned_information;
}

void ask_information_in_the_direction(game *game, int direction, position position) {
    int cell_revealed;
    if (direction == LEFT) {
        fprintf(stdout, "REVEALL %d %d\n", position.x, MOST_LEFT);
        fflush(stdout);
        fscanf(stdin, "%d", &cell_revealed);
        for (int i = 0; i < cell_revealed; i++) {
            game->grid[position.x][i] = WHITE;
        }
        game->grid[position.x][cell_revealed] = BLACK;
    } else if (direction == DOWN) {
        fprintf(stdout, "REVEALC %d %d\n", position.y, MOST_UP);
        fflush(stdout);
        fscanf(stdin, "%d", &cell_revealed);
        for (int i = cell_revealed + 1; i < NUMBER_LINES; i++) {
            game->grid[i][position.y] = WHITE;
        }
        game->grid[cell_revealed][position.y] = BLACK;
    } else if (direction == UP) {
        fprintf(stdout, "REVEALC %d %d\n", position.y, MOST_DOWN);
        fflush(stdout);
        fscanf(stdin, "%d", &cell_revealed);
        for (int i = 0; i < cell_revealed; i++) {
            game->grid[i][position.y] = WHITE;
        }
        game->grid[cell_revealed][position.y] = BLACK;
    } else if (direction == RIGHT) {
        fprintf(stdout, "REVEALL %d %d\n", position.x, MOST_RIGHT);
        fflush(stdout);
        fscanf(stdin, "%d", &cell_revealed);
        for (int i = cell_revealed + 1; i < NUMBER_COLUMNS; i++) {
            game->grid[position.x][i] = WHITE;
        }
        game->grid[position.x][cell_revealed] = BLACK;
    }
    update_grid(game);
}

information_from_position information_left(game game, position position) {
    information_from_position information_left;
    information_left.position = position;
    information_left.direction = LEFT;
    information_left.possible_score = 0;
    information_left.number_unknown = 0;
    for (int i = position.y; i > 0; i--) {
        /*
         * Si à notre gauche, on a une case noire qui n'est pas déjà cochée
         * alors on augmente le score de gauche de 1
         */
        if (game.grid[position.x][i] == BLACK
            && game.grid_checked[position.x][i] == FALSE) {
            information_left.possible_score += 2;
            information_left.position_last_black_cell_unchecked.x = position.x;
            information_left.position_last_black_cell_unchecked.y = i;
        }
        if (game.grid[position.x][i] == UNKNOWN) {
            information_left.number_unknown++;
        }
    }
    return information_left;
}

information_from_position information_right(game game, position position) {
    information_from_position information_right;
    information_right.position = position;
    information_right.direction = RIGHT;
    information_right.possible_score = 0;
    information_right.number_unknown = 0;
    for (int i = position.y; i < NUMBER_COLUMNS; i++) {
        /*
         * Si à notre droite, on a une case noire qui n'est pas déjà cochée
         * alors on augmente le score de gauche de 1
         */
        if (game.grid[position.x][i] == BLACK
            && game.grid_checked[position.x][i] == FALSE) {
            information_right.possible_score += 2;
            information_right.position_last_black_cell_unchecked.x = position.x;
            information_right.position_last_black_cell_unchecked.y = i;
        }
        if (game.grid[position.x][i] == UNKNOWN) {
            information_right.number_unknown++;
        }
    }
    return information_right;
}

information_from_position information_down(game game, position position) {
    information_from_position information_down;
    information_down.position = position;
    information_down.direction = DOWN;
    information_down.possible_score = 0;
    information_down.number_unknown = 0;
    for (int i = position.x + 1; i < NUMBER_LINES; i++) {
        /*
         * Si au-dessus de nous, on a une case noire qui n'est pas déjà cochée
         * alors on augmente le score de gauche de 1
         */
        if (game.grid[i][position.y] == BLACK
            && game.grid_checked[i][position.y] == FALSE) {
            information_down.possible_score += 2;
            information_down.position_last_black_cell_unchecked.x = i;
            information_down.position_last_black_cell_unchecked.y = position.y;
        }
        if (game.grid[i][position.y] == UNKNOWN) {
            information_down.number_unknown++;
        }
    }
    return information_down;
}

information_from_position information_up(game game, position position) {
    information_from_position information_up;
    information_up.position = position;
    information_up.direction = UP;
    information_up.possible_score = 0;
    information_up.number_unknown = 0;
    for (int i = position.x; i >= 0; i--) {
        /*
         * Si en-dessous de nous, on a une case noire qui n'est pas déjà cochée
         * alors on augmente le score de gauche de 1
         */
        if (game.grid[i][position.y] == BLACK
            && game.grid_checked[i][position.y] == FALSE) {
            information_up.possible_score += 2;
            information_up.position_last_black_cell_unchecked.x = i;
            information_up.position_last_black_cell_unchecked.y = position.y;
        }
        if (game.grid[i][position.y] == UNKNOWN) {
            information_up.number_unknown++;
        }
    }
    return information_up;
}

int get_number_cells_between_two_positions(position from, position to, int direction) {
    int number = 0;
    switch (direction) {
        case LEFT:
            number = from.y - to.y;
            break;
        case RIGHT:
            number = to.y - from.y;
            break;
        case UP:
            number = from.x - to.x;
            break;
        case DOWN:
            number = to.x - from.x;
            break;
    }
    return number;
}
