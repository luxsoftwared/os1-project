//
// Created by os on 7/21/24.
//

#include "../h/syscall_c.h"
#include "../h/print.hpp"

# define FIBONACCI_ARGUMENT_N 9

void workerBodyA_(void *arg)
{
    printString("ThreadA started\n");
    for (int i = 0; i < 10; i++)
    {
        printString("A: i=");
        printInteger(i);
        printString("\n");
        __asm__("li t1,10");
        thread_dispatch();
        uint64 volatile temp;
        __asm__("mv %0, t1" : "=r"(temp));
        if(temp != 10){
            printString("!!!ERROR!!!ThreadA: temporary register t1 changed value while thread was inactive\nNew t1 value:");
            printInteger(temp);
            printString("\n");
        }
    }
    printString("ThreadA finished\n");
}

// Tests thread exit
void workerBodyB_(void *arg)
{
    printString("ThreadB started\n");
    for (int i = 0; i < 10; i++)
    {

        printString("B: i=");
        printInteger(i);
        printString("\n");
        if(i == 5){
            __asm__("li t1,11");
            thread_exit();
        }
        if(i > 5){
            printString("!!!ERROR!!! ThreadB: Thread should have exited, but it continued\n");
        }
    }
    printString("ThreadB finished\n");
}

static uint64 fibonacci(uint64 n)
{
    if (n == 0 || n == 1) { return n; }
    if (n % 3 == 0) {
        __asm__("li t1,12");

        thread_dispatch();

        uint64 volatile temp;
        __asm__("mv %0, t1" : "=r"(temp));
        if(temp != 12){
            printString("!!!ERROR!!! ThreadC: Fibonacci: temporary register t1 changed value while thread was inactive\nNew t1 value:");
            printInteger(temp);
            printString("\n");
        }
    }
    printString("Fibonacci ");
    printInteger(n);
    printString("\n");
    return fibonacci(n - 1) + fibonacci(n - 2);
}

// Tests argument passing, stack durability(recursive function calls)
void workerBodyC_(void *arg)
{
    printString("ThreadC started\n");
    if(  *((uint64*)arg) != FIBONACCI_ARGUMENT_N){
        printString("!!!ERROR!!! ThreadC: Argument passed to thread is not equal to expected value\n");
        printString("Expected: ");
        printInteger(FIBONACCI_ARGUMENT_N );
        printString("\tActual value: ");
        printInteger(*((uint64*)arg) );
        printString("\n");
    }


    uint64 fibRes = fibonacci(*((uint64*)arg) );
    printString("C: FIbonacci: ");
    printInteger(fibRes);
    printString("\n");

    switch(*((uint64*)arg) ){
        case 9:
            if(fibRes != 34){
                printString("!!!ERROR!!! ThreadC: Fibonacci did not return 34 for n=9\n");
            }
            break;
        case 12:
            if(fibRes != 144){
                printString("!!!ERROR!!! ThreadC: Fibonacci did not return 144 for n=12\n");
            }
            break;
        case 16:
            if(fibRes != 987){
                printString("!!!ERROR!!! ThreadC: Fibonacci did not return 987 for n=16\n");
            }
            break;
    }


    printString("ThreadC finished\n");
}

// Tests thread creation
int tryThreadCreation(TCB** handle, TCB::Body startFunction, void* arg) {
    int ret = thread_create(handle, startFunction, arg);
    if (ret != 0) {
        printString("!!!ERROR!!! Thread creation failed. ThreadFunction: ");
        printAddress((void*)startFunction);
        printString("\tError: ");
        printInteger(ret);
        printString("\n");
    }else if(handle == nullptr || *handle == nullptr) {
        printString("!!!ERROR!!! Thread creation error - invalid handle value after succesfull creation. ThreadFunction: ");
        printAddress((void*)startFunction);
        printString("\n");
        return -2;
    }
    return ret;
}

// Tests thread creation, thread dispatch, thread exit, argument passing, stack continuity(every thread changes t1 register value)
int runThreadTest_C_API(){

    printString("Tests thread creation, thread dispatch, thread exit, "
                    "argument passing, stack continuity(every thread changes t1 register value)\n");

    TCB *threads[4];

    int createMainThread = tryThreadCreation(&threads[0], (TCB::Body)nullptr, nullptr);

    if (createMainThread != 0){//err happened
        return -1;
    }
    TCB::running = threads[0];


    if(tryThreadCreation(&threads[1], workerBodyA_, nullptr) == 0){
        printString("ThreadA_ created\n");
    }
    if(tryThreadCreation(&threads[2], workerBodyB_, nullptr) == 0){
        printString("ThreadB_ created\n");
    }
    uint64* argC = new uint64;
    *argC = FIBONACCI_ARGUMENT_N;
    if(tryThreadCreation(&threads[3], workerBodyC_, argC) == 0){
        printString("ThreadC_ created\n");
    }

    //Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE); dont want timer interrupts now

    while (!(threads[1]->isFinished() &&
             threads[2]->isFinished() &&
             threads[3]->isFinished() ))
    {
        thread_dispatch();
    }

    for (auto &thread: threads)
    {
        delete thread;
    }
    printString("Finished\n");

    return 0;

};