#ifndef SYSCALL_C_H
#define SYSCALL_C_H

#include "../lib/hw.h"
#include "../h/tcb.hpp"

void* mem_alloc (size_t size);

int mem_free (void*);

int thread_create (TCB** handle, void(*startFunction)(void*)  , void* arg);

int thread_exit ();

void thread_dispatch ();

#endif //SYSCALL_C_H
