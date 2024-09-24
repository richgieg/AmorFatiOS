#ifndef MM_H
#define MM_H

#include <types.h>

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

void mm_init(void);
void *mm_alloc(size_t size);
void mm_free(void *ptr);
void *mm_alloc_for_pid(int pid, size_t size);
void mm_free_for_pid(int pid, void *ptr);
void mm_free_all_for_pid(int pid);
void mm_get_stats(struct mm_stats *stats);

#endif
