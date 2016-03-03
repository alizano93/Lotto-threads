
#include <stdio.h>
#include <stdlib.h>
#include "sched.h"
#include "task_t.h"

struct task_t * nextTask(){
	
	struct task_t * newTask = (struct task_t * ) malloc(sizeof(struct task_t));
	
	return newTask;
}


void addTask(struct task_t *newTask){


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





