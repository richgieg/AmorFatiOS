// https://wiki.osdev.org/%228042%22_PS/2_Controller

#include "ps2.h"
#include "port.h"
#include "vga.h"

static inline void wait_input_clear(void) {
    while (inb(PS2_STATUS_PORT) & PS2_INPUT_BUFFER_FULL) {
        __asm__("pause");
    }
}

static inline void wait_output_full(void) {
    while (!(inb(PS2_STATUS_PORT) & PS2_OUTPUT_BUFFER_FULL)) {
        __asm__("pause");
    }
}

void ps2_init(void) {
    // Step 3: Disable Devices
    outb(PS2_COMMAND_PORT, PS2_DISABLE_FIRST);
    outb(PS2_COMMAND_PORT, PS2_DISABLE_SECOND);

    // Step 4: Flush The Output Buffer
    inb(PS2_DATA_PORT);

    // Step 5: Set the Controller Configuration Byte
    outb(PS2_COMMAND_PORT, PS2_READ_CONFIG);
    wait_output_full();
    uint8_t config = inb(PS2_DATA_PORT);
    char *s = " ";
    s[0] = config;
    vga_print_at(s, 0, 3);

    // Clear bit to disable IRQs for port 1.
    config &= ~PS2_CFG_FIRST_PORT;
    // Clear bit to disable translation for port 1.
    config &= ~PS2_CFG_TRANSLATION;
    // Clear bit to enable clock for port 1.
    config &= ~PS2_CFG_FIRST_CLOCK;

    s[0] = config;
    vga_print_at(s, 1, 3);

    outb(PS2_COMMAND_PORT, PS2_WRITE_CONFIG);
    wait_input_clear();
    outb(PS2_DATA_PORT, config);

    // Step 6: Perform Controller Self Test
    outb(PS2_COMMAND_PORT, PS2_SELF_TEST);
    wait_output_full();
    uint8_t result = inb(PS2_DATA_PORT);
    s[0] = result;
    vga_print_at(s, 2, 3);

    // TODO: Handle result other than PS2_CONTROLLER_TEST_OK

    // Step 7: Determine If There Are 2 Channels
    outb(PS2_COMMAND_PORT, PS2_ENABLE_SECOND);
    outb(PS2_COMMAND_PORT, PS2_READ_CONFIG);
    wait_output_full();
    config = inb(PS2_DATA_PORT);
    s[0] = config;
    vga_print_at(s, 3, 3);

    if (!(config & PS2_CFG_SECOND_CLOCK)) {
        outb(PS2_COMMAND_PORT, PS2_DISABLE_SECOND);
        outb(PS2_COMMAND_PORT, PS2_READ_CONFIG);
        wait_output_full();
        config = inb(PS2_DATA_PORT);
        s[0] = config;
        vga_print_at(s, 4, 3);

        // Clear bit to disable IRQs for port 2.
        config &= ~PS2_CFG_SECOND_PORT;
        // Clear bit to enable clock for port 2.
        config &= ~PS2_CFG_SECOND_CLOCK;

        s[0] = config;
        vga_print_at(s, 5, 3);

        outb(PS2_COMMAND_PORT, PS2_WRITE_CONFIG);
        wait_input_clear();
        outb(PS2_DATA_PORT, config);
    }

    // Step 8: Perform Interface Tests
    outb(PS2_COMMAND_PORT, PS2_TEST_FIRST);
    wait_output_full();
    result = inb(PS2_DATA_PORT);
    if (result == PS2_PORT_TEST_OK) {
        s[0] = 'Y';
    } else {
        s[0] = 'N';
    }
    vga_print_at(s, 6, 3);

    if (!(config & PS2_CFG_SECOND_CLOCK)) {
        outb(PS2_COMMAND_PORT, PS2_TEST_SECOND);
        wait_output_full();
        result = inb(PS2_DATA_PORT);
        if (result == PS2_PORT_TEST_OK) {
            s[0] = 'Y';
        } else {
            s[0] = 'N';
        }
        vga_print_at(s, 7, 3);
    }

    // Step 9: Enable Devices
    // TODO: Only enable devices that pass the previous test.
    outb(PS2_COMMAND_PORT, PS2_ENABLE_FIRST);
    outb(PS2_COMMAND_PORT, PS2_ENABLE_SECOND);
    outb(PS2_COMMAND_PORT, PS2_READ_CONFIG);
    wait_output_full();
    config = inb(PS2_DATA_PORT);

    // Set bit to enable IRQs for port 1.
    config |= PS2_CFG_FIRST_PORT;
    // Set bit to enable IRQs for port 2.
    config |= PS2_CFG_SECOND_PORT;

    s[0] = config;
    vga_print_at(s, 8, 3);

    outb(PS2_COMMAND_PORT, PS2_WRITE_CONFIG);
    wait_input_clear();
    outb(PS2_DATA_PORT, config);

    // Step 10: Reset Devices
    // Reset device at port 1:
    wait_input_clear();
    outb(PS2_DATA_PORT, PS2_DEV_RESET);
    wait_output_full();
    uint8_t r1 = inb(PS2_DATA_PORT);
    wait_output_full();
    uint8_t r2 = inb(PS2_DATA_PORT);
    if ((r1 == PS2_DEV_ACK && r2 == PS2_DEV_RESET_ACK) || (r1 == PS2_DEV_RESET_ACK && r2 == PS2_DEV_ACK)) {
        s[0] = 'Y';
    } else {
        s[0] = 'N';
    }
    vga_print_at(s, 9, 3);

    // Reset device at port 2:
    outb(PS2_COMMAND_PORT, PS2_WRITE_SECOND);
    wait_input_clear();
    outb(PS2_DATA_PORT, PS2_DEV_RESET);
    wait_output_full();
    r1 = inb(PS2_DATA_PORT);
    wait_output_full();
    r2 = inb(PS2_DATA_PORT);
    if ((r1 == PS2_DEV_ACK && r2 == PS2_DEV_RESET_ACK) || (r1 == PS2_DEV_RESET_ACK && r2 == PS2_DEV_ACK)) {
        s[0] = 'Y';
    } else {
        s[0] = 'N';
    }
    vga_print_at(s, 10, 3);

    // Step 11: Enable Scanning
    outb(PS2_COMMAND_PORT, PS2_WRITE_SECOND);
    wait_input_clear();
    outb(PS2_DATA_PORT, PS2_DEV_ENABLE_SCAN);
}
