#include "pci.h"
#include "vga.h"
#include "string.h"

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

sdt_header_t * find_mcfg(xsdp_t *xsdp) {
    xsdt_t *xsdt = (xsdt_t *)(uint32_t)xsdp->xsdt_address;
    int num_entries = xsdt->header.length - sizeof(xsdt->header) / 8;

    for (int i = 0; i < num_entries; i++) {
        sdt_header_t *header = (sdt_header_t *)(uint32_t)xsdt->sdts[i];
        if (memcmp(header->signature, "MCFG", 4) == 0) {
            return header;
        }
    }
    return 0;
}

sdt_header_t * find_mcfg2(rsdp_t *rsdp) {
    rsdt_t *rsdt = (rsdt_t *)rsdp->rsdt_address;
    int num_entries = rsdt->header.length - sizeof(rsdt->header) / 4;

    for (int i = 0; i < num_entries; i++) {
        sdt_header_t *header = (sdt_header_t *)rsdt->sdts[i];
        if (memcmp(header->signature, "MCFG", 4) == 0) {
            return header;
        }
    }
    return 0;
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

    sdt_header_t *mcfg = find_mcfg(xsdp);
    if (!mcfg) {
        vga_puts("Could not find MCFG");
        return;
    }
    vga_puts("Found MCFG at 0x");
    vga_putdw((uint32_t)mcfg);
}
