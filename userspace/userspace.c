#include <userspace.h>
#include <program/conproc.h>
#include <sys.h>

void userspace_init(void) {
    sys_process_create(conproc);
    sys_process_exit();
}
