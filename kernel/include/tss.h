#ifndef TSS_H
#define TSS_H

#include <types.h>

void tss_init(void);
void tss_set_kernel_stack(u32 esp);

#endif
