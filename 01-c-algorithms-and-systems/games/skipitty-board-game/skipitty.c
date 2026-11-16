#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SAVE_FILE "skipitty_save.bin"
#define EMPTY 0
#define PLAYER_A 'A'
#define PLAYER_B 'B'
#define PLAYER_C 'C'
#define PLAYER_D 'D'
#define PLAYER_E 'E'


// Structure representing a move
typedef struct {
    int from_x, from_y, to_x, to_y;
    char piece;
    int captured_x, captured_y; // Coordinates of the captured piece
    char captured_piece; // Captured piece
} Move;

// Structure representing the game state
typedef struct {
    char **board;
    int size;
    int score_player_A;
    int score_player_B;
    int score_player_C;
    int score_player_D;
    int score_player_E;
    int score_computer_A;
    int score_computer_B;
    int score_computer_C;
    int score_computer_D;
    int score_computer_E;
    int player_score;
    int computer_score;
    int player_turn;
    Move last_move_A;
    Move last_move_B;
} GameState;

// Initialises the game board
void initialize_board(GameState *game) {
    int mid = game->size / 2;
    char pieces[] = {PLAYER_A, PLAYER_B, PLAYER_C, PLAYER_D, PLAYER_E};
    
    for (int i = 0; i < game->size; i++) {
        for (int j = 0; j < game->size; j++) {
            if ((i == mid - 1 && (j == mid - 1 || j == mid)) || (i == mid && (j == mid - 1 || j == mid))) {
                game->board[i][j] = EMPTY;
            } else {
                game->board[i][j] = pieces[rand() % 5];
            }
        }
    }
}

// Prints the game board
void print_board(GameState *game) {
    printf("  ");
    for (int j = 0; j < game->size; j++) {
        printf("%2d  ", j);
    }
    printf("\n");

    for (int i = 0; i < game->size; i++) {
        printf("%2d", i);
        for (int j = 0; j < game->size; j++) {
            printf("+---");
        }
        printf("+\n");
        printf("  ");
        for (int j = 0; j < game->size; j++) {
            if (game->board[i][j] == EMPTY) {
                printf("|   ");
            } else {
                printf("| %c ", game->board[i][j]);
            }
        }
        printf("|\n");
    }
    printf("  ");
    for (int j = 0; j < game->size; j++) {
        printf("+---");
    }
    printf("+\n");
}

// Moves a piece
int move_piece(GameState *game, int from_x, int from_y, int to_x, int to_y, Move *last_move) {
    if (from_x < 0 || from_x >= game->size || from_y < 0 || from_y >= game->size ||
        to_x < 0 || to_x >= game->size || to_y < 0 || to_y >= game->size) {
        printf("Invalid coordinates.\n");
        return 0;
    }

    if (game->board[from_x][from_y] == EMPTY || game->board[to_x][to_y] != EMPTY) {
        printf("Invalid move.\n");
        return 0;
    }

    int mid_x = (from_x + to_x) / 2;
    int mid_y = (from_y + to_y) / 2;
    char middle_piece = game->board[mid_x][mid_y];

    if (middle_piece != EMPTY && (abs(from_x - to_x) == 2 || abs(from_y - to_y) == 2)) {
        game->board[mid_x][mid_y] = EMPTY;
        // Store the captured piece
        last_move->captured_x = mid_x;
        last_move->captured_y = mid_y;
        last_move->captured_piece = middle_piece;

        // Update the scores
        if (game->player_turn == PLAYER_A) {
            if (middle_piece == PLAYER_A) {
                game->score_player_A++;
            } else if (middle_piece == PLAYER_B) {
                game->score_player_B++;
            } else if (middle_piece == PLAYER_C) {
                game->score_player_C++;
            } else if (middle_piece == PLAYER_D) {
                game->score_player_D++;
            } else if (middle_piece == PLAYER_E) {
                game->score_player_E++;
            }
        } else if (game->player_turn == PLAYER_B) {
            if (middle_piece == PLAYER_A) {
                game->score_computer_A++;
            } else if (middle_piece == PLAYER_B) {
                game->score_computer_B++;
            } else if (middle_piece == PLAYER_C) {
                game->score_computer_C++;
            } else if (middle_piece == PLAYER_D) {
                game->score_computer_D++;
            } else if (middle_piece == PLAYER_E) {
                game->score_computer_E++;
            }
        }
    } else {
        printf("Invalid move: you must jump over a piece.\n");
        return 0;
    }

    char piece = game->board[from_x][from_y];
    game->board[from_x][from_y] = EMPTY;
    game->board[to_x][to_y] = piece;

    last_move->from_x = from_x;
    last_move->from_y = from_y;
    last_move->to_x = to_x;
    last_move->to_y = to_y;
    last_move->piece = piece;

    return 1;
}

