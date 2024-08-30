//
// Created by luksu on 18-Jul-24.
//

#include "../h/syscall_c.h"
#include "../lib/hw.h"

#include "../lib/console.h"


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



/**
 *
 * @param handle - pointer to TCB* variable where thread handle will be stored
 * @param startFunction - body of the function
 * @param arg - function argument
 * @return 0 for correct execution,\n
 * -1 for unsuccesfull TCB::createThread call,\n
 * -2 for forwarded handle is nullptr,\n
 * -3 for error in creating stack
 */
int thread_create (TCB** handle, void(*startFunction)(void*)  , void* arg)
{
    void* stack = nullptr;
    if(startFunction){
        stack = MemoryAllocator::mem_alloc(MemoryAllocator::bytesInBlocks(DEFAULT_STACK_SIZE*sizeof(uint64) ) );    //new uint64[DEFAULT_STACK_SIZE];
        if(stack == nullptr){
            return -3;
        }
    }
    __asm__ volatile ("mv t1, %0" : : "r"(handle));
    __asm__ volatile ("mv t2, %0" : : "r"(startFunction));
    __asm__ volatile ("mv t3, %0" : : "r"(arg));
    __asm__ volatile ("mv t4, %0" : : "r"(stack));

    __asm__ volatile ("mv a1, t1");
    __asm__ volatile ("mv a2, t2");
    __asm__ volatile ("mv a3, t3");
    __asm__ volatile ("mv a4, t4");
    __asm__ volatile ("li a0, 0x11");
    __asm__ volatile ("ecall");

    int volatile returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

int thread_create_without_starting(TCB** handle, void(*startFunction)(void*)  , void* arg){
    void* stack = nullptr;
    if(startFunction){
        stack = MemoryAllocator:: mem_alloc(MemoryAllocator::bytesInBlocks(DEFAULT_STACK_SIZE*sizeof(uint64)) );     //new uint64[DEFAULT_STACK_SIZE];
        if(stack == nullptr){
            return -3;
        }
    }
    __asm__ volatile ("mv t1, %0" : : "r"(handle));
    __asm__ volatile ("mv t2, %0" : : "r"(startFunction));
    __asm__ volatile ("mv t3, %0" : : "r"(arg));
    __asm__ volatile ("mv t4, %0" : : "r"(stack));

    __asm__ volatile ("mv a1, t1");
    __asm__ volatile ("mv a2, t2");
    __asm__ volatile ("mv a3, t3");
    __asm__ volatile ("mv a4, t4");
    __asm__ volatile ("li a0, 0x14");
    __asm__ volatile ("ecall");

    int volatile returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

int thread_start (TCB* handle){

    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("li a0, 0x15");
    __asm__ volatile ("ecall");

    int volatile returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}


// what should it do with the thread? Delete it? currently only stops its further execution, but all its resources are still allocated
int thread_exit (){
    __asm__ volatile ("li a0, 0x12");
    __asm__ volatile ("ecall");

    int volatile returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

void thread_dispatch (){
    __asm__ volatile ("li a0, 0x13");
    __asm__ volatile ("ecall");
}


int sem_open (Sem** handle, uint64 init){
    __asm__ volatile ("mv t1, %0" : : "r"(handle));
    __asm__ volatile ("mv t2, %0" : : "r"(init));

    __asm__ volatile ("mv a2, t2");
    __asm__ volatile ("mv a1, t1");
    __asm__ volatile ("li a0, 0x21");
    __asm__ volatile ("ecall");

    int volatile returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;

}

int sem_close (Sem* handle){
    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("li a0, 0x22");
    __asm__ volatile ("ecall");

    int volatile returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

int sem_wait (Sem* handle){
    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("li a0, 0x23");
    __asm__ volatile ("ecall");

    int volatile returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

int sem_signal (Sem* handle){
    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("li a0, 0x24");
    __asm__ volatile ("ecall");

    int volatile returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}
// Da li sam ovo namestio
int sem_timedwait (Sem* handle, time_t timeout){
    __asm__ volatile ("mv t1, %0" : : "r"(handle));
    __asm__ volatile ("mv t2, %0" : : "r"(timeout));

    __asm__ volatile ("mv a2, t2");
    __asm__ volatile ("mv a1, t1");
    __asm__ volatile ("li a0, 0x25");
    __asm__ volatile ("ecall");

    int volatile returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

int sem_trywait (Sem* handle){
    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("li a0, 0x26");
    __asm__ volatile ("ecall");

    int volatile returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

int time_sleep (time_t time){
    __asm__ volatile ("mv a1, %0" : : "r"(time));
    __asm__ volatile ("li a0, 0x31");
    __asm__ volatile ("ecall");

    int volatile returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

char getc(){
    __asm__ volatile ("li a0, 0x41");
    __asm__ volatile ("ecall");

    int volatile returnValue;
    __asm__ volatile ("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

void putc(char c){
    __asm__ volatile ("mv a1, %0" : : "r"(c));
    __asm__ volatile ("li a0, 0x42");
    __asm__ volatile ("ecall");
}


