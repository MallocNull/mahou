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

#define MM_OPT_COUNT   4
#define MM_MAX_OPT_LEN 15
char MM_OPTIONS[MM_OPT_COUNT][MM_MAX_OPT_LEN + 1] = {
    "Join Game",
    "Create Account",
    "How to Play",
    "Quit"
};
#define MM_OPT_JOIN 0
#define MM_OPT_REG  1
#define MM_OPT_HOW  2
#define MM_OPT_QUIT 3

struct {
    socket_t *sock;
} client_ctx;

static int main_menu();
static void how_to_play();
static void create_account();
static void client_loop();

void client() {
    int selected, err;
    BOOL running = TRUE;
    client_ctx.sock = sock_client_init("127.0.0.1", "6770");

    printf("Connecting to server...\n");
    err = sock_start(client_ctx.sock);
    if(err != SOCK_SUCCESS) {
        printf("Connection failed, error %d.", err);
        getchar();
        return;
    }

    initscr(); 

    start_color();
    keypad(stdscr, TRUE);

    while(running == TRUE) {
        selected = main_menu();

        switch(selected) {
            case MM_OPT_JOIN:
                client_loop();
                break;
            case MM_OPT_REG:
                create_account();
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
   
    scr_fill();
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

        scr_hide_cursor();
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
    mvwprintw(win, 1, 1, "to do: write this garbage");
    wrefresh(win);
    scr_hide_cursor();
    getch();
    delwin(win);
}

void create_account() {

}

void client_loop() {
    
}