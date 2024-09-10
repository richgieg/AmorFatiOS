#include <program/shell.h>
#include <sys.h>

void shell(void) {
    bool is_left_shift_down = false;
    bool is_right_shift_down = false;

    struct key_event ke;
    char c;

    while (true) {
        sys_console_read_key_event(&ke);
        c = 0;

        // sys_console_putw(ke.scancode);
        // sys_console_putc(' ');
        // ke.is_release ? sys_console_putc('R') : sys_console_putc('P');
        // sys_console_putc('\n');

        if (!ke.is_release) {
            switch (ke.scancode) {
                case SC_LEFT_SHIFT:
                    is_left_shift_down = true;
                    break;
                case SC_RIGHT_SHIFT:
                    is_right_shift_down = true;
                    break;
                case SC_A:
                    c = (is_left_shift_down || is_right_shift_down) ? 'A' : 'a';
                    break;
                case SC_B:
                    c = (is_left_shift_down || is_right_shift_down) ? 'B' : 'b';
                    break;
                case SC_C:
                    c = (is_left_shift_down || is_right_shift_down) ? 'C' : 'c';
                    break;
                case SC_D:
                    c = (is_left_shift_down || is_right_shift_down) ? 'D' : 'd';
                    break;
                case SC_E:
                    c = (is_left_shift_down || is_right_shift_down) ? 'E' : 'e';
                    break;
                case SC_F:
                    c = (is_left_shift_down || is_right_shift_down) ? 'F' : 'f';
                    break;
                case SC_G:
                    c = (is_left_shift_down || is_right_shift_down) ? 'G' : 'g';
                    break;
                case SC_H:
                    c = (is_left_shift_down || is_right_shift_down) ? 'H' : 'h';
                    break;
                case SC_I:
                    c = (is_left_shift_down || is_right_shift_down) ? 'I' : 'i';
                    break;
                case SC_J:
                    c = (is_left_shift_down || is_right_shift_down) ? 'J' : 'j';
                    break;
                case SC_K:
                    c = (is_left_shift_down || is_right_shift_down) ? 'K' : 'k';
                    break;
                case SC_L:
                    c = (is_left_shift_down || is_right_shift_down) ? 'L' : 'l';
                    break;
                case SC_M:
                    c = (is_left_shift_down || is_right_shift_down) ? 'M' : 'm';
                    break;
                case SC_N:
                    c = (is_left_shift_down || is_right_shift_down) ? 'N' : 'n';
                    break;
                case SC_O:
                    c = (is_left_shift_down || is_right_shift_down) ? 'O' : 'o';
                    break;
                case SC_P:
                    c = (is_left_shift_down || is_right_shift_down) ? 'P' : 'p';
                    break;
                case SC_Q:
                    c = (is_left_shift_down || is_right_shift_down) ? 'Q' : 'q';
                    break;
                case SC_R:
                    c = (is_left_shift_down || is_right_shift_down) ? 'R' : 'r';
                    break;
                case SC_S:
                    c = (is_left_shift_down || is_right_shift_down) ? 'S' : 's';
                    break;
                case SC_T:
                    c = (is_left_shift_down || is_right_shift_down) ? 'T' : 't';
                    break;
                case SC_U:
                    c = (is_left_shift_down || is_right_shift_down) ? 'U' : 'u';
                    break;
                case SC_V:
                    c = (is_left_shift_down || is_right_shift_down) ? 'V' : 'v';
                    break;
                case SC_W:
                    c = (is_left_shift_down || is_right_shift_down) ? 'W' : 'w';
                    break;
                case SC_X:
                    c = (is_left_shift_down || is_right_shift_down) ? 'X' : 'x';
                    break;
                case SC_Y:
                    c = (is_left_shift_down || is_right_shift_down) ? 'Y' : 'y';
                    break;
                case SC_Z:
                    c = (is_left_shift_down || is_right_shift_down) ? 'Z' : 'z';
                    break;
                case SC_0:
                    c = (is_left_shift_down || is_right_shift_down) ? ')' : '0';
                    break;
                case SC_1:
                    c = (is_left_shift_down || is_right_shift_down) ? '!' : '1';
                    break;
                case SC_2:
                    c = (is_left_shift_down || is_right_shift_down) ? '@' : '2';
                    break;
                case SC_3:
                    c = (is_left_shift_down || is_right_shift_down) ? '#' : '3';
                    break;
                case SC_4:
                    c = (is_left_shift_down || is_right_shift_down) ? '$' : '4';
                    break;
                case SC_5:
                    c = (is_left_shift_down || is_right_shift_down) ? '%' : '5';
                    break;
                case SC_6:
                    c = (is_left_shift_down || is_right_shift_down) ? '^' : '6';
                    break;
                case SC_7:
                    c = (is_left_shift_down || is_right_shift_down) ? '&' : '7';
                    break;
                case SC_8:
                    c = (is_left_shift_down || is_right_shift_down) ? '*' : '8';
                    break;
                case SC_9:
                    c = (is_left_shift_down || is_right_shift_down) ? '(' : '9';
                    break;
            }
        } else {
            switch (ke.scancode) {
                case SC_LEFT_SHIFT:
                    is_left_shift_down = false;
                    break;
                case SC_RIGHT_SHIFT:
                    is_right_shift_down = false;
                    break;
            }
        }

        if (c) {
            sys_console_putc(c);
        }
    }
}
