//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"

#include "../tests/MemoryAllocatorTest.cpp"
#include "../tests/testFunctions.h"

#ifndef DEBUG_LVL
#define DEBUG_LVL 0
#endif

//TODO fix:not working anymore bcs create thread was changed
int mainThreadTest()
{
    /*
    TCB *threads[5];

    threads[0] = TCB::createThread(nullptr);
    TCB::running = threads[0];

    threads[1] = TCB::createThread(workerBodyA);
    printString("ThreadA created\n");
    threads[2] = TCB::createThread(workerBodyB);
    printString("ThreadB created\n");
    threads[3] = TCB::createThread(workerBodyC);
    printString("ThreadC created\n");
    threads[4] = TCB::createThread(workerBodyD);
    printString("ThreadD created\n");

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    while (!(threads[1]->isFinished() &&
             threads[2]->isFinished() &&
             threads[3]->isFinished() &&
             threads[4]->isFinished()))
    {
        TCB::yield();
    }

    for (auto &thread: threads)
    {
        delete thread;
    }
    printString("Finished\n");
*/
    return 0;
}


int mainAllocationTest(){
    MemoryAllocator::initFreeMem();
    MemoryAllocatorTest::runAll();
    return 0;
}

int main(){
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    MemoryAllocator::initFreeMem();
    //Riscv::mc_sip(Riscv::SIP_SSIP);
    return runThreadTest_C_API();


}




