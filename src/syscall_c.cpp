//
// Created by luksu on 18-Jul-24.
//

#include "../h/syscall_c.hpp"
#include "../lib/hw.h"

void* mem_alloc (size_t size){
    size_t sizeInBlocks = (size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;

    __asm__ volatile ("mv a1, %0" : : "r"(sizeInBlocks));
    __asm__ volatile ("li a0, 0x01");
    __asm__ volatile ("ecall");

    void* volatile returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

int mem_free (void* addr){
    __asm__ volatile ("mv a1, %0" : : "r"(addr));
    __asm__ volatile ("li a0, 0x02");
    __asm__ volatile ("ecall");

    int volatile returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}
