//
// Created by os on 7/25/24.
//

#include "SemaphoreTest.h"
#include "../h/print.hpp"
#include "../h/syscall_c.h"
#include "../h/tcb.hpp"



void SemaphoreTest::initializeSemaphore() {
    Sem* sem = nullptr;
    if (sem_open(&sem, 1) != 0 || sem == nullptr) {
        printString("initializeSemaphore: Failed\n");
        return;
    }
    if (sem->getValue() != 1) {
        printString("initializeSemaphore: Incorrect initial value\n");
        return;
    }
    sem_close(sem);
    printString("initializeSemaphore: Passed\n");
}

void SemaphoreTest::signalSemaphore() {
    Sem* sem = nullptr;
    sem_open(&sem, 1);
    sem_signal(sem);
    if (sem->getValue() != 2) {
        printString("signalSemaphore: Failed\n");
        return;
    }
    sem_close(sem);
    printString("signalSemaphore: Passed\n");
}

void SemaphoreTest::waitSemaphore() {
    Sem* sem = nullptr;
    sem_open(&sem, 1);
    sem_wait(sem);
    if (sem->getValue() != 0) {
        printString("waitSemaphore: Failed\n");
        return;
    }
    sem_close(sem);
    printString("waitSemaphore: Passed\n");
}

void SemaphoreTest::multipleThreadsSemaphore() {
    Sem* sem = nullptr;
    sem_open(&sem, 1);
    TCB* threads[2];
    thread_create(&threads[0], (TCB::Body)workerBodyA, (void*)sem);
    thread_create(&threads[1], (TCB::Body)workerBodyB, (void*)sem);

    while (!(threads[0]->isFinished() && threads[1]->isFinished())) {
        thread_dispatch();
    }

    if (sem->getValue() != 1) {
        printString("multipleThreadsSemaphore: Failed\n");
        return;
    }

    for (auto &thread : threads) {
        delete thread;
    }
    sem_close(sem);
    printString("multipleThreadsSemaphore: Passed\n");
}

void SemaphoreTest::edgeCases() {
    if (sem_signal(nullptr) != -1) {
        printString("edgeCases: failed; signal null semaphore should fail\n");
        return;
    }
    if (sem_wait(nullptr) != -1) {
        printString("edgeCases: failed; wait null semaphore should fail\n");
        return;
    }
    printString("edgeCases: Passed\n");
}

void SemaphoreTest::runAll() {
    initializeSemaphore();
    signalSemaphore();
    waitSemaphore();
    edgeCases();
    multipleThreadsSemaphore();

}


int SemaphoreTest::mutex = 1;

void SemaphoreTest::workerBodyA(void* arg) {
    Sem* sem = (Sem*)arg;

    while(test_and_set(&mutex, 1, 0));
    printString("A: Value of semaphore before wait:");
    printInteger(sem->getValue());
    printString("\n");
    printString("Wait called for workerA\n");
    sem_wait(sem);
    test_and_set(&mutex, 0, 1);

    printString("Waiting ended for workerA\n");



    for(int i=0;i<numberOfWorkerIterations;i++){
        if(i%(numberOfWorkerIterations/10)==0){
            printString("A: ");
            printInteger(i/(numberOfWorkerIterations/10));
            printString("\n");
        }
    }
    printString("A: Value of semaphore before signal:");
    printInteger(sem->getValue());
    printString("\n");
    printString("Signal called for worker A\n");
    sem_signal(sem);
}

void SemaphoreTest::workerBodyB(void* arg) {
    Sem* sem = (Sem*)arg;

    while(test_and_set(&mutex, 1, 0));
    printString("B: Value of semaphore before wait:");
    printInteger(sem->getValue());
    printString("\n");
    printString("Wait called for workerB\n");
    sem_wait(sem);
    test_and_set(&mutex, 0, 1);

    printString("Waiting ended for workerB\n");


    for(int i=0;i<numberOfWorkerIterations;i++){
        if(i%(numberOfWorkerIterations/10)==0){
            printString("B: ");
            printInteger(i/(numberOfWorkerIterations/10));
            printString("\n");
        }

    }
    printString("B: Value of semaphore before signal:");
    printInteger(sem->getValue());
    printString("\n");
    printString("Signal called for worker B\n");
    sem_signal(sem);
}

void SemaphoreTest::workerBodyAtomic(void *arg)
{

    if(arg == nullptr) return;
    //Sem* sem= (Sem*)arg;
    for (uint64 i = 0; i < SEM_INTENDED_VALUE/NUM_OF_THREADS_ACCESING_SEM; i++) // 10^7
    {

        if(i%printingIteration==0) { // 10^6
            printInteger(i/printingIteration);
            printString("\n");
        }
        sem_signal((Sem*)arg);
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
int SemaphoreTest::testAtomic() {

    Sem* sem = nullptr;
    sem_open(&sem, 1);
    if(sem == nullptr) return -1;
    if(sem->getValue() != 1) return -2;

    TCB* threads[NUM_OF_THREADS_ACCESING_SEM];

    for(int i=0;i<NUM_OF_THREADS_ACCESING_SEM;i++){
        if(thread_create(&threads[i], (TCB::Body)workerBodyAtomic, (void*)sem ) != 0) return -3;
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
    int semVal = sem->getValue();
    sem_close(sem);
    printInteger((uint64)semVal);
    printString("\nIntended value: \t");
    printInteger(SEM_INTENDED_VALUE);
    if(sem->getValue() == SEM_INTENDED_VALUE)
        return 0;
    else
        return -5;
}