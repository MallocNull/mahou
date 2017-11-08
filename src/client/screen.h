#ifndef CLIENT_SCREEN_H
#define CLIENT_SCREEN_H


#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "common.h"

#define SCR_BLACK   0
#define SCR_BLUE    1
#define SCR_GREEN   2
#define SCR_CYAN    3
#define SCR_RED     4
#define SCR_MAGENTA 5
#define SCR_YELLOW  6
#define SCR_WHITE   7

#define SCR_PAIR(X, Y) COLOR_PAIR(__SCR_PAIRS[X][Y])

short __SCR_PAIRS[8][8];
void scr_ctx_init();

void scr_fill();
void scr_char_fill(char);
void scr_wchar_fill(wchar_t);
void scr_hide_cursor();
void scr_center_write(char*, int, int);

void scr_box(int, int, int, int);
WINDOW* scr_win_box(int, int, int, int);

void scr_alert(int, char*);
BOOL scr_prompt(int, char*);
void scr_prompt_string(int, char*, char*, int);
int scr_prompt_options(int, char*, char**, int); 

#endif
