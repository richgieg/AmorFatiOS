#ifndef PROCESS_H
#define PROCESS_H

#define MAX_PROCESSES 8

enum process_state {
    PROCESS_STATE_NULL,
    PROCESS_STATE_RUNNING,
    PROCESS_STATE_RUNNABLE,
    PROCESS_STATE_WAITING,
    PROCESS_STATE_WAITING_FOR_KEY_EVENT,
    PROCESS_STATE_WAITING_FOR_EXIT
};

void process_init(void);
int process_create(void (*start)());
void process_switch(enum process_state state);
void process_exit(void);
void process_wait_for_exit(int pid);
int process_get_console_index(void);

#endif
