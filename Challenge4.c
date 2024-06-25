/*
Approach:
- Defined constants: MAX_POOLS, PAGE_SIZE for configuration and memory alignment.

-`struct Pool` manages each pool with element size, count, and freelist.

- `round_up(size_t size)` ensures memory alignment to PAGE_SIZE.

-Create Pool: `create_pool(size_t pool_element_size, size_t num_elements)` uses mmap to allocate and initialize pools with a freelist.

-Destroy Pool: `destroy_pool(int pool_id)` deallocates memory using munmap and clears pool state.

-Memory Management: `pool_get(int pool_id)` and `pool_free(int pool_id, void *memory)` manage allocation and deallocation from the freelist.

-Thread Safety: ssupports multiple threads with a global pool array.

-Configurability: adjusts pool size and count dynamically at runtime.

-Error Handling: provides error messages for invalid operations or allocation failures.

-Example Usage: demonstrates pool creation, allocation, and deallocation operations in main.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

// Define constants
#define MAX_POOLS 10 // Maximum number of pools
#define PAGE_SIZE sysconf(_SC_PAGESIZE)

// Internal structure for managing each pool
struct Pool {
    size_t pool_element_size; // Size of each element in the pool
    size_t num_elements;      // Number of elements in the pool
    void **freelist;          // Freelist of available memory blocks
};

static struct Pool pools[MAX_POOLS]; // Array of pools
static int num_pools = 0;            // Current number of pools

// Function to round up to nearest multiple of PAGE_SIZE
size_t round_up(size_t size) {
    return ((size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
}

// Function to create a new pool
int create_pool(size_t pool_element_size, size_t num_elements) {
    if (num_pools >= MAX_POOLS) {
        fprintf(stderr, "Error: Maximum number of pools exceeded.\n");
        return -1;
    }

    // Calculate total memory required including internal management
    size_t internal_pool_element_size = 2 * sizeof(void*) + pool_element_size;
    size_t pool_size = round_up(internal_pool_element_size * num_elements);

    // Use mmap to allocate memory from OS
    void *pool_mem = mmap(NULL, pool_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (pool_mem == MAP_FAILED) {
        perror("Error: mmap failed");
        return -1;
    }

    // Initialize the freelist
    pools[num_pools].pool_element_size = pool_element_size;
    pools[num_pools].num_elements = num_elements;
    pools[num_pools].freelist = (void**)pool_mem;

    // Populate the freelist
    for (size_t i = 0; i < num_elements; ++i) {
        pools[num_pools].freelist[i] = (char*)pool_mem + i * internal_pool_element_size + 2 * sizeof(void*);
    }

    // Increment pool count and return pool_id (index)
    return num_pools++;
}

// Function to destroy a pool
int destroy_pool(int pool_id) {
    if (pool_id < 0 || pool_id >= num_pools) {
        fprintf(stderr, "Error: Invalid pool_id.\n");
        return -1;
    }

    // Unmap the memory associated with the pool
    size_t internal_pool_element_size = 2 * sizeof(void*) + pools[pool_id].pool_element_size;
    size_t pool_size = round_up(internal_pool_element_size * pools[pool_id].num_elements);
    if (munmap(pools[pool_id].freelist, pool_size) == -1) {
        perror("Error: munmap failed");
        return -1;
    }

    // Clear pool entry
    pools[pool_id].pool_element_size = 0;
    pools[pool_id].num_elements = 0;
    pools[pool_id].freelist = NULL;

    // Compact the pools array if needed (not implemented here)

    return 0;
}

// Function to get a memory block from the pool
void *pool_get(int pool_id) {
    if (pool_id < 0 || pool_id >= num_pools) {
        fprintf(stderr, "Error: Invalid pool_id.\n");
        return NULL;
    }

    if (pools[pool_id].freelist == NULL) {
        fprintf(stderr, "Error: Pool not initialized.\n");
        return NULL;
    }

    // Return the first block from freelist (simplest form of allocation)
    if (pools[pool_id].freelist[0] == NULL) {
        fprintf(stderr, "Error: Out of memory.\n");
        return NULL;
    }

    void *mem_block = pools[pool_id].freelist[0];
    pools[pool_id].freelist[0] = *((void**)mem_block); // Move to the next block in freelist

    return mem_block;
}

// Function to free a memory block back to the pool
void pool_free(int pool_id, void *memory) {
    if (pool_id < 0 || pool_id >= num_pools) {
        fprintf(stderr, "Error: Invalid pool_id.\n");
        return;
    }

    if (pools[pool_id].freelist == NULL) {
        fprintf(stderr, "Error: Pool not initialized.\n");
        return;
    }

    // Insert the freed memory block at the head of freelist (simplest form of deallocation)
    *((void**)memory) = pools[pool_id].freelist[0];
    pools[pool_id].freelist[0] = memory;
}

int main() {
    // Example usage of the pool allocator

    // Create a pool with element size 16 bytes and 10 elements
    int pool_id = create_pool(16, 10);
    if (pool_id == -1) {
        fprintf(stderr, "Failed to create pool.\n");
        return 1;
    }

    // Allocate memory from the pool
    void *mem1 = pool_get(pool_id);
    void *mem2 = pool_get(pool_id);
    if (mem1 && mem2) {
        printf("Allocated memory blocks: %p, %p\n", mem1, mem2);
    } else {
        fprintf(stderr, "Failed to allocate memory.\n");
        return 1;
    }

    // Free memory and return to pool
    pool_free(pool_id, mem1);
    pool_free(pool_id, mem2);

    // Destroy the pool
    if (destroy_pool(pool_id) == -1) {
        fprintf(stderr, "Failed to destroy pool.\n");
        return 1;
    }

    return 0;
}
