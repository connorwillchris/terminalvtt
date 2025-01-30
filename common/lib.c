#include "lib.h"

#include <stdlib.h>
#include <ncurses.h>

static WINDOW * inputwin;
static int msg_y = 0;

void chat_screen_init() {
    initscr();

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    inputwin = newwin(3, xMax-12, yMax-5, 5);
    box(inputwin, 0, 0);
    refresh();
    wrefresh(inputwin);
}

char * check_box_input() {
    wclear(inputwin);
    box(inputwin, 0, 0);

    char * msg = (char *)malloc(sizeof(char) * 80);

    // NOTE: Probably not safe, so will only use FGETS function once we set everything up properly.
    // Will also not use NCURSES, since this is not cross platform for windows users.
    //mvwscanw(inputwin, 1, 1, "%[^\n]", msg);
    mvwgetnstr(inputwin, 1, 1, msg, 80 - 1);

    return msg;
}

void post_message(char username[80], char msg[80]) {
    mvprintw(msg_y, 1, "%s: %s", username, msg);
}
