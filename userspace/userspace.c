#include <userspace.h>
#include <sys.h>
#include <program/shell.h>
#include <program/memmap.h>
#include <program/memdump.h>
#include <program/counter.h>

void userspace_init(void) {
    sys_process_create(shell);

    while (1) {
        // TODO: Do some userspace stuff here.
    }
}
