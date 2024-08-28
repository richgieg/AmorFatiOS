// NOTE: Currently a mix of PCI and Intel 82545EM network driver.
// Network driver portion will be moved at some point.

#include "pci.h"
#include "vga.h"
#include "string.h"
#include "pic.h"
#include "idt.h"
#include "port.h"

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

// CTRL Register Bits
#define CTRL_SLU    0x00000040
#define CTRL_RST    0x04000000

// RCTL Register Bits
#define RCTL_EN     0x00000002

// ICR Register Bits
#define ICR_RXDMT0  0x00000010
#define ICR_RXT0    0x00000080

#define RX_BUFFER_COUNT 64
#define RX_BUFFER_SIZE 2048

typedef struct {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdt_address;
} __attribute__((packed)) rsdp_t;

typedef struct {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdt_address;
    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __attribute__((packed)) xsdp_t;

typedef struct {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    char oem_table_id[8];
    int32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed)) sdt_header_t;

typedef struct {
    sdt_header_t header;
    uint32_t sdts[];
} __attribute__((packed)) rsdt_t;

typedef struct {
    sdt_header_t header;
    uint64_t sdts[];
} __attribute__((packed)) xsdt_t;

typedef struct {
    uint64_t base_address;
    uint16_t group;
    uint8_t start_bus;
    uint8_t end_bus;
    uint8_t reserved[4];
} __attribute__((packed)) mcfg_entry_t;

typedef struct {
    sdt_header_t header;
    uint8_t reserved[8];
    mcfg_entry_t entries[];
} __attribute__((packed)) mcfg_t;

rsdp_t * find_rsdp(void) {
    for (uint32_t address = 0x000e0000; address < 0x00100000; address += 16) {
        if (memcmp((void *)address, "RSD PTR ", 8) == 0) {
            return (rsdp_t *)address;
        }
    }
    return 0;
}

xsdp_t * find_xsdp(void) {
    for (uint32_t address = 0x000e0000; address < 0x00100000; address += 16) {
        if (memcmp((void *)address, "RSD PTR ", 8) == 0) {
            return (xsdp_t *)address;
        }
    }
    return 0;
}

mcfg_t * find_mcfg_via_xsdp(xsdp_t *xsdp) {
    xsdt_t *xsdt = (xsdt_t *)(uint32_t)xsdp->xsdt_address;
    int num_entries = (xsdt->header.length - sizeof(xsdt->header)) / 8;

    for (int i = 0; i < num_entries; i++) {
        sdt_header_t *header = (sdt_header_t *)(uint32_t)xsdt->sdts[i];
        if (memcmp(header->signature, "MCFG", 4) == 0) {
            return (mcfg_t *)header;
        }
    }
    return 0;
}

mcfg_t * find_mcfg_via_rsdp(rsdp_t *rsdp) {
    rsdt_t *rsdt = (rsdt_t *)rsdp->rsdt_address;
    int num_entries = (rsdt->header.length - sizeof(rsdt->header)) / 4;

    for (int i = 0; i < num_entries; i++) {
        sdt_header_t *header = (sdt_header_t *)rsdt->sdts[i];
        if (memcmp(header->signature, "MCFG", 4) == 0) {
            return (mcfg_t *)header;
        }
    }
    return 0;
}

uint32_t read_mmio_config(uint64_t base_address, uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint64_t address = base_address +
        (bus << 20) +
        (device << 15) +
        (function << 12) +
        offset;
    return *(volatile uint32_t *)(uint32_t)(address);
}

void write_mmio_config(uint64_t base_address, uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint32_t data) {
    uint64_t address = base_address +
        (bus << 20) +
        (device << 15) +
        (function << 12) +
        offset;
    *(volatile uint32_t *)(uint32_t)(address) = data;
}

uint32_t read_mmio(uint32_t mmio_base, uint32_t offset) {
    volatile uint32_t *mmio_address = (volatile uint32_t *)(mmio_base + offset);
    return *mmio_address;
}

void write_mmio(uint32_t mmio_base, uint32_t offset, uint32_t value) {
    volatile uint32_t *mmio_address = (volatile uint32_t *)(mmio_base + offset);
    *mmio_address = value;
}

void enumerate_pci_devices(uint64_t base_address, uint8_t start_bus, uint8_t end_bus) {
    for (uint8_t bus = start_bus; bus <= end_bus; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            for (uint8_t function = 0; function < 8; function++) {
                uint32_t vendor_device_id = read_mmio_config(base_address, bus, device, function, 0);
                if (vendor_device_id != 0xffffffff) {
                    uint16_t vendor_id = vendor_device_id;
                    uint16_t device_id = vendor_device_id >> 16;
                    vga_putw(vendor_id);
                    vga_putc(' ');
                    vga_putw(device_id);
                    vga_putc(' ');
                    vga_putb(bus);
                    vga_putc(' ');
                    vga_putb(device);
                    vga_putc(' ');
                    vga_putb(function);
                    vga_puts(" | ");
                }
            }
        }
    }
}

static uint32_t mmio_base;

typedef struct {
    uint64_t address;
    uint16_t length;
    uint16_t checksum;
    uint8_t status;
    uint8_t errors;
    uint8_t reserved[2];
} __attribute__((packed)) rx_descriptor_t;

__attribute__((aligned(0x10)))
static rx_descriptor_t rx_descriptors[RX_BUFFER_COUNT];

static uint8_t rx_buffers[RX_BUFFER_COUNT][RX_BUFFER_SIZE];

__attribute__((naked))
static void interrupt_service_routine(void) {
    uint32_t icr = read_mmio(mmio_base, R_ICR);
    vga_puts("82525EM interrupt -- ICR: ");
    vga_putdw(icr);
    vga_putc('\n');

    if (icr & ICR_RXDMT0) {
        vga_puts("RXDMT0: Receive Descriptor Minimum Threshold Reached\n");
    }

    if (icr & ICR_RXT0) {
        vga_puts("RXT0: Receiver Timer Interrupt\n");
    }

    outb(PIC1_COMMAND, PIC_EOI);
    __asm__("iret");
}

void pci_init(void) {
    xsdp_t *xsdp = find_xsdp();
    if (!xsdp) {
        vga_puts("Could not find XSDP");
        return;
    }
    vga_puts("Found XSDP at 0x");
    vga_putdw((uint32_t)xsdp);
    vga_putc('\n');

    vga_puts("XSDT is at 0x");
    vga_putdw((uint32_t)xsdp->xsdt_address);
    vga_putc('\n');

    mcfg_t *mcfg = find_mcfg_via_xsdp(xsdp);
    if (!mcfg) {
        vga_puts("Could not find MCFG");
        return;
    }
    vga_puts("Found MCFG at 0x");
    vga_putdw((uint32_t)mcfg);
    vga_putc('\n');

    int num_entries = (mcfg->header.length - sizeof(mcfg->header)) / sizeof(mcfg_entry_t);
    vga_puts("MCFG Entries: ");
    vga_putdw(num_entries);
    vga_putc('\n');

    for (int i = 0; i < num_entries; i++) {
        vga_putqw(mcfg->entries[i].base_address);
        vga_putc(' ');
        vga_putw(mcfg->entries[i].group);
        vga_putc(' ');
        vga_putb(mcfg->entries[i].start_bus);
        vga_putc(' ');
        vga_putb(mcfg->entries[i].end_bus);
        vga_putc('\n');
    }

    // for (int i = 0; i < num_entries; i++) {
    //     enumerate_pci_devices(mcfg->entries[i].base_address, mcfg->entries[i].start_bus, mcfg->entries[i].end_bus);
    // }

    // VMware Workstation NIC:
    // uint32_t vendor_device_id = read_mmio_config(mcfg->entries[0].base_address, 2, 0, 0, 0);
    uint32_t bar0 = read_mmio_config(mcfg->entries[0].base_address, 2, 0, 0, 0x10);
    vga_puts("bar0 = ");
    vga_putdw(bar0);
    vga_putc('\n');

    uint32_t bar1 = read_mmio_config(mcfg->entries[0].base_address, 2, 0, 0, 0x14);
    vga_puts("bar1 = ");
    vga_putdw(bar1);
    vga_putc('\n');

    // NOTE: If bit 0 in bar is 1 then I/O port should be used instead.
    // uint32_t mmio_base = bar0 & 0xfffffff0;
    mmio_base = bar0 & 0xfffffff0;
    vga_puts("mmio_base = ");
    vga_putdw(mmio_base);
    vga_putc('\n');

    uint32_t ctrl = read_mmio(mmio_base, R_CTRL);
    vga_puts("82525EM CTRL: ");
    vga_putdw(ctrl);
    vga_putc('\n');

    write_mmio(mmio_base, R_CTRL, ctrl | CTRL_SLU);
    for (volatile int i = 0; i < 100000000; i++);
    ctrl = read_mmio(mmio_base, R_CTRL);
    vga_puts("82525EM CTRL: ");
    vga_putdw(ctrl);
    vga_putc('\n');

    uint32_t status = read_mmio(mmio_base, R_STATUS);
    vga_puts("82525EM STATUS: ");
    vga_putdw(status);
    vga_putc('\n');

    uint32_t rctl = read_mmio(mmio_base, R_RCTL);
    vga_puts("82525EM RCTL: ");
    vga_putdw(rctl);
    vga_putc('\n');

    // Register ISR and unmask the interrupt in the PIC.
    idt_set_descriptor(IRQ11_INTERRUPT, interrupt_service_routine, 0x8e);
    pic_unmask_irq(0xb);

    // Enable all interrupts.
    write_mmio(mmio_base, R_IMS, 0xffffffff);

    // // Raise "link status change" interrupt.
    // write_mmio(mmio_base, R_ICS, 0x2);

    // Store receive descriptor base address.
    write_mmio(mmio_base, R_RDBAL, (uint32_t)rx_descriptors);
    write_mmio(mmio_base, R_RDBAH, 0);

    // Store receive descriptor buffer length.
    write_mmio(mmio_base, R_RDLEN, sizeof(rx_descriptors));

    // Set the receive descriptor head and tail.
    write_mmio(mmio_base, R_RDH, 0);
    write_mmio(mmio_base, R_RDT, RX_BUFFER_COUNT);

    // Store address of first buffer into first descriptor.
    *(uint32_t *)rx_descriptors = (uint32_t)rx_buffers;

    for (int i = 0; i < RX_BUFFER_COUNT; i++) {
        rx_descriptors[i].address = (uint32_t)(&rx_buffers[i]);
    }

    write_mmio(mmio_base, R_RCTL, rctl | RCTL_EN);
}
