//
// Created by os on 7/21/24.
//

#ifndef PROJECT_BASE_V1_1_SEM_H
#define PROJECT_BASE_V1_1_SEM_H

#include "list.hpp"
#include "tcb.hpp"

//0 if succesfull, 1 if not
extern "C" uint64 test_and_set(int* lock, uint64 expected, uint64 desired);


class Sem {
public:


    static int open(Sem** handle, int init = 1);
    static int close(Sem* handle);
    static int wait(Sem* handle);
    static int signal(Sem* handle);
    static int timedwait(Sem* handle, time_t timeout);
    /**
     * @brief Try to decrement the semaphore value; if it is greater than 0, decrement it and return 0, otherwise return 1, or return -1 in case of an error
     * @param handle
     * @return
     */
    static int trywait(Sem* handle);

    int getValue() const {
        return value;
    }
private:
    Sem(int init = 1) : value(init) {}

    int value;
    bool deleted=false;
    int mutex=1;
    List<TCB> blockedThreads;
    List<TCB> mutexBlockedThreads;

    int block();
    int unblock(bool semaphoreDeleted = false);


};


#endif //PROJECT_BASE_V1_1_SEM_H
