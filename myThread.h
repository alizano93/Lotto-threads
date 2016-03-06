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

#ifndef MY_THREAD_H
#define MY_THREAD_H
//typedef long thread_t;

//Inicializa la libreria
//inicializa la lista de hilos
//debe llamarse antes de cualquier otro metodo
void thread_init(long nquantum, int totalProcessInit, struct sched_t *);

//creo el contexto, lo meto a la cola de los threads devuelvo 1 si funciono todo
int thread_create(thread_t id, int tickets, void (*rutina)(int), int arg);

//con esto deberia bastar mandando la sennal se captura y cambio segun el scheduler
void thread_yield();

//deberia terminar el contexto y podriamos y deberiamos tener una estructura para meter a los threads terminados, asi que tambien lo meteria a la lista de terminados
void thread_exit();

//devuelvo el id del thread actual // pensaria tener una lista y en la estructura de la lista tener el actual. o tener el thread actual por aparte. estoy analizando
thread_t self();

//espera a que todos hayan terminado
int thread_join();

static void muchoTrabajo(int n);

//actualiza el porcentaje de progreso y el resultado temporal (y final) para cada termino del calculo
void updateThread(float percent, double result);

#endif //MY_THREAD_H