// Adds points for the pieces
void add_pieces_to_score(GameState *game, int *score_A, int *score_B, int *score_C, int *score_D, int *score_E) {
    if (*score_A > 1) game->score_player_A += (*score_A - 1);
    if (*score_B > 1) game->score_player_B += (*score_B - 1);
    if (*score_C > 1) game->score_player_C += (*score_C - 1);
    if (*score_D > 1) game->score_player_D += (*score_D - 1);
    if (*score_E > 1) game->score_player_E += (*score_E - 1);
}

// Checks the scores
void check_scores(GameState *game) {
    int remaining_A = game->score_player_A > 1 ? game->score_player_A - 1 : 0;
    int remaining_B = game->score_player_B > 1 ? game->score_player_B - 1 : 0;
    int remaining_C = game->score_player_C > 1 ? game->score_player_C - 1 : 0;
    int remaining_D = game->score_player_D > 1 ? game->score_player_D - 1 : 0;
    int remaining_E = game->score_player_E > 1 ? game->score_player_E - 1 : 0;

    if (game->score_player_A >= 1 && game->score_player_B >= 1 && game->score_player_C >= 1 &&
        game->score_player_D >= 1 && game->score_player_E >= 1) {
        game->player_score++;
        add_pieces_to_score(game, &remaining_A, &remaining_B, &remaining_C, &remaining_D, &remaining_E);
        game->score_player_A = game->score_player_B = game->score_player_C = game->score_player_D = game->score_player_E = 0;
    }

    remaining_A = game->score_computer_A > 1 ? game->score_computer_A - 1 : 0;
    remaining_B = game->score_computer_B > 1 ? game->score_computer_B - 1 : 0;
    remaining_C = game->score_computer_C > 1 ? game->score_computer_C - 1 : 0;
    remaining_D = game->score_computer_D > 1 ? game->score_computer_D - 1 : 0;
    remaining_E = game->score_computer_E > 1 ? game->score_computer_E - 1 : 0;

    if (game->score_computer_A >= 1 && game->score_computer_B >= 1 && game->score_computer_C >= 1 &&
        game->score_computer_D >= 1 && game->score_computer_E >= 1) {
        game->computer_score++;
        add_pieces_to_score(game, &remaining_A, &remaining_B, &remaining_C, &remaining_D, &remaining_E);
        game->score_computer_A = game->score_computer_B = game->score_computer_C = game->score_computer_D = game->score_computer_E = 0;
    }
}

