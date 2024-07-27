//
// Created by marko on 20.4.22..
//

#include "../h/scheduler.hpp"
#include "../h/tcb.hpp"

List<TCB> Scheduler::readyThreadQueue;

TCB *Scheduler::get()
{
    return readyThreadQueue.removeFirst();
}

void Scheduler::put(TCB *ccb)
{
    readyThreadQueue.addLast(ccb);
}

void Scheduler::putToSleepQueue(TCB *tcb, uint64 sleepTime)
{
    int sleepDifference = 0;
    if(sleepingThreadsQueue.peekFirst()!= nullptr){
        sleepDifference = sleepingThreadsQueue.peekFirst()->getSleepTime() - TCB::timeSliceCounterForSleeping; //time left for first thread to wake up

        if(sleepDifference > sleepTime){
            TCB::timeSliceCounterForSleeping = 0;
            tcb->setSleepTime(sleepTime);
            sleepingThreadsQueue.peekFirst()->setSleepTime(sleepDifference - sleepTime);
            sleepingThreadsQueue.addFirst(tcb);
            return;
        }
        Elem* iter = sleepingThreadsQueue.head;

        sleepTime -= sleepDifference;
        if(iter->next == nullptr){
            tcb->setSleepTime(sleepTime);
            sleepingThreadsQueue.addLast(tcb);
            return;
        }
        //else
        sleepDifference = iter->next->data->getSleepTime();
        while(sleepDifference <= sleepTime && iter->next != nullptr){
            sleepTime -= sleepDifference; //time left for new thread after curr thread wakes up
            sleepDifference = iter->next->data->getSleepTime();
            iter=iter->next;
        }
        //insert after iter
        tcb->setSleepTime(sleepTime);
        if(iter->next == nullptr){
            sleepingThreadsQueue.addLast(tcb);
            return;
        }
        Elem* newElem = new Elem(tcb, iter->next);
        iter->next = newElem;
        newElem->next->data->setSleepTime(newElem->next->data->getSleepTime() - sleepTime);


    }else{
        //no other threads are asleep
        tcb->setSleepTime(sleepTime);
        sleepingThreadsQueue.addLast(tcb);
    }
    for()
    tcb->setSleepTime(sleepTime);
    sleepingThreadsQueue.addLast(tcb);
}

void Scheduler::removeFromSleepQueue(TCB *tcb);