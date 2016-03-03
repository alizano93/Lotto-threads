#include <stdio.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <uniststd.h>
#include <string.h>
#include <stdlib.h>

typedef long thread_t;

typedef struct Thread{
    thread_t tid;
    ucontext_t context;
    struct ThreadBlocked *blocked; // por si tenemos threads bloqueados; puede que no lo utilicemos, esta seria la lista
    int hasStackSpace;
    int isCompleted;
    int isBlocked;
};

//Aqui puede crear las estructuras para manejar el scheduler y los hilos. seria bueno
//tener una estructura para los blocquados, otro para los que estan en cola y otra para los terminados
//y obviamente las funciones para manejarlas
