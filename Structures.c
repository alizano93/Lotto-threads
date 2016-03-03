#include "Structures.h"

Thread getNewThreadnoStack()
{
    Thread newThread = (Thread)malloc(sizeof(Thread));
    if(newThread == null){
        return null;
    }
    newThread->context.uc_link = 0;
    newThread->context.uc_stack.ss_flags = 0;
    newThread->next = null;
    newThread->blockedThreads = null;
    newThread->isBlocked = 0;
    newThread->tid = getNextThreadId();
    newThread->hasStackSpace = 0;
    newThread->isCompleted = 0;
    return newThread;

}

Thread getNewThreadwithStack()
{
    Thread newThread = (Thread)malloc(sizeof(Thread));
    if(newThread == null){
        return null;
    }
    newThread->context.uc_stack.ss_sp = malloc(STACKSIZE);
    newThread->next = null;
    if(newThread->context.uc_stack.ss_sp == null){
        free(newThread);
        return null;
    }
    newThread->context.uc_link = 0;
    newThread->hasStackSpace = 1;
    newThread->isCompleted = 0;
    newThread->context.uc_stack.ss_size= STACKSIZE;
    newThread->context.uc_stack.ss_flags = 0;
    newThread->blockedThreads = null;
    newThread->isBlocked = 0;
    newThread->tid = getNextThreadId();

    return newThread;

}

int getNextThreadId(){
    static long currentId = 0;
    return ++currentId;
}

void flush(Thread thread){
    if(thread != null){

    }
}