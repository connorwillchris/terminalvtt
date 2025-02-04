#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include <time.h>
#include "lib.h"
#include "ansi.h"
#include "../libs/pcg/pcg_basic.h"

static pcg32_random_t rng;

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

void trim_whitespace(char * str) {
	// should work on both LINUX and WINDOWS
	str[strcspn(str, "\n\r")] = 0;
}

void init_random_seed() {
	
}

int roll_dice(const char * roll) {
    int r;
	pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, (intptr_t)&round);

    return r;
}
