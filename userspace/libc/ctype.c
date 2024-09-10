#include <ctype.h>

int isspace(int c) {
    switch (c) {
        case ' ':
        case '\f':
        case '\n':
        case '\r':
        case '\t':
        case '\v':
            return 1;
    }
    return 0;
}
