#include "ansi.h"
#include <stdio.h>

void move_cursor(int line, int col) {
    printf("\x1b[{%d};{%d}H\x1b[{%d}{%d}f", line, col, line, col);
}
