
#include "../h/Sem.h"
#include "../h/riscv.hpp"





int Sem::open(Sem** handle, int init){
    if(handle==nullptr) return -1;
    *handle = new Sem(init);
    if(*handle == nullptr) return -1;
    return 0;

}
int Sem::close(Sem* handle){
    if(handle==nullptr) return -1;

    while( handle->blockedThreads.peekFirst() != nullptr){
        handle->unblock(true);
    }
    delete handle;
    return 0;
}


int Sem::wait(Sem* handle) {
    if(handle==nullptr) return -1;

    if ( --(handle->value) < 0) {
        return handle->block();
    }
    return 0;
}

int Sem::signal(Sem* handle) {
    if(handle==nullptr) {
        return -1;
    }
    if ( ++(handle->value) <= 0) {
        return handle->unblock();
    }
    return 0;
}

int Sem::block() {
    if(TCB::running == nullptr) {return -2;}
    TCB::running->setBlocked(true);
    blockedThreads.addLast(TCB::running);
    TCB::dispatch();
    return TCB::running->isSemaphoreDeleted() ? -1 : 0;
}


int Sem::unblock(bool semaphoreDeleted) {
    TCB *thread= nullptr;
    do {
        thread = blockedThreads.removeFirst();
        if (thread == nullptr) return -1;
    }while(thread->isBlocked() == false); //if thread is not blocked(it already unblocked via timer wake up), remove next one
    thread->setBlocked(false);
    thread->setSemaphoreDeleted(semaphoreDeleted);
    Scheduler::put(thread);
    return 0;
}


int Sem::timedwait(Sem* handle, time_t timeout){
    return -1;
}


int Sem::trywait(Sem* handle){
    if(handle==nullptr) return -1;
    if(handle->value == 1){
        handle->value--;
        return 0;
    }else{
        return 1;
    }
}

