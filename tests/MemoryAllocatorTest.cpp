#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATORTEST_CPP
#define PROJECT_BASE_V1_1_MEMORYALLOCATORTEST_CPP

#include "../h/MemoryAllocator.hpp"
#include "../h/print.hpp"

class MemoryAllocatorTest {
public:
    static const size_t numOfBlocksForMemElem = (sizeof(MemoryAllocator::MemElem) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;

    static void allocateAndFreeMemory() {
        void* block1 = MemoryAllocator::mem_alloc(10);
        if (block1 == nullptr) {
            printString("allocateAndFreeMemory: Allocation failed\n");
            return;
        }
        if (MemoryAllocator::mem_free(block1) != 0) {
            printString("allocateAndFreeMemory: Freeing failed\n");
            return;
        }
        printString("allocateAndFreeMemory: Passed\n");
    }

    static void allocateTooLargeBlock() {
        //MemoryAllocator* allocator = MemoryAllocator::getInstance();
        void* block = MemoryAllocator::mem_alloc((char*)HEAP_END_ADDR-(char*)HEAP_START_ADDR+1);
        if (block != nullptr) {
            printString("allocateTooLargeBlock: Allocation should have failed\n");
            return;
        }
        printString("allocateTooLargeBlock: Passed\n");
    }

    static void freeNullPointer() {
        //MemoryAllocator* allocator = MemoryAllocator::getInstance();
        if (MemoryAllocator::mem_free(nullptr) != -1) {
            printString("freeNullPointer: Freeing null pointer should have failed\n");
            return;
        }
        printString("freeNullPointer: Passed\n");
    }

    static void printMemoryLists(){
        printString("FreeMem:\n");
        for(MemoryAllocator::MemElem* iter = MemoryAllocator::free_head;iter;iter=iter->next){
            printInteger((uint64)(iter));
            printString(" ");
            printInteger((uint64)(iter->size));
            printString("\n");
        }
        printString("FUllMem:\n");
        for(MemoryAllocator::MemElem* iter = MemoryAllocator::full_head;iter;iter=iter->next){
            printInteger((uint64)(iter));
            printString(" ");
            printInteger((uint64)(iter->size));
            printString("\n");
        }
    }

    static void allocateExactSizeBlock() {
        //MemoryAllocator* allocator = MemoryAllocator::getInstance();
        size_t size = MemoryAllocator::free_head->size / MEM_BLOCK_SIZE - numOfBlocksForMemElem;
        void* targetFreeSegment = MemoryAllocator::free_head+1; // !!! test failed bcs free_head points to header, and mem_alloc returns adress after header
        printMemoryLists();
        void* block = MemoryAllocator::mem_alloc(size);
        printMemoryLists();
        if (block == nullptr) {
            printString("allocateExactSizeBlock: Allocation failed\n");
            return;
        }
        if(targetFreeSegment != block){
            printString("free:");
            printInteger((uint64)(targetFreeSegment));
            printString("block:");
            printInteger((uint64)(block));
            printString("allocateExactSizeBlock: Block not allocated with intended free segment of the same size\n");

            return;
        }
        if (MemoryAllocator::mem_free(block) != 0) {
            printString("allocateExactSizeBlock: Freeing failed\n");
            return;
        }
        printString("allocateExactSizeBlock: Passed\n");
    }

    static void allocateAndFreeMultipleBlocks() {
        //MemoryAllocator* allocator = MemoryAllocator::getInstance();
        void* block1 = MemoryAllocator::mem_alloc(5);
        void* block2 = MemoryAllocator::mem_alloc(10);
        if (block1 == nullptr || block2 == nullptr) {
            printString("allocateAndFreeMultipleBlocks: Allocation failed\n");
            return;
        }
        if (MemoryAllocator::mem_free(block1) != 0 || MemoryAllocator::mem_free(block2) != 0) {
            printString("allocateAndFreeMultipleBlocks: Freeing failed\n");
            return;
        }
        printString("allocateAndFreeMultipleBlocks: Passed\n");
    }

    static void coalesceFreeBlocks() {
        //MemoryAllocator* allocator = MemoryAllocator::getInstance();
        void* block1 = MemoryAllocator::mem_alloc(5);
        void* block2 = MemoryAllocator::mem_alloc(5);
        if (block1 == nullptr || block2 == nullptr) {
            printString("coalesceFreeBlocks: Allocation failed\n");
            return;
        }
        if (MemoryAllocator::mem_free(block1) != 0 || MemoryAllocator::mem_free(block2) != 0) {
            printString("coalesceFreeBlocks: Freeing failed\n");
            return;
        }
        if (MemoryAllocator::free_head->size < 10 * MEM_BLOCK_SIZE) {
            printString("coalesceFreeBlocks: Coalescing failed\n");
            return;
        }
        printString("coalesceFreeBlocks: Passed\n");
    }

    static void allocateZeroSize() {
        //MemoryAllocator* allocator = MemoryAllocator::getInstance();
        void* block = MemoryAllocator::mem_alloc(0);
        if (block != nullptr) {
            printString("allocateZeroSize: Allocation should have failed\n");
            return;
        }
        printString("allocateZeroSize: Passed\n");
    }

    static void allocateNegativeSize() {
        //MemoryAllocator* allocator = MemoryAllocator::getInstance();
        void* block = MemoryAllocator::mem_alloc(-1);
        printInteger((uint64)block);
        if (block != nullptr) {
            printString("allocateNegativeSize: Allocation should have failed\n");
            return;
        }
        printString("allocateNegativeSize: Passed\n");
    }

    static void runAll() {

        allocateAndFreeMemory();
        allocateTooLargeBlock();
        freeNullPointer();
        allocateExactSizeBlock();
        allocateAndFreeMultipleBlocks();
        coalesceFreeBlocks();
        allocateZeroSize();
        allocateNegativeSize();
    }
};
#endif //PROJECT_BASE_V1_1_MEMORYALLOCATORTEST_CPP
