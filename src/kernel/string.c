#include "string.h"
#include "types.h"

void * memcpy(void *destination, const void *source, size_t num) {
    while (num--) {
        *(u8 *)destination++ = *(u8 *)source++;
    }
    return destination;
}

int memcmp(const void *p1, const void *p2, size_t num) {
    int result = 0;
    while (num-- && result == 0) {
        result = *(u8 *)p1++ - *(u8 *)p2++;
    }
    return result;
}
