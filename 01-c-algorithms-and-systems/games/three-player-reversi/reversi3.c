#include <stdio.h>
#define MAX_SIZE 23

int main() {
    int size, x, y, currentPlayer = 0;
    char players[] = {'R', 'Y', 'B'};
    char board[MAX_SIZE][MAX_SIZE];
    int i, j;
    int gameOver = 0;

    printf("*****************************************\n");
    printf("*         THREE-PLAYER REVERSI          *\n");
    printf("*****************************************\n");
    printf("*                                       *\n");
    printf("*    1. Start game                      *\n");
    printf("*    2. Quit                            *\n");
    printf("*                                       *\n");
    printf("*****************************************\n");
    printf("Choose an option (1 or 2): ");
    scanf("%d", &j);

    if (j == 2) {
        printf("Quitting the game. Goodbye!\n");
        return 0;
    }

    printf("Enter the board size (between 4 and %d): ", MAX_SIZE);
    scanf("%d", &size);

    if (size < 4 || size > MAX_SIZE) {
        printf("Invalid size. Exiting.\n");
        return 0;
    }

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            board[i][j] = ' ';
        }
    }

    while (!gameOver) {
        int emptyCells = 0;

        printf("\n     ");
        for (i = 0; i < size; i++) {
            printf("%2d  ", i);
        }
        printf("\n");
        for (i = 0; i < size; i++) {
            printf("%2d  ", i);
            for (j = 0; j < size; j++) {
                printf(" %c ", board[i][j]);
                if (j < size - 1) printf("|");
            }
            printf("\n");
            if (i < size - 1) {
                printf("     ");
                for (j = 0; j < size; j++) {
                    printf("---");
                    if (j < size - 1) printf("+");
                }
                printf("\n");
            }
        }

        printf("Player %c's turn. Enter row and column: ", players[currentPlayer]);
        scanf("%d %d", &x, &y);

        if (x >= 0 && x < size && y >= 0 && y < size && board[x][y] == ' ') {
            board[x][y] = players[currentPlayer];

            int directions[8][2] = {{-1, -1}, {-1, 0}, {-1, 1},
                                    {0, -1},           {0, 1},
                                    {1, -1},  {1, 0},  {1, 1}};
            for (i = 0; i < 8; i++) {
                int dx = directions[i][0];
                int dy = directions[i][1];
                int nx = x + dx, ny = y + dy, pathLength = 0, opponentFound = 0;

                while (nx >= 0 && nx < size && ny >= 0 && ny < size && board[nx][ny] != ' ' && board[nx][ny] != players[currentPlayer]) {
                    pathLength++;
                    nx += dx;
                    ny += dy;
                    opponentFound = 1;
                }
                if (opponentFound && nx >= 0 && nx < size && ny >= 0 && ny < size && board[nx][ny] == players[currentPlayer]) {
                    int tx = x + dx, ty = y + dy;
                    while (tx != nx || ty != ny) {
                        board[tx][ty] = players[currentPlayer];
                        tx += dx;
                        ty += dy;
                    }
                }
            }
            currentPlayer = (currentPlayer + 1) % 3;
        } else {
            printf("Invalid move! The turn passes to the next player.\n");
            currentPlayer = (currentPlayer + 1) % 3;
        }

        for (i = 0; i < size; i++) {
            for (j = 0; j < size; j++) {
                if (board[i][j] == ' ') {
                    emptyCells++;
                }
            }
        }

        if (emptyCells == 0) {
            gameOver = 1;
        }
    }

    int redCount = 0, yellowCount = 0, blueCount = 0;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (board[i][j] == 'R') redCount++;
            if (board[i][j] == 'Y') yellowCount++;
            if (board[i][j] == 'B') blueCount++;
        }
    }

    printf("Results:\n");
    printf("Red (R): %d\n", redCount);
    printf("Yellow (Y): %d\n", yellowCount);
    printf("Blue (B): %d\n", blueCount);

    if (redCount > yellowCount && redCount > blueCount) {
        printf("Winner: Red!\n");
    } else if (yellowCount > redCount && yellowCount > blueCount) {
        printf("Winner: Yellow!\n");
    } else if (blueCount > redCount && blueCount > yellowCount) {
        printf("Winner: Blue!\n");
    } else {
        printf("It's a draw!\n");
    }

    return 0;
}
