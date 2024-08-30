// https://wiki.osdev.org/%228042%22_PS/2_Controller

#include "ps2.h"
#include "port.h"

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
    u8 config = inb(PS2_DATA_PORT);
    // vga_putb_at(config, 0, 3);

    // Clear bit to disable IRQs for port 1.
    config &= ~PS2_CFG_FIRST_PORT;
    // Clear bit to disable translation for port 1.
    config &= ~PS2_CFG_TRANSLATION;
    // Clear bit to enable clock for port 1.
    config &= ~PS2_CFG_FIRST_CLOCK;
    // vga_putb_at(config, 2, 3);

    outb(PS2_COMMAND_PORT, PS2_WRITE_CONFIG);
    wait_input_clear();
    outb(PS2_DATA_PORT, config);

    // Step 6: Perform Controller Self Test
    outb(PS2_COMMAND_PORT, PS2_SELF_TEST);
    wait_output_full();
    u8 result = inb(PS2_DATA_PORT);
    // vga_putb_at(result, 4, 3);

    // TODO: Handle result other than PS2_CONTROLLER_TEST_OK

    // Step 7: Determine If There Are 2 Channels
    outb(PS2_COMMAND_PORT, PS2_ENABLE_SECOND);
    outb(PS2_COMMAND_PORT, PS2_READ_CONFIG);
    wait_output_full();
    config = inb(PS2_DATA_PORT);
    // vga_putb_at(config, 6, 3);

    if (!(config & PS2_CFG_SECOND_CLOCK)) {
        outb(PS2_COMMAND_PORT, PS2_DISABLE_SECOND);
        outb(PS2_COMMAND_PORT, PS2_READ_CONFIG);
        wait_output_full();
        config = inb(PS2_DATA_PORT);
        // vga_putb_at(config, 8, 3);

        // Clear bit to disable IRQs for port 2.
        config &= ~PS2_CFG_SECOND_PORT;
        // Clear bit to enable clock for port 2.
        config &= ~PS2_CFG_SECOND_CLOCK;
        // vga_putb_at(config, 10, 3);

        outb(PS2_COMMAND_PORT, PS2_WRITE_CONFIG);
        wait_input_clear();
        outb(PS2_DATA_PORT, config);
    }

    // Step 8: Perform Interface Tests
    outb(PS2_COMMAND_PORT, PS2_TEST_FIRST);
    wait_output_full();
    result = inb(PS2_DATA_PORT);
    if (result == PS2_PORT_TEST_OK) {
        // vga_putc_at('Y', 12, 3);
    } else {
        // vga_putc_at('N', 12, 3);
    }

    if (!(config & PS2_CFG_SECOND_CLOCK)) {
        outb(PS2_COMMAND_PORT, PS2_TEST_SECOND);
        wait_output_full();
        result = inb(PS2_DATA_PORT);
        if (result == PS2_PORT_TEST_OK) {
            // vga_putc_at('Y', 13, 3);
        } else {
            // vga_putc_at('N', 13, 3);
        }
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
    // vga_putb_at(config, 14, 3);

    outb(PS2_COMMAND_PORT, PS2_WRITE_CONFIG);
    wait_input_clear();
    outb(PS2_DATA_PORT, config);

    // Step 10: Reset Devices
    // Reset device at port 1:
    wait_input_clear();
    outb(PS2_DATA_PORT, PS2_DEV_RESET);
    wait_output_full();
    u8 r1 = inb(PS2_DATA_PORT);
    wait_output_full();
    u8 r2 = inb(PS2_DATA_PORT);
    if ((r1 == PS2_DEV_ACK && r2 == PS2_DEV_RESET_ACK) || (r1 == PS2_DEV_RESET_ACK && r2 == PS2_DEV_ACK)) {
        // vga_putc_at('Y', 16, 3);
    } else {
        // vga_putc_at('N', 16, 3);
    }

    // Reset device at port 2:
    outb(PS2_COMMAND_PORT, PS2_WRITE_SECOND);
    wait_input_clear();
    outb(PS2_DATA_PORT, PS2_DEV_RESET);
    wait_output_full();
    r1 = inb(PS2_DATA_PORT);
    wait_output_full();
    r2 = inb(PS2_DATA_PORT);
    if ((r1 == PS2_DEV_ACK && r2 == PS2_DEV_RESET_ACK) || (r1 == PS2_DEV_RESET_ACK && r2 == PS2_DEV_ACK)) {
        // vga_putc_at('Y', 17, 3);
    } else {
        // vga_putc_at('N', 17, 3);
    }

    // Step 11: Enable Scanning
    outb(PS2_COMMAND_PORT, PS2_WRITE_SECOND);
    wait_input_clear();
    outb(PS2_DATA_PORT, PS2_DEV_ENABLE_SCAN);
    wait_output_full();
    result = inb(PS2_DATA_PORT);
    // TODO: Handle failure.
    if (result == PS2_DEV_ACK) {
        // Scanning enabled.
    } else {
        // Failed to enable scan.
    }
}
