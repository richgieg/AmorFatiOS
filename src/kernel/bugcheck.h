#ifndef BUGCHECK_H
#define BUGCHECK_H

void bugcheck(const char *file, int line, const char *message);

#define BUGCHECK(msg) bugcheck(__FILE__, __LINE__, msg)

#endif
