// NOTE: This memory manager implementation is suuuper rough!

#include <mm.h>
#include <types.h>
#include <process.h>

#define MAX_PAGE_ALLOCS 4096 // Uses 16 MB of memory
#define PAGE_ALLOCATOR_BASE_ADDRESS 0x100000

#define MAX_CHUNK_ALLOCS 256 // Uses 16 MB of memory
#define CHUNK_ALLOCATOR_BASE_ADDRESS (PAGE_ALLOCATOR_BASE_ADDRESS + (MAX_PAGE_ALLOCS * PAGE_SIZE))

#define FREE -1

static int page_allocs[MAX_PAGE_ALLOCS];
static int chunk_allocs[MAX_CHUNK_ALLOCS];

void mm_init(void) {
    for (int i = 0; i < MAX_PAGE_ALLOCS; i++) {
        page_allocs[i] = FREE;
    }
    for (int i = 0; i < MAX_CHUNK_ALLOCS; i++) {
        chunk_allocs[i] = FREE;
    }
}

void *mm_alloc_page(void) {
    int pid = process_get_current_pid();
    return mm_alloc_page_for_pid(pid);
}

void mm_free_page(void *page) {
    int pid = process_get_current_pid();
    mm_free_page_for_pid(pid, page);
}

void *mm_alloc_chunk(void) {
    int pid = process_get_current_pid();
    return mm_alloc_chunk_for_pid(pid);
}

void mm_free_chunk(void *chunk) {
    int pid = process_get_current_pid();
    mm_free_chunk_for_pid(pid, chunk);
}

void *mm_alloc_page_for_pid(int pid) {
    int page_index = 0;
    while (page_index < MAX_PAGE_ALLOCS && page_allocs[page_index] != FREE) {
        page_index++;
    }
    if (page_index == MAX_PAGE_ALLOCS) {
        return NULL;
    }
    page_allocs[page_index] = pid;
    return (void *)(PAGE_ALLOCATOR_BASE_ADDRESS + (PAGE_SIZE * page_index));
}

void mm_free_page_for_pid(int pid, void *page) {
    int page_index = ((u32)page - PAGE_ALLOCATOR_BASE_ADDRESS) / PAGE_SIZE;
    if (page_index < 0 || page_index >= MAX_PAGE_ALLOCS) {
        return;
    }
    if (page_allocs[page_index] != pid) {
        return;
    }
    page_allocs[page_index] = FREE;
}

void *mm_alloc_chunk_for_pid(int pid) {
    int chunk_index = 0;
    while (chunk_index < MAX_CHUNK_ALLOCS && chunk_allocs[chunk_index] != FREE) {
        chunk_index++;
    }
    if (chunk_index == MAX_CHUNK_ALLOCS) {
        return NULL;
    }
    chunk_allocs[chunk_index] = pid;
    return (void *)(CHUNK_ALLOCATOR_BASE_ADDRESS + (CHUNK_SIZE * chunk_index));
}

void mm_free_chunk_for_pid(int pid, void *chunk) {
    int chunk_index = ((u32)chunk - CHUNK_ALLOCATOR_BASE_ADDRESS) / CHUNK_SIZE;
    if (chunk_index < 0 || chunk_index >= MAX_CHUNK_ALLOCS) {
        return;
    }
    if (chunk_allocs[chunk_index] != pid) {
        return;
    }
    chunk_allocs[chunk_index] = FREE;
}

void mm_free_all_for_pid(int pid) {
    for (int i = 0; i < MAX_PAGE_ALLOCS; i++) {
        if (page_allocs[i] == pid) {
            page_allocs[i] = FREE;
        }
    }
    for (int i = 0; i < MAX_CHUNK_ALLOCS; i++) {
        if (chunk_allocs[i] == pid) {
            chunk_allocs[i] = FREE;
        }
    }
}

void mm_get_stats(struct mm_stats *stats) {
    stats->page_size = PAGE_SIZE;
    stats->chunk_size = CHUNK_SIZE;
    stats->num_pages_allocated = 0;
    stats->num_chunks_allocated = 0;

    for (int i = 0; i < MAX_PAGE_ALLOCS; i++) {
        if (page_allocs[i] != FREE) {
            stats->num_pages_allocated++;
        }
    }

    for (int i = 0; i < MAX_CHUNK_ALLOCS; i++) {
        if (chunk_allocs[i] != FREE) {
            stats->num_chunks_allocated++;
        }
    }
}
