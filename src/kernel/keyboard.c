#include "keyboard.h"
#include "pic.h"

void keyboard_init(void) {
    pic_unmask_irq(1);
}
