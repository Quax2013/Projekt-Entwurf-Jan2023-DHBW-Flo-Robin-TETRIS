#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>

// Verschiedene Blöcke:
int square[2][2] = {1, 1,
                    1, 1};

// Linkes L
int leftL0[2][3] = {1, 1, 1,
                    0, 0, 1};

int leftL1[3][2] = {0, 1,
                    0, 1,
                    1, 1};

int leftL2[2][3] = {1, 0, 0,
                    1, 1, 1};

int leftL3[3][2] = {1, 1,
                    1, 0,
                    1, 0};

// Rechtes L
int rightL0[2][3] = {1, 1, 1,
                     1, 0, 0};

int rightL1[3][2] = {1, 1,
                     0, 1,
                     0, 1};

int rightL2[2][3] = {0, 0, 1,
                     1, 1, 1};

int rightL3[3][2] = {1, 0,
                     1, 0,
                     1, 1};

// Linkes S
int leftS0[2][3] = {0, 1, 1,
                    1, 1, 0};

int leftS1[3][2] = {1, 0,
                    1, 1,
                    0, 1};

// Rechtes S
int rightS0[2][3] = {1, 1, 0,
                     0, 1, 1};

int rightS1[3][2] = {0, 1,
                     1, 1,
                     1, 0};

// T-Block
int tBlock0[2][3] = {0, 1, 0,
                     1, 1, 1};

int tBlock1[3][2] = {1, 0,
                     1, 1,
                     1, 0};

int tBlock2[2][3] = {1, 1, 1,
                     0, 1, 0};

int tBlock3[3][2] = {0, 1,
                     1, 1,
                     0, 1};

// Line
int lineBlock0[1][4] = {1, 1, 1, 1};

int lineBlock1[4][1] = {1,
                        1,
                        1,
                        1};

#define HEIGHT 18 + 4 + 1
#define WIDTH 2 + 10 + 2

int field[HEIGHT][WIDTH] = {};

void gotoxy(int x, int y);
void createField();
void drawField();
int spawnBlock();
void rowFull(int *score);
void moveBlockDown(int *block, int *rot);
void border();
void moveBlockRight();
void moveBlockLeft();
int spinBlock(int block, int rot, int offset, int depth);
void gameOverTest(int *ptrGameOver);
void control(int *block, int *rot, int *score);

int main()
{

    srand(time(0));
    int block = 0;
    int rotation = 0;
    int score = 0;

    system("cls");
    border();
    // createField();
    block = spawnBlock();
    control(&block, &rotation, &score);

    gotoxy(0, 30);
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int k = 0; k < WIDTH; k++)
        {
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

void border()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        field[i][0] = 2;
        field[i][1] = 2;
        field[i][WIDTH - 1] = 2;
        field[i][WIDTH - 2] = 2;
    }
    for (int k = 0; k < WIDTH; k++)
    {
        field[HEIGHT - 1][k] = 2;
    }
}

void createField()
{
    system("cls");
    gotoxy(0, 0);
    printf("   Tetris   \n");
    for (int i = 0; i < HEIGHT; i++)
    {
        printf("|");
        for (int j = 0; j < WIDTH - 2; j++)
        {
            printf(" ");
        }
        printf("|\n");
    }
    for (int i = 0; i < WIDTH; i++)
    {
        printf("-");
    }
    printf("\n");
}

void drawField()
{
    system("cls");
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int k = 0; k < WIDTH; k++)
        {
            /*switch (field[i][k])
            {
            case 0:
                printf(" ");
                break;
            case 1:
                printf("+");
                break;
            case 2:
                printf("%c", 254);
                break;
            }*/
            printf("%d", field[i][k]);
        }
        printf("\n");
    }
}

int spawnBlock()
{
    int block = rand() % 7;
    int *blockPaste;

    printf("%d\n\n", block);
    if (block == 0)
    {
        for (int i = 0; i < 2; i++)
        {
            for (int k = 0; k < 2; k++)
            {
                field[2 + i][4 + k] = square[i][k];
            }
        }
    }
    else if (block == 6)
    {
        for (int k = 0; k < 4; k++)
        {
            field[3][3 + k] = lineBlock0[0][k];
        }
    }
    else
    {
        switch (block)
        {
        case 1:
            blockPaste = leftL0;
            break;
        case 2:
            blockPaste = rightL0;
            break;
        case 3:
            blockPaste = leftS0;
            break;
        case 4:
            blockPaste = rightS0;
            break;
        case 5:
            blockPaste = tBlock0;
            break;
        }

        for (int i = 0; i < 2; i++)
        {
            for (int k = 0; k < 3; k++)
            {
                field[2 + i][4 + k] = *blockPaste;
                blockPaste++;
            }
        }
    }
    return block;
}

