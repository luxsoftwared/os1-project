#ifndef SYSCALL_C_H
#define SYSCALL_C_H

#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/Sem.h"

/** size in blocks
 * */
void* mem_alloc (size_t size);

int mem_free (void*);

void thread_join(TCB** handle);

int thread_create (TCB** handle, void(*startFunction)(void*)  , void* arg);

int thread_create_without_starting(TCB** handle, void(*startFunction)(void*)  , void* arg);

int thread_start (TCB* handle);

int thread_exit ();

void thread_dispatch ();

int sem_open (Sem** handle, uint64 init);

int sem_close (Sem* handle);

int sem_wait (Sem* handle);

int sem_signal (Sem* handle);

int sem_trywait (Sem* handle);

int sem_timedwait (Sem* handle, time_t timeout);

int time_sleep (time_t time);

char getc();
void putc(char c);

#endif //SYSCALL_C_H
