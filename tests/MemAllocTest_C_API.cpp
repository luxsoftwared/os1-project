//
// Created by luksu on 18-Jul-24.
//
#include "../h/syscall_c.hpp"
#include "../h/print.hpp"

void allocates_memory_successfully() {
    size_t size = 1024; // 1KB
    void* ptr = mem_alloc(size);
    if (ptr == nullptr) {
        printString("allocates_memory_successfully failed.\n");
        return;
    }
    mem_free(ptr); // Cleanup
}

void frees_allocated_memory_successfully() {
    size_t size = 1024; // 1KB
    void* ptr = mem_alloc(size);
    if (ptr == nullptr || mem_free(ptr) != 0) {
        printString("frees_allocated_memory_successfully failed.\n");
    }
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

void allocates_minimum_block_size_even_for_smaller_size() {
    size_t size = 1; // Request smaller than MEM_BLOCK_SIZE
    void* ptr = mem_alloc(size);
    if (ptr == nullptr) {
        printString("allocates_minimum_block_size_even_for_smaller_size failed.\n");
        return;
    }
    mem_free(ptr); // Cleanup
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

int main() {
    allocates_memory_successfully();
    frees_allocated_memory_successfully();
    allocation_fails_with_zero_size();
    freeing_null_pointer_returns_error();
    allocates_minimum_block_size_even_for_smaller_size();
    allocation_returns_different_pointers_for_subsequent_calls();

    printString("All tests executed.\n");
    return 0;
}