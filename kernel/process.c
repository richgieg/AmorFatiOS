#include <process.h>
#include <types.h>
#include <bugcheck.h>
#include <console.h>
#include <tss.h>
#include <mm.h>

#define MAX_PROCESSES 32
#define STACK_SIZE 4096

struct process {
    int pid;
    void (*start)();
    bool is_started;
    enum process_state state;
    void *kernel_stack;
    void *user_stack;
    u32 kernel_esp;
    u32 user_esp;
    int console_index;
    int waiting_for_exit_pid;
    bool is_killable;
    bool has_pending_kill_signal;
    struct tree_node *tree_node;
    struct list_node *list_node;
};

struct tree_node {
    struct process *process;
    struct tree_node *parent;
    struct tree_node *next_sibling;
    struct tree_node *first_child;
};

struct list_node {
    struct process *process;
    struct list_node *next;
};

// Statically allocate all nodes for now (until there's memory manager).
static struct process processes[MAX_PROCESSES];
static struct tree_node tree_nodes[MAX_PROCESSES];
static struct list_node list_nodes[MAX_PROCESSES];

static struct process *system_process;
static struct process *running_process;

static struct tree_node tree_root;

static struct list_node *runnable_list;
static struct list_node *waiting_list;

static void enqueue_list_node(struct list_node **head, struct list_node *node);
static struct list_node *dequeue_list_node(struct list_node **head);
static void remove_list_node(struct list_node **head, struct list_node *node);
static void add_child_to_parent(struct tree_node *parent, struct tree_node *child);
static void remove_process_from_tree(struct process *process);

void process_init(void) {
    system_process = &processes[0];
    system_process->pid = 0;
    system_process->is_started = true;
    system_process->state = PROCESS_STATE_RUNNING;
    system_process->console_index = 0;
    system_process->is_killable = false;
    system_process->has_pending_kill_signal = false;

    system_process->list_node = &list_nodes[0];
    system_process->list_node->process = system_process;
    system_process->list_node->next = NULL;

    system_process->tree_node = &tree_nodes[0];
    system_process->tree_node->process = system_process;
    system_process->tree_node->parent = &tree_root;
    system_process->tree_node->next_sibling = NULL;
    system_process->tree_node->first_child = NULL;

    tree_root.process = NULL;
    tree_root.parent = NULL;
    tree_root.next_sibling = NULL;
    tree_root.first_child = system_process->tree_node;

    running_process = system_process;
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

int process_create(void (*start)()) {
    return process_create_in_console(start, running_process->console_index, true);
}

int process_create_ex(void (*start)(), bool is_killable) {
    return process_create_in_console(start, running_process->console_index, is_killable);
}

int process_create_in_console(void (*start)(), int console_index, bool is_killable) {
    int index = find_available_process_index();
    if (index == -1) {
        return -1;
    }

    struct process *process = &processes[index];
    process->pid = index; // this approach is only temporary
    process->start = start;
    process->is_started = false;
    process->state = PROCESS_STATE_RUNNABLE;
    process->kernel_stack = mm_alloc_for_pid(process->pid, STACK_SIZE);
    process->kernel_esp = (u32)process->kernel_stack + STACK_SIZE;
    process->user_stack = mm_alloc_for_pid(process->pid, STACK_SIZE);
    process->user_esp = (u32)process->user_stack + STACK_SIZE;
    process->console_index = console_index;
    process->is_killable = is_killable;
    process->has_pending_kill_signal = false;

    process->list_node = &list_nodes[index];
    process->list_node->process = process;
    process->list_node->next = NULL;

    process->tree_node = &tree_nodes[index];
    process->tree_node->process = process;
    process->tree_node->parent = running_process->tree_node;
    process->tree_node->next_sibling = NULL;
    process->tree_node->first_child = NULL;

    add_child_to_parent(running_process->tree_node, process->tree_node);
    enqueue_list_node(&runnable_list, process->list_node);

    return index;
}

void process_switch(enum process_state new_state) {
    // Process pending kill signals.
    // TODO: Make this better!
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (processes[i].has_pending_kill_signal) {
            processes[i].state = PROCESS_STATE_NULL;
            remove_process_from_tree(&processes[i]);
            console_handle_process_kill(processes[i].console_index,
                                        processes[i].pid);
            remove_list_node(&runnable_list, processes[i].list_node);
            remove_list_node(&waiting_list, processes[i].list_node);
            mm_free_all_for_pid(processes[i].pid);
        }
    }

    struct list_node *waiting_node = waiting_list;

    // Find a waiting process that is ready to run (if any) and move it to the
    // runnable queue.
    while (waiting_node) {
        if (waiting_node->process->state == PROCESS_STATE_WAITING_FOR_KEY_EVENT) {
            if (console_has_key_event_for_process(waiting_node->process->console_index,
                                                  waiting_node->process->pid)) {
                waiting_node->process->state = PROCESS_STATE_RUNNABLE;
                remove_list_node(&waiting_list, waiting_node);
                enqueue_list_node(&runnable_list, waiting_node);
                break;
            }
        }
        if (waiting_node->process->state == PROCESS_STATE_WAITING_FOR_EXIT) {
            if (processes[waiting_node->process->waiting_for_exit_pid].state == PROCESS_STATE_NULL) {
                waiting_node->process->state = PROCESS_STATE_RUNNABLE;
                remove_list_node(&waiting_list, waiting_node);
                enqueue_list_node(&runnable_list, waiting_node);
                break;
            }
        }
        waiting_node = waiting_node->next;
    }

    struct list_node *next_node = dequeue_list_node(&runnable_list);

    if (!next_node) {
        // If currently running the system process, just stay on it.
        if (running_process == system_process) {
            return;
        }
        // This should never happen, since the system process never dies.
        BUGCHECK("Could not find a runnable process.");
    }

    switch (new_state) {
        case PROCESS_STATE_NULL: // process is exiting
            console_handle_process_exit(running_process->console_index,
                                        running_process->pid);
            remove_process_from_tree(running_process);
            mm_free_all_for_pid(running_process->pid);
            break;
        case PROCESS_STATE_RUNNABLE:
            enqueue_list_node(&runnable_list, running_process->list_node);
            break;
        case PROCESS_STATE_RUNNING:
            BUGCHECK("Cannot pass PROCESS_STATE_RUNNING to process_switch.");
            break;
        case PROCESS_STATE_WAITING:
        case PROCESS_STATE_WAITING_FOR_EXIT:
        case PROCESS_STATE_WAITING_FOR_KEY_EVENT:
            enqueue_list_node(&waiting_list, running_process->list_node);
            break;
    }

    running_process->state = new_state;

    // Save the context of the running process.
    __asm__ volatile(
        "pushfd;"
        "pushad;"
        "mov %[old_esp], esp;"
        : [old_esp] "=m" (running_process->kernel_esp)
        :
        : "memory"
    );

    // Switch to the next process.
    running_process = next_node->process;
    running_process->state = PROCESS_STATE_RUNNING;

    // Set to the bottom of the kernel stack.
    tss_set_kernel_stack((u32)running_process->kernel_stack + PAGE_SIZE);

    if (!running_process->is_started) {
        running_process->is_started = true;
        console_handle_process_start(running_process->console_index,
                                     running_process->pid);

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
            "ord [esp], 0x200;" // enables interrupts upon iret
            "push 0x1b;"
            "push %[start];"
            "iret;"
            :
            : [new_esp] "m" (running_process->user_esp), [start] "m" (running_process->start)
            : "memory"
        );
    } else {
        // Restore the context of the process.
        __asm__(
            "mov esp, %[new_esp];"
            "popad;"
            "popfd;"
            :
            : [new_esp] "m" (running_process->kernel_esp)
            : "memory"
        );
    }
}

