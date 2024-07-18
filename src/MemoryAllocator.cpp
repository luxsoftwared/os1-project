
#include "../h/MemoryAllocator.hpp"



// [MemElem][actual data][potential padding]
// MemElem->size refers to total size of the block, including the MemElem header and the padding
//TODO change to something that doesnt lose this much mem to fragmentation, bcs currently i add at least one whole block for header which can be a lot smaller(header is max 3x64 but block can be 1024)



MemoryAllocator::MemElem* MemoryAllocator::full_head = nullptr;
MemoryAllocator::MemElem* MemoryAllocator::free_head  = nullptr;

//MemoryAllocator::MemElem* MemoryAllocator::free_head = nullptr;
/*
MemoryAllocator::MemElem* MemoryAllocator::full_head = nullptr;
MemoryAllocator::MemElem* MemoryAllocator::free_head  = (MemElem*)HEAP_START_ADDR;

MemoryAllocator::MemElem* MemoryAllocator::free_head ->next = nullptr;
MemoryAllocator::MemElem* MemoryAllocator::free_head ->prev = nullptr;
MemoryAllocator::MemElem* MemoryAllocator::free_head ->size = (char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR;
*/
//MemoryAllocator* MemoryAllocator::memoryAllocator = nullptr;

const size_t numOfBlocksForMemElem = (sizeof(MemoryAllocator::MemElem) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
/*
MemoryAllocator* MemoryAllocator::getInstance() {
    if( memoryAllocator==nullptr ) {
        full_head = nullptr;
        free_head = (MemElem*)HEAP_START_ADDR;
        free_head->next = nullptr;
        free_head->prev = nullptr;
        free_head->size = (char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR;
        memoryAllocator = (MemoryAllocator*) mem_alloc(( sizeof(MemoryAllocator)+MEM_BLOCK_SIZE-1  )/MEM_BLOCK_SIZE);
    }

    return memoryAllocator;
}
*/
void* MemoryAllocator::mem_alloc(size_t sizeInBlocks) {
    if(sizeInBlocks <= 0 || sizeInBlocks >= ( (char*)HEAP_END_ADDR-(char*)HEAP_START_ADDR )/MEM_BLOCK_SIZE) return nullptr;
    size_t numOfBlocksNeeded = sizeInBlocks + numOfBlocksForMemElem;//space needed is at least sizeInBlocks+numOfBlocks(MemElem)

    MemElem* newBlock = nullptr;
    for(MemElem* iter = free_head;iter;iter=iter->next){
        if( (iter->size)/MEM_BLOCK_SIZE >= numOfBlocksNeeded ) {
            //sufficient free space is found
            //adding new FreeMem fragment
            if(sizeof(MemElem) <= iter->size - numOfBlocksNeeded * MEM_BLOCK_SIZE){
                //if there is enough space left at the end, make a new Free space block, and chain it in the free list
                MemElem* newFreeFragment = (MemElem*) (((char*)iter ) + numOfBlocksNeeded * MEM_BLOCK_SIZE ) ;
                newFreeFragment->next = iter->next;
                newFreeFragment->prev = iter->prev;
                newFreeFragment->size = iter->size - numOfBlocksNeeded * MEM_BLOCK_SIZE;
                if(iter->prev)  iter->prev->next = newFreeFragment;
                else    free_head = newFreeFragment;
                if(iter->next)  iter->next->prev = newFreeFragment;
            }else{
                //no new free block, keep FreeMem list consistent
                if(iter->next) iter->next->prev = iter->prev;
                if(iter->prev) iter->prev->next = iter->next;
                else free_head = iter->next;
            }

            // adding new FullMem fragment
            newBlock = iter;
            newBlock->size = numOfBlocksNeeded * MEM_BLOCK_SIZE;

            //searching for the place to insert the new block
            //  so blocks stay sorted by address
            iter = full_head;
            if(iter==nullptr){
                full_head = newBlock;
                newBlock->next = nullptr;
                newBlock->prev = nullptr;
            }else{
                for(;iter->next && iter->next < newBlock; iter=iter->next);
                newBlock->next = iter->next;
                newBlock->prev = iter;
                if(iter->next) iter->next->prev = newBlock;
                iter->next = newBlock;
            }
            /* adding segment to beginning of the list - OLD
            newBlock->next = full_head;
            newBlock->prev = nullptr;
            if(full_head) full_head->prev = newBlock;
            full_head = newBlock;*/

            return (void*)( (char*)newBlock + sizeof(MemElem)); //address right after the MemElem header
        }
    }
    return nullptr;
}

int MemoryAllocator::mem_free(void* addr){
    if(addr==nullptr) return -1;

    MemElem* block = (MemElem*)addr - 1;
    //remove from full list
    if(block->prev) block->prev->next = block->next;
    else full_head = block->next;
    if(block->next) block->next->prev = block->prev;

    //add to free list
    MemElem* iter = free_head;
    if(iter==nullptr){
        free_head = block;
        block->next = nullptr;
        block->prev = nullptr;
        return 0;
    }
    for(;iter->next && iter->next < block; iter=iter->next);
    if(iter->next){ //better visibility like this
        block->next = iter->next;
        block->prev = iter;
        iter->next->prev = block;
        iter->next = block;
    }else{
        block->next = nullptr;
        block->prev = iter;
        iter->next = block;
    }

    //try to join with adjacent blocks
    /*
    MemElem* newFreeBlock = joinBlocks(block, block->next);
    if(newFreeBlock) joinBlocks(newFreeBlock->prev, newFreeBlock);
    else joinBlocks(block->prev, block);*/
    joinBlocks(block,block->next); //if successful block will still point to correct location
    joinBlocks(block->prev,block);


    return 0;
}

void* MemoryAllocator::joinBlocks(MemElem* block1, MemElem* block2){
    if(block1==nullptr || block2==nullptr) return nullptr;
    if( (char*)block1 + block1->size == (char*)block2 ){
        //join block2 to block1
        block1->size += block2->size;
        //remove block2 from free list
        if(block2->prev) block2->prev->next = block2->next;
        else free_head = block2->next;
        if(block2->next) block2->next->prev = block2->prev;
        return block1;
    }else if( (char*)block2 + block2->size == (char*)block1 ){
        //join block1 to block2
        block2->size += block1->size;
        //remove block1 from free list
        if(block1->prev) block1->prev->next = block1->next;
        else free_head = block1->next;
        if(block1->next) block1->next->prev = block1->prev;
        return block2;
    }
    return nullptr;
}