#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>

#define DIM 150
#define EMPTY -128
#define WALK_DIST 1.1

signed char ants[DIM][DIM];

void fillEmpty(signed char array[DIM][DIM]) {
    for (int i = 0; i < DIM; i++)
        for (int j = 0; j < DIM; j++)
            array[i][j] = EMPTY;
}

int frame = 0;
void update() {
    frame++;

    signed char temp[DIM][DIM];
    fillEmpty(temp);

    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            if (ants[i][j] > EMPTY) {
                signed char direction = ants[i][j];

                // Calculate new location
                double angle = M_PI * direction / 127;
                int x = (int) round(WALK_DIST * cos(angle)), y = (int) round(WALK_DIST * sin(angle));

                // Calculate new direction
                signed int overflowDirection = direction + (rand() % 21 - 10);
                if (overflowDirection > 127)
                    overflowDirection -= 254;
                else if (overflowDirection < -127)
                    overflowDirection += 254;
                signed char newDirection = overflowDirection;

                // Bounds checks
                int newRow = i - y, newCol = j + x;
                if (newRow < 0) {
                    // Top
                    newRow = 0;
                    newDirection = -newDirection;
                }
                else if (newRow >= DIM) {
                    // Bottom
                    newRow = DIM - 1;
                    newDirection = -newDirection;
                }
                if (newCol < 0) {
                    // Left
                    newCol = 0;
                    newDirection = 127 - newDirection;
                }
                else if (newCol >= DIM) {
                    // Right
                    newCol = DIM - 1;
                    newDirection = -127 - newDirection;
                }

                if (temp[newRow][newCol] == EMPTY)
                    temp[newRow][newCol] = newDirection;
                else
                    temp[i][j] = direction;
            }
        }
    }

    for (int i = 0; i < DIM; i++)
        for (int j = 0; j < DIM; j++)
            ants[i][j] = temp[i][j];
}

void printScene() {
    printf("\e[1;1H\e[2J");

    for (int i = 0; i < DIM; i++) {
        for (int j = 0; j < DIM; j++) {
            signed char direction = ants[i][j];
            if (direction == -128)
                printf("　");
            else {
                if (direction < -111 || direction > 111)
                    printf("←");
                else if (direction < -79)
                    printf("↙");
                else if (direction < -47)
                    printf("↓");
                else if (direction < -15)
                    printf("↘");
                else if (direction < 16)
                    printf("→");
                else if (direction < 48)
                    printf("↗");
                else if (direction < 80)
                    printf("↑");
                else if (direction < 112)
                    printf("↖");
            }
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));

    fillEmpty(ants);

    for (int i = DIM * 1 / 4; i <= DIM * 3 / 4; i++)
        ants[DIM / 2][i] = rand() % 254 - 127;

    while (1) {
        update();
        printScene();
        usleep(50000);
    }

    return EXIT_SUCCESS;
}
