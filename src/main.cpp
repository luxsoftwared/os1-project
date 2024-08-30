//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"

#include "../tests/MemoryAllocatorTest.cpp"
#include "../tests/testFunctions.h"
#include "../h/syscall_c.h"
#include "../h/syscall_cpp.hpp"
#include "../tests/SemaphoreTest.h"

#ifndef DEBUG_LVL
#define DEBUG_LVL 0
#endif

// should only be called from main, once on the beginning of the program
int initMainThread(){
    TCB* mainThread=nullptr;
    int createMainThread = thread_create(&mainThread,nullptr,nullptr);
    if(0!= createMainThread || mainThread==nullptr) {
        printString("Error creatring main thread");
        return -1;
    }
    mainThread->setSystemThread(true);
    TCB::running = mainThread;
    return 0;
}


int ThreadTest()
{
    TCB *threads[4];

/*
    int createMainThread = thread_create(&threads[0],nullptr,nullptr);  //TCB::createThread(nullptr);
    if(0!= createMainThread) {
        printString("Error creatring main thread");
        return -1;
    }
    TCB::running = threads[0];

    threads[1] = TCB::createThread(workerBodyA);
    printString("ThreadA created\n");
    threads[2] = TCB::createThread(workerBodyB);
    printString("ThreadB created\n");
    threads[3] = TCB::createThread(workerBodyC);
    printString("ThreadC created\n");
    threads[4] = TCB::createThread(workerBodyD);
    printString("ThreadD created\n");
     */
    int create = 0;
    create = thread_create(&threads[0],(TCB::Body)workerBodyA,nullptr);
    if(create!=0) {
        printString("Error creatring thread A");
        return -1;
    }
    create = thread_create(&threads[1],(TCB::Body)workerBodyB,nullptr);
    if(create!=0) {
        printString("Error creatring thread B");
        return -1;
    }
    create = thread_create(&threads[2],(TCB::Body)workerBodyC,nullptr);
    if(create!=0) {
        printString("Error creatring thread C");
        return -1;
    }
    create = thread_create(&threads[3],(TCB::Body)workerBodyD,nullptr);
    if(create!=0) {
        printString("Error creatring thread D");
        return -1;
    }

    //Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    while (!(threads[0]->isFinished()&&
            threads[1]->isFinished() &&
             threads[2]->isFinished() &&
             threads[3]->isFinished()
             ))
    {
        TCB::yield();
    }

    for (auto &thread: threads)
    {
        delete thread;
    }
    printString("Finished\n");

    return 0;
}


int mainAllocationTest(){
    MemoryAllocator::initFreeMem();
    MemoryAllocatorTest::runAll();
    return 0;
}

int mainThreadTest_C_API(){
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    MemoryAllocator::initFreeMem();
    if(initMainThread()!=0){
        return -1;
    }
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    return runThreadTest_C_API();

}

int mainThreadTestHere(){
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    MemoryAllocator::initFreeMem();
    if(initMainThread()!=0){
        return -1;
    }
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    return ThreadTest();

}

int MainMain(){
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    MemoryAllocator::initFreeMem();
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    TCB* mainThread;
    thread_create(&mainThread, nullptr, nullptr);
    TCB::running = mainThread;
    //...
    return 0;
}

int mainWRet(){
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    MemoryAllocator::initFreeMem();
    if(initMainThread()!=0){
        return -1;
    }
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    int ret = SemaphoreTest::testAtomic();
    if( ret==0)    //if( mainSemClassTest()==0)
        printString("\nFinished sucessfully\n");
    else
        printString("\nTest failed.\n");
    printInteger(ret);
    return ret;
}

void userMain();

void userMainWrapper(void* arg){
    userMain();
    //((Semaphore*)arg )->signal();
    //thread_exit();
}


int main(){
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    MemoryAllocator::initFreeMem();
    if(initMainThread()!=0){
        return -1;
    }
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    //userMain();       V1

/*  V2
    Semaphore sem(0);
    Thread(userMainWrapper, &sem);
    sem.wait();
*/

    int *a=new int;
    *a=1;
    printInteger((uint64)a);
    delete a;
    printString("Deleted a!\n");

    TCB* userMainThread;
    if( thread_create(&userMainThread, userMainWrapper, nullptr) != 0){
        printString("Error creating user main thread\n");
        return 0;
    }

    while(!userMainThread->isFinished()){
        thread_dispatch();
    }

    printString("Mainfinished\n");

    // manually shutting down the system
    Riscv::shutDownEmulator();
    return 0;
}



