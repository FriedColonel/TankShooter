#pragma once
#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

static struct termios old, newa;

void gotoxy(int x, int y);
void initTermios(int echo);
void resetTermios(void);

bool kbhit();

char getch_(int echo);

char getch(void);

char getche(void);

int getmax_x();

int getmax_y();
