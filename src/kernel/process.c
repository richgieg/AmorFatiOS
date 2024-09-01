#include "process.h"
#include "types.h"
#include "bugcheck.h"

#define MAX_PROCESSES 8
#define STACK_SIZE 4096
#define STACK_AREA_BASE 0x100000

enum process_state {
    PROCESS_STATE_NULL,
    PROCESS_STATE_RUNNING,
    PROCESS_STATE_RUNNABLE,
    PROCESS_STATE_WAITING,
};

struct process {
    void (*start)();
    bool is_started;
    enum process_state state;
    u32 esp;
};

static struct process processes[MAX_PROCESSES];
static int current_process_index = -1;

static int find_available_process_index() {
    int index;
    for (index = 0; index < MAX_PROCESSES; index++) {
        if (processes[index].state == PROCESS_STATE_NULL) {
            break;
        }
    }
    return (index < MAX_PROCESSES) ? index : -1;
}

void process_create(void (*start)()) {
    int index = find_available_process_index();
    if (index == -1) {
        BUGCHECK("Maximum number of processes reached.");
    }

    struct process *p = &processes[index];
    p->start = start;
    p->is_started = false;
    p->state = PROCESS_STATE_RUNNABLE;
    p->esp = STACK_AREA_BASE + (index + 1) * STACK_SIZE;
}

void process_switch(void) {
    struct process *curr_proc;
    struct process *next_proc;

    if (current_process_index != -1) {
        curr_proc = &processes[current_process_index];
    } else {
        curr_proc = 0;
    }

    int count = MAX_PROCESSES;
    int next_process_index = -1;
    int index = (current_process_index + 1) % MAX_PROCESSES;

    while (count--) {
        if (processes[index].state == PROCESS_STATE_RUNNABLE) {
            next_process_index = index;
            break;
        }
        index = (index + 1) % MAX_PROCESSES;
    }

    if (next_process_index == -1) {
        BUGCHECK("Could not find a runnable process.");
    }

    current_process_index = next_process_index;
    next_proc = &processes[current_process_index];

    if (curr_proc) {
        curr_proc->state = PROCESS_STATE_RUNNABLE;
    }
    next_proc->state = PROCESS_STATE_RUNNING;

    if (!next_proc->is_started) {
        u32 *esp = (u32 *)next_proc->esp;

        esp--; // eip
        *esp = (u32)next_proc->start;

        u32 eflags;
        __asm__ volatile(
            "pushfd;"
            "pop %0"
            : "=r"(eflags)
            :
            : "memory"
        );

        esp--; // eflags
        *esp = eflags;

        esp--; // eax
        *esp = 0;

        esp--; // ecx
        *esp = 0;

        esp--; // edx
        *esp = 0;

        esp--; // ebx
        *esp = 0;

        esp--; // esp
        *esp = next_proc->esp;

        esp--; // ebp
        *esp = 0;

        esp--; // esi
        *esp = 0;

        esp--; // edi
        *esp = 0;

        next_proc->esp = (u32)esp;
        next_proc->is_started = true;

        if (curr_proc) {
            __asm__ volatile(
                "pushfd;"
                "pushad;"
                "mov %[old_esp], esp;"
                "mov esp, %[new_esp];"
                "popad;"
                "popfd;"
                "ret;"
                : [old_esp] "=m" (curr_proc->esp)
                : [new_esp] "m" (next_proc->esp)
                : "memory"
            );
        } else {
            __asm__(
                "mov esp, %[new_esp];"
                "popad;"
                "popfd;"
                "ret;"
                :
                : [new_esp] "m" (next_proc->esp)
                : "memory"
            );
        }
    } else {
        __asm__ volatile(
            "pushfd;"
            "pushad;"
            "mov %[old_esp], esp;"
            "mov esp, %[new_esp];"
            "popad;"
            "popfd;"
            : [old_esp] "=m" (curr_proc->esp)
            : [new_esp] "m" (next_proc->esp)
            : "memory"
        );
    }
}