// Undoes a move
void undo_move(GameState *game, Move *last_move) {
    if (last_move->from_x != -1) {
        game->board[last_move->to_x][last_move->to_y] = EMPTY;
        game->board[last_move->from_x][last_move->from_y] = last_move->piece;

        if (last_move->captured_piece != EMPTY) {
            game->board[last_move->captured_x][last_move->captured_y] = last_move->captured_piece;
            
            if (game->player_turn == PLAYER_A) {
                if (last_move->captured_piece == PLAYER_A) {
                    game->score_player_A--;
                } else if (last_move->captured_piece == PLAYER_B) {
                    game->score_player_B--;
                } else if (last_move->captured_piece == PLAYER_C) {
                    game->score_player_C--;
                } else if (last_move->captured_piece == PLAYER_D) {
                    game->score_player_D--;
                } else if (last_move->captured_piece == PLAYER_E) {
                    game->score_player_E--;
                }
            } else if (game->player_turn == PLAYER_B) {
                if (last_move->captured_piece == PLAYER_A) {
                    game->score_computer_A--;
                } else if (last_move->captured_piece == PLAYER_B) {
                    game->score_computer_B--;
                } else if (last_move->captured_piece == PLAYER_C) {
                    game->score_computer_C--;
                } else if (last_move->captured_piece == PLAYER_D) {
                    game->score_computer_D--;
                } else if (last_move->captured_piece == PLAYER_E) {
                    game->score_computer_E--;
                }
            }
        }

        last_move->from_x = -1;
    } else {
        printf("No undo available.\n");
    }
}

// Redoes a move
void redo_move(GameState *game, Move *last_move) {
    if (last_move->from_x != -1) {
        game->board[last_move->from_x][last_move->from_y] = EMPTY;
        game->board[last_move->to_x][last_move->to_y] = last_move->piece;
        last_move->from_x = -1;
    } else {
        printf("No redo available.\n");
    }
}

// Saves the game
void save_game(GameState *game) {
    FILE *file = fopen(SAVE_FILE, "wb");
    if (!file) {
        printf("An error occurred while saving the game.\n");
        return;
    }

    fwrite(&game->size, sizeof(int), 1, file);
    fwrite(&game->score_player_A, sizeof(int), 1, file);
    fwrite(&game->score_player_B, sizeof(int), 1, file);
    fwrite(&game->score_player_C, sizeof(int), 1, file);
    fwrite(&game->score_player_D, sizeof(int), 1, file);
    fwrite(&game->score_player_E, sizeof(int), 1, file);
    fwrite(&game->score_computer_A, sizeof(int), 1, file);
    fwrite(&game->score_computer_B, sizeof(int), 1, file);
    fwrite(&game->score_computer_C, sizeof(int), 1, file);
    fwrite(&game->score_computer_D, sizeof(int), 1, file);
    fwrite(&game->score_computer_E, sizeof(int), 1, file);
    fwrite(&game->player_score, sizeof(int), 1, file);
    fwrite(&game->computer_score, sizeof(int), 1, file);
    fwrite(&game->player_turn, sizeof(int), 1, file);
    for (int i = 0; i < game->size; i++) {
        fwrite(game->board[i], sizeof(char), game->size, file);
    }

    fclose(file);
    printf("Game saved successfully.\n");
}

// Loads the game
int load_game(GameState *game) {
    FILE *file = fopen(SAVE_FILE, "rb");
    if (!file) {
        printf("No saved game found.\n");
        return 0;
    }

    fread(&game->size, sizeof(int), 1, file);
    fread(&game->score_player_A, sizeof(int), 1, file);
    fread(&game->score_player_B, sizeof(int), 1, file);
    fread(&game->score_player_C, sizeof(int), 1, file);
    fread(&game->score_player_D, sizeof(int), 1, file);
    fread(&game->score_player_E, sizeof(int), 1, file);
    fread(&game->score_computer_A, sizeof(int), 1, file);
    fread(&game->score_computer_B, sizeof(int), 1, file);
    fread(&game->score_computer_C, sizeof(int), 1, file);
    fread(&game->score_computer_D, sizeof(int), 1, file);
    fread(&game->score_computer_E, sizeof(int), 1, file);
    fread(&game->player_score, sizeof(int), 1, file);
    fread(&game->computer_score, sizeof(int), 1, file);
    fread(&game->player_turn, sizeof(int), 1, file);

    game->board = (char**)malloc(game->size * sizeof(char*));
    for (int i = 0; i < game->size; i++) {
        game->board[i] = (char*)malloc(game->size * sizeof(char));
        fread(game->board[i], sizeof(char), game->size, file);
    }

    fclose(file);
    printf("Game loaded successfully.\n");
    return 1;
}

// Evaluates a move
int evaluate_move(GameState *game, int from_x, int from_y, int to_x, int to_y) {
    int score = 0;
    if (from_x < 0 || from_x >= game->size || from_y < 0 || from_y >= game->size ||
        to_x < 0 || to_x >= game->size || to_y < 0 || to_y >= game->size ||
        game->board[from_x][from_y] == EMPTY || game->board[to_x][to_y] != EMPTY) {
        return -1;
    }

    int mid_x = (from_x + to_x) / 2;
    int mid_y = (from_y + to_y) / 2;
    char middle_piece = game->board[mid_x][mid_y];

    if (middle_piece != EMPTY && (abs(from_x - to_x) == 2 || abs(from_y - to_y) == 2) &&
        (from_x == to_x || from_y == to_y)) {
        score += 1;
    }

    return score;
}

// Makes the computer's move
void computer_move(GameState *game) {
    int best_score = -1000;
    int best_from_x = -1, best_from_y = -1, best_to_x = -1, best_to_y = -1;

    for (int from_x = 0; from_x < game->size; from_x++) {
        for (int from_y = 0; from_y < game->size; from_y++) {
            if (game->board[from_x][from_y] == PLAYER_B) {
                for (int to_x = 0; to_x < game->size; to_x++) {
                    for (int to_y = 0; to_y < game->size; to_y++) {
                        if ((abs(from_x - to_x) == 2 || abs(from_y - to_y) == 2) &&
                            game->board[to_x][to_y] == EMPTY &&
                            (from_x == to_x || from_y == to_y)) {
                            int current_score = evaluate_move(game, from_x, from_y, to_x, to_y);
                            if (current_score > best_score) {
                                best_score = current_score;
                                best_from_x = from_x;
                                best_from_y = from_y;
                                best_to_x = to_x;
                                best_to_y = to_y;
                            }
                        }
                    }
                }
            }
        }
    }

    if (best_from_x != -1 && best_from_y != -1 && best_to_x != -1 && best_to_y != -1) {
        move_piece(game, best_from_x, best_from_y, best_to_x, best_to_y, &game->last_move_B);
        printf("Computer's move: (%d, %d) -> (%d, %d)\n", best_from_x, best_from_y, best_to_x, best_to_y);
        print_board(game);
    }
}

