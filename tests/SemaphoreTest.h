//
// Created by os on 7/25/24.
//

#ifndef PROJECT_BASE_V1_1_SEMAPHORETEST_H
#define PROJECT_BASE_V1_1_SEMAPHORETEST_H


#include "../lib/hw.h"


class SemaphoreTest {
public:


    static void initializeSemaphore();

    static void signalSemaphore();

    static void waitSemaphore() ;

    static void multipleThreadsSemaphore();

    static void edgeCases();

    static void runAll();

private:
    static int mutex;
    static int const numberOfWorkerIterations = 1000000000; //10^9
    static int const SEM_INTENDED_VALUE = 10000001;    // 10^7
    static int const NUM_OF_THREADS_ACCESING_SEM = 10;
    static uint64 const printingIteration = SEM_INTENDED_VALUE/NUM_OF_THREADS_ACCESING_SEM/10;

    static void workerBodyA(void* arg);

    static void workerBodyB(void* arg);

    static void workerBodyAtomic(void *arg);


public:
    /**
 * Test for atomic operations on semaphore
 * @return 0 for correct execution\n
 * -1 for error in creating semaphore\n
    * -2 for incorrect initial semaphore value\n
    * -3 for error in thread creation\n
    * -5 for incorrect semaphore value after threads have finished\n
 */
    static int testAtomic();
};


#endif //PROJECT_BASE_V1_1_SEMAPHORETEST_H
