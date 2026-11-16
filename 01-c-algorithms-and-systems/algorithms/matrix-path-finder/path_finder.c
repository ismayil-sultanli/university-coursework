#include <stdio.h>

#define MAX 100

int main() {
    int n, m;
    int matrix[MAX][MAX];
    int visited[MAX][MAX] = {0};
    int dx[] = {0, 0, 1, -1, 1, 1, -1, -1};
    int dy[] = {1, -1, 0, 0, 1, -1, 1, -1};

    printf("Enter matrix dimensions (n m): ");
    scanf("%d %d", &n, &m);

    printf("Enter the matrix row by row:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }

    printf("\nResult:\n");

    for (int i = 0; i < n; i++) {
        if (matrix[i][0] != 0) {
            for (int x = 0; x < n; x++) {
                for (int y = 0; y < m; y++) {
                    visited[x][y] = 0;
                }
            }

            int length = 0, stackX[MAX * MAX], stackY[MAX * MAX], top = 0;
            int pathValue = matrix[i][0];
            stackX[top] = i;
            stackY[top++] = 0;
            visited[i][0] = 1;

            while (top > 0) {
                int x = stackX[--top];
                int y = stackY[top];
                length++;

                for (int d = 0; d < 8; d++) {
                    int newX = x + dx[d];
                    int newY = y + dy[d];

                    if (newX >= 0 && newX < n && newY >= 0 && newY < m &&
                        matrix[newX][newY] == pathValue && !visited[newX][newY]) {
                        stackX[top] = newX;
                        stackY[top++] = newY;
                        visited[newX][newY] = 1;
                    }
                }
            }

            int reachesLastColumn = 0;
            for (int k = 0; k < n; k++) {
                if (matrix[k][m - 1] == pathValue && visited[k][m - 1]) {
                    reachesLastColumn = 1;
                    break;
                }
            }

            if (reachesLastColumn) {
                printf("Path %d -> Length %d\n", pathValue, length);
            }
        }
    }

    return 0;
}
