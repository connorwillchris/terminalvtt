#include "lib.h"
#include "ansi.h"

#include <stdlib.h>
#include <ncurses.h>

/*
static WINDOW * inputwin;
static int msg_y = 0;
*/
void chat_screen_init() {
    printf("%s%s", ERASE_SCREEN, CURSOR_MOVE_HOME);
}

char * check_box_input() {
    char * msg = (char *)malloc(sizeof(char) * 80);
    // NOTE: Probably not safe, so will only use FGETS function once we set
    // everything up properly. Will also not use NCURSES, since this is not cross
    // platform for windows users.
    fgets(msg, 80, stdin);
    return msg;
}

void post_message(char username[80], char msg[80]) {
    //mvprintw(msg_y, 1, "%s: %s", username, msg);
    printf("%s: %s", username, msg);
}
