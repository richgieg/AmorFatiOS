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

struct process_context {
    u32 eax;
    u32 ebx;
    u32 ecx;
    u32 edx;
    u32 esi;
    u32 edi;
    u32 esp;
    u32 ebp;
    u16 cs;
    u16 ds;
    u16 es;
    u16 fs;
    u16 gs;
    u32 eflags;
};

struct process {
    void (*start)();
    bool is_started;
    enum process_state state;
    struct process_context context;
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
    __asm__("cli"); // TODO: Replace with locking mechanism

    int index = find_available_process_index();
    if (index == -1) {
        BUGCHECK("Maximum number of processes reached.");
    }

    struct process *p = &processes[index];
    p->start = start;
    p->is_started = false;
    p->state = PROCESS_STATE_RUNNABLE;
    p->context.eax = 0;
    p->context.ebx = 0;
    p->context.ecx = 0;
    p->context.edx = 0;
    p->context.esi = 0;
    p->context.edi = 0;
    p->context.esp = STACK_AREA_BASE + (index + 1) * STACK_SIZE;
    p->context.ebp = 0;
    p->context.cs = 0x8;
    p->context.ds = 0x10;
    p->context.es = 0x10;
    p->context.fs = 0x10;
    p->context.gs = 0x10;

    __asm__("sti"); // TODO: Replace with locking mechanism
}

void process_switch(void) {
    __asm__("cli"); // TODO: Replace with locking mechanism

    if (current_process_index != -1) {
        // TODO: Store context of current process and update its state.
        struct process *p = &processes[current_process_index];
        p->state = PROCESS_STATE_RUNNABLE;
        // TODO: Use inline assembly to save these registers.
        // p->context.eax = 0;
        // p->context.ebx = 0;
        // p->context.ecx = 0;
        // p->context.edx = 0;
        // p->context.esi = 0;
        // p->context.edi = 0;
        // p->context.esp = 0;
        // p->context.ebp = 0;
        // p->context.cs = 0x8;
        // p->context.ds = 0x10;
        // p->context.es = 0x10;
        // p->context.fs = 0x10;
        // p->context.gs = 0x10;
    }

    // TODO: Find next process to run, load its context, and update its state.
    int count = MAX_PROCESSES;
    int next_process_index = -1;
    int index = current_process_index + 1;

    while (count--) {
        struct process *p = &processes[index];
        if (p->state == PROCESS_STATE_RUNNABLE) {
            next_process_index = index;
        }
        index = (index + 1) % MAX_PROCESSES;
    }

    if (next_process_index == -1) {
        BUGCHECK("Could not find a runnable process.");
    }

    __asm__("sti"); // TODO: Replace with locking mechanism
}
