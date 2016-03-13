
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Structures.h"
#include "myThread.h"
#include "gtk_ui.h"

int totalProcess;
int actualNumberOfProcess;
int totalTickets;
int mode;
Thread **tasks;
extern ucontext_t mainContext;
extern state *states;
Thread *current;
ucontext_t *contexts;
int completed;
int currentId;
void init(int totalProcessInit, int modeIn){

	totalProcess = totalProcessInit;
	tasks = (Thread **) malloc (sizeof(Thread*) * totalProcess);
	contexts = (ucontext_t *)malloc(sizeof(ucontext_t) * totalProcess);		
	actualNumberOfProcess = 0;
	mode = modeIn;
	srand (time(NULL));
	completed = 0;
}	

void reCalculateBoundaries(char *taskToRemove){

	Thread **auxArray = (Thread **) malloc((actualNumberOfProcess - 1) * sizeof(Thread *));
	
	int x, dx = 0;
	int lowerLimit = 0;
	int upperLimit = 0;
	for(x = 0; x < actualNumberOfProcess; x++){
		
		if(tasks[x]->tid != taskToRemove){

			Thread *actual = tasks[x];
			actual->lowerLimit = lowerLimit;
			actual->upperLimit = lowerLimit + (actual->tickets - 1);
			lowerLimit = actual->upperLimit + 1;
			Thread *newTask = (Thread *) malloc(sizeof(Thread));
			memcpy(newTask, actual, sizeof(Thread));			
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

Thread * nextTask(){
	
	int limiteInferior = 0;
	int limiteSuperior = actualNumberOfProcess - 1;

	int winnerTicket = rand() % totalTickets; //Sacar numero random
	//binary search to find the winner
	while(limiteInferior <= limiteSuperior){

		int medio = (limiteInferior + limiteSuperior)/2;
		Thread *actual = tasks[medio];
		if(winnerTicket < actual->lowerLimit){
			
			limiteSuperior = medio - 1;
				
		}else{
			if(winnerTicket > actual->upperLimit){
				
				limiteInferior = medio +1;
			
			}else{
				current = actual;
				return actual;
			}
		}
			
	}	

	return NULL;
	
}
	
void run(){
	
	activeTimer();
	while(actualNumberOfProcess > 0){
		
		current = nextTask(); 
		currentId = atoi(current->tid) - 1;
		states[currentId].active = 1;
		swapcontext(&mainContext, &contexts[currentId]);
		states[currentId].active = 0;	
		if(states[currentId].finish){
			completed++;			
			reCalculateBoundaries(current->tid); 
		}
	}
	//stopTimer(timer);
}



void addTask(Thread *newTask, ucontext_t context){

	tasks[actualNumberOfProcess] = newTask;
	contexts[actualNumberOfProcess] = context;
	totalTickets += newTask->tickets;
	int lowerBoundary = 0;
	if(actualNumberOfProcess == 0){
		lowerBoundary = 0;
	}else{
		lowerBoundary = tasks[actualNumberOfProcess - 1]->upperLimit + 1;
	}
	newTask->lowerLimit = lowerBoundary;
	newTask->upperLimit = lowerBoundary + (newTask->tickets - 1);
	actualNumberOfProcess++;
}


void removeTask(int idTask){

}

void manageTimer(){
		swapcontext(&contexts[currentId], &mainContext);
}

void updateWork(float percent, double result){
//	update_row_work(current->tid, percent, result);
}


struct sched_t * sched_ls_alloc(int totalProcessInit, int modeIn){

	
	struct sched_t * ls = (struct sched_t * ) malloc(sizeof(struct sched_t));
	ls->nextTask = nextTask;
	ls->addTask = addTask;
	ls->removeTask = removeTask;
	ls->manageTimer = manageTimer;
	ls->init = init;
	ls->run = run;
	ls->init(totalProcessInit, modeIn);
	
	return ls;
}
