#pragma once

#include <stddef.h>
#include <stdint.h>

#define PAGE_SIZE 4096
#define HEAP_START 0x400000  // Start at 4MB

typedef struct {
    uint64_t total_memory;
    uint64_t used_memory;
    uint64_t free_memory;
} memory_info_t;

void memory_init(uint64_t mem_size);
void* kmalloc(size_t size);
void kfree(void* ptr);
void memory_get_info(memory_info_t* info);