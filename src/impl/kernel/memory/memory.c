#include "memory.h"

#define BLOCK_SIZE 16
#define MAX_BLOCKS 1024
#define HEAP_START 0x400000 // 4MB

typedef struct block
{
    size_t size;
    int is_free;
    struct block *next;
} block_t;

block_t *heap_start = NULL;
static memory_info_t mem_info = {0};
static char *heap_current = (char *)HEAP_START;

void memory_init(uint64_t mem_size)
{
    mem_info.total_memory = mem_size;
    mem_info.used_memory = 0;
    mem_info.free_memory = mem_size;

    // Reset heap position
    heap_current = (char *)HEAP_START;
    heap_start = NULL;
}

static block_t *find_free_block(block_t **last, size_t size)
{
    block_t *current = heap_start;
    *last = NULL;

    while (current)
    {
        if (current->is_free && current->size >= size)
        {
            return current;
        }
        *last = current;
        current = current->next;
    }
    return NULL;
}

static block_t *request_space(block_t *last, size_t size)
{
    if (mem_info.free_memory < (size + sizeof(block_t)))
    {
        return NULL; // Out of memory
    }

    block_t *block;

    if (last == NULL)
    {
        // First block
        block = (block_t *)heap_current;
        heap_current += sizeof(block_t) + size;
    }
    else
    {
        // Calculate the end of the last block
        char *last_end = (char *)last + sizeof(block_t) + last->size;
        block = (block_t *)last_end;

        // Update heap_current if needed
        char *block_end = (char *)block + sizeof(block_t) + size;
        if (block_end > heap_current)
        {
            heap_current = block_end;
        }
    }

    // Check if block address is within valid memory range
    if ((uint64_t)block >= (HEAP_START + mem_info.total_memory))
    {
        return NULL;
    }

    // Check if we have enough space for this allocation
    if ((char *)block + sizeof(block_t) + size > (char *)(HEAP_START + mem_info.total_memory))
    {
        return NULL; // Not enough memory
    }

    block->size = size;
    block->is_free = 0;
    block->next = NULL;

    if (last)
    {
        last->next = block;
    }

    mem_info.used_memory += size + sizeof(block_t);
    mem_info.free_memory -= size + sizeof(block_t);

    return block;
}

void *kmalloc(size_t size)
{
    if (size == 0)
        return NULL;

    // Align size to BLOCK_SIZE
    size = (size + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);

    // Check if allocation is too large
    if (size > mem_info.free_memory)
    {
        return NULL;
    }

    block_t *block = NULL;
    block_t *last = heap_start;

    // Check if heap_start is NULL instead of accesing heap_start->size
    if (heap_start == NULL)
    { // First allocation
        block = request_space(NULL, size);
        if (!block)
            return NULL;
        heap_start = block;
    }
    else
    {
        block = find_free_block(&last, size);
        if (!block)
        { // Failed to find free block
            block = request_space(last, size);
            if (!block)
                return NULL;
        }
        else
        { // Found free block
            block->is_free = 0;

            if (block->size >= size + sizeof(block_t) + BLOCK_SIZE)
            {
                // Split block
                block_t *new_block = (block_t *)((char *)block + sizeof(block_t) + size);

                new_block->size = block->size - size - sizeof(block_t);
                new_block->is_free = 1;
                new_block->next = block->next;

                block->size = size;
                block->next = new_block;

                mem_info.used_memory += sizeof(block_t);
                mem_info.free_memory -= sizeof(block_t);
            }
        }
    }

    return (char *)block + sizeof(block_t);
}

void kfree(void *ptr)
{
    if (!ptr)
        return;

    // Check if ptr is within valid memory range
    if ((uint64_t)ptr < (HEAP_START + sizeof(block_t)) ||
        (uint64_t)ptr >= (HEAP_START + mem_info.total_memory))
    {
        return;
    }

    block_t *block = (block_t *)((char *)ptr - sizeof(block_t));

    // Validate block header (basic sanity checks)
    if (block->size == 0 || block->size > mem_info.total_memory)
    {
        return; // Invalid block
    }

    if (block->is_free)
    {
        return; // Double free detected
    }

    block->is_free = 1;

    mem_info.used_memory -= block->size + sizeof(block_t);
    mem_info.free_memory += block->size + sizeof(block_t);

    // Merge adjacent free blocks
    block_t *current = heap_start;
    while (current && current->next)
    {
        if (current->is_free && current->next->is_free)
        {
            // Check if blocks are actually adjacent
            char *current_end = (char *)current + sizeof(block_t) + current->size;
            if (current_end == (char *)current->next)
            {
                // Merge blocks
                current->size += current->next->size + sizeof(block_t);
                current->next = current->next->next;

                // Update memory info
                mem_info.used_memory -= sizeof(block_t);
                mem_info.free_memory += sizeof(block_t);
                continue; // Check this block again
            }
        }
        current = current->next;
    }
}

void memory_get_info(memory_info_t *info)
{
    if (info)
    {
        *info = mem_info;
    }
}