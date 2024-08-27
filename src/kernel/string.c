#include "string.h"
#include <stdint.h>

int memcmp(const void *p1, const void *p2, size_t num) {
    int result = 0;
    while (num-- && result == 0) {
        result = *(uint8_t *)p1++ - *(uint8_t *)p2++;
    }
    return result;
}
