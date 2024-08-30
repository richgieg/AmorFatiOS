#ifndef BUGCHECK_H
#define BUGCHECK_H

#include <stdint.h>

void bugcheck(const char *file, int line, const char *message);
void exception(uint8_t vector);
void exception_with_code(uint8_t vector, uint32_t code);

#define BUGCHECK(msg) bugcheck(__FILE__, __LINE__, msg)

#endif
