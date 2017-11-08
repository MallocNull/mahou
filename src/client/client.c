#include "client.h"

#define BANNER_WIDTH  57
#define BANNER_HEIGHT 6
#define AFTER_BANNER BANNER_HEIGHT + 3
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
} static ctx;

static int main_menu();
static void how_to_play();
static void login_prompt(char*, uint16_t*);
static void create_account();
static void client_loop();

void client() {
    int selected, err;
    BOOL running = TRUE;
    ctx.sock = sock_client_init("127.0.0.1", "6770");

    printf("Connecting to server...\n");
    err = sock_start(ctx.sock);
    if(err != SOCK_SUCCESS) {
        printf("Connection failed, error %d.", err);
        getchar();
        return;
    }

	setlocale(LC_ALL, "");
    initscr(); 

    start_color();
    scr_ctx_init();
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
	
    sock_stop(ctx.sock);
    sock_free(ctx.sock);
}

static void draw_banner() {
    int i;
    for(i = 0; i < BANNER_HEIGHT; ++i)
        scr_center_write(BANNER[i], BANNER_WIDTH, 3 + i);
}

static int main_menu() {
    BOOL polling = TRUE;
    int i, j, selected = 0, color, ch;

    erase();
    noecho();
    cbreak();

    #ifdef WORK
    attron(SCR_PAIR(SCR_WHITE, SCR_BLACK) | A_BOLD);
    #else
    attron(SCR_PAIR(SCR_WHITE, SCR_BLUE) | A_BOLD);
    #endif
   
    scr_fill();
    draw_banner();

    attroff(SCR_PAIR(SCR_WHITE, SCR_BLACK) | A_BOLD);

    while(polling == TRUE) {
        for(i = 0; i < MM_OPT_COUNT; ++i) {
            color = selected == i 
                ? SCR_PAIR(SCR_BLACK, SCR_CYAN) 
                #ifdef WORK
                : SCR_PAIR(SCR_WHITE, SCR_BLACK);
                #else
                : SCR_PAIR(SCR_WHITE, SCR_BLUE);
                #endif

            attron(color);
            mvprintw(AFTER_BANNER + 3 + i*2, (COLS - (MM_MAX_OPT_LEN + 2)) / 2, 
                "* %s", MM_OPTIONS[i]);
            attroff(color);
        }

        refresh();

        scr_hide_cursor();
        ch = getch();
        switch(ch) {
            case KEY_UP:
                selected = selected == 0 ? MM_OPT_COUNT - 1 : selected - 1;
                break;
            case KEY_DOWN:
                selected = selected == MM_OPT_COUNT - 1 ? 0 : selected + 1;
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
    
	char input[9];
    scr_prompt_string(20, "this prompt box is a test of a prompt box is a test of a prompt box is a test of a prompt box is a test of a prompt box is a test of a prompt box is a test of a prompt box is a test of a prompt box is a test of a prompt box is a test", input, 8);
    
    /*int a = 0;
    while(a != KEY_LF)
        printw("%i ", (a = getch()));*/
}

void client_loop() {
    
}
