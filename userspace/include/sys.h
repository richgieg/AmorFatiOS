#ifndef SYS_H
#define SYS_H

#include <console.h>

#define PAGE_SIZE 4096
#define CHUNK_SIZE (PAGE_SIZE * 16)

struct mm_stats {
    int num_8b_allocs;
    int num_64b_allocs;
    int num_512b_allocs;
    int num_4k_allocs;
    int num_32k_allocs;
    int num_256k_allocs;
    int num_2m_allocs;
};

int sys_process_create(void (*start)());
int sys_process_create_ex(void (*start)(), bool is_killable);
void sys_process_exit(void);
void sys_process_wait_for_exit(int pid);
int sys_process_get_console_index(void);
void sys_process_get_child_pids(int pid, int *buf, size_t buf_size, int *count);

void *sys_mm_alloc(size_t size);
void sys_mm_free(void *ptr);
void sys_mm_get_stats(struct mm_stats *stats);

void sys_console_clear(void);
enum vga_color sys_console_get_bg_color(void);
void sys_console_set_bg_color(enum vga_color bg_color);
enum vga_color sys_console_get_text_color(void);
void sys_console_set_text_color(enum vga_color text_color);
u8 sys_console_get_column(void);
u8 sys_console_get_row(void);
void sys_console_set_pos(u8 col, u8 row);
void sys_console_writec(char c);
void sys_console_putc(char c);
void sys_console_puts(const char *str);
void sys_console_putb(u8);
void sys_console_putw(u16 w);
void sys_console_putdw(u32 dw);
void sys_console_putqw(u64 qw);
void sys_console_putp(void *p);
void sys_console_putc_at(char c, u8 col, u8 row);
void sys_console_puts_at(const char *str, u8 col, u8 row);
void sys_console_putb_at(u8 b, u8 col, u8 row);
void sys_console_putw_at(u16 w, u8 col, u8 row);
void sys_console_putdw_at(u32 dw, u8 col, u8 row);
void sys_console_putqw_at(u64 qw, u8 col, u8 row);
void sys_console_putp_at(void *p, u8 col, u8 row);
void sys_console_read_key_event(struct key_event *ke);
int sys_console_get_num_columns(void);
int sys_console_get_num_rows(void);

void sys_net_subscribe(void);
void sys_net_unsubscribe(void);
ssize_t sys_net_read_frame(u8 *buf, size_t buf_size);

#endif
