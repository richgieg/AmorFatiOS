// NOTE: This memory manager implementation is suuuper rough!

#include <mm.h>
#include <types.h>
#include <process.h>

#define ALLOC_SIZE_2M (2 * 1024 * 1024) // 2 MB
#define MAX_2M_ALLOCS 8 // 16 MB
#define BASE_ADDRESS_2M 0x100000
#define LIMIT_2M (BASE_ADDRESS_2M + (ALLOC_SIZE_2M * MAX_2M_ALLOCS) - 1)

#define ALLOC_SIZE_256K (256 * 1024) // 256 KB
#define MAX_256K_ALLOCS 64 // 16 MB
#define BASE_ADDRESS_256K (LIMIT_2M + 1)
#define LIMIT_256K (BASE_ADDRESS_256K + (ALLOC_SIZE_256K * MAX_256K_ALLOCS) - 1)

#define ALLOC_SIZE_32K (32 * 1024) // 32 KB
#define MAX_32K_ALLOCS 512 // 16 MB
#define BASE_ADDRESS_32K (LIMIT_256K + 1)
#define LIMIT_32K (BASE_ADDRESS_32K + (ALLOC_SIZE_32K * MAX_32K_ALLOCS) - 1)

#define ALLOC_SIZE_4K (4 * 1024) // 4 KB
#define MAX_4K_ALLOCS 4096 // 16 MB
#define BASE_ADDRESS_4K (LIMIT_32K + 1)
#define LIMIT_4K (BASE_ADDRESS_4K + (ALLOC_SIZE_4K * MAX_4K_ALLOCS) - 1)

#define ALLOC_SIZE_512B 512
#define MAX_512B_ALLOCS 4096 // 2 MB
#define BASE_ADDRESS_512B (LIMIT_4K + 1)
#define LIMIT_512B (BASE_ADDRESS_512B + (ALLOC_SIZE_512B * MAX_512B_ALLOCS) - 1)

#define ALLOC_SIZE_64B 64
#define MAX_64B_ALLOCS 4096 // 256 KB
#define BASE_ADDRESS_64B (LIMIT_512B + 1)
#define LIMIT_64B (BASE_ADDRESS_64B + (ALLOC_SIZE_64B * MAX_64B_ALLOCS) - 1)

#define ALLOC_SIZE_8B 8
#define MAX_8B_ALLOCS 4096 // 32 KB
#define BASE_ADDRESS_8B (LIMIT_64B + 1)
#define LIMIT_8B (BASE_ADDRESS_8B + (ALLOC_SIZE_8B * MAX_8B_ALLOCS) - 1)

#define FREE -1

static int allocs_8b[MAX_8B_ALLOCS];
static int allocs_64b[MAX_64B_ALLOCS];
static int allocs_512b[MAX_512B_ALLOCS];
static int allocs_4k[MAX_4K_ALLOCS];
static int allocs_32k[MAX_32K_ALLOCS];
static int allocs_256k[MAX_256K_ALLOCS];
static int allocs_2m[MAX_2M_ALLOCS];

void mm_init(void) {
    for (int i = 0; i < MAX_8B_ALLOCS; i++) {
        allocs_8b[i] = FREE;
    }
    for (int i = 0; i < MAX_64B_ALLOCS; i++) {
        allocs_64b[i] = FREE;
    }
    for (int i = 0; i < MAX_512B_ALLOCS; i++) {
        allocs_512b[i] = FREE;
    }
    for (int i = 0; i < MAX_4K_ALLOCS; i++) {
        allocs_4k[i] = FREE;
    }
    for (int i = 0; i < MAX_32K_ALLOCS; i++) {
        allocs_32k[i] = FREE;
    }
    for (int i = 0; i < MAX_256K_ALLOCS; i++) {
        allocs_256k[i] = FREE;
    }
    for (int i = 0; i < MAX_2M_ALLOCS; i++) {
        allocs_2m[i] = FREE;
    }
}

void *mm_alloc(size_t size) {
    int pid = process_get_current_pid();
    return mm_alloc_for_pid(pid, size);
}

void mm_free(void *ptr) {
    int pid = process_get_current_pid();
    mm_free_for_pid(pid, ptr);
}

static void *alloc_8b_for_pid(int pid) {
    int index = 0;
    while (index < MAX_8B_ALLOCS && allocs_8b[index] != FREE) {
        index++;
    }
    if (index == MAX_8B_ALLOCS) {
        return NULL;
    }
    allocs_8b[index] = pid;
    return (void *)(BASE_ADDRESS_8B + (ALLOC_SIZE_8B * index));
}

