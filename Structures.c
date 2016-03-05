#include "Structures.h"

int totalProcess;
int actualNumberOfProcess;
int totalTickets;
struct Thread **tasks;

int getNextThreadId(){
    static long currentId = 0;
    return ++currentId;
}

struct Thread * nextTask(){
    
    int limiteInferior = 0;
    int limiteSuperior = actualNumberOfProcess;

    int winnerTicket = 01; //Sacar numero random
    //binary search to find the winner
    while(limiteInferior <= limiteSuperior){

        int medio = (limiteInferior + limiteSuperior)/2;
        
        struct Thread *actual = tasks[medio];
        
        if(winnerTicket < actual->lowerLimit){
            
            limiteSuperior = medio - 1;
                
        }
        if(winnerTicket > actual->upperLimit){
            limiteInferior = medio +1;
            
        }
        
        return actual;
    }   

    return NULL;
    
}

void reCalculateBoundaries(thread_t taskToRemove){

    struct Thread **auxArray = (struct Thread **) malloc((actualNumberOfProcess - 1) * sizeof(struct Thread *));
    
    int x, dx = 0;
    int lowerLimit = 0;
    int upperLimit = 0;
    for(x = 0; x < actualNumberOfProcess; x++){
        
        if(tasks[x]->tid != taskToRemove){

            struct Thread *actual = tasks[x];
            actual->lowerLimit = lowerLimit;
            actual->upperLimit = lowerLimit + actual->tickets;
            lowerLimit = actual->upperLimit + 1;
            struct Thread *newTask = (struct Thread *) malloc(sizeof(struct Thread));
            memcpy(newTask, actual, sizeof(struct Thread));
            auxArray[dx] = newTask;
            dx++;
        }else{
            totalTickets -= tasks[x]->tickets;
        }
        
    }
    actualNumberOfProcess--;
    free(tasks);
    tasks = auxArray;
}

struct Thread* getNewThreadnoStack(int tickets)
{
    struct Thread *newThread = (struct Thread *)malloc(sizeof(struct Thread));
    
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
    newThread->context.uc_stack.ss_flags = 0;
    newThread->tid = getNextThreadId();
    newThread->hasStackSpace = 0;
    newThread->isCompleted = 0;

    //tasks[actualNumberOfProcess++] = newThread;
    return newThread;

}

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



