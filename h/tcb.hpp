//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"

// Thread Control Block
class TCB
{
public:
    ~TCB() { delete[] stack; }

    bool isFinished() const { return finished; }

    void setFinished(bool value) { finished = value; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)();

    static TCB *createThread(Body body);

    static void yield();

    static TCB *running;

private:
    // body is nullptr for main, is shpuldnt execute from beggining and it already has stack and shoul not be put in scheduler during creation,
    //evrthng will be consistent after first context change
    TCB(Body body, uint64 timeSlice) :
            body(body),
            stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            timeSlice(timeSlice),
            finished(false)
    {
        if (body != nullptr) { Scheduler::put(this); }
    }

    struct Context
    {
        uint64 ra;//pc before yield for that thread
        uint64 sp;
    };

    Body body;
    uint64* stack;
    Context context;
    uint64 timeSlice;
    bool finished;

    friend class Riscv;

    /**
    * Setting the thread up for initial execution if its body, getting spp and spie
    */
    static void threadWrapper();

    /**
    * Saves old context and restores new context
    */
    static void contextSwitch(Context *oldContext, Context *runningContext);


    /**
    * Puts current thread back to scheduler and gets new thread from scheduler, then switches context
    */
    static void dispatch();

    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = 1024;//1024 uint64
    static uint64 constexpr TIME_SLICE = 2;
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
