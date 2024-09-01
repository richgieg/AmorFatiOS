#ifndef PROCESS_H
#define PROCESS_H

void process_init(void);
void process_create(void (*start)());
void process_switch(void);

#endif
