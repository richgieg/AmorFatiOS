#include <userspace.h>
#include <program/conproc.h>
#include <sys.h>

void userspace_init(void) {
    sys_process_create_ex(conproc, false);
    sys_process_exit();
}
