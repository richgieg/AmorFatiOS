#include "string.h"
#include <stdint.h>

void * memcpy(void *destination, const void *source, size_t num) {
    while (num--) {
        *(uint8_t *)destination++ = *(uint8_t *)source++;
    }
    return destination;
}

int memcmp(const void *p1, const void *p2, size_t num) {
    int result = 0;
    while (num-- && result == 0) {
        result = *(uint8_t *)p1++ - *(uint8_t *)p2++;
    }
    return result;
}
