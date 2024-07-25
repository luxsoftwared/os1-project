//
// Created by os on 7/21/24.
//

#include "../h/Sem.h"
#include "../h/syscall_c.h"
#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"

#define SEM_INTENDED_VALUE 100000001    // 10^8
#define NUM_OF_THREADS_ACCESING_SEM 10
const uint64 printingIteration = SEM_INTENDED_VALUE/NUM_OF_THREADS_ACCESING_SEM/10;

void semWorkerBodyA(void *arg)
{
    uint64 rando = 0;
    if(arg == nullptr) return;
    //Sem* sem= (Sem*)arg;
    for (uint64 i = 0; i < SEM_INTENDED_VALUE/NUM_OF_THREADS_ACCESING_SEM; i++) // 10^8
    {
        rando++;
        if(i%printingIteration==0) { // 10^7
            printInteger(i/printingIteration);
            //printString("\n");
        }
        Sem::signal((Sem*)arg);
    }

}
/**
 * Test for atomic operations on semaphore
 * @return 0 for correct execution\n
 * -1 for error in creating semaphore\n
    * -2 for incorrect initial semaphore value\n
    * -3 for error in thread creation\n
    * -5 for incorrect semaphore value after threads have finished\n
 */
int TestAtomicOperation() {
    //Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    //MemoryAllocator::initFreeMem();

    Sem* sem = nullptr;
    Sem::open(&sem, 1);
    if(sem == nullptr) return -1;
    if(sem->getValue() != 1) return -2;

    TCB* threads[NUM_OF_THREADS_ACCESING_SEM];

    for(int i=0;i<NUM_OF_THREADS_ACCESING_SEM;i++){
        if(thread_create(&threads[i], (TCB::Body)semWorkerBodyA, (void*)sem ) != 0) return -3;
    }

    while (!(threads[0]->isFinished() &&
            threads[1]->isFinished() &&
             threads[2]->isFinished() &&
             threads[3]->isFinished() &&
            threads[4]->isFinished() &&
            threads[5]->isFinished() &&
            threads[6]->isFinished() &&
            threads[7]->isFinished() &&
            threads[8]->isFinished() &&
            threads[9]->isFinished() /*&&
            threads[10]->isFinished() &&
            threads[11]->isFinished() &&
            threads[12]->isFinished() &&
            threads[13]->isFinished() &&
            threads[14]->isFinished() &&
            threads[15]->isFinished() &&
            threads[16]->isFinished() &&
            threads[17]->isFinished() &&
            threads[18]->isFinished() &&
            threads[19]->isFinished() &&
             threads[20]->isFinished() &&
             threads[21]->isFinished() &&
                threads[22]->isFinished() &&
                threads[23]->isFinished() &&
                threads[24]->isFinished() &&
                threads[25]->isFinished() &&
                threads[26]->isFinished() &&
                threads[27]->isFinished() &&
                threads[28]->isFinished() &&
                threads[29]->isFinished()*/ ))
    {
        thread_dispatch();
    }

    for (auto &thread: threads)
    {
        delete thread;
    }

    printString("Semaphore value:\t");
    printInteger(sem->getValue());
    printString("\nIntended value: \t");
    printInteger(SEM_INTENDED_VALUE);
    if(sem->getValue() == SEM_INTENDED_VALUE)
        return 0;
    else
        return -5;
}

int mainSemClassTest(){
    return TestAtomicOperation();
}