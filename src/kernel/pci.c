#include "pci.h"
#include "vga.h"
#include "string.h"
#include "i82545em.h"

struct rsdp {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdt_address;
} __attribute__((packed));

struct xsdp {
    char signature[8];
    uint8_t checksum;
    char oemid[6];
    uint8_t revision;
    uint32_t rsdt_address;
    uint32_t length;
    uint64_t xsdt_address;
    uint8_t extended_checksum;
    uint8_t reserved[3];
} __attribute__((packed));

struct sdt_header {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemid[6];
    char oem_table_id[8];
    int32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed));

struct rsdt {
    struct sdt_header header;
    uint32_t sdts[];
} __attribute__((packed));

struct xsdt {
    struct sdt_header header;
    uint64_t sdts[];
} __attribute__((packed));

struct mcfg_entry {
    uint64_t base_address;
    uint16_t group;
    uint8_t start_bus;
    uint8_t end_bus;
    uint8_t reserved[4];
} __attribute__((packed));

struct mcfg {
    struct sdt_header header;
    uint8_t reserved[8];
    struct mcfg_entry entries[];
} __attribute__((packed));

struct rsdp * find_rsdp(void) {
    for (uint32_t address = 0x000e0000; address < 0x00100000; address += 16) {
        if (memcmp((void *)address, "RSD PTR ", 8) == 0) {
            return (struct rsdp *)address;
        }
    }
    return 0;
}

struct xsdp * find_xsdp(void) {
    for (uint32_t address = 0x000e0000; address < 0x00100000; address += 16) {
        if (memcmp((void *)address, "RSD PTR ", 8) == 0) {
            return (struct xsdp *)address;
        }
    }
    return 0;
}

struct mcfg * find_mcfg_via_xsdp(struct xsdp *xsdp) {
    struct xsdt *xsdt = (struct xsdt *)(uint32_t)xsdp->xsdt_address;
    int num_entries = (xsdt->header.length - sizeof(xsdt->header)) / 8;

    for (int i = 0; i < num_entries; i++) {
        struct sdt_header *header = (struct sdt_header *)(uint32_t)xsdt->sdts[i];
        if (memcmp(header->signature, "MCFG", 4) == 0) {
            return (struct mcfg *)header;
        }
    }
    return 0;
}

struct mcfg * find_mcfg_via_rsdp(struct rsdp *rsdp) {
    struct rsdt *rsdt = (struct rsdt *)rsdp->rsdt_address;
    int num_entries = (rsdt->header.length - sizeof(rsdt->header)) / 4;

    for (int i = 0; i < num_entries; i++) {
        struct sdt_header *header = (struct sdt_header *)rsdt->sdts[i];
        if (memcmp(header->signature, "MCFG", 4) == 0) {
            return (struct mcfg *)header;
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

void pci_init(void) {
    struct xsdp *xsdp = find_xsdp();
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

    struct mcfg *mcfg = find_mcfg_via_xsdp(xsdp);
    if (!mcfg) {
        vga_puts("Could not find MCFG");
        return;
    }
    vga_puts("Found MCFG at 0x");
    vga_putdw((uint32_t)mcfg);
    vga_putc('\n');

    int num_entries = (mcfg->header.length - sizeof(mcfg->header)) / sizeof(struct mcfg_entry);
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

    // Hard-coded VMware Workstation NIC initialization:
    // uint32_t vendor_device_id = read_mmio_config(mcfg->entries[0].base_address, 2, 0, 0, 0);
    uint32_t i82545em_bar0 = read_mmio_config(mcfg->entries[0].base_address, 2, 0, 0, 0x10);
    i82545em_init(i82545em_bar0);
}
