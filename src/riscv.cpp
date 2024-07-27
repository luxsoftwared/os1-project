//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/print.hpp"
#include "../h/Sem.h"

enum ABI_codes{
    MEM_ALLOC = 0x01,
    MEM_FREE  = 0x02,
    THREAD_CREATE = 0x11,
    THREAD_EXIT = 0x12,
    THREAD_DISPATCH = 0x13,
    //THREAD_CREATE_ONLY = 0x14,
    //THREAD_START = 0x15,
    SEM_OPEN = 0x21,
    SEM_CLOSE = 0x22,
    SEM_WAIT = 0x23,
    SEM_SIGNAL = 0x24,
    SEM_TIMEDWAIT = 0x25,
    SEM_TRYWAIT = 0x26,
    TIME_SLEEP = 0x31,
    GETC = 0x41,
    PUTC = 0x42
};

void Riscv::popSppSpie()
{
    //Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}



void Riscv::handleSupervisorTrap()
{

    uint64 scause = r_scause();
    uint64 codeOperation = r_a0();
    uint64 sepc = r_sepc()+4;
    uint64 sstatus = r_sstatus();


    //printAddress((void*)scause);
    //printString("\t");
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)

        /*
        printAddress((void*)sepc);
        printString("\t");
        printAddress((void*)sstatus);
        printString("\t");

        printInteger(codeOperation);
        printString("\n");
         */
        switch (codeOperation) {
            case MEM_ALLOC:{
                size_t size;
                size = r_a1();
                void* returnValue=MemoryAllocator::mem_alloc(size);
                __asm__ volatile ("mv a0, %0" : : "r"(returnValue));
                __asm__ volatile("sd a0, 10*8(fp)"); //saving ret value on stack, in place where supervisorTrap is holding x10(a0)
                break;
                }
            case MEM_FREE:{
                void* volatile addr;
                __asm__ volatile ("mv %0, a1" : "=r" (addr));
                int returnValue;
                returnValue = MemoryAllocator::mem_free(addr);

                __asm__ volatile ("mv a0, %0" : : "r"(returnValue));
                __asm__ volatile("sd a0, 10*8(fp)"); //saving ret value on stack, in place where supervisorTrap is holding x10(a0)
                break;
                }
            case THREAD_CREATE:{
                TCB** volatile handle;
                void* volatile startFunction;
                void* volatile arg;
                void* volatile stack;

                __asm__ volatile("ld t1, 11*8(fp)"); //getting a1/x11 from stack
                __asm__ volatile ("mv %[handle], t1" : [handle] "=r"(handle));

                __asm__ volatile("ld t1, 12*8(fp)"); //getting a2/x12 from stack
                __asm__ volatile ("mv %[startFunction], t1" : [startFunction] "=r"(startFunction));

                __asm__ volatile("ld t1, 13*8(fp)"); //getting a3/x13 from stack
                __asm__ volatile ("mv %[arg], t1" : [arg] "=r"(arg));

                __asm__ volatile("ld t1, 14*8(fp)"); //getting a4/x14 from stack
                __asm__ volatile ("mv %[stack], t1" : [stack] "=r"(stack));
                int returnValue = -10;
                if(handle!= nullptr){
                    *handle = TCB::createThread((TCB::Body)startFunction, stack, arg);
                    if(*handle == nullptr){
                        //printString("ERROR! unsuccesfull TCB::createThread call\n");
                        returnValue = -1;
                    }else{
                        returnValue = 0;
                    }
                }else{
                    //printString("ERROR! forwarded handle is nullptr\n");
                    returnValue = -2;
                }

                __asm__ volatile ("mv a0, %0" : : "r"(returnValue));
                __asm__ volatile("sd a0, 10*8(fp)"); //saving ret value on stack, in place where supervisorTrap is holding x10(a0)
                break;
            }
            case THREAD_EXIT:{
                int returnValue = 0;
                if(TCB::running == nullptr) {
                    returnValue = -1;
                }
                else{
                    TCB::running->setFinished(true);
                    TCB::timeSliceCounter = 0;
                    TCB::dispatch();
                }

                __asm__ volatile ("mv a0, %0" : : "r"(returnValue));
                __asm__ volatile ("sw t0, 80(x8)");
            }
            case THREAD_DISPATCH:{
                TCB::timeSliceCounter = 0;
                TCB::dispatch();
                break;
                }
            case SEM_OPEN:{
                Sem** volatile handle;
                uint64 volatile init;

                __asm__ volatile("ld t1, 11*8(fp)"); //getting a1/x11 from stack
                __asm__ volatile ("mv %[handle], t1" : [handle] "=r"(handle));

                __asm__ volatile("ld t1, 12*8(fp)"); //getting a2/x12 from stack
                __asm__ volatile ("mv %[init], t1" : [init] "=r"(init));

                int returnValue = -10;
                if(handle!= nullptr){
                    int ret = Sem::open(handle, init);
                    if(*handle == nullptr || ret != 0){
                        //printString("ERROR! unsuccessful Sem::open call\n");
                        returnValue = -1;
                    }else{
                        returnValue = 0;
                    }
                }else{
                    //printString("ERROR! forwarded handle is nullptr\n");
                    returnValue = -2;
                }

                __asm__ volatile ("mv a0, %0" : : "r"(returnValue));
                __asm__ volatile("sd a0, 10*8(fp)"); //saving ret value on stack, in place where supervisorTrap is holding x10(a0)
                break;
            }
            case SEM_CLOSE:{
                Sem* volatile handle;

                __asm__ volatile("ld t1, 11*8(fp)"); //getting a1/x11 from stack
                __asm__ volatile ("mv %[handle], t1" : [handle] "=r"(handle));

                int returnValue = -10;
                returnValue = Sem::close(handle);

                __asm__ volatile ("mv a0, %0" : : "r"(returnValue));
                __asm__ volatile("sd a0, 10*8(fp)"); //saving ret value on stack, in place where supervisorTrap is holding x10(a0)
                break;
            }
            case SEM_WAIT:{
                Sem* volatile handle;

                __asm__ volatile("ld t1, 11*8(fp)"); //getting a1/x11 from stack
                __asm__ volatile ("mv %[handle], t1" : [handle] "=r"(handle));

                int returnValue = -10;
                returnValue = Sem::wait(handle);

                __asm__ volatile ("mv a0, %0" : : "r"(returnValue));
                __asm__ volatile("sd a0, 10*8(fp)"); //saving ret value on stack, in place where supervisorTrap is holding x10(a0)
                break;
            }
            case SEM_SIGNAL:{
                Sem* volatile handle;

                __asm__ volatile("ld t1, 11*8(fp)"); //getting a1/x11 from stack
                __asm__ volatile ("mv %[handle], t1" : [handle] "=r"(handle));

                int returnValue = -10;
                returnValue = Sem::signal(handle);

                __asm__ volatile ("mv a0, %0" : : "r"(returnValue));
                __asm__ volatile("sd a0, 10*8(fp)"); //saving ret value on stack, in place where supervisorTrap is holding x10(a0)
                break;
            }
            case SEM_TIMEDWAIT: {
                Sem* volatile handle;
                time_t volatile timeout;

                __asm__ volatile("ld t1, 11*8(fp)"); //getting a1/x11 from stack
                __asm__ volatile ("mv %[handle], t1" : [handle] "=r"(handle));

                __asm__ volatile("ld t1, 12*8(fp)"); //getting a2/x12 from stack
                __asm__ volatile ("mv %[timeout], t1" : [timeout] "=r"(timeout));

                int returnValue = -10;
                returnValue = Sem::timedwait(handle, timeout);

                __asm__ volatile ("mv a0, %0" : : "r"(returnValue));
                __asm__ volatile("sd a0, 10*8(fp)"); //saving ret value on stack, in place where supervisorTrap is holding x10(a0)
                break;
            }
            case SEM_TRYWAIT:{
                Sem* volatile handle;

                __asm__ volatile("ld t1, 11*8(fp)"); //getting a1/x11 from stack
                __asm__ volatile ("mv %[handle], t1" : [handle] "=r"(handle));

                int returnValue = -10;
                returnValue = Sem::trywait(handle);

                __asm__ volatile ("mv a0, %0" : : "r"(returnValue));
                __asm__ volatile("sd a0, 10*8(fp)"); //saving ret value on stack, in place where supervisorTrap is holding x10(a0)
                break;
            }
            case TIME_SLEEP:{
                time_t volatile time;
                __asm__ volatile("ld t1, 11*8(fp)"); //getting a1/x11 from stack
                __asm__ volatile ("mv %[time], t1" : [time] "=r"(time));
                int returnValue = 0;
                if(TCB::running == nullptr) {
                    returnValue = -1;
                }
                else{
                    TCB::running->setBlocked(true);
                    Scheduler::putToSleepQueue(TCB::running, time);
                    TCB::dispatch();
                }


                break;
            }
            case GETC:{
                break;
            }
            case PUTC:{
                break;
            }
            default:{
                printString("ERROR! CODE OPERATION:");
                printInteger(codeOperation);
                printString("\n");
                break;
            }

        }

        w_sstatus(sstatus);
        w_sepc(sepc);

    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        //TIMER change context
        Riscv::mc_sip(Riscv::SIP_SSIP);
        TCB::timeSliceCounter++;

        if(Scheduler::sleepingThreadsQueue.peekFirst() != nullptr){
            TCB::timeSliceCounterForSleeping++;
            while(Scheduler::sleepingThreadsQueue.peekFirst() != nullptr && TCB::timeSliceCounterForSleeping >= Scheduler::sleepingThreadsQueue.peekFirst()->getSleepTime()){
                TCB::timeSliceCounterForSleeping = 0;
                TCB *thread = Scheduler::sleepingThreadsQueue.removeFirst();
                if (thread == nullptr) return;
                if(thread->isBlocked() == false)    continue;//if thread is not blocked(it already unblocked via signal unblock), check next one
                thread->setBlocked(false);
                Scheduler::put(thread);
            }
        }

        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            TCB::timeSliceCounter = 0;
            TCB::dispatch();

        }
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        // unexpected trap cause
        printString("ERROR! SCAUSE:");
        printInteger(scause);
        printString("\tSEPC:");
        printAddress((void*)sepc);
        printString("\n");
    }

}