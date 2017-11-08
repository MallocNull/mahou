#include "screen.h"

void scr_ctx_init() {
    short i, j, val;
    short colors[] = {COLOR_BLACK, COLOR_BLUE,    COLOR_GREEN,  COLOR_CYAN,
                      COLOR_RED,   COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE};
                      
    for(i = 0; i < 8; ++i) {
        for(j = 0; j < 8; ++j) {
            val = i * 8 + j;
            __SCR_PAIRS[i][j] = val;
            init_pair(val, colors[i], colors[j]);
        }
    }
}

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
    
    mvaddstr(top + height - 1, left, "\u255A");
    for(i = 0; i < width - 2; ++i)
        addstr("\u2550");
    addstr("\u255D");
}

WINDOW* scr_win_box(int top, int left, int width, int height) {
    WINDOW *win = newwin(height - 2, width - 2, top + 1, left + 1);
    scr_box(top, left, width, height);
    refresh();
    
    return win;
}

static int calculate_height(int width, char *text) {
    int len = strlen(text), lines = 0, line_width, line_len, char_width, i;
    char *ptr, *cpy = (char*)malloc(len * sizeof(char));
    strcpy(cpy, text);
    
    ptr = strtok(cpy, "\n");
    while(ptr != NULL) {
        line_width = 0;
        line_len = strlen(ptr);
        for(i = 0; i < line_len; ++i) {
            if(ptr[i] & 0x80 == 0 && isprint(ptr[i])) {
                char_width = 0;
                ++line_width;
            } else if(ptr[i] & 0xE0 == 0xC0) {
                char_width = 1;
                ++line_width;
            } else if(ptr[i] & 0xF0 == 0xE0) {
                char_width = 2;
                ++line_width;
            } else if(ptr[i] & 0xF8 == 0xF0) {
                char_width = 3;
                ++line_width;
            } else if(ptr[i] & 0xC0 == 0x80) {
                if(char_width > 0)
                    --char_width;
                else
                    ++line_width;
            }
        }
        
        
        
        ptr = strtok(NULL, "\n");
    }
    
    return lines;
}

void scr_alert(int width, char *text) {
    erase();
    noecho();
    cbreak();
    
    short pair;
    WINDOW *win;
    int text_height = calculate_height(width, strlen(text));
    
    #ifdef WORK
    pair = SCR_PAIR(SCR_WHITE, SCR_BLACK);
    #else
    pair = SCR_PAIR(SCR_WHITE, SCR_BLUE);
    #endif
    
    attron(pair);
    scr_fill();
    attron(pair);
    
    attron(SCR_PAIR(SCR_WHITE, SCR_BLACK));
    win = scr_win_box(1, (COLS - (width + 2)) / 2, width + 2, text_height + 4);
    attron(SCR_PAIR(SCR_WHITE, SCR_BLACK));
    
    wattron(win, SCR_PAIR(SCR_WHITE, SCR_BLACK));
    wprintw(win, text);
    wattroff(win, SCR_PAIR(SCR_WHITE, SCR_BLACK));
    
    wattron(win, SCR_PAIR(SCR_BLACK, SCR_CYAN));
    mvwprintw(win, text_height + 1, (width - 6) / 2, "[ OK ]");
    wrefresh(win);
    wattroff(win, SCR_PAIR(SCR_BLACK, SCR_CYAN));
    
    scr_hide_cursor();
    getch();
    delwin(win);
}

BOOL scr_prompt(int width, char *text) {
    erase();
    noecho();
    cbreak();
    
    int ch;
    short pair;
    BOOL retval = TRUE, loop = TRUE;
    char check[] = "[ \u2714 ]";
    char cross[] = "[ \u2718 ]";
    WINDOW *win;
    int text_height = calculate_height(width, strlen(text));
    
    #ifdef WORK
    pair = SCR_PAIR(SCR_WHITE, SCR_BLACK);
    #else
    pair = SCR_PAIR(SCR_WHITE, SCR_BLUE);
    #endif
    
    attron(pair);
    scr_fill();
    attron(pair);
    
    attron(SCR_PAIR(SCR_WHITE, SCR_BLACK));
    win = scr_win_box(1, (COLS - (width + 2)) / 2, width + 2, text_height + 4);
    attron(SCR_PAIR(SCR_WHITE, SCR_BLACK));
    
    wattron(win, SCR_PAIR(SCR_WHITE, SCR_BLACK));
    wprintw(win, text);
    wrefresh(win);
    wattroff(win, SCR_PAIR(SCR_WHITE, SCR_BLACK));
    
    while(loop) {
        wmove(win, text_height + 1, (width - 12) / 2);
        
        pair = retval == TRUE 
            ? SCR_PAIR(SCR_BLACK, SCR_CYAN)
            : SCR_PAIR(SCR_WHITE, SCR_BLACK);
            
        wattron(win, pair);
        wprintw(win, check);
        wattroff(win, pair);
        
        wprintw(win, "  ");
        
        pair = retval == FALSE
            ? SCR_PAIR(SCR_BLACK, SCR_CYAN)
            : SCR_PAIR(SCR_WHITE, SCR_BLACK);
        
        wattron(win, pair);
        wprintw(win, cross);
        wattroff(win, pair);
        
        wrefresh(win);
        
        scr_hide_cursor();
        ch = getch();
        switch(ch) {
            case KEY_LEFT:
                retval = TRUE;
                break;
            case KEY_RIGHT:
                retval = FALSE;
                break;
            case KEY_LF:
            case KEY_ENTER:
                loop = FALSE;
                break;
        }
    }
    
    delwin(win);
	return retval;
}

