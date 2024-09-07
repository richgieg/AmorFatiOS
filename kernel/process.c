#include <process.h>
#include <types.h>
#include <bugcheck.h>
#include <console.h>
#include <tss.h>

#define STACK_SIZE 4096
#define STACK_AREA_BASE 0x100000

struct process {
    void (*start)();
    bool is_started;
    enum process_state state;
    u32 kernel_esp;
    u32 user_esp;
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
    p->kernel_esp = STACK_AREA_BASE + STACK_SIZE + (index * STACK_SIZE * 2);
    p->user_esp = p->kernel_esp + STACK_SIZE;
}

// static u32 process_switch_count;

void process_switch(enum process_state state) {
    // console_dbg_puts("process_switch ");
    // console_dbg_putdw(process_switch_count);
    // console_dbg_putc('\n');
    // process_switch_count++;

    int count = MAX_PROCESSES;
    int next_process_index = -1;
    int index = (current_process_index + 1) % MAX_PROCESSES;

    while (count--) {
        if (processes[index].state == PROCESS_STATE_RUNNABLE) {
            next_process_index = index;
            break;
        }
        if (processes[index].state == PROCESS_STATE_WAITING_FOR_KEY_EVENT) {
            if (console_has_key_event(index)) {
                next_process_index = index;
                break;
            }
        }
        index = (index + 1) % MAX_PROCESSES;
    }

    if (next_process_index == -1) {
        // If currently running idle process, just stay on idle process.
        if (current_process_index == 0) {
            return;
        }
        // This should never happen, since the idle process never dies.
        BUGCHECK("Could not find a runnable process.");
    }

    struct process *current_process = &processes[current_process_index];
    struct process *next_process = &processes[next_process_index];

    current_process_index = next_process_index;

    current_process->state = state;
    next_process->state = PROCESS_STATE_RUNNING;

    // Save the context of the current process.
    __asm__ volatile(
        "pushfd;"
        "pushad;"
        "mov %[old_esp], esp;"
        : [old_esp] "=m" (current_process->kernel_esp)
        :
        : "memory"
    );

    console_dbg_puts("next proc id = ");
    console_dbg_putdw(next_process_index);
    console_dbg_puts(", kernel_esp = ");
    console_dbg_putdw(next_process->kernel_esp);
    console_dbg_putc('\n');

    tss_set_kernel_stack(next_process->kernel_esp);

    if (!next_process->is_started) {
        next_process->is_started = true;

        // Start the next process.
        __asm__(
            "push ax;"
            "mov ax, 0x23;"
            "mov ds, ax;"
            "mov es, ax;"
            "mov fs, ax;"
            "mov gs, ax;"
            "pop ax;"
            "push 0x23;"
            "push %[new_esp];"
            "pushfd;"
            "ord [esp], 0x200;"
            "push 0x1b;"
            "push %[start];"
            "iret;"
            :
            : [new_esp] "m" (next_process->user_esp), [start] "m" (next_process->start)
            : "memory"
        );
    } else {
        // Restore the context of the next process.
        __asm__(
            "mov esp, %[new_esp];"
            "popad;"
            "popfd;"
            :
            : [new_esp] "m" (next_process->kernel_esp)
            : "memory"
        );
    }
}

int process_get_current_index(void) {
    return current_process_index;
}
