#include "screen.h"

void scr_char_fill(char c) {
    int i, j;
    for(i = 0; i < LINES; ++i)
        for(j = 0; j < COLS; ++j)
            addch(c);
}

void scr_str_fill(char *s) {
    int i, j;
    for(i = 0; i < LINES; ++i)
        for(j = 0; j < COLS; ++j)
            addstr(s);
}

void scr_fill() {
    scr_char_fill(' ');
}

void scr_hide_cursor() {
    move(LINES - 1, COLS - 1);
}

void scr_center_write(char *string, int length, int row) {
    if(length == 0)
        length = strlen(string);
    
    mvprintw(row, (COLS - length) / 2, string);
}

void scr_box(int top, int left, int width, int height) {
    int i, j;
    
    mvaddstr(top, left, "\u2554");
    for(i = 0; i < width - 2; ++i)
        addstr("\u2550");
    addstr("\u2557");
    
    for(i = 0; i < height - 2; ++i) {
        mvaddstr(top + i + 1, left, "\u2551");
        for(j = 0; j < width - 2; ++j)
            addch(' ');
        addstr("\u2551");
    }
    
    mvaddstr(top, left, "\u255A");
    for(i = 0; i < width - 2; ++i)
        addstr("\u2550");
    addstr("\u255D");
}

static int calculate_height(int width, int length) {
    return (int)ciel((float)length / (float)width);
}

void scr_alert(int width, char *text) {
    clear();
    noecho();
    cbreak();
    
    int text_height = calculate_height(width, strlen(text));
    
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    
    attron(COLOR_PAIR(3));
    scr_fill();
    attroff(COLOR_PAIR(3));
    
    attron(COLOR_PAIR(1));
    scr_box(1, 1, 10, 10);
    attroff(COLOR_PAIR(1));
}