void idt_init(void);
void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags);
