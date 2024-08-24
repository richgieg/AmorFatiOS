#include "timer.h"
#include "pic.h"

void timer_init(void) {
    pic_unmask_irq(0);
}