void rowFull(int *score)
{

    int intRowFull = 0;
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (field[i][j] == 2)
            {
                intRowFull++;
            }
        }
        if (intRowFull == 14)
        {
            *score += 100;
            for (int k = i; k > 0; k--)
            {
                for (int j = 0; j < WIDTH; j++)
                {
                    field[k][j] = field[k - 1][j];

                    /*gotoxy(j, k + 1);
                    if (field[k][j] == 2)
                    {
                        printf("%c", 254);
                    }
                    else
                    {
                        printf(" ");
                    }
                    gotoxy(0, 22);*/
                }
            }
        }
    }
}

void moveBlockDown(int *block, int *rot)
{
    int move = 1;

    for (int i = HEIGHT - 1; i >= 0; i--)
    {
        for (int k = WIDTH - 1; k >= 0; k--)
        {
            if (field[i][k] == 1)
            {
                if (field[i + 1][k] == 2)
                {
                    move = 0;
                }
            }
        }
    }
    if (move)
    {
        for (int i = HEIGHT - 1; i >= 0; i--)
        {
            for (int k = WIDTH - 1; k >= 0; k--)
            {
                if (field[i][k] == 1)
                {
                    field[i][k] = 0;
                    field[i + 1][k] = 1;
                    gotoxy(k, i + 1);
                    printf(" ");
                    gotoxy(k, i + 2);
                    printf("+");

                    gotoxy(0, 22);
                }
            }
        }
    }
    else
    {
        for (int i = HEIGHT - 1; i >= 0; i--)
        {
            for (int k = WIDTH - 1; k >= 0; k--)
            {
                if (field[i][k] == 1)
                {
                    field[i][k] = 2;

                    gotoxy(k, i + 1);
                    printf(" ");

                    gotoxy(k, i + 2);
                    printf("%c", 254);

                    gotoxy(0, 22);
                }
            }
        }
        *block = spawnBlock();
        *rot = 0;
    }
}

void moveBlockRight()
{
    int move = 1;
    for (int i = HEIGHT - 1; i >= 0; i--)
    {
        for (int k = WIDTH - 1; k >= 0; k--)
        {
            if (field[i][k] == 1)
            {
                if (field[i][k + 1] == 2)
                {
                    move = 0;
                }
            }
        }
    }
    if (move)
    {
        for (int i = HEIGHT - 1; i >= 0; i--)
        {
            for (int k = WIDTH - 1; k >= 0; k--)
            {
                if (field[i][k] == 1)
                {
                    field[i][k] = 0;
                    field[i][k + 1] = 1;

                    gotoxy(k, i + 1);
                    printf(" ");

                    gotoxy(k + 1, i + 1);
                    printf("+");

                    gotoxy(0, 22);
                }
            }
        }
    }
    else
    {
        move = 0;
    }
}

void moveBlockLeft()
{
    int move = 1;
    for (int i = HEIGHT - 1; i >= 0; i--)
    {
        for (int k = 0; k < WIDTH; k++)
        {
            if (field[i][k] == 1)
            {
                if (field[i][k - 1] == 2)
                {
                    move = 0;
                }
            }
        }
    }
    if (move)
    {
        for (int i = HEIGHT - 1; i >= 0; i--)
        {
            for (int k = 0; k < WIDTH; k++)
            {
                if (field[i][k] == 1)
                {
                    field[i][k] = 0;
                    field[i][k - 1] = 1;

                    gotoxy(k, i + 1);
                    printf(" ");

                    gotoxy(k - 1, i + 1);
                    printf("+");

                    gotoxy(0, 22);
                }
            }
        }
    }
    else
    {
        move = 0;
    }
}

