// NOTE: This memory manager implementation is suuuper rough!

#include <mm.h>
#include <types.h>

#define MAX_PAGE_ALLOCS 4096 // Uses 16 MB of memory
#define PAGE_ALLOCATOR_BASE_ADDRESS 0x100000

#define MAX_CHUNK_ALLOCS 256 // Uses 16 MB of memory
#define CHUNK_ALLOCATOR_BASE_ADDRESS (PAGE_ALLOCATOR_BASE_ADDRESS + (MAX_PAGE_ALLOCS * PAGE_SIZE))

static bool page_allocs[MAX_PAGE_ALLOCS];
static bool chunk_allocs[MAX_CHUNK_ALLOCS];

static int num_pages_allocated;
static int num_chunks_allocated;

void mm_init(void) {

}

void *mm_alloc_page(void) {
    int page_index = 0;
    while (page_index < MAX_PAGE_ALLOCS && page_allocs[page_index]) {
        page_index++;
    }
    if (page_index == MAX_PAGE_ALLOCS) {
        return NULL;
    }
    page_allocs[page_index] = true;
    num_pages_allocated++;
    return (void *)(PAGE_ALLOCATOR_BASE_ADDRESS + (PAGE_SIZE * page_index));
}

void mm_free_page(void *page) {
    int page_index = ((u32)page - PAGE_ALLOCATOR_BASE_ADDRESS) / PAGE_SIZE;
    if (page_index < 0 || page_index >= MAX_PAGE_ALLOCS) {
        return;
    }
    if (!page_allocs[page_index]) {
        return;
    }
    page_allocs[page_index] = false;
    num_pages_allocated--;
}

void *mm_alloc_chunk(void) {
    int chunk_index = 0;
    while (chunk_index < MAX_CHUNK_ALLOCS && chunk_allocs[chunk_index]) {
        chunk_index++;
    }
    if (chunk_index == MAX_CHUNK_ALLOCS) {
        return NULL;
    }
    chunk_allocs[chunk_index] = true;
    num_chunks_allocated++;
    return (void *)(CHUNK_ALLOCATOR_BASE_ADDRESS + (CHUNK_SIZE * chunk_index));
}

void mm_free_chunk(void *chunk) {
    int chunk_index = ((u32)chunk - CHUNK_ALLOCATOR_BASE_ADDRESS) / CHUNK_SIZE;
    if (chunk_index < 0 || chunk_index >= MAX_CHUNK_ALLOCS) {
        return;
    }
    if (!chunk_allocs[chunk_index]) {
        return;
    }
    chunk_allocs[chunk_index] = false;
    num_chunks_allocated--;
}

void mm_get_stats(struct mm_stats *stats) {
    stats->page_size = PAGE_SIZE;
    stats->chunk_size = CHUNK_SIZE;
    stats->num_pages_allocated = num_pages_allocated;
    stats->num_chunks_allocated = num_chunks_allocated;
}
