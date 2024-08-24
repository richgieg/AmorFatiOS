#include "mouse.h"
#include "port.h"
#include "pic.h"

#define STATUS_PORT 0x64
#define COMMAND_PORT 0x64
#define DATA_PORT 0x60

#define ENABLE_AUX_PORT 0xa8
#define WRITE_AUX_DEVICE 0xd4
#define RESET_MOUSE 0xff
#define ENABLE_DATA 0xf4

void mouse_init(void) {
    while (inb(STATUS_PORT) & 2); // wait_input_clear
    outb(COMMAND_PORT, ENABLE_AUX_PORT);

    while (inb(STATUS_PORT) & 2); // wait_input_clear
    outb(COMMAND_PORT, WRITE_AUX_DEVICE);

    while (inb(STATUS_PORT) & 2); // wait_input_clear
    outb(DATA_PORT, RESET_MOUSE);

    while ((inb(STATUS_PORT) & 1) == 0); // wait_output_full
    inb(DATA_PORT);

    while ((inb(STATUS_PORT) & 1) == 0); // wait_output_full
    inb(DATA_PORT);

    while (inb(STATUS_PORT) & 2); // wait_input_clear
    outb(COMMAND_PORT, WRITE_AUX_DEVICE);

    while (inb(STATUS_PORT) & 2); // wait_input_clear
    outb(DATA_PORT, ENABLE_DATA);

    pic_unmask_irq(2);
    pic_unmask_irq(12);
}
