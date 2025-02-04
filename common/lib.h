#ifndef LIB_H
#define LIB_H

void chat_screen_init();

char * check_box_input();

void post_message(char username[80], char msg[80]);

void trim_whitespace(char * str);

int roll_dice(const char * roll);

#endif
