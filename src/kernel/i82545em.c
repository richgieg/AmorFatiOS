#include "i82545em.h"
#include "pic.h"
#include "port.h"
#include "idt.h"
#include "string.h"
#include "vga.h"
#include "bugcheck.h"

// General Registers
#define R_CTRL      0x00000
#define R_STATUS    0x00008

// Interrupt Registers
#define R_ICR       0x000c0
#define R_ICS       0x000c8
#define R_IMS       0x000d0

// Receive Registers
#define R_RCTL      0x00100
#define R_RDBAL     0x02800
#define R_RDBAH     0x02804
#define R_RDLEN     0x02808
#define R_RDH       0x02810
#define R_RDT       0x02818

// Transmit Registers
#define R_TCTL      0x00400
#define R_TDBAL     0x03800
#define R_TDBAH     0x03804
#define R_TDLEN     0x03808
#define R_TDH       0x03810
#define R_TDT       0x03818

// CTRL Register Bits
#define CTRL_SLU    0x00000040
#define CTRL_RST    0x04000000

// RCTL Register Bits
#define RCTL_EN     0x00000002 // Receiver Enable
#define RCTL_BAM    0x00008000 // Broadcast Accept Mode
#define RCTL_SECRC  0x04000000 // Strip Ethernet CRC

// TCTL Register Bits
#define TCTL_EN     0x00000002 // Transmit Enable

// ICR/ICS Register Bits
#define ICR_TXDW    0x00000001 // Transmit Descriptor Written Back
#define ICR_TXQE    0x00000002 // Transmit Queue Empty
#define ICR_RXDMT0  0x00000010 // Receive Descriptor Minimum Threshold Reached
#define ICR_RXT0    0x00000080 // Reciever Timer Interrupt

// Receive Descriptor Status Bits
#define RX_DESC_STATUS_DD       0x01 // Descriptor Done
#define RX_DESC_STATUS_EOP      0x02 // End Of Packet

// Transmit Descriptor Command Bits
#define TX_DESC_COMMAND_EOP     0x01 // End Of Packet
#define TX_DESC_COMMAND_IFCS    0x02 // Insert FCS
#define TX_DESC_COMMAND_RS      0x08 // Repot Status

// Transmit Descriptor Status Bits
#define TX_DESC_STATUS_DD       0x01 // Descriptor Done

#define RX_BUFFER_COUNT 64
#define RX_BUFFER_SIZE 2048

#define TX_BUFFER_COUNT 64
#define TX_BUFFER_SIZE 2048

struct rx_descriptor {
    u64 address;
    u16 length;
    u16 checksum;
    u8 status;
    u8 errors;
    u8 reserved[2];
} __attribute__((packed));

struct tx_descriptor {
    u64 address;
    u16 length;
    u8 checksum_offset;
    u8 command;
    u8 status;
    u8 checksum_start;
    u16 special;
} __attribute__((packed));

static u32 mmio_base;

__attribute__((aligned(0x10)))
static struct rx_descriptor rx_descriptors[RX_BUFFER_COUNT];
static u8 rx_buffers[RX_BUFFER_COUNT][RX_BUFFER_SIZE];
static u16 rx_cur_idx;

__attribute__((aligned(0x10)))
static struct tx_descriptor tx_descriptors[TX_BUFFER_COUNT];
static u8 tx_buffers[TX_BUFFER_COUNT][TX_BUFFER_SIZE];
static u16 tx_cur_idx;

u32 read_mmio(u32 mmio_base, u32 offset) {
    volatile u32 *mmio_address = (volatile u32 *)(mmio_base + offset);
    return *mmio_address;
}

void write_mmio(u32 mmio_base, u32 offset, u32 value) {
    volatile u32 *mmio_address = (volatile u32 *)(mmio_base + offset);
    *mmio_address = value;
}

static void interrupt_service_routine(void) {
    u32 icr = read_mmio(mmio_base, R_ICR);
    vga_puts("82525EM interrupt -- ICR: ");
    vga_putdw(icr);
    vga_putc('\n');

    if (icr & ICR_TXDW) {
        vga_puts("TXDW: Transmit Descriptor Written Back\n");
    }

    if (icr & ICR_TXQE) {
        vga_puts("TXQE: Transmit Queue Empty\n");
    }

    if (icr & ICR_RXDMT0) {
        vga_puts("RXDMT0: Receive Descriptor Minimum Threshold Reached\n");
    }

    if (icr & ICR_RXT0) {
        vga_puts("RXT0: Receiver Timer Interrupt\n");

        while (rx_descriptors[rx_cur_idx].status & RX_DESC_STATUS_DD) {

            // TODO: Handle multi-descriptor packets.
            if (!(rx_descriptors[rx_cur_idx].status & RX_DESC_STATUS_EOP)) {
                BUGCHECK("Multi-descriptor packets not supported yet.");
            }

            vga_puts("Status=");
            vga_putb(rx_descriptors[rx_cur_idx].status);
            vga_puts(" Length=");
            vga_putw(rx_descriptors[rx_cur_idx].length);
            vga_putc('\n');

            for (u16 i = 0; i < rx_descriptors[rx_cur_idx].length; i++) {
                vga_putb(rx_buffers[rx_cur_idx][i]);
                vga_putc(' ');
            }
            vga_putc('\n');

            rx_descriptors[rx_cur_idx].status = 0;
            rx_cur_idx = (rx_cur_idx + 1) % RX_BUFFER_COUNT;
        }

        write_mmio(mmio_base, R_RDT, (rx_cur_idx - 1) % RX_BUFFER_COUNT);
    }

    outb(PIC1_COMMAND, PIC_EOI);
    outb(PIC2_COMMAND, PIC_EOI);
}

__attribute__((naked))
static void interrupt_service_routine_stub(void) {
    interrupt_service_routine();
    __asm__("iret");
}

void i82545em_init(u32 bar0) {
    // NOTE: If bit 0 in bar is 1 then I/O port should be used instead.
    // u32 mmio_base = bar0 & 0xfffffff0;
    mmio_base = bar0 & 0xfffffff0;
    vga_puts("mmio_base = ");
    vga_putdw(mmio_base);
    vga_putc('\n');

    u32 ctrl = read_mmio(mmio_base, R_CTRL);
    vga_puts("82525EM CTRL: ");
    vga_putdw(ctrl);
    vga_putc('\n');

    write_mmio(mmio_base, R_CTRL, ctrl | CTRL_SLU);
    for (volatile int i = 0; i < 100000000; i++);
    ctrl = read_mmio(mmio_base, R_CTRL);
    vga_puts("82525EM CTRL: ");
    vga_putdw(ctrl);
    vga_putc('\n');

    u32 status = read_mmio(mmio_base, R_STATUS);
    vga_puts("82525EM STATUS: ");
    vga_putdw(status);
    vga_putc('\n');

    u32 rctl = read_mmio(mmio_base, R_RCTL);
    vga_puts("82525EM RCTL: ");
    vga_putdw(rctl);
    vga_putc('\n');

    u32 tctl = read_mmio(mmio_base, R_TCTL);
    vga_puts("82525EM TCTL: ");
    vga_putdw(tctl);
    vga_putc('\n');

    // Register ISR and unmask the interrupt in the PIC.
    idt_set_descriptor(IRQ11_INTERRUPT, interrupt_service_routine_stub, 0x8e);
    pic_unmask_irq(11);

    // Enable all interrupts.
    write_mmio(mmio_base, R_IMS, 0xffffffff);

    // Store receive descriptor base address.
    write_mmio(mmio_base, R_RDBAL, (u32)rx_descriptors);
    write_mmio(mmio_base, R_RDBAH, 0);

    // Store receive descriptor buffer length.
    write_mmio(mmio_base, R_RDLEN, sizeof(rx_descriptors));

    // Set current receive descriptor index to beginning of ring buffer.
    rx_cur_idx = 0;

    // Initialize the receive descriptor ring head and tail.
    write_mmio(mmio_base, R_RDH, rx_cur_idx);
    write_mmio(mmio_base, R_RDT, (rx_cur_idx - 1) % RX_BUFFER_COUNT);

    // Initialize the receive descriptors.
    for (int i = 0; i < RX_BUFFER_COUNT; i++) {
        rx_descriptors[i].address = (u32)(&rx_buffers[i]);
    }

    // Store transmit descriptor base address.
    write_mmio(mmio_base, R_TDBAL, (u32)tx_descriptors);
    write_mmio(mmio_base, R_TDBAH, 0);

    // Store transmit descriptor buffer length.
    write_mmio(mmio_base, R_TDLEN, sizeof(tx_descriptors));

    // Set current transmit descriptor index to beginning of ring buffer.
    tx_cur_idx = 0;

    // Initialize the transmit descriptor ring head and tail.
    write_mmio(mmio_base, R_TDH, tx_cur_idx);
    write_mmio(mmio_base, R_TDT, tx_cur_idx);

    // Initialize the transmit descriptors.
    for (int i = 0; i < TX_BUFFER_COUNT; i++) {
        tx_descriptors[i].address = (u32)(&tx_buffers[i]);
        tx_descriptors[i].command = TX_DESC_COMMAND_RS | TX_DESC_COMMAND_EOP | TX_DESC_COMMAND_IFCS;
    }

    // Enable receive function.
    write_mmio(mmio_base, R_RCTL, rctl | RCTL_EN | RCTL_BAM | RCTL_SECRC);

    // Enable transmit function.
    write_mmio(mmio_base, R_TCTL, tctl | TCTL_EN);

    // Send a test ping.
    u8 frame[] = {
        // Destination
        0x00, 0x0c, 0x29, 0x3c, 0x62, 0x6c,
        // Source
        0x00, 0x0c, 0x29, 0xb6, 0xd3, 0x84,
        // Type
        0x08, 0x00,
        // Data
        0x45, 0x00, 0x00, 0x54, 0x3e, 0x6f, 0x40, 0x00, 0x40, 0x01, 0x79, 0x8c, 0xc0, 0xa8, 0x00, 0xfa,
        0xc0, 0xa8, 0x00, 0x63, 0x08, 0x00, 0x86, 0x6a, 0x00, 0x01, 0x00, 0x01, 0x0c, 0xc4, 0xd0, 0x66,
        0x00, 0x00, 0x00, 0x00, 0xcb, 0x95, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x11, 0x12, 0x13,
        0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
        0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33,
        0x34, 0x35, 0x36, 0x37,
    };

    memcpy(tx_buffers[0], frame, sizeof(frame));
    tx_descriptors[0].length = sizeof(frame);
    write_mmio(mmio_base, R_TDT, 1);
}