void process_exit(void) {
    process_switch(PROCESS_STATE_NULL);
}

void process_wait_for_exit(int pid) {
    running_process->waiting_for_exit_pid = pid;
    process_switch(PROCESS_STATE_WAITING_FOR_EXIT);
}

int process_get_console_index(void) {
    return running_process->console_index;
}

static struct process *get_process_by_pid(int pid) {
    if (pid < 0 || pid >= MAX_PROCESSES) {
        return NULL;
    }
    if (processes[pid].state == PROCESS_STATE_NULL) {
        return NULL;
    }
    return &processes[pid];
}

void process_get_child_pids(int pid, int *buf, size_t buf_size, int *count) {
    struct tree_node *child;
    int max = buf_size / sizeof(int);
    int c = 0;

    if (pid == -1) {
        child = tree_root.first_child;
    } else {
        struct process *process = get_process_by_pid(pid);
        if (!process) {
            return;
        }
        child = process->tree_node->first_child;
    }

    while (child && c < max) {
        buf[c] = child->process->pid;
        child = child->next_sibling;
        c++;
    }

    *count = c;
}

int process_get_current_pid(void) {
    return running_process->pid;
}

void process_send_kill_signal(int pid) {
    if (pid < 0 || pid >= MAX_PROCESSES) {
        return;
    }
    struct process *process = &processes[pid];
    if (process->state == PROCESS_STATE_NULL) {
        return;
    }
    if (process->is_killable) {
        process->has_pending_kill_signal = true;
    }
}

static void enqueue_list_node(struct list_node **head, struct list_node *node) {
    if (*head) {
        struct list_node *n = *head;
        while (n->next) {
            n = n->next;
        }
        n->next = node;
    } else {
        *head = node;
    }
    node->next = NULL;
}

static struct list_node *dequeue_list_node(struct list_node **head) {
    struct list_node *node = *head;
    if (node) {
        *head = node->next;
        node->next = NULL;
    }
    return node;
}

static void remove_list_node(struct list_node **head, struct list_node *node) {
    if (!head) {
        return;
    }
    if (*head == node) {
        *head = node->next;
    } else {
        struct list_node *prev = *head;
        while (prev && prev->next != node) {
            prev = prev->next;
        }
        if (prev) {
            prev->next = node->next;
        }
    }
    node->next = NULL;
}

static void add_child_to_parent(struct tree_node *parent, struct tree_node *child) {
    if (parent->first_child) {
        struct tree_node *node = parent->first_child;
        while (node->next_sibling) {
            node = node->next_sibling;
        }
        node->next_sibling = child;
    } else {
        parent->first_child = child;
    }
}

static void remove_process_from_tree(struct process *process) {
    struct tree_node *node = process->tree_node;

    // Remove the node from parent's children.
    if (node->parent->first_child == node) {
        node->parent->first_child = NULL;
    } else {
        struct tree_node *prev = node->parent->first_child;
        while (prev && prev->next_sibling != node) {
            prev = prev->next_sibling;
        }
        if (prev) {
            prev->next_sibling = node->next_sibling;
        }
    }

    // Add node's children to root node's children.
    add_child_to_parent(&tree_root, node->first_child);

    // Set parent to tree root for all children.
    struct tree_node *child = node->first_child;
    while (child) {
        child->parent = &tree_root;
        child = child->next_sibling;
    }
}
