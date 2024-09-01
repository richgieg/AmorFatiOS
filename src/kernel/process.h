#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROCESSES 8

void process_init(void);
void process_create(void (*start)());
void process_switch(void);
int process_get_current_index(void);

#endif
