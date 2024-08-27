#ifndef MM_H
#define MM_H

void mm_init(void);
void mm_show_mmap(void);

void mm_show_mdump(void);
void mm_mdump_next_line(void);
void mm_mdump_prev_line(void);
void mm_mdump_next_page(void);
void mm_mdump_prev_page(void);
void mm_mdump_next_mb(void);
void mm_mdump_prev_mb(void);

#endif