static void *alloc_64b_for_pid(int pid) {
    int index = 0;
    while (index < MAX_64B_ALLOCS && allocs_64b[index] != FREE) {
        index++;
    }
    if (index == MAX_64B_ALLOCS) {
        return NULL;
    }
    allocs_64b[index] = pid;
    return (void *)(BASE_ADDRESS_64B + (ALLOC_SIZE_64B * index));
}

static void *alloc_512b_for_pid(int pid) {
    int index = 0;
    while (index < MAX_512B_ALLOCS && allocs_512b[index] != FREE) {
        index++;
    }
    if (index == MAX_512B_ALLOCS) {
        return NULL;
    }
    allocs_512b[index] = pid;
    return (void *)(BASE_ADDRESS_512B + (ALLOC_SIZE_512B * index));
}

static void *alloc_4k_for_pid(int pid) {
    int index = 0;
    while (index < MAX_4K_ALLOCS && allocs_4k[index] != FREE) {
        index++;
    }
    if (index == MAX_4K_ALLOCS) {
        return NULL;
    }
    allocs_4k[index] = pid;
    return (void *)(BASE_ADDRESS_4K + (ALLOC_SIZE_4K * index));
}

static void *alloc_32k_for_pid(int pid) {
    int index = 0;
    while (index < MAX_32K_ALLOCS && allocs_32k[index] != FREE) {
        index++;
    }
    if (index == MAX_32K_ALLOCS) {
        return NULL;
    }
    allocs_32k[index] = pid;
    return (void *)(BASE_ADDRESS_32K + (ALLOC_SIZE_32K * index));
}

static void *alloc_256k_for_pid(int pid) {
    int index = 0;
    while (index < MAX_256K_ALLOCS && allocs_256k[index] != FREE) {
        index++;
    }
    if (index == MAX_256K_ALLOCS) {
        return NULL;
    }
    allocs_256k[index] = pid;
    return (void *)(BASE_ADDRESS_256K + (ALLOC_SIZE_256K * index));
}

static void *alloc_2m_for_pid(int pid) {
    int index = 0;
    while (index < MAX_2M_ALLOCS && allocs_2m[index] != FREE) {
        index++;
    }
    if (index == MAX_2M_ALLOCS) {
        return NULL;
    }
    allocs_2m[index] = pid;
    return (void *)(BASE_ADDRESS_2M + (ALLOC_SIZE_2M * index));
}

void *mm_alloc_for_pid(int pid, size_t size) {
    if (size <= ALLOC_SIZE_8B) {
        return alloc_8b_for_pid(pid);
    }
    if (size <= ALLOC_SIZE_64B) {
        return alloc_64b_for_pid(pid);
    }
    if (size <= ALLOC_SIZE_512B) {
        return alloc_512b_for_pid(pid);
    }
    if (size <= ALLOC_SIZE_4K) {
        return alloc_4k_for_pid(pid);
    }
    if (size <= ALLOC_SIZE_32K) {
        return alloc_32k_for_pid(pid);
    }
    if (size <= ALLOC_SIZE_256K) {
        return alloc_256k_for_pid(pid);
    }
    if (size <= ALLOC_SIZE_2M) {
        return alloc_2m_for_pid(pid);
    }
    return NULL;
}

static void free_8b_for_pid(int pid, void *ptr) {
    int index = ((u32)ptr - BASE_ADDRESS_8B) / ALLOC_SIZE_8B;
    if (allocs_8b[index] != pid) {
        return;
    }
    allocs_8b[index] = FREE;
}

static void free_64b_for_pid(int pid, void *ptr) {
    int index = ((u32)ptr - BASE_ADDRESS_64B) / ALLOC_SIZE_64B;
    if (allocs_64b[index] != pid) {
        return;
    }
    allocs_64b[index] = FREE;
}

static void free_512b_for_pid(int pid, void *ptr) {
    int index = ((u32)ptr - BASE_ADDRESS_512B) / ALLOC_SIZE_512B;
    if (allocs_512b[index] != pid) {
        return;
    }
    allocs_512b[index] = FREE;
}

static void free_4k_for_pid(int pid, void *ptr) {
    int index = ((u32)ptr - BASE_ADDRESS_4K) / ALLOC_SIZE_4K;
    if (allocs_4k[index] != pid) {
        return;
    }
    allocs_4k[index] = FREE;
}

static void free_32k_for_pid(int pid, void *ptr) {
    int index = ((u32)ptr - BASE_ADDRESS_32K) / ALLOC_SIZE_32K;
    if (allocs_32k[index] != pid) {
        return;
    }
    allocs_32k[index] = FREE;
}

