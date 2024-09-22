#ifndef MM_H
#define MM_H

#include <types.h>

#define PAGE_SIZE 4096
#define CHUNK_SIZE (PAGE_SIZE * 16)

struct mm_stats {
    size_t page_size;
    size_t chunk_size;
    int num_pages_allocated;
    int num_chunks_allocated;
};

void mm_init(void);
void *mm_alloc_page(void);
void mm_free_page(void *page);
void *mm_alloc_chunk(void);
void mm_free_chunk(void *chunk);

void *mm_alloc_page_for_pid(int pid);
void mm_free_page_for_pid(int pid, void *page);
void *mm_alloc_chunk_for_pid(int pid);
void mm_free_chunk_for_pid(int pid, void *chunk);
void mm_free_all_for_pid(int pid);

void mm_get_stats(struct mm_stats *stats);

#endif
