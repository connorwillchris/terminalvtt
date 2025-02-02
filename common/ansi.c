#include "ansi.h"
#include <stdio.h>

void move_cursor(int line, int col) {
    printf("\x1b[%d;%dH\x1b[%d;%df", line, col, line, col);
}
