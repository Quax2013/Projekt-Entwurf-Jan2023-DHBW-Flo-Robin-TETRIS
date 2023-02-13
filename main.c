#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

// Alle verschiedene Blöcke:
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
void drawField(int score, char *highscoreName, int highscore);
int spawnBlock();
void rowFull(int *score);
void moveBlockDown(int *block, int *rot, int *score, int forced);
void border();
void moveBlockRight();
void moveBlockLeft();
int spinBlock(int block, int rot, int offset, int depth);
void gameOverTest(int *ptrGameOver);
void control(int *block, int *rot, int *score, char *highscoreName, int highscore);

// Hauptfunktion in der das Spiel einmal durchgeführt wird.
int main()
{

    srand(time(0));
    int block = 0;
    int rotation = 0;
    int score = 0;
    char highscoreName[11] = "";
    int highscore = 0;

    // Auslesen der Highscore-Daten
    FILE *fp = fopen("./scoreFile.txt", "r");
    if (fp == NULL)
    {
        strcpy(highscoreName, "NONE");
        highscore = 0;
    }
    else
    {
        fscanf(fp, "%s\n%d", highscoreName, &highscore);
    }
    fclose(fp);

    // Ausgeben der Anleitung und warten auf Bereitschaft der Anwenders
    system("cls");
    printf("Herzlich willkommen zu Tetris.\nUm den Block zu steuern nutzen Sie [W,A,S,D] und [F] um das Spiel abzurechen.\nUm zu Starten druecken Sie eine beliebige Taste.");
    getch();
    fflush(stdin);

    system("cls");
    border();
    block = spawnBlock();
    control(&block, &rotation, &score, highscoreName, highscore);

    // Einlesen und Speichern der neuen Highscore-Daten
    if (highscore < score)
    {
        printf("Please enter your name(10 characters max):\n");
        for (int i = 0; i < 10; i++)
        {
            highscoreName[i] = getchar();
        }
        highscoreName[10] = '\0';

        FILE *fp = fopen("./scoreFile.txt", "w");
        if (fp == NULL)
        {
            fprintf(stderr, "Unable to save highscore.");
        }
        else
        {
            fprintf(fp, "%s\n%d", highscoreName, score);
        }
        fclose(fp);
    }

    gotoxy(0, 30);
    printf("Press any key to close.");
    getche();
    return 0;
}

// Quelle: https://www.quora.com/What-is-the-gotoxy-function-used-in-C Zugegriffen am 03.02.2023
// Springt an einen bestimmten Punkt im Terminal
void gotoxy(int x, int y)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = {x, y};
    SetConsoleCursorPosition(h, position);
}

// Fügt jeweils 2 Spalten am Rand und eine Zeile unten im Array mit festen Blöcken
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

// Funktion wird momentan nicht benutzt
// Gibt ein Feld Template auf dem Bildschirm aus in dem das Spiel stattfindet
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

// Gibt das Spielfeld-Array in formatierter Form aus
void drawField(int score, char *highscoreName, int highscore)
{
    system("cls");
    for (int i = 3; i < HEIGHT; i++)
    {
        for (int k = 1; k < WIDTH - 1; k++)
        {
            switch (field[i][k])
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
            }
            // printf("%d", field[i][k]); // DEBUG
        }
        printf("\n");
    }

    printf("\nScore: %d\nHighscore:%s | %d", score, highscoreName, highscore);
}

