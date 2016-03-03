
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sched.h"
#include "task_t.h"


int totalProcess;
int actualNumberOfProcess;
int totalTickets;
struct task_t **tasks;
ucontext_t uctx_main;

void init(int totalProcessInit){

	totalProcess = totalProcessInit;
	tasks = (struct task_t **) malloc (sizeof(struct task_t*) * totalProcess);		
	actualNumberOfProcess = 0;
}	

void reCalculateBoundaries(int taskToRemove){

	struct task_t **auxArray = (struct task_t **) malloc((actualNumberOfProcess - 1) * sizeof(struct task_t *));
	
	int x, dx = 0;
	int lowerLimit = 0;
	int upperLimit = 0;
	for(x = 0; x < actualNumberOfProcess; x++){
		
		if(tasks[x]->id != taskToRemove){

			struct task_t *actual = tasks[x];
			actual->lowerLimit = lowerLimit;
			actual->upperLimit = lowerLimit + actual->tickets;
			lowerLimit = actual->upperLimit + 1;
			struct task_t *newTask = (struct task_t *) malloc(sizeof(struct task_t));
			memcpy(newTask, actual, sizeof(struct task_t));
			auxArray[dx] = newTask;
			dx++;
		}
		
	}
	actualNumberOfProcess--;
	free(tasks);
	tasks = auxArray;
}

struct task_t * nextTask(){
	
	int limiteInferior = 0;
	int limiteSuperior = actualNumberOfProcess;

	int winnerTicket = 01; //Sacar numero random
	//binary search to find the winner
	while(limiteInferior <= limiteSuperior){

		int medio = (limiteInferior + limiteSuperior)/2;
		
		struct task_t *actual = tasks[medio];
		
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

void run(){

	while(actualNumberOfProcess > 0){

		struct task_t *currentTask = nextTask();
		swapcontext(&uctx_main, &currentTask->context);
		if(currentTask->finish){			
			reCalculateBoundaries(currentTask->id);
		}

	}

}



void addTask(struct task_t *newTask){
	tasks[actualNumberOfProcess] = newTask;
	totalTickets += newTask->tickets;
	int lowerBoundary = 0;
	if(actualNumberOfProcess == 0){
		lowerBoundary = 0;
	}else{
		lowerBoundary = tasks[actualNumberOfProcess - 1]->upperLimit + 1;
	}
	newTask->lowerLimit = lowerBoundary;
	newTask->upperLimit = lowerBoundary + newTask->tickets;
}


void removeTask(int idTask){

}


struct sched_t *sched_ls_alloc(){

	struct sched_t * ls = (struct sched_t * ) malloc(sizeof(struct sched_t));
	ls->nextTask = nextTask;
	ls->addTask = addTask;
	ls->removeTask = removeTask;
	
	return ls;
}


int main(){

	

}


