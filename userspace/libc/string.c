#include <string.h>

int strcmp(const char *lhs, const char *rhs) {
    int result;

    while ((result = (unsigned char)(*lhs) - (unsigned char)(*rhs)) == 0) {
        if (*lhs == '\0') {
            break;
        }
        lhs++;
        rhs++;
    }

    if (result < 0) {
        return -1;
    }
    if (result > 0) {
        return 1;
    }
    return result;
}

size_t strlen(const char *str) {
    size_t len = 0;
    while (*str++ != '\0') {
        len++;
    }
    return len;
}
