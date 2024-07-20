//
// Created by luksu on 18-Jul-24.
//


#include "../h/syscall_c.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"


void FreeAllocatedMemory(void* ptr) {
    //size_t size = 1024; // 1KB
    //void* ptr = mem_alloc(size);
    int ret = mem_free(ptr);
    if (ret != 0) {
        printString("FreeAllocatedMemory failed.\n");
    }else{
        printString("FreeAllocatedMemory Passed.\n");
    }
}

void AllocateMemory() {
    size_t size = 1024; // 1KB
    void* ptr = mem_alloc(size);
    if (ptr == nullptr) {
        printString("AllocateMemory failed.\n");
        return;
    }else{
        printAddress(ptr);
        printString("AllocateMemory Passed.\n");

    }
    FreeAllocatedMemory(ptr);
}

void allocation_fails_with_zero_size() {
    void* ptr = mem_alloc(0);
    if (ptr != nullptr) {
        printString("allocation_fails_with_zero_size failed.\n");
        mem_free(ptr); // Cleanup in case allocation somehow succeeds
    }
}

void freeing_null_pointer_returns_error() {
    if (mem_free(nullptr) == 0) {
        printString("freeing_null_pointer_returns_error failed.\n");
    }
}


void allocation_returns_different_pointers_for_subsequent_calls() {
    size_t size = 1024; // 1KB
    void* ptr1 = mem_alloc(size);
    void* ptr2 = mem_alloc(size);
    if (ptr1 == nullptr || ptr2 == nullptr || ptr1 == ptr2) {
        printString("allocation_returns_different_pointers_for_subsequent_calls failed.\n");
    }
    mem_free(ptr1); // Cleanup
    mem_free(ptr2); // Cleanup
}

int mainT() {
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    AllocateMemory();
    allocation_fails_with_zero_size();
    freeing_null_pointer_returns_error();
    allocation_returns_different_pointers_for_subsequent_calls();

    printString("All tests executed.\n");

    return 0;
}
