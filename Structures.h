#include <stdio.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <uniststd.h>
#include <string.h>
#include <stdlib.h>

#define STACKSIZE 1024*8

typedef long thread_t;

typedef struct Thread{
    thread_t tid;
    ucontext_t context;
    struct ThreadList *blockedThreads; // por si tenemos threads bloqueados; puede que no lo utilicemos, esta seria la lista
    int hasStackSpace;
    int isCompleted;
    int isBlocked;
} *Thread;

typedef struct ThreadList {
    Thread node;
    struct ThreadList *next;
} *ThreadList;

Thread getNewThreadnoStack();
Thread getNewThreadwithStack();
//Aqui puede crear las estructuras para manejar el scheduler y los hilos. seria bueno
//tener una estructura para los blocquados, otro para los que estan en cola y otra para los terminados
//y obviamente las funciones para manejarlas
