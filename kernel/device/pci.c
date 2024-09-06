#include <device/pci.h>
#include <console.h>
#include <lib/string.h>
#include <device/i82545em.h>

struct rsdp {
    char signature[8];
    u8 checksum;
    char oemid[6];
    u8 revision;
    u32 rsdt_address;
} __attribute__((packed));

struct xsdp {
    char signature[8];
    u8 checksum;
    char oemid[6];
    u8 revision;
    u32 rsdt_address;
    u32 length;
    u64 xsdt_address;
    u8 extended_checksum;
    u8 reserved[3];
} __attribute__((packed));

struct sdt_header {
    char signature[4];
    u32 length;
    u8 revision;
    u8 checksum;
    char oemid[6];
    char oem_table_id[8];
    s32 oem_revision;
    u32 creator_id;
    u32 creator_revision;
} __attribute__((packed));

struct rsdt {
    struct sdt_header header;
    u32 sdts[];
} __attribute__((packed));

struct xsdt {
    struct sdt_header header;
    u64 sdts[];
} __attribute__((packed));

struct mcfg_entry {
    u64 base_address;
    u16 group;
    u8 start_bus;
    u8 end_bus;
    u8 reserved[4];
} __attribute__((packed));

struct mcfg {
    struct sdt_header header;
    u8 reserved[8];
    struct mcfg_entry entries[];
} __attribute__((packed));

struct rsdp * find_rsdp(void) {
    for (u32 address = 0x000e0000; address < 0x00100000; address += 16) {
        if (memcmp((void *)address, "RSD PTR ", 8) == 0) {
            return (struct rsdp *)address;
        }
    }
    return 0;
}

struct xsdp * find_xsdp(void) {
    for (u32 address = 0x000e0000; address < 0x00100000; address += 16) {
        if (memcmp((void *)address, "RSD PTR ", 8) == 0) {
            return (struct xsdp *)address;
        }
    }
    return 0;
}

struct mcfg * find_mcfg_via_xsdp(struct xsdp *xsdp) {
    struct xsdt *xsdt = (struct xsdt *)(u32)xsdp->xsdt_address;
    int num_entries = (xsdt->header.length - sizeof(xsdt->header)) / 8;

    for (int i = 0; i < num_entries; i++) {
        struct sdt_header *header = (struct sdt_header *)(u32)xsdt->sdts[i];
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

u32 read_mmio_config(u64 base_address, u8 bus, u8 device, u8 function, u8 offset) {
    u64 address = base_address +
        (bus << 20) +
        (device << 15) +
        (function << 12) +
        offset;
    return *(volatile u32 *)(u32)(address);
}

void write_mmio_config(u64 base_address, u8 bus, u8 device, u8 function, u8 offset, u32 data) {
    u64 address = base_address +
        (bus << 20) +
        (device << 15) +
        (function << 12) +
        offset;
    *(volatile u32 *)(u32)(address) = data;
}

void enumerate_pci_devices(u64 base_address, u8 start_bus, u8 end_bus) {
    for (u8 bus = start_bus; bus <= end_bus; bus++) {
        for (u8 device = 0; device < 32; device++) {
            for (u8 function = 0; function < 8; function++) {
                u32 vendor_device_id = read_mmio_config(base_address, bus, device, function, 0);
                if (vendor_device_id != 0xffffffff) {
                    u16 vendor_id = vendor_device_id;
                    u16 device_id = vendor_device_id >> 16;
                    console_dbg_putw(vendor_id);
                    console_dbg_putc(' ');
                    console_dbg_putw(device_id);
                    console_dbg_putc(' ');
                    console_dbg_putb(bus);
                    console_dbg_putc(' ');
                    console_dbg_putb(device);
                    console_dbg_putc(' ');
                    console_dbg_putb(function);
                    console_dbg_puts(" | ");
                }
            }
        }
    }
}

void pci_init(void) {
    struct xsdp *xsdp = find_xsdp();
    if (!xsdp) {
        console_dbg_puts("Could not find XSDP");
        return;
    }
    console_dbg_puts("Found XSDP at 0x");
    console_dbg_putdw((u32)xsdp);
    console_dbg_putc('\n');

    console_dbg_puts("XSDT is at 0x");
    console_dbg_putdw((u32)xsdp->xsdt_address);
    console_dbg_putc('\n');

    struct mcfg *mcfg = find_mcfg_via_xsdp(xsdp);
    if (!mcfg) {
        console_dbg_puts("Could not find MCFG");
        return;
    }
    console_dbg_puts("Found MCFG at 0x");
    console_dbg_putdw((u32)mcfg);
    console_dbg_putc('\n');

    int num_entries = (mcfg->header.length - sizeof(mcfg->header)) / sizeof(struct mcfg_entry);
    console_dbg_puts("MCFG Entries: ");
    console_dbg_putdw(num_entries);
    console_dbg_putc('\n');

    for (int i = 0; i < num_entries; i++) {
        console_dbg_putqw(mcfg->entries[i].base_address);
        console_dbg_putc(' ');
        console_dbg_putw(mcfg->entries[i].group);
        console_dbg_putc(' ');
        console_dbg_putb(mcfg->entries[i].start_bus);
        console_dbg_putc(' ');
        console_dbg_putb(mcfg->entries[i].end_bus);
        console_dbg_putc('\n');
    }

    // for (int i = 0; i < num_entries; i++) {
    //     enumerate_pci_devices(mcfg->entries[i].base_address, mcfg->entries[i].start_bus, mcfg->entries[i].end_bus);
    // }

    // Hard-coded VMware Workstation NIC initialization:
    // u32 vendor_device_id = read_mmio_config(mcfg->entries[0].base_address, 2, 0, 0, 0);
    u32 i82545em_bar0 = read_mmio_config(mcfg->entries[0].base_address, 2, 0, 0, 0x10);
    i82545em_init(i82545em_bar0);
}
