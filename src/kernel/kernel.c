#include "init_idt.h"

char *const screen = (char *)0xb8000;
const char greeting[] = "AmorFatiOS v0.0.1";

void _start(void) {
    for (int i = 0; i < sizeof(greeting); i++) {
        screen[i * 2] = greeting[i];
    }
    init_idt();

    __asm__ volatile ("int $0");
    __asm__ volatile ("int $1");
    __asm__ volatile ("int $2");
    __asm__ volatile ("int $3");
    __asm__ volatile ("int $4");
    __asm__ volatile ("int $5");
    __asm__ volatile ("int $6");
    __asm__ volatile ("int $7");
    __asm__ volatile ("int $8");
    __asm__ volatile ("int $9");
    __asm__ volatile ("int $10");
    __asm__ volatile ("int $11");
    __asm__ volatile ("int $12");
    __asm__ volatile ("int $13");
    __asm__ volatile ("int $14");
    __asm__ volatile ("int $15");
    __asm__ volatile ("int $16");
    __asm__ volatile ("int $17");
    __asm__ volatile ("int $18");
    __asm__ volatile ("int $19");
    __asm__ volatile ("int $20");
    __asm__ volatile ("int $21");
    __asm__ volatile ("int $22");
    __asm__ volatile ("int $23");
    __asm__ volatile ("int $24");
    __asm__ volatile ("int $25");
    __asm__ volatile ("int $26");
    __asm__ volatile ("int $27");
    __asm__ volatile ("int $28");
    __asm__ volatile ("int $29");
    __asm__ volatile ("int $30");
    __asm__ volatile ("int $31");

    char c = 0;
    while (1) {
        screen[140] = c;
        c++;
    }
}
