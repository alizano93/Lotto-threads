#include "Structures.h"

int totalProcess;
int actualNumberOfProcess;
int totalTickets;
struct Thread **tasks;

//int getNextThreadId(){
 //   static long currentId = 0;
  //  return ++currentId;/
//}
/*
struct Thread* getNewThreadnoStack(int tickets)
{
    struct Thread *newThread = (struct Thread *)malloc(sizeof(struct Thread));
 
    newThread->tickets = tickets;
    newThread->context.uc_link = 0;
    newThread->context.uc_stack.ss_flags = 0;
    newThread->tid = getNextThreadId();
    newThread->finish = 0;
	
    return newThread;

}*/
/*
struct Thread* getNewThreadwithStack(int tickets)
{
    struct Thread * newThread = (struct Thread *)malloc(sizeof(struct Thread));
    
    newThread->context.uc_stack.ss_sp = malloc(STACKSIZE);
    
    int lowerBoundary = 0; 

    if(actualNumberOfProcess == 0){
        lowerBoundary = 0;
    }
    else{
        lowerBoundary = tasks[actualNumberOfProcess-1]->upperLimit+1;
    }
    newThread->tickets = tickets;
    newThread->lowerLimit = lowerBoundary;
    newThread->upperLimit = lowerBoundary + newThread->tickets;
    newThread->context.uc_link = 0;
    newThread->hasStackSpace = 1;
    newThread->isCompleted = 0;
    newThread->context.uc_stack.ss_size= STACKSIZE;
    newThread->context.uc_stack.ss_flags = 0;
    newThread->tid = getNextThreadId();

    //tasks[actualNumberOfProcess++] = newThread;
    return newThread;
}
*/