void scr_prompt_string(int width, char *text, char *out, int outlen) {
    erase();
    noecho();
    cbreak();
    
    int ch, curlen = 0, i, origin;
    short pair;
    WINDOW *win;
    int text_height = calculate_height(width, strlen(text));
    
    #ifdef WORK
    pair = SCR_PAIR(SCR_WHITE, SCR_BLACK);
    #else
    pair = SCR_PAIR(SCR_WHITE, SCR_BLUE);
    #endif
    
    attron(pair);
    scr_fill();
    attron(pair);
    
    attron(SCR_PAIR(SCR_WHITE, SCR_BLACK));
    win = scr_win_box(1, (COLS - (width + 2)) / 2, width + 2, text_height + 4);
    attroff(SCR_PAIR(SCR_WHITE, SCR_BLACK));
    
    wattron(win, SCR_PAIR(SCR_WHITE, SCR_BLACK));
    wprintw(win, text);
    wrefresh(win);
    wattroff(win, SCR_PAIR(SCR_WHITE, SCR_BLACK));
    
    out[0] = '\0';
    for(;;) {
		origin = (width - (outlen + 3)) / 2;
        wmove(win, text_height + 1, origin);
        
        pair = SCR_PAIR(SCR_BLACK, SCR_CYAN);
		
        wattron(win, pair);
        wprintw(win, "-> ");
        
        wattron(win, A_UNDERLINE);
        wprintw(win, out);
		for(i = curlen; i < outlen; ++i)
			waddch(win, ' ');
        wattroff(win, A_UNDERLINE);
        wattroff(win, pair);
        
		//if(curlen < outlen)
			wmove(win, text_height + 1, origin + 3 + curlen);
		//else
			//scr_hide_cursor();
		
        wrefresh(win);
        ch = getch();
		if(curlen < outlen && isprint(ch)) {
			out[curlen] = ch;
			out[curlen + 1] = '\0';
			++curlen;
		} else if(curlen != 0 && (ch == KEY_BS || ch == KEY_BACKSPACE)) {
			--curlen;
			out[curlen] = '\0';
		} else if(ch == KEY_LF || ch == KEY_ENTER)
			break;
    }
    
    delwin(win);
}

int scr_prompt_options(int width, char *text, char **options, int optcount) {
    erase();
    noecho();
    cbreak();
    
    int ch, selected = 0, i, origin, maxlen = 0;
    short pair;
    BOOL loop = TRUE;
    WINDOW *win;
    int text_height = calculate_height(width, strlen(text));
    
    #ifdef WORK
    pair = SCR_PAIR(SCR_WHITE, SCR_BLACK);
    #else
    pair = SCR_PAIR(SCR_WHITE, SCR_BLUE);
    #endif
    
    attron(pair);
    scr_fill();
    attron(pair);
    
    attron(SCR_PAIR(SCR_WHITE, SCR_BLACK));
    win = scr_win_box(1, (COLS - (width + 2)) / 2, width + 2, text_height + 3 + optcount);
    attroff(SCR_PAIR(SCR_WHITE, SCR_BLACK));
    
    wattron(win, SCR_PAIR(SCR_WHITE, SCR_BLACK));
    wprintw(win, text);
    wrefresh(win);
    wattroff(win, SCR_PAIR(SCR_WHITE, SCR_BLACK));
    
    for(i = 0; i < optcount; ++i)
        maxlen = MAX(maxlen, strlen(options[i]) + 3);
    origin = (width - maxlen) / 2;
    
    while(loop) {
		origin = (width - (outlen + 3)) / 2;
        wmove(win, text_height + 1, origin);
        
        for(i = 0; i < optcount; ++i) {
            pair = i == selected ? SCR_PAIR(SCR_BLACK, SCR_CYAN)
                                 : SCR_PAIR(SCR_WHITE, SCR_BLACK);
            
            
        }
		
        wattron(win, pair);
        wprintw(win, "-> ");
        
        wattron(win, A_UNDERLINE);
        wprintw(win, out);
		for(i = curlen; i < outlen; ++i)
			waddch(win, ' ');
        wattroff(win, A_UNDERLINE);
        wattroff(win, pair);
        wrefresh(win);
        
        scr_hide_cursor();
        ch = getch();
        switch(ch) {
            case KEY_UP:
                selected = selected == 0 ? optcount - 1 : selected - 1;
                break;
            case KEY_DOWN:
                selected = selected == optcount - 1 ? 0 : selected + 1;
                break;
            case KEY_LF:
            case KEY_ENTER:
                loop = FALSE;
                break;
        }
    }
    
    delwin(win);
    return selected;
}