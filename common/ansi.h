#ifndef ANSI_H
#define ANSI_H

#define CLEAR_ALL   "\x1b[0m"
#define BOLD        "\x1b[1m"

// colors
#define FG_BLACK    "\x1b[30m"
#define FG_RED      "\x1b[31m"
#define FG_GREEN    "\x1b[32m"
#define FG_YELLOW   "\x1b[33m"
#define FG_BLUE     "\x1b[34m"
#define FG_MAGENTA  "\x1b[35m"
#define FG_CYAN     "\x1b[36m"
#define FG_WHITE    "\x1b[37m"
#define FG_DEFAULT  "\x1b[39m"

#define ERASE_SCREEN "\x1b[2J"
#define CURSOR_MOVE_HOME "\x1b[H"

void move_cursor(int line, int col);

#endif