// Spawnt einen zufällig generierten Block am oberen Spielfeldrand
int spawnBlock()
{
    int block = rand() % 7;
    int *blockPaste;

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
            blockPaste = leftL0; // Pointer geben Warnungen aus. Absicht: Ausnutzung der Speicherungsart von 2D-Arrays
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

// Checkt ob eine Reihe im Array voll ist, löscht diese und vergibt Punkte
void rowFull(int *score)
{
    int intRowFull = 0;
    int fullRows = 0;

    for (int i = 0; i < HEIGHT - 1; i++)
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
            fullRows++;
            for (int k = i; k > 0; k--)
            {
                for (int j = 2; j < WIDTH - 2; j++)
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
        intRowFull = 0;
    }
    if (fullRows)
    {
        *score += 150 + 100 * (fullRows * fullRows / 2.0 - 1); // Extra Punkte für Multi-Reihen
    }
}

// Checkt ob der aktive Block nach unten Platz hat und bewegt ihn nach unten oder konvertiert ihn zu einem festen Block
void moveBlockDown(int *block, int *rot, int *score, int forced)
{
    int move = 1;

    for (int i = HEIGHT - 1; i >= 0; i--) // Check nach Platz
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
    if (move) // Bewegung nach unten
    {
        for (int i = HEIGHT - 1; i >= 0; i--)
        {
            for (int k = WIDTH - 1; k >= 0; k--)
            {
                if (field[i][k] == 1)
                {
                    field[i][k] = 0;
                    field[i + 1][k] = 1;
                    /*gotoxy(k, i + 1);
                    printf(" ");
                    gotoxy(k, i + 2);
                    printf("+");

                    gotoxy(0, 22);*/
                }
            }
        }
    }
    else // Konvertierung vom aktiven Block zu einem festen
    {
        for (int i = HEIGHT - 1; i >= 0; i--)
        {
            for (int k = WIDTH - 1; k >= 0; k--)
            {
                if (field[i][k] == 1)
                {
                    field[i][k] = 2;

                    /*gotoxy(k, i + 1);
                    printf(" ");

                    gotoxy(k, i + 2);
                    printf("%c", 254);

                    gotoxy(0, 22);*/
                }
            }
        }
        *block = spawnBlock();
        *rot = 0;
    }
    if (forced)
    {
        *score += 2;
    }
}

// Bewegt den aktiven Block nach rechts falls Platz ist
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

                    /*gotoxy(k, i + 1);
                    printf(" ");

                    gotoxy(k + 1, i + 1);
                    printf("+");

                    gotoxy(0, 22);*/
                }
            }
        }
    }
    else
    {
        move = 0;
    }
}

// Bewegt den Block nach links falls Platz frei ist
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

                    /*gotoxy(k, i + 1);
                    printf(" ");

                    gotoxy(k - 1, i + 1);
                    printf("+");

                    gotoxy(0, 22);*/
                }
            }
        }
    }
    else
    {
        move = 0;
    }
}

// Dreht den Block falls Platz ist, wenn nicht wird der Block in einer verschobenen Position gedreht
int spinBlock(int block, int rot, int offset, int depth)
{
    int testField[HEIGHT][WIDTH] = {};
    int safeX = 0;
    int safeY = 0;
    int newRot = 0;

    // Kopieren vom Spielfeld in eine Test-Array
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

    // Hardgecodete Rotationen
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

    // Test auf Überscheidungen nach dem Dreher des Block in der Test-Kopie
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
                    return spinBlock(block, rot, offset + 2, depth + 1); // Rekursion mit Offset zum Testen auf freie Positionen
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

    // Kopieren vom Test-Array in Spielfeld-Array bei erfolgreier Drehung
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int k = 0; k < WIDTH; k++)
        {
            field[i][k] = testField[i][k];
        }
    }
    return newRot;
}

// Test ob ein Block in den Spawnbereich (4 obere Reihen) ragt und beendet das Spiel
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

//Haupt-Loop des Spiels mit Abfrage der Spieler-Einbagen
void control(int *block, int *rot, int *score, char *highscoreName, int highscore)
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
                drawField(*score, highscoreName, highscore);
            }
            else if (temp == 'a')
            {
                moveBlockLeft();
                drawField(*score, highscoreName, highscore);
            }
            else if (temp == 's')
            {
                moveBlockDown(block, rot, score, 1);
                rowFull(score);
                drawField(*score, highscoreName, highscore);
                i = 0;
            }
            else if (temp == 'd')
            {
                moveBlockRight();
                drawField(*score, highscoreName, highscore);
            }
            else if (temp == 'f')
            {
                gameOver = 1;
                i = 4;
            }
        }

        moveBlockDown(block, rot, score, 0);
        rowFull(score);
        gameOverTest(&gameOver);
        drawField(*score, highscoreName, highscore);

        if (gameOver)
        {
            printf("\nGAMEOVER\n");
        }
        /*Sleep(75);            Wozu ist das? @Flo
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
