/* mi lebreria de hilos basica

*/


#include <stdio.h>
#include <setjmp.h>
#include <sys/ucontext.h>
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
ucontext_t mainContext; //funciona como notificador
struct itimerval quantum; //estrutura para el timer
long timeIntervalInMSec;
sigset_t sigsetMask;
struct Thread * current;

void scheduler(int sigNum){
    if(!ignorarsennal){
        sigprocmask(SIG_BLOCK,&sigsetMask,NULL);
        if(actualNumberOfProcess == 1) {
            if(current->isCompleted){
                sigprocmask(SIG_UNBLOCK,&sigsetMask,NULL);
                exit(0);
            }
        }
        else if(actualNumberOfProcess > 0){
            int currentNodeCompleted = 0;
            if(current != NULL){

                struct Thread *nextNode;
                if(current->isCompleted){
                    currentNodeCompleted = 1;
                    reCalculateBoundaries(current->tid);
                }
                nextNode = nextTask();
                if(nextNode != NULL){
                    sigprocmask(SIG_UNBLOCK, &sigsetMask, NULL);
                    exit(0);
                }
                if(nextNode != current){
                    if(currentNodeCompleted){
                        setcontext(&(nextNode->context));
                    }
                    else {
                        swapcontext(&(current->context),&(nextNode->context));
                    }
                }
            }

        }
        sigprocmask(SIG_UNBLOCK,&sigsetMask,NULL);
    }

}

void thread_init(long nquantum, int totalProcessInit) {
    totalProcess = totalProcessInit;
    tasks = (struct Thread **) malloc (sizeof(struct Thread*) * totalProcess);
    actualNumberOfProcess = 0;
    if(actualNumberOfProcess == 0){
        sigemptyset(&sigThreadMask);
        sigaddset(&sigThreadMask, SIGPROF); //para mandar sennal cuando expire el timer
        ignorarsennal = 0;

        //TODO: inicializo la cola de threads

        if(tasks != NULL) {

            timeIntervalInMSec = nquantum;

            getNewThreadnoStack(1);

            getcontext(&(tasks[0]->context));

            current = tasks[0];

            memset(&schedulerHandle, 0, sizeof(schedulerHandle));
            schedulerHandle.sa_handler = &scheduler; //http://linux.die.net/man/2/sigaction
            sigaction(SIGPROF, &schedulerHandle, NULL);
            //inicializo el quantum
            quantum.it_value.tv_sec = 0;// http://linux.die.net/man/2/setitimer
            quantum.it_value.tv_usec = timeIntervalInMSec;
            quantum.it_interval.tv_sec=0;
            quantum.it_interval.tv_usec = timeIntervalInMSec;
            setitimer(ITIMER_PROF, &quantum, NULL);
        }

    }
}

int thread_create(thread_t *id, void (*rutina)(void *), int arg) {
   if(tasks != NULL){
        sigprocmask(SIG_BLOCK, &sigThreadMask, NULL);
        struct Thread *newThread = getNewThreadnoStack(50);
        getcontext(&(newThread->context));
        newThread->context.uc_stack.ss_sp = malloc(STACKSIZE);

        newThread->context.uc_stack.ss_size = STACKSIZE;
        newThread->context.uc_stack.ss_flags = 0;
        newThread->hasStackSpace = 1;

        newThread->context.uc_link = &mainContext;

        makecontext(&(newThread->context),rutina,0);

        *id = newThread->tid;

        //TODO:METER EL THREAD A LA LISTA
        tasks[actualNumberOfProcess++] = newThread;

        sigprocmask(SIG_UNBLOCK, &sigThreadMask,NULL);
        return 1;
   }
   return -1;
}

void thread_yield(){
    raise(SIGPROF); //con esto deberia bastar mandando la sennal se captura y cambio segun el scheduler
}

void thread_exit(){
    //deberia terminar el contexto y podriamos y deberiamos tener una estructura para meter a los threads terminados, asi que tambien lo meteria a la lista de terminados
}

thread_t self(){
   return 0; //devuelvo el id del thread actual // pensaria tener una lista y en la estructura de la lista tener el actual. o tener el thread actual por aparte. estoy analizando
}

int thread_join(){
    return 0;//espera a que todos hayan terminado
}






