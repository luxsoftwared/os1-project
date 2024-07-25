#ifndef SYSCALL_C_H
#define SYSCALL_C_H

#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/Sem.h"

void* mem_alloc (size_t size);

int mem_free (void*);

int thread_create (TCB** handle, void(*startFunction)(void*)  , void* arg);

int thread_exit ();

void thread_dispatch ();

int sem_open (Sem** handle, uint64 init);

int sem_close (Sem* handle);

int sem_wait (Sem* handle);

int sem_signal (Sem* handle);


char getc();
void putc(char c);

#endif //SYSCALL_C_H
