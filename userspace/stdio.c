#include <stdio.h>
#include <sys.h>

char * gets(char *buf) {
    char * const buf_start = buf;

    bool is_lshift_down = false;
    bool is_rshift_down = false;

    struct key_event ke;
    char c;
    bool is_end_of_input = false;

    while (!is_end_of_input) {
        sys_console_read_key_event(&ke);
        c = 0;

        if (!ke.is_release) {
            switch (ke.scancode) {
                case SC_LEFT_SHIFT:
                    is_lshift_down = true;
                    break;
                case SC_RIGHT_SHIFT:
                    is_rshift_down = true;
                    break;
                case SC_SPACE:
                    c = ' ';
                    break;
                case SC_A:
                    c = (is_lshift_down || is_rshift_down) ? 'A' : 'a';
                    break;
                case SC_B:
                    c = (is_lshift_down || is_rshift_down) ? 'B' : 'b';
                    break;
                case SC_C:
                    c = (is_lshift_down || is_rshift_down) ? 'C' : 'c';
                    break;
                case SC_D:
                    c = (is_lshift_down || is_rshift_down) ? 'D' : 'd';
                    break;
                case SC_E:
                    c = (is_lshift_down || is_rshift_down) ? 'E' : 'e';
                    break;
                case SC_F:
                    c = (is_lshift_down || is_rshift_down) ? 'F' : 'f';
                    break;
                case SC_G:
                    c = (is_lshift_down || is_rshift_down) ? 'G' : 'g';
                    break;
                case SC_H:
                    c = (is_lshift_down || is_rshift_down) ? 'H' : 'h';
                    break;
                case SC_I:
                    c = (is_lshift_down || is_rshift_down) ? 'I' : 'i';
                    break;
                case SC_J:
                    c = (is_lshift_down || is_rshift_down) ? 'J' : 'j';
                    break;
                case SC_K:
                    c = (is_lshift_down || is_rshift_down) ? 'K' : 'k';
                    break;
                case SC_L:
                    c = (is_lshift_down || is_rshift_down) ? 'L' : 'l';
                    break;
                case SC_M:
                    c = (is_lshift_down || is_rshift_down) ? 'M' : 'm';
                    break;
                case SC_N:
                    c = (is_lshift_down || is_rshift_down) ? 'N' : 'n';
                    break;
                case SC_O:
                    c = (is_lshift_down || is_rshift_down) ? 'O' : 'o';
                    break;
                case SC_P:
                    c = (is_lshift_down || is_rshift_down) ? 'P' : 'p';
                    break;
                case SC_Q:
                    c = (is_lshift_down || is_rshift_down) ? 'Q' : 'q';
                    break;
                case SC_R:
                    c = (is_lshift_down || is_rshift_down) ? 'R' : 'r';
                    break;
                case SC_S:
                    c = (is_lshift_down || is_rshift_down) ? 'S' : 's';
                    break;
                case SC_T:
                    c = (is_lshift_down || is_rshift_down) ? 'T' : 't';
                    break;
                case SC_U:
                    c = (is_lshift_down || is_rshift_down) ? 'U' : 'u';
                    break;
                case SC_V:
                    c = (is_lshift_down || is_rshift_down) ? 'V' : 'v';
                    break;
                case SC_W:
                    c = (is_lshift_down || is_rshift_down) ? 'W' : 'w';
                    break;
                case SC_X:
                    c = (is_lshift_down || is_rshift_down) ? 'X' : 'x';
                    break;
                case SC_Y:
                    c = (is_lshift_down || is_rshift_down) ? 'Y' : 'y';
                    break;
                case SC_Z:
                    c = (is_lshift_down || is_rshift_down) ? 'Z' : 'z';
                    break;
                case SC_0:
                    c = (is_lshift_down || is_rshift_down) ? ')' : '0';
                    break;
                case SC_1:
                    c = (is_lshift_down || is_rshift_down) ? '!' : '1';
                    break;
                case SC_2:
                    c = (is_lshift_down || is_rshift_down) ? '@' : '2';
                    break;
                case SC_3:
                    c = (is_lshift_down || is_rshift_down) ? '#' : '3';
                    break;
                case SC_4:
                    c = (is_lshift_down || is_rshift_down) ? '$' : '4';
                    break;
                case SC_5:
                    c = (is_lshift_down || is_rshift_down) ? '%' : '5';
                    break;
                case SC_6:
                    c = (is_lshift_down || is_rshift_down) ? '^' : '6';
                    break;
                case SC_7:
                    c = (is_lshift_down || is_rshift_down) ? '&' : '7';
                    break;
                case SC_8:
                    c = (is_lshift_down || is_rshift_down) ? '*' : '8';
                    break;
                case SC_9:
                    c = (is_lshift_down || is_rshift_down) ? '(' : '9';
                    break;
                case SC_COMMA:
                    c = (is_lshift_down || is_rshift_down) ? '<' : ',';
                    break;
                case SC_PERIOD:
                    c = (is_lshift_down || is_rshift_down) ? '>' : '.';
                    break;
                case SC_SLASH:
                    c = (is_lshift_down || is_rshift_down) ? '?' : '/';
                    break;
                case SC_SEMICOLON:
                    c = (is_lshift_down || is_rshift_down) ? ':' : ';';
                    break;
                case SC_APOSTROPHE:
                    c = (is_lshift_down || is_rshift_down) ? '"' : '\'';
                    break;
                case SC_LEFT_BRACKET:
                    c = (is_lshift_down || is_rshift_down) ? '{' : '[';
                    break;
                case SC_RIGHT_BRACKET:
                    c = (is_lshift_down || is_rshift_down) ? '}' : ']';
                    break;
                case SC_BACKSLASH:
                    c = (is_lshift_down || is_rshift_down) ? '|' : '\\';
                    break;
                case SC_HYPHEN:
                    c = (is_lshift_down || is_rshift_down) ? '_' : '-';
                    break;
                case SC_EQUAL:
                    c = (is_lshift_down || is_rshift_down) ? '+' : '=';
                    break;
                case SC_BACKTICK:
                    c = (is_lshift_down || is_rshift_down) ? '~' : '`';
                    break;
                case SC_BACKSPACE:
                    c = '\b';
                    break;
                case SC_ENTER:
                    c = '\n';
                    is_end_of_input = true;
                    break;
            }
        } else {
            switch (ke.scancode) {
                case SC_LEFT_SHIFT:
                    is_lshift_down = false;
                    break;
                case SC_RIGHT_SHIFT:
                    is_rshift_down = false;
                    break;
            }
        }

        if (c) {
            if (c == '\n') {
                sys_console_putc(c);
            } else if (c == '\b') {
                if (buf > buf_start) {
                    buf--;
                    sys_console_putc(c);
                }
            } else {
                *buf = c;
                buf++;
                sys_console_putc(c);
            }
        }
    }

    *buf = '\0';
    return buf_start;
}

void puts(const char *str) {
    sys_console_puts(str);
}
