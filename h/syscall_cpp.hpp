//
// Created by os on 8/28/24.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
#define PROJECT_BASE_V1_1_SYSCALL_CPP_HPP

#ifndef _syscall_cpp
#define _syscall_cpp
#include "syscall_c.h"

void* operator new (size_t);
void operator delete (void*)  noexcept;
void* operator new[](size_t);
void operator delete[](void*) noexcept;

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    static int sleep (time_t);

    static void runWrapper(void* thread);
protected:
    Thread ();
    virtual void run () {}
private:
    TCB* myHandle;
    void (*body)(void*);
    void* arg;
};



class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
    int timedWait (time_t);
    int tryWait();
private:
    Sem* myHandle;
};



class PeriodicThread : public Thread {
public:
    void terminate ();
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
private:
    time_t period;
};
class Console {
public:
    static char getc ();
    static void putc (char);
};
#endif


#endif //PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
