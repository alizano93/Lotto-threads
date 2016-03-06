/* mi lebreria de hilos basica

*/



#include <stdio.h>
#include <setjmp.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "Structures.h"
#include "myThread.h"

//typedef long thread_t;
extern int totalProcess;
extern int actualNumberOfProcess;
extern int totalTickets;
extern struct Thread **tasks;

long mode; // 1 = expropiativo 2= no expropiativo
sigset_t sigThreadMask; // para el manejo de las sennales
int ignorarsennal;
struct sigaction schedulerHandle; //con este es que voy a inicializar el scheduler (ver el thread_init)
ucontext_t notifierContext; //funciona como notificador
struct itimerval quantum; //estrutura para el timer
long timeIntervalInMSec;
sigset_t sigsetMask;
struct Thread * current;
ucontext_t mainContext;

void generalFunction(void (*rutine)(void*),void *arg){
    (*rutine)(arg);
    return;
}
/*
void threadCompletedNotifier(){
    current->isCompleted=1;
    raise(SIGPROF);
}*/

static long currentId = 0;

int getNextThreadId(){
    
    return ++currentId;
}

struct Thread* getNewThreadnoStack(int tickets)
{
    struct Thread *newThread = (struct Thread *)malloc(sizeof(struct Thread));
 
    newThread->tickets = tickets;
    newThread->tid = getNextThreadId();
    newThread->finish = 0;
	
    return newThread;

}

struct sched_t *scheduler;

void timer_handler(int sig, siginfo_t *si, void *uc){

	scheduler->manageTimer();
		
}


void thread_init(long nquantum, int totalProcessInit, struct sched_t *schedulerIn) {
	
            scheduler = schedulerIn;
	    totalProcess = totalProcessInit;
    	    actualNumberOfProcess = 0;
	    getcontext(&mainContext);
          /*  sigemptyset(&sigsetMask);
            sigaddset(&sigsetMask, SIGPROF); //para mandar sennal cuando expire el timer
            timeIntervalInMSec = nquantum;
            schedulerHandle.sa_sigaction = timer_handler;
            schedulerHandle.sa_flags = SA_SIGINFO|SA_RESTART;
            sigemptyset(&schedulerHandle.sa_mask);
            sigaction(SIGRTMIN, &schedulerHandle, NULL);

            //inicializo el quantum
            quantum.it_value.tv_sec = 0;// http://linux.die.net/man/2/setitimer
            quantum.it_value.tv_usec = timeIntervalInMSec;
            quantum.it_interval.tv_sec=0;
            quantum.it_interval.tv_usec = timeIntervalInMSec;

            setitimer(ITIMER_PROF, &quantum, NULL);*/
}


int thread_create(thread_t id, int tickets, void (*rutina)(int), int arg) {
	
        struct Thread *newThread = getNewThreadnoStack(tickets);
        getcontext(&(newThread->context));
        newThread->context.uc_stack.ss_sp = malloc(STACKSIZE);
        newThread->context.uc_stack.ss_size = STACKSIZE;
        newThread->context.uc_stack.ss_flags = 0;
        newThread->context.uc_link = &mainContext;
        makecontext (&(newThread->context), (void (*) (void))rutina, 1,1);
     	scheduler->addTask(newThread);
	
	return 1;
}

void thread_yield(){

	
	    
//raise(SIGPROF); //con esto deberia bastar mandando la sennal se captura y cambio segun el scheduler
}

void thread_exit(){
    //deberia terminar el contexto y podriamos y deberiamos tener una estructura para meter a los threads terminados, asi que tambien lo meteria a la lista de terminados
}

thread_t self(){
   return 0; //devuelvo el id del thread actual // pensaria tener una lista y en la estructura de la lista tener el actual. o tener el thread actual por aparte. estoy analizando
}

int thread_join(){
	scheduler->run();
    return 0;//espera a que todos hayan terminado
}








