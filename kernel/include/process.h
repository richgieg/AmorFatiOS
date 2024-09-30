#ifndef PROCESS_H
#define PROCESS_H

#include <types.h>

enum process_state {
    PROCESS_STATE_NULL,
    PROCESS_STATE_RUNNING,
    PROCESS_STATE_RUNNABLE,
    PROCESS_STATE_WAITING,
    PROCESS_STATE_WAITING_FOR_EXIT,
    PROCESS_STATE_WAITING_FOR_KEY_EVENT,
    PROCESS_STATE_WAITING_FOR_NET_FRAME
};

void process_init(void);
int process_create(void (*start)());
int process_create_ex(void (*start)(), bool is_killable);
int process_create_in_console(void (*start)(), int console_index, bool is_killable);
void process_switch(enum process_state new_state);
void process_exit(void);
void process_wait_for_exit(int pid);
void process_wait_for_key_event(void);
void process_wait_for_net_frame(void);
int process_get_console_index(void);
void process_get_child_pids(int pid, int *buf, size_t buf_size, int *count);
int process_get_current_pid(void);
void process_send_kill_signal(int pid);

#endif