static void free_256k_for_pid(int pid, void *ptr) {
    int index = ((u32)ptr - BASE_ADDRESS_256K) / ALLOC_SIZE_256K;
    if (allocs_256k[index] != pid) {
        return;
    }
    allocs_256k[index] = FREE;
}

static void free_2m_for_pid(int pid, void *ptr) {
    int index = ((u32)ptr - BASE_ADDRESS_2M) / ALLOC_SIZE_2M;
    if (allocs_2m[index] != pid) {
        return;
    }
    allocs_2m[index] = FREE;
}

void mm_free_for_pid(int pid, void *ptr) {
    if ((u32)ptr >= BASE_ADDRESS_8B && (u32)ptr <= LIMIT_8B) {
        free_8b_for_pid(pid, ptr);
    }
    if ((u32)ptr >= BASE_ADDRESS_64B && (u32)ptr <= LIMIT_64B) {
        free_64b_for_pid(pid, ptr);
    }
    if ((u32)ptr >= BASE_ADDRESS_512B && (u32)ptr <= LIMIT_512B) {
        free_512b_for_pid(pid, ptr);
    }
    if ((u32)ptr >= BASE_ADDRESS_4K && (u32)ptr <= LIMIT_4K) {
        free_4k_for_pid(pid, ptr);
    }
    if ((u32)ptr >= BASE_ADDRESS_32K && (u32)ptr <= LIMIT_32K) {
        free_32k_for_pid(pid, ptr);
    }
    if ((u32)ptr >= BASE_ADDRESS_256K && (u32)ptr <= LIMIT_256K) {
        free_256k_for_pid(pid, ptr);
    }
    if ((u32)ptr >= BASE_ADDRESS_2M && (u32)ptr <= LIMIT_2M) {
        free_2m_for_pid(pid, ptr);
    }
}

void mm_free_all_for_pid(int pid) {
    for (int i = 0; i < MAX_8B_ALLOCS; i++) {
        if (allocs_8b[i] == pid) {
            allocs_8b[i] = FREE;
        }
    }
    for (int i = 0; i < MAX_64B_ALLOCS; i++) {
        if (allocs_64b[i] == pid) {
            allocs_64b[i] = FREE;
        }
    }
    for (int i = 0; i < MAX_512B_ALLOCS; i++) {
        if (allocs_512b[i] == pid) {
            allocs_512b[i] = FREE;
        }
    }
    for (int i = 0; i < MAX_4K_ALLOCS; i++) {
        if (allocs_4k[i] == pid) {
            allocs_4k[i] = FREE;
        }
    }
    for (int i = 0; i < MAX_32K_ALLOCS; i++) {
        if (allocs_32k[i] == pid) {
            allocs_32k[i] = FREE;
        }
    }
    for (int i = 0; i < MAX_256K_ALLOCS; i++) {
        if (allocs_256k[i] == pid) {
            allocs_256k[i] = FREE;
        }
    }
    for (int i = 0; i < MAX_2M_ALLOCS; i++) {
        if (allocs_2m[i] == pid) {
            allocs_2m[i] = FREE;
        }
    }
}

void mm_get_stats(struct mm_stats *stats) {
    stats->num_8b_allocs = 0;
    stats->num_64b_allocs = 0;
    stats->num_512b_allocs = 0;
    stats->num_4k_allocs = 0;
    stats->num_32k_allocs = 0;
    stats->num_256k_allocs = 0;
    stats->num_2m_allocs = 0;

    for (int i = 0; i < MAX_8B_ALLOCS; i++) {
        if (allocs_8b[i] != FREE) {
            stats->num_8b_allocs++;
        }
    }

    for (int i = 0; i < MAX_64B_ALLOCS; i++) {
        if (allocs_64b[i] != FREE) {
            stats->num_64b_allocs++;
        }
    }

    for (int i = 0; i < MAX_512B_ALLOCS; i++) {
        if (allocs_512b[i] != FREE) {
            stats->num_512b_allocs++;
        }
    }

    for (int i = 0; i < MAX_4K_ALLOCS; i++) {
        if (allocs_4k[i] != FREE) {
            stats->num_4k_allocs++;
        }
    }

    for (int i = 0; i < MAX_32K_ALLOCS; i++) {
        if (allocs_32k[i] != FREE) {
            stats->num_32k_allocs++;
        }
    }

    for (int i = 0; i < MAX_256K_ALLOCS; i++) {
        if (allocs_256k[i] != FREE) {
            stats->num_256k_allocs++;
        }
    }

    for (int i = 0; i < MAX_2M_ALLOCS; i++) {
        if (allocs_2m[i] != FREE) {
            stats->num_2m_allocs++;
        }
    }
}
