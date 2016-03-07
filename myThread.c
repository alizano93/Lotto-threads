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

extern int mode; // 1 = expropiativo 2= no expropiativo
sigset_t sigThreadMask; // para el manejo de las sennales
int ignorarsennal;
struct sigaction schedulerHandle; //con este es que voy a inicializar el scheduler (ver el thread_init)
ucontext_t notifierContext; //funciona como notificador
struct itimerval quantum; //estrutura para el timer
long timeIntervalInMSec;
sigset_t sigsetMask;
extern Thread * current;
ucontext_t mainContext;

void generalFunction(void (*rutine)(int),int arg){
    (*rutine)(arg);
    current->finish = 1;
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

struct Thread* getNewThreadnoStack(int tickets, char *id)
{
    struct Thread *newThread = (struct Thread *)malloc(sizeof(struct Thread));
 
    newThread->tickets = tickets;
    newThread->tid = id;	//getNextThreadId();
    newThread->finish = 0;
	
    return newThread;

}

struct sched_t *scheduler;

void timer_handler(int sigNum){
if(ignorarsennal != 1)
	scheduler->manageTimer();
		
}


void thread_init(long nquantum, int totalProcessInit, struct sched_t *schedulerIn) {
	    ignorarsennal = 1;
        scheduler = schedulerIn;
        sigemptyset(&sigsetMask);
        sigaddset(&sigsetMask,SIGPROF);
	    totalProcess = totalProcessInit;
    	actualNumberOfProcess = 0;
	    getcontext(&mainContext);
	    memset(&schedulerHandle,0,sizeof(schedulerHandle));
	    schedulerHandle.sa_handler = timer_handler;
	    sigaction(SIGPROF,&schedulerHandle,NULL);
	    timeIntervalInMSec = nquantum;
	    quantum.it_value.tv_sec = 0;// http://linux.die.net/man/2/setitimer
        quantum.it_value.tv_usec = timeIntervalInMSec;
        quantum.it_interval.tv_sec=0;
        quantum.it_interval.tv_usec = timeIntervalInMSec;
        if(mode == EXPROPIATIVO)
            setitimer(ITIMER_PROF, &quantum, NULL);

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


int thread_create(char *id, int tickets, void (*rutina)(int), int arg) {
	
        struct Thread *newThread = getNewThreadnoStack(tickets, id);
        getcontext(&(newThread->context));
        newThread->context.uc_stack.ss_sp = malloc(STACKSIZE);
        newThread->context.uc_stack.ss_size = STACKSIZE;
        newThread->context.uc_stack.ss_flags = 0;
        newThread->context.uc_link = &mainContext;
        makecontext (&(newThread->context), (void (*) (void))generalFunction, 2,rutina, arg);
     	scheduler->addTask(newThread);
	
	return 1;
}

void thread_yield(){

	ignorarsennal=0;
	raise(SIGPROF);
	    
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
char* getCurrentid(){
    return current->tid;
}
float getCurrentPercent(){
    return current->percent;
}

double getCurrentResult(){
    return current->result;
}
int isFinished(){
    return current->finish;
}

void updateThread(float percent, double result){
	current->percent = percent;
	current->result = 2 * result;
}








