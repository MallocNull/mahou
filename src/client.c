#include "client.h"

#define BANNER_WIDTH  57
#define BANNER_HEIGHT 6
char BANNER[BANNER_HEIGHT][BANNER_WIDTH + 1] = {
    ".        :    :::.      ::   .:      ...      ...    :::",
    ";;,.    ;;;   ;;`;;    ,;;   ;;,  .;;;;;;;.   ;;     ;;;",
    "[[[[, ,[[[[, ,[[ '[[, ,[[[,,,[[[ ,[[     \\[[,[['     [[[",
    "$$$$$$$$\"$$$c$$$cc$$$c\"$$$\"\"\"$$$ $$$,     $$$$$      $$$",
    "888 Y88\" 888o888   888,888   \"88o\"888,_ _,88P88    .d888",
    "MMM  M'  \"MMMYMM   \"\"` MMM    YMM  \"YMMMMMP\"  \"YmmMMMM\"\""
};

#define MM_OPT_COUNT   3
#define MM_MAX_OPT_LEN 11
char MM_OPTIONS[MM_OPT_COUNT][MM_MAX_OPT_LEN + 1] = {
    "Join Game",
    "How to Play",
    "Quit"
};
#define MM_OPT_JOIN 0
#define MM_OPT_HOW  1
#define MM_OPT_QUIT 2

struct {
    socket_t *sock;
} client_ctx;

static void hide_cursor();
static int main_menu();
static void how_to_play();

void client() {
    int selected;
    BOOL running = TRUE;

    initscr(); 

    start_color();
    keypad(stdscr, TRUE);

    while(running == TRUE) {
        selected = main_menu();

        switch(selected) {
            case MM_OPT_JOIN:
                
                break;
            case MM_OPT_HOW:
                how_to_play();                
                break;
            case MM_OPT_QUIT:
                running = FALSE;
                break;
        }
    }
    
    endwin();
    
    /*char in[10];
    sock_recv(sock, in, 10); 
    printf("got %s\r\n", in);

    sock_send(sock, "hello", 6);
    printf("sent message");

    sock_stop(sock);
    sock_free(sock);*/
}

static void hide_cursor() {
    move(LINES - 1, COLS - 1);
}

static int main_menu() {
    BOOL polling = TRUE;
    int i, j, selected = 0, color, ch;

    clear();
    noecho();
    //raw();
    cbreak();

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_CYAN);

    attron(COLOR_PAIR(1) | A_BOLD);
    
    for(i = 0; i < LINES; ++i)
        for(j = 0; j < COLS; ++j)
            addch(' ');

    for(i = 0; i < BANNER_HEIGHT; ++i)
        mvprintw(3 + i, (COLS - BANNER_WIDTH) / 2, BANNER[i]);

    attroff(COLOR_PAIR(1) | A_BOLD);

    while(polling == TRUE) {
        for(i = 0; i < MM_OPT_COUNT; ++i) {
            color = selected == i ? 2 : 1;

            attron(COLOR_PAIR(color));
            mvprintw(6 + BANNER_HEIGHT + i*2, (COLS - (MM_MAX_OPT_LEN + 2)) / 2, 
                "* %s", MM_OPTIONS[i]);
            attroff(COLOR_PAIR(color));
        }

        refresh();

        hide_cursor();
        ch = getch();
        switch(ch) {
            case KEY_UP:
                selected = selected == 0 ? 0 : selected - 1;
                break;
            case KEY_DOWN:
                selected = selected == MM_OPT_COUNT - 1 ? MM_OPT_COUNT - 1 : selected + 1;
                break;
            case KEY_LF:
            case KEY_ENTER:
                polling = FALSE;
                break;
        }
    }

    return selected;
}

void how_to_play() {
    WINDOW *win;
    win = newwin(10, 10, 4, 4);
    wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwprintw(win, 1, 1, "test");
    wrefresh(win);
    hide_cursor();
    getch();
}


