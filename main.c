#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

#define EMPTY 254
#define MOVING 1
#define SOLID 2


#define HEIGHT 18
#define WIDTH 10

int field[HEIGHT][WIDTH] = {};


void gotoxy(int x, int y);

int main()
{
    char stop = "";

        int i = 0;
        system("cls");
        //gotoxy(20 + i, 1);
        int a = 8;
        for (int i = 0; i < HEIGHT; i++){
            for (int k = 0; k< WIDTH; k++){
                printf("%d", field[i][k]);
            }
            printf("\n");
        }


    getche();
    return 0;
}

// Quelle: https://www.quora.com/What-is-the-gotoxy-function-used-in-C Zugegriffen am 03.02.2023
void gotoxy(int x, int y)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {x, y};
    SetConsoleCursorPosition(h, position);
}
