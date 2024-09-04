#ifndef BUGCHECK_H
#define BUGCHECK_H

#include <types.h>

void bugcheck(const char *file, int line, const char *message);
void exception(u8 vector);
void exception_with_code(u8 vector, u32 code);

#define BUGCHECK(msg) bugcheck(__FILE__, __LINE__, msg)

#endif