// Checks whether any move is possible
int has_possible_moves(GameState *game) {
    for (int i = 0; i < game->size; i++) {
        for (int j = 0; j < game->size; j++) {
            if (game->board[i][j] != EMPTY) {
                int directions[4][2] = {{2, 0}, {-2, 0}, {0, 2}, {0, -2}};
                for (int d = 0; d < 4; d++) {
                    int to_x = i + directions[d][0];
                    int to_y = j + directions[d][1];
                    if (to_x >= 0 && to_x < game->size && to_y >= 0 && to_y < game->size &&
                        game->board[to_x][to_y] == EMPTY) {
                        int mid_x = (i + to_x) / 2;
                        int mid_y = (j + to_y) / 2;
                        if (game->board[mid_x][mid_y] != EMPTY) {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

// Calculates the final score
void calculate_final_score(GameState *game) {
    game->player_score += game->score_player_A + game->score_player_B + game->score_player_C + game->score_player_D + game->score_player_E;
    game->computer_score += game->score_computer_A + game->score_computer_B + game->score_computer_C + game->score_computer_D + game->score_computer_E;
}

// Runs the game loop
void play_game(GameState *game, int mode) {
    int from_x, from_y, to_x, to_y;
    char command[10];

    while (1) {
        print_board(game);
        printf("Scores - Player piece A: %d, B: %d, C: %d, D: %d, E: %d\n", game->score_player_A, game->score_player_B, game->score_player_C, game->score_player_D, game->score_player_E);
        printf("Computer piece A: %d, B: %d, C: %d, D: %d, E: %d\n", game->score_computer_A, game->score_computer_B, game->score_computer_C, game->score_computer_D, game->score_computer_E);
        printf("Total score - Player: %d, Computer: %d\n", game->player_score, game->computer_score);
        printf("Type 'save' to save and exit. Type 'quit' to end the game.\n");

        if (!has_possible_moves(game)) {
            printf("Game over. Calculating the final score...\n");
            calculate_final_score(game);
            printf("Final scores - Player piece A: %d, B: %d, C: %d, D: %d, E: %d\n", game->score_player_A, game->score_player_B, game->score_player_C, game->score_player_D, game->score_player_E);
            printf("Computer piece A: %d, B: %d, C: %d, D: %d, E: %d\n", game->score_computer_A, game->score_computer_B, game->score_computer_C, game->score_computer_D, game->score_computer_E);
            printf("Total score - Player: %d, Computer: %d\n", game->player_score, game->computer_score);
            if (game->player_score > game->computer_score) {
                printf("Player wins!\n");
            } else if (game->computer_score > game->player_score) {
                printf("Computer wins!\n");
            } else {
                printf("The game is a draw!\n");
            }
            break;
        }

        if (game->player_turn == PLAYER_A) {
            printf("Player A's turn. Enter the coordinates of the piece to move (x y) or type 'undo': ");
            scanf("%s", command);
            if (strcmp(command, "undo") == 0) {
                undo_move(game, &game->last_move_A);
            } else if (strcmp(command, "redo") == 0) {
                redo_move(game, &game->last_move_A);
            } else if (strcmp(command, "save") == 0) {
                save_game(game);
                break;
            } else if (strcmp(command, "quit") == 0) {
                printf("Game over. Calculating the final score...\n");
                calculate_final_score(game);
                printf("Final scores - Player piece A: %d, B: %d, C: %d, D: %d, E: %d\n", game->score_player_A, game->score_player_B, game->score_player_C, game->score_player_D, game->score_player_E);
                printf("Computer piece A: %d, B: %d, C: %d, D: %d, E: %d\n", game->score_computer_A, game->score_computer_B, game->score_computer_C, game->score_computer_D, game->score_computer_E);
                printf("Total score - Player: %d, Computer: %d\n", game->player_score, game->computer_score);
                if (game->player_score > game->computer_score) {
                    printf("Player wins!\n");
                } else if (game->computer_score > game->player_score) {
                    printf("Computer wins!\n");
                } else {
                    printf("The game is a draw!\n");
                }
                break;
            } else {
                from_x = atoi(command);
                if (scanf("%d", &from_y) != 1) {
                    printf("Invalid input.\n");
                    continue;
                }
                printf("Enter the coordinates of the target cell (x y): ");
                if (scanf("%d %d", &to_x, &to_y) != 2) {
                    printf("Invalid input.\n");
                    continue;
                }

                if (move_piece(game, from_x, from_y, to_x, to_y, &game->last_move_A)) {
                    check_scores(game);
                    if (game->player_score >= 5) {
                        printf("Player wins!\n");
                        break;
                    }
                    game->player_turn = PLAYER_B;
                }
            }
        } else if (game->player_turn == PLAYER_B) {
            if (mode == 1) {
                computer_move(game);
                check_scores(game);
                if (game->computer_score >= 5) {
                    printf("Computer wins!\n");
                    break;
                }
                game->player_turn = PLAYER_A;
            } else {
                printf("Player B's turn. Enter the coordinates of the piece to move (x y) or type 'undo': ");
                scanf("%s", command);
                if (strcmp(command, "undo") == 0) {
                    undo_move(game, &game->last_move_B);
                } else if (strcmp(command, "redo") == 0) {
                    redo_move(game, &game->last_move_B);
                } else if (strcmp(command, "save") == 0) {
                    save_game(game);
                    break;
                } else if (strcmp(command, "quit") == 0) {
                    printf("Game over. Calculating the final score...\n");
                    calculate_final_score(game);
                    printf("Final scores - Player piece A: %d, B: %d, C: %d, D: %d, E: %d\n", game->score_player_A, game->score_player_B, game->score_player_C, game->score_player_D, game->score_player_E);
                    printf("Computer piece A: %d, B: %d, C: %d, D: %d, E: %d\n", game->score_computer_A, game->score_computer_B, game->score_computer_C, game->score_computer_D, game->score_computer_E);
                    printf("Total score - Player: %d, Computer: %d\n", game->player_score, game->computer_score);
                    if (game->player_score > game->computer_score) {
                        printf("Player wins!\n");
                    } else if (game->computer_score > game->player_score) {
                        printf("Computer wins!\n");
                    } else {
                        printf("The game is a draw!\n");
                    }
                    break;
                } else {
                    from_x = atoi(command);
                    if (scanf("%d", &from_y) != 1) {
                        printf("Invalid input.\n");
                        continue;
                    }
                    printf("Enter the coordinates of the target cell (x y): ");
                    if (scanf("%d %d", &to_x, &to_y) != 2) {
                        printf("Invalid input.\n");
                        continue;
                    }

                    if (move_piece(game, from_x, from_y, to_x, to_y, &game->last_move_B)) {
                        check_scores(game);
                        if (game->player_score >= 5) {
                            printf("Player wins!\n");
                            break;
                        }
                        game->player_turn = PLAYER_A;
                    }
                }
            }
        }
    }
}

// Shows the main menu
void display_menu() {
    printf("****************************\n");
    printf("*         SKIPITTY         *\n");
    printf("****************************\n");
    printf("1. New game\n");
    printf("2. Continue saved game\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

// Shows the game mode menu
void display_game_mode_menu() {
    printf("*********************************\n");
    printf("*       CHOOSE GAME MODE        *\n");
    printf("*********************************\n");
    printf("0. Two players\n");
    printf("1. Against the computer\n");
    printf("Enter your choice: ");
}

int main() {
    GameState game;
    game.size = 0;
    game.score_player_A = 0;
    game.score_player_B = 0;
    game.score_player_C = 0;
    game.score_player_D = 0;
    game.score_player_E = 0;
    game.score_computer_A = 0;
    game.score_computer_B = 0;
    game.score_computer_C = 0;
    game.score_computer_D = 0;
    game.score_computer_E = 0;
    game.player_score = 0;
    game.computer_score = 0;
    game.player_turn = PLAYER_A;
    game.last_move_A = (Move){-1, -1, -1, -1, EMPTY, -1, -1, EMPTY};
    game.last_move_B = (Move){-1, -1, -1, -1, EMPTY, -1, -1, EMPTY};

    srand(time(NULL));

    int choice;
    while (1) {
        display_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }

        if (choice == 1) {
            int mode;
            display_game_mode_menu();
            while (scanf("%d", &mode) != 1 || (mode != 0 && mode != 1)) {
                printf("Invalid choice. Please try again: ");
                while (getchar() != '\n');
            }

            printf("Enter the board size (at most 20): ");
            while (scanf("%d", &game.size) != 1 || game.size <= 0 || game.size > 20) {
                printf("Invalid size. Please enter a value between 1 and 20: ");
                while (getchar() != '\n');
            }

            game.board = (char**)malloc(game.size * sizeof(char*));
            for (int i = 0; i < game.size; i++) {
                game.board[i] = (char*)malloc(game.size * sizeof(char));
            }
            initialize_board(&game);

            play_game(&game, mode);
            break;

        } else if (choice == 2) {
            if (!load_game(&game)) {
                printf("No saved game found.\n");
                continue;
            }

            int mode;
            display_game_mode_menu();
            while (scanf("%d", &mode) != 1 || (mode != 0 && mode != 1)) {
                printf("Invalid choice. Please try again: ");
                while (getchar() != '\n');
            }

            play_game(&game, mode);
            break;

        } else if (choice == 3) {
            printf("Exiting...\n");
            break;

        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    if (game.size > 0) {
        for (int i = 0; i < game.size; i++) {
            free(game.board[i]);
        }
        free(game.board);
    }

    return 0;
}