int spinBlock(int block, int rot, int offset, int depth)
{
    int testField[HEIGHT][WIDTH] = {};
    int safeX = 0;
    int safeY = 0;
    int newRot = 0;

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int k = 0; k < WIDTH; k++)
        {
            if (field[i][k] == 1)
            {
                safeX = k;
                safeY = i;
                testField[i][k] = 0;
            }
            else
            {
                testField[i][k] = field[i][k];
            }
        }
    }

    switch (block)
    {
    case 0: // Quadrat
        safeX -= 1;
        safeY -= 1;
        for (int i = 0; i < 2; i++)
        {
            for (int k = 0; k < 2; k++)
            {
                testField[safeY + i][safeX + k] = 1;
            }
        }
        break;
    case 1: // Linkes L
        switch (rot)
        {
        case 0:
            safeX -= 2;
            safeY -= 2;
            for (int i = 0; i < 3; i++)
            {
                for (int k = 0; k < 2; k++)
                {
                    testField[safeY + i][safeX + k + offset] = testField[safeY + i][safeX + k + offset] + leftL1[i][k];
                }
            }
            newRot = 1;
            break;
        case 1:
            safeX -= 1;
            safeY -= 1;
            for (int i = 0; i < 2; i++)
            {
                for (int k = 0; k < 3; k++)
                {
                    testField[safeY + i][safeX + k + offset] = testField[safeY + i][safeX + k + offset] + leftL2[i][k];
                }
            }
            newRot = 2;
            break;
        case 2:
            safeX -= 1;
            safeY -= 2;
            for (int i = 0; i < 3; i++)
            {
                for (int k = 0; k < 2; k++)
                {
                    testField[safeY + i][safeX + k + offset] = testField[safeY + i][safeX + k + offset] + leftL3[i][k];
                }
            }
            newRot = 3;
            break;
        case 3:
            safeX -= 1;
            safeY -= 1;
            for (int i = 0; i < 2; i++)
            {
                for (int k = 0; k < 3; k++)
                {
                    testField[safeY + i][safeX + k + offset] = testField[safeY + i][safeX + k + offset] + leftL0[i][k];
                }
            }
            newRot = 0;
            break;
        }
        break;
    case 2: // Rechtes L
        switch (rot)
        {
        case 0:
            safeY -= 2;
            for (int i = 0; i < 3; i++)
            {
                for (int k = 0; k < 2; k++)
                {
                    testField[safeY + i][safeX + k + offset] = testField[safeY + i][safeX + k + offset] + rightL1[i][k];
                }
            }
            newRot = 1;
            break;
        case 1:
            safeX -= 1;
            safeY -= 1;
            for (int i = 0; i < 2; i++)
            {
                for (int k = 0; k < 3; k++)
                {
                    testField[safeY + i][safeX + k + offset] = testField[safeY + i][safeX + k + offset] + rightL2[i][k];
                }
            }
            newRot = 2;
            break;
        case 2:
            safeX -= 1;
            safeY -= 2;
            for (int i = 0; i < 3; i++)
            {
                for (int k = 0; k < 2; k++)
                {
                    testField[safeY + i][safeX + k + offset] = testField[safeY + i][safeX + k + offset] + rightL3[i][k];
                }
            }
            newRot = 3;
            break;
        case 3:
            safeX -= 2;
            safeY -= 1;
            for (int i = 0; i < 2; i++)
            {
                for (int k = 0; k < 3; k++)
                {
                    testField[safeY + i][safeX + k + offset] = testField[safeY + i][safeX + k + offset] + rightL0[i][k];
                }
            }
            newRot = 0;
            break;
        }
        break;
    case 3: // Linkes S
        switch (rot)
        {
        case 0:
            safeY -= 2;
            for (int i = 0; i < 3; i++)
            {
                for (int k = 0; k < 2; k++)
                {
                    testField[safeY + i][safeX + k + offset] = testField[safeY + i][safeX + k + offset] + leftS1[i][k];
                }
            }
            newRot = 1;
            break;
        case 1:
            safeX -= 2;
            safeY -= 1;
            for (int i = 0; i < 2; i++)
            {
                for (int k = 0; k < 3; k++)
                {
                    testField[safeY + i][safeX + k + offset] = testField[safeY + i][safeX + k + offset] + leftS0[i][k];
                }
            }
            newRot = 0;
            break;
        }
        break;
    case 4: // Rechtes S
        switch (rot)
        {
        case 0:
            safeX -= 1;
            safeY -= 2;
            for (int i = 0; i < 3; i++)
            {
                for (int k = 0; k < 2; k++)
                {
                    testField[safeY + i][safeX + k + offset] = testField[safeY + i][safeX + k + offset] + rightS1[i][k];
                }
            }
            newRot = 1;
            break;
        case 1:
            safeX -= 1;
            safeY -= 1;
            for (int i = 0; i < 2; i++)
            {
                for (int k = 0; k < 3; k++)
                {
                    testField[safeY + i][safeX + k + offset] = testField[safeY + i][safeX + k + offset] + rightS0[i][k];
                }
            }
            newRot = 0;
            break;
        }
        break;
    case 5: // T-Block
        switch (rot)
        {
        case 0:
            safeX -= 1;
            safeY -= 2;
            for (int i = 0; i < 3; i++)
            {
                for (int k = 0; k < 2; k++)
                {
                    testField[safeY + i][safeX + k + offset] = testField[safeY + i][safeX + k + offset] + tBlock1[i][k];
                }
            }
            newRot = 1;
            break;
        case 1:
            safeX -= 1;
            safeY -= 1;
            for (int i = 0; i < 2; i++)
            {
                for (int k = 0; k < 3; k++)
                {
                    testField[safeY + i][safeX + k + offset] = testField[safeY + i][safeX + k + offset] + tBlock2[i][k];
                }
            }
            newRot = 2;
            break;
        case 2:
            safeX -= 1;
            safeY -= 2;
            for (int i = 0; i < 3; i++)
            {
                for (int k = 0; k < 2; k++)
                {
                    testField[safeY + i][safeX + k + offset] = testField[safeY + i][safeX + k + offset] + tBlock3[i][k];
                }
            }
            newRot = 3;
            break;
        case 3:
            safeX -= 1;
            safeY -= 1;
            for (int i = 0; i < 2; i++)
            {
                for (int k = 0; k < 3; k++)
                {
                    testField[safeY + i][safeX + k + offset] = testField[safeY + i][safeX + k + offset] + tBlock0[i][k];
                }
            }
            newRot = 0;
            break;
        }
        break;
    case 6: // Linien-Block
        switch (rot)
        {
        case 0:
            safeX -= 1;
            safeY -= 3;
            for (int i = 0; i < 4; i++)
            {
                testField[safeY + i][safeX + offset] = testField[safeY + i][safeX + offset] + 1;
            }
            newRot = 1;
            break;
        case 1:
            safeX -= 2;
            for (int i = 0; i < 4; i++)
            {
                testField[safeY][safeX + i + offset] = testField[safeY][safeX + i + offset] + 1;
            }
            newRot = 0;
            break;
        }
        break;
    }
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int k = 0; k < WIDTH; k++)
        {
            if (testField[i][k] == 3)
            {
                if (depth > 3)
                {
                    return rot;
                }
                if (block == 6 && rot == 1 && k == safeX && testField[i][k + 1] == 3)
                {
                    return spinBlock(block, rot, offset + 2, depth + 1);
                }
                else if (k > safeX)
                {
                    return spinBlock(block, rot, offset - 1, depth + 1);
                }
                else
                {
                    return spinBlock(block, rot, offset + 1, depth + 1);
                }
            }
        }
    }
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int k = 0; k < WIDTH; k++)
        {
            field[i][k] = testField[i][k];
        }
    }
    return newRot;
}

