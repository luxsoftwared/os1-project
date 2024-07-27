//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/print.hpp"

TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;
uint64 TCB::timeSliceCounterForSleeping = 0;

TCB *TCB::createThread(Body body, void* stack_space, void* arg)
{
    return new TCB(body, stack_space, arg, TIME_SLICE);
}


void TCB::yield()
{
    __asm__ volatile ("li a0, 0x13"); //setting op code
    __asm__ volatile ("ecall");
}

void TCB::dispatch()
{
    if(running == nullptr) { printString("There us no running\n");return; }
    TCB *old = running;
    if (!old->isFinished() && !old->isBlocked() ) { Scheduler::put(old); }
    running = Scheduler::get();
    TCB::contextSwitch(&old->context, &running->context);
}

//jumping here from ret in contextSwitch from dispatch, first time executing this thread
//znaci umesto u nastavak hendler koda ide ovde, i onda se krasj (sret) hendera ne odradi pa treba sve ovde
void TCB::threadWrapper()
{
    //change mode (from S to U? jumped here from handler wich is S mode)
    // take SPP val and set SPIE  and restore ||||ovo dole
    Riscv::popSppSpie(); //stavlja ra u sepc(jer tren sepc je od stare niti), i sa sret skida spp spie i vraca se ovde
    // no//bcs on start of the execution thread pushes random values on stack during initial contextSwitch(only second half is done)
    running->body(running->arg);
    running->setFinished(true);
    TCB::yield();
}



