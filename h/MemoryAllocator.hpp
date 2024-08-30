//
// Created by os on 7/17/24.
//

#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"
#include "../h/print.hpp"

class MemoryAllocator {
public:
    /** @size number of bytes that this memory block occupies, including the header
     */
    struct MemElem{
        MemElem* next;
        MemElem* prev;
        size_t size;
    };



private:
    friend class MemoryAllocatorTest;
    static MemoryAllocator* memoryAllocator;
    static MemElem* free_head;
    static MemElem* full_head;
    MemoryAllocator(){/*
        free_head ->next = nullptr;
        free_head->prev = nullptr;
        free_head->size = (char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR;*/
        printString("MemoryAllocator created\n");
    };
    /**
     * Joins two adjacent free blocks if possible
     * @param block1
     * @param block2
     * @return void* to the address of the newly joined block, or nullptr in case of failure
     */
    static void * joinBlocks(MemElem* block1, MemElem* block2);



public:
    MemoryAllocator(MemoryAllocator &) = delete;
    void operator=(const MemoryAllocator &) = delete;
    //MemoryAllocator( MemoryAllocator const&)= delete;
    //static MemoryAllocator *getInstance();

    /**
     * Initializes the free memory block that spans from HEAP_START_ADDR to HEAP_END_ADDR\n
     * This function should and must be called only once, before any memory allocation
     */
    static void initFreeMem(){
        free_head = (MemElem*)HEAP_START_ADDR;
        free_head->next = nullptr;
        free_head->prev = nullptr;
        free_head->size = (char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR;
    }

    /**
     *  Allocates a memory block of (sizeInBlocks + sizeOfHeaderInBlocks) MemBlocks, instantiates a MemElem header at the beginning of the block\n
     *
     * @param sizeInBlocks in MemBlocks
     * @return void* to the address of the newly allocated space, address RIGHT AFTER MemElem header, or nullptr in case of failure
     */
    static void* mem_alloc(size_t sizeInBlocks);

    /**
     * Frees up the memory block that starts at addr (allocated with mem_alloc - addr is address after MemElem header of that block)\n
     * If addr doesnt point to memory block allocated using mem_alloc, error code -1
     * @param addr
     * @return 0 for success; negative in case of an error
     */
    static int mem_free(void* addr);

    using size_t = decltype(sizeof(0));

    static size_t bytesInBlocks(size_t n);
};



#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
