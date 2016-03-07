#include <stdio.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define STACKSIZE 1024*8

#ifndef STRUCTURES_H
#define	STRUCTURES_H

typedef long thread_t;

typedef struct Thread{
    char *tid;
    ucontext_t context;
    int tickets;
    int lowerLimit;
    int upperLimit;
    int finish;
    float percent;
    double result;

}Thread;

struct Thread* getNewThreadnoStack(int, char*);
struct Thread* getNewThreadwithStack();
//Aqui puede crear las estructuras para manejar el scheduler y los hilos. seria bueno
//tener una estructura para los blocquados, otro para los que estan en cola y otra para los terminados
//y obviamente las funciones para manejarlas


enum{EXPROPIATIVO, NO_EXPROPIATIVO};
/* scheduler struct */
struct sched_t {

    Thread *(*nextTask) (void);
    void (*addTask)(Thread *);
    void (*removeTask)(int);
    void (*manageTimer)();
    void (*init)(int, int);
    //void (*run)();
    void (*updateWork)(float, double);
    void(*updateCurrent)();
    void (*switchContext)();
    
};

/* scheduler alloc functions */
struct sched_t *sched_ls_alloc();
struct sched_t *sched_fifo_alloc();


#endif //STRUCTURES_H
