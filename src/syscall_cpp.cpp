
#include "../h/syscall_c.h"
#include "../h/syscall_cpp.hpp"
//#include "../h/MemoryAllocator.hpp"

Thread::Thread (void (*body)(void*), void* arg){
    if( thread_create(& (myHandle), body, arg) == 0){
        this->body = body;
        this->arg = arg;
    }
    else{
        myHandle = nullptr;
    }
}

Thread::~Thread (){
    //thread_exit(); NE TREBA, ovo radi exit iz trenutne niti, a ja brisem iz zpoljne

    //mem_free(myHandle);
}

void Thread::runWrapper(void* thread){
    if(thread!=nullptr)
        ((Thread*)thread)->run();
}

Thread::Thread (){
    thread_create_without_starting(& (myHandle), runWrapper, this);
    this->body = runWrapper;
    this->arg = this;
}

int Thread::start (){
    return thread_start(myHandle);
}

void Thread::dispatch (){
    thread_dispatch();
}

int Thread::sleep (time_t time){
    return -1;
   // return thread_sleep(time); TREBA ZA 4 TEK**************************
}



int Semaphore::wait (){
    return sem_wait(myHandle);
}

int Semaphore::signal (){
    return sem_signal(myHandle);
}

int Semaphore::timedWait (time_t time){
    return -1;
    //sem_timedwait(&myHandle, time); TREBA ZA 4 TEK*****************
}

int Semaphore::tryWait (){
    return sem_trywait(myHandle);
}

Semaphore::Semaphore (unsigned init){
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore (){
    sem_close(myHandle);
}


void Console::putc (char c){
    SYSCALL_C_H::putc(c);
}

char Console::getc (){
    return SYSCALL_C_H::getc();
}

void PeriodicThread::terminate (){
    //thread_exit(); TREBA ZA 4 TEK*****************
}

PeriodicThread::PeriodicThread (time_t period){
    //thread_create(&myHandle, periodicActivation, period); TREBA ZA 4 TEK*****************
}


#include "../h/MemoryAllocator.hpp"

using size_t = decltype(sizeof(0));

size_t bytesInBlocks(size_t n)
{
    return (n + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
}

void *operator new(size_t n)
{
    //printString("before new operator\n");
    return mem_alloc(n);
}

void *operator new[](size_t n)
{
    return mem_alloc(n);
}

void operator delete(void *p) noexcept
{
    //printString("before delete operator\n");
    mem_free(p);
    //printString("after delete operator\n");
}

void operator delete[](void *p) noexcept
{
    //printString("before delete[] operator[]\n");
    mem_free(p);
    //printString("after delete[] operator\n");
}