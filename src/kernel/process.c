#include "process.h"
#include "types.h"
#include "bugcheck.h"

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
static int current_process_index;

void process_init(void) {
    current_process_index = 0;
    struct process *p = &processes[current_process_index];
    p->is_started = true;
    p->state = PROCESS_STATE_RUNNING;
}

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
    __asm__("cli");

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

    struct process *current_process = &processes[current_process_index];
    struct process *next_process = &processes[next_process_index];

    current_process_index = next_process_index;

    current_process->state = PROCESS_STATE_RUNNABLE;
    next_process->state = PROCESS_STATE_RUNNING;

    // Save the context of the current process.
    __asm__ volatile(
        "pushfd;"
        "pushad;"
        "mov %[old_esp], esp;"
        : [old_esp] "=m" (current_process->esp)
        :
        : "memory"
    );

    if (!next_process->is_started) {
        next_process->is_started = true;

        // Start the next process.
        __asm__(
            "mov esp, %[new_esp];"
            "call %[start];"
            :
            : [new_esp] "m" (next_process->esp), [start] "m" (next_process->start)
            : "memory"
        );
    } else {

        // Restore the context of the next process.
        __asm__(
            "mov esp, %[new_esp];"
            "popad;"
            "popfd;"
            :
            : [new_esp] "m" (next_process->esp)
            : "memory"
        );
    }

    __asm__("sti");
}

int process_get_current_index(void) {
    return current_process_index;
}
