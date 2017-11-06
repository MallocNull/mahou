#ifndef CLIENT_SCREEN_H
#define CLIENT_SCREEN_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "common.h"

void scr_fill();
void scr_char_fill(char);
void scr_wchar_fill(wchar_t);
void scr_hide_cursor();

void scr_center_write(char*, int, int);

void scr_box(int, int, int, int);

void scr_alert(int, char*);
BOOL scr_prompt_string(int, char*, char*, int);
BOOL scr_prompt_int(int, char*, int*);
BOOL scr_prompt_options(int, char*, char**, int, int*); 

#endif