void gameOverTest(int *ptrGameOver)
{
    for (int i = 0; i < 4; i++)
    {
        for (
            int k = 2; k < WIDTH - 2; k++)
        {
            if (field[i][k] == 2)
            {
                *ptrGameOver = 1;
            }
        }
    }
}

void control(int *block, int *rot, int *score)
{
    char temp = 'k';
    int gameOver = 0;

    while (gameOver != 1)
    {

        for (int i = 0; i < 4; i++)
        {
            Sleep(250);
            temp = 'k';
            fflush(stdin);
            if (kbhit())
            {
                temp = getch();
                fflush(stdin);
            }
            if (temp == 'w')
            {
                *rot = spinBlock(*block, *rot, 0, 0);
                drawField();
            }
            else if (temp == 'a')
            {
                moveBlockLeft();
                drawField();
            }
            else if (temp == 's')
            {
                moveBlockDown(block, rot);
                drawField();
                i = 0;
            }
            else if (temp == 'd')
            {
                moveBlockRight();
                drawField();
            }
            else if (temp == 'f')
            {
                gameOver = 1;
                i = 4;
            }
        }
        moveBlockDown(block, rot);
        rowFull(score);
        gameOverTest(&gameOver);
        drawField();

        if (gameOver)
        {
            printf("\nGAMEOVER\n");
        }
        /*Sleep(75);
        fflush(stdin);
        if (kbhit())
        {
            temp = getch();
            fflush(stdin);
        }
        drawField();
        if (temp == 'f')
        {
            gameOver = 1;
            printf("\nGAMEOVER\n");
        }*/
    }
}
