// https://wiki.osdev.org/%228042%22_PS/2_Controller

#ifndef PS2_H
#define PS2_H

// Ports
#define PS2_STATUS_PORT 0x64
#define PS2_COMMAND_PORT 0x64
#define PS2_DATA_PORT 0x60

// Status Flags
#define PS2_OUTPUT_BUFFER_FULL 1
#define PS2_INPUT_BUFFER_FULL (1 << 1)

// Configuration Flags
#define PS2_CFG_FIRST_PORT 1
#define PS2_CFG_SECOND_PORT (1 << 1)
#define PS2_CFG_SYSTEM_FLAG (1 << 2)
#define PS2_CFG_FIRST_CLOCK (1 << 4)
#define PS2_CFG_SECOND_CLOCK (1 << 5)
#define PS2_CFG_TRANSLATION (1 << 6)
#define PS2_CFG_MUST_BE_ZERO (1 << 7)

// Controller Commands
#define PS2_DISABLE_FIRST 0xad
#define PS2_DISABLE_SECOND 0xa7
#define PS2_ENABLE_FIRST 0xae
#define PS2_ENABLE_SECOND 0xa8
#define PS2_READ_CONFIG 0x20
#define PS2_WRITE_CONFIG 0x60
#define PS2_SELF_TEST 0xaa
#define PS2_TEST_FIRST 0xab
#define PS2_TEST_SECOND 0xa9
#define PS2_WRITE_SECOND 0xd4

// Controller Responses
#define PS2_CONTROLLER_TEST_OK 0x55
#define PS2_PORT_TEST_OK 0x00

// Device Commands
#define PS2_DEV_RESET 0xff
#define PS2_DEV_IDENTIFY 0xf2
#define PS2_DEV_ENABLE_SCAN 0xf4
#define PS2_DEV_DISABLE_SCAN 0xf5

// Device Responses
#define PS2_DEV_ACK 0xfa
#define PS2_DEV_RESET_ACK 0xaa

void ps2_init(void);

#endif
