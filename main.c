#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<time.h>
#include<stdlib.h>


void gotoxy(int x, int y);


int main()
{
    system("cls");
    gotoxy(1,1);    
    int a = 8;
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");


    return 0;
}


// Quelle: https://www.quora.com/What-is-the-gotoxy-function-used-in-C Zugegriffen am 03.02.2023
void gotoxy(int x, int y)
{
    HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position={x,y};
    SetConsoleCursorPosition(h,position);
}
