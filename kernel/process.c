#include <process.h>
#include <types.h>
#include <bugcheck.h>
#include <console.h>
#include <tss.h>

#define MAX_PROCESSES 32
#define STACK_SIZE 4096
#define STACK_AREA_BASE 0x100000

struct process {
    void (*start)();
    bool is_started;
    enum process_state state;
    u32 kernel_esp_bottom;
    u32 kernel_esp;
    u32 user_esp;
    int console_index;
    int waiting_for_exit_pid;
};

struct process_list_node {
    struct process p;
    struct process_list_node *next;
};

// Statically allocate all nodes for now (until there's memory manager).
static struct process_list_node plnodes[MAX_PROCESSES];

static struct process_list_node *runnable_list;
static struct process_list_node *waiting_list;
static struct process_list_node *running_node;
static struct process_list_node *system_node;

void enqueue(struct process_list_node **head, struct process_list_node *node);
struct process_list_node *dequeue(struct process_list_node **head);
void remove(struct process_list_node **head, struct process_list_node *node);

void process_init(void) {
    system_node = &plnodes[0];
    system_node->p.is_started = true;
    system_node->p.state = PROCESS_STATE_RUNNING;
    system_node->p.console_index = 0;
    running_node = system_node;
}

static int find_available_process_list_node_index() {
    int index;
    for (index = 0; index < MAX_PROCESSES; index++) {
        if (plnodes[index].p.state == PROCESS_STATE_NULL) {
            break;
        }
    }
    return (index < MAX_PROCESSES) ? index : -1;
}

int process_create(void (*start)()) {
    return process_create_in_console(
        start, running_node->p.console_index);
}

int process_create_in_console(void (*start)(), int console_index) {
    int index = find_available_process_list_node_index();
    if (index == -1) {
        return -1;
    }
    struct process *p = &plnodes[index].p;
    p->start = start;
    p->is_started = false;
    p->state = PROCESS_STATE_RUNNABLE;
    p->kernel_esp_bottom = STACK_AREA_BASE + STACK_SIZE + (index * STACK_SIZE * 2);
    p->kernel_esp = p->kernel_esp_bottom;
    p->user_esp = p->kernel_esp + STACK_SIZE;
    p->console_index = console_index;
    enqueue(&runnable_list, &plnodes[index]);
    return index;
}

void process_switch(enum process_state state) {
    struct process_list_node *waiting_node = waiting_list;

    // Find a waiting process that is ready to run (if any) and move it to the
    // runnable queue.
    while (waiting_node) {
        if (waiting_node->p.state == PROCESS_STATE_WAITING_FOR_KEY_EVENT) {
            if (console_has_key_event(waiting_node->p.console_index)) {
                waiting_node->p.state = PROCESS_STATE_RUNNABLE;
                remove(&waiting_list, waiting_node);
                enqueue(&runnable_list, waiting_node);
                break;
            }
        }
        if (waiting_node->p.state == PROCESS_STATE_WAITING_FOR_EXIT) {
            if (plnodes[waiting_node->p.waiting_for_exit_pid].p.state == PROCESS_STATE_NULL) {
                waiting_node->p.state = PROCESS_STATE_RUNNABLE;
                remove(&waiting_list, waiting_node);
                enqueue(&runnable_list, waiting_node);
                break;
            }
        }
        waiting_node = waiting_node->next;
    }

    struct process_list_node *next_node = dequeue(&runnable_list);

    if (!next_node) {
        // If currently running the system process, just stay on it.
        if (running_node == system_node) {
            return;
        }
        // This should never happen, since the system process never dies.
        BUGCHECK("Could not find a runnable process.");
    }

    switch (state) {
        case PROCESS_STATE_NULL:
            // The process exited.
            break;
        case PROCESS_STATE_RUNNABLE:
            enqueue(&runnable_list, running_node);
            break;
        case PROCESS_STATE_RUNNING:
            BUGCHECK("Cannot pass PROCESS_STATE_RUNNING to process_switch.");
            break;
        case PROCESS_STATE_WAITING:
        case PROCESS_STATE_WAITING_FOR_EXIT:
        case PROCESS_STATE_WAITING_FOR_KEY_EVENT:
            enqueue(&waiting_list, running_node);
            break;
    }

    running_node->p.state = state;

    // Save the context of the running process.
    __asm__ volatile(
        "pushfd;"
        "pushad;"
        "mov %[old_esp], esp;"
        : [old_esp] "=m" (running_node->p.kernel_esp)
        :
        : "memory"
    );

    // Switch to the next process.
    running_node = next_node;
    running_node->p.state = PROCESS_STATE_RUNNING;

    tss_set_kernel_stack(running_node->p.kernel_esp_bottom);

    if (!running_node->p.is_started) {
        running_node->p.is_started = true;

        // Start the process.
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
            : [new_esp] "m" (running_node->p.user_esp), [start] "m" (running_node->p.start)
            : "memory"
        );
    } else {
        // Restore the context of the process.
        __asm__(
            "mov esp, %[new_esp];"
            "popad;"
            "popfd;"
            :
            : [new_esp] "m" (running_node->p.kernel_esp)
            : "memory"
        );
    }
}

void process_exit(void) {
    process_switch(PROCESS_STATE_NULL);
}

void process_wait_for_exit(int pid) {
    running_node->p.waiting_for_exit_pid = pid;
    process_switch(PROCESS_STATE_WAITING_FOR_EXIT);
}

int process_get_console_index(void) {
    return running_node->p.console_index;
}

void enqueue(struct process_list_node **head, struct process_list_node *node) {
    if (*head) {
        struct process_list_node *n = *head;
        while (n->next) {
            n = n->next;
        }
        n->next = node;
    } else {
        *head = node;
    }
    node->next = NULL;
}

struct process_list_node *dequeue(struct process_list_node **head) {
    struct process_list_node *node = *head;
    if (node) {
        *head = node->next;
        node->next = NULL;
    }
    return node;
}

void remove(struct process_list_node **head, struct process_list_node *node) {
    if (!head) {
        return;
    }
    if (*head == node) {
        *head = node->next;
    } else {
        struct process_list_node *prev = *head;
        while (prev && prev->next != node) {
            prev = prev->next;
        }
        if (prev) {
            prev->next = node->next;
        }
    }
    node->next = NULL;
}
