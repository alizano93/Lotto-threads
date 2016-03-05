#include <stdio.h>
#include <sys/ucontext.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define STACKSIZE 1024*8

#ifndef STRUCTURES_H
#define	STRUCTURES_H

typedef long thread_t;

struct Thread{
    thread_t tid;
    ucontext_t context;
    int hasStackSpace;
    int isCompleted;
    int tickets;
	int lowerLimit;
	int upperLimit;
};
void reCalculateBoundaries(thread_t taskToRemove);
struct Thread* getNewThreadnoStack();
struct Thread* getNewThreadwithStack();
struct Thread * nextTask();
//Aqui puede crear las estructuras para manejar el scheduler y los hilos. seria bueno
//tener una estructura para los blocquados, otro para los que estan en cola y otra para los terminados
//y obviamente las funciones para manejarlas

#endif //STRUCTURES_H