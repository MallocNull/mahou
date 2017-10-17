#include "ui.h"

struct {
    WINDOW *main_win;
    WINDOW *chat_win;
    WINDOW *info_win;
} ctx;

static void ui_draw_borders();

void ui_init() {
    
}

void ui_refresh() {
    clear();
    
    
    ui_draw_borders();
}

static void ui_draw_borders() {
    
}

void ui_free() {

}
