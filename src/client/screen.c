#include "screen.h"

void scr_char_fill(char c) {
    int i, j;
    for(i = 0; i < LINES; ++i)
        for(j = 0; j < COLS; ++j)
            addch(c);
}

void scr_fill() {
    scr_char_fill(' ');
}

void scr_hide_cursor() {
    move(LINES - 1, COLS - 1);
}
