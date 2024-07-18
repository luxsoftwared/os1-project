//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"

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
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{
    uint64 scause = r_scause();
    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL)
    {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        /* code for syncr thread inter from V7
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();
        TCB::timeSliceCounter = 0;
        TCB::dispatch();
        w_sstatus(sstatus);
        w_sepc(sepc);*/

        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        uint64 codeOperation = r_a0();
        switch (codeOperation) {
            case MEM_ALLOC:{
                size_t size;
                size = r_a1();
                void* returnValue;
                returnValue = MemoryAllocator::mem_alloc(size);
                //t0,fp+80??
                __asm__ volatile ("mv a0, %0" : : "r"(returnValue));
                break;
                }
            case MEM_FREE:{
                void* addr;
                __asm__ volatile ("mv %0, a1" : "=r" (addr));
                int returnValue;
                returnValue = MemoryAllocator::mem_free(addr);
                //t0,fp+80??
                __asm__ volatile ("mv a0, %0" : : "r"(returnValue));
                break;
                }
            case THREAD_CREATE:

                break;
            case THREAD_EXIT:

                break;
            case THREAD_DISPATCH:

                break;
            case SEM_OPEN:

                break;
            case SEM_CLOSE:

                break;
            case SEM_WAIT:

                break;
            case SEM_SIGNAL:

                break;
            case SEM_TIMEDWAIT:

                    break;
        }
        w_sstatus(sstatus);
        w_sepc(sepc);

    }
    else if (scause == 0x8000000000000001UL)
    {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        TCB::timeSliceCounter++;
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            uint64 volatile sepc = r_sepc();
            uint64 volatile sstatus = r_sstatus();
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
            w_sstatus(sstatus);
            w_sepc(sepc);
        }
    }
    else if (scause == 0x8000000000000009UL)
    {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
    else
    {
        // unexpected trap cause
    }
}