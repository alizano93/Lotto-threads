
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <assert.h>
#include <signal.h>
#include "Structures.h"
#include "myThread.h"
#include "task_t.h"

int nThreads;
int mode;
int *ticketsByThread;
int *workByThread;
int quantum;


extern int totalProcess;
extern int actualNumberOfProcess;

static void muchoTrabajo(int n){
    printf("imprimo parametro=%d", n);

}
//O(3n)

char **splitString(char *str, const char separator){

	int count = 1;
	int length = strlen(str);
	int x;
	for(x = 0; x < length; x++){

		if(str[x] == separator){

			count++;
		}
	}

	int sizes[count];
	int lowerBound = 0;
	int c;
	
	for(x = 0, c = 0; x < length; x++){

		if(str[x] == separator){

			sizes[c] = (x - lowerBound);
			lowerBound = x + 1;
			c++;
		}
	}	
	sizes[c] = (x - lowerBound);
	
	int j = 0;
	char **tokens = malloc(sizeof(char*) * count);
	x = 0;
	
	while(j < count){

		int sizeWord = sizes[j];
		char *newWord = malloc(sizeof(char) * (sizeWord + 1));
		int k, t;
		for(k = 0, t = 0; k < sizeWord; k++){
			newWord[t] = str[x];
			t++;
			x++;
		}
		if(newWord[k-1] == '\n'){
			newWord[k-1] = '\0';
		}else{
			newWord[k] = '\0';
		}
		x++;//jump the separator
		tokens[j] = newWord; 
		j++;
	}
	return tokens;

}

void startArrayWork(char **tokens){
	workByThread = (int*)malloc(sizeof(int) * nThreads);
	int i;
        for (i = 0; i < nThreads; i++){
		
            workByThread[i] = atoi(tokens[i]);
        }
	free(tokens);
}

void startArrayTickets(char **tokens){
		
	ticketsByThread = (int*)malloc(sizeof(int) * nThreads);
	int i;
        for (i = 0; i < nThreads; i++){
            ticketsByThread[i] = atoi(tokens[i]);
        }
	free(tokens);
}	

void readFileProperties(char *path){

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(path, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    char **workByT;
    char **ticketsByT;
	
    while ((read = getline(&line, &len, fp)) != -1) {
        char **tokens = splitString(line, '=');

	if(!strcmp(tokens[0], "mode")){

		if(!strcmp(tokens[1], "EXPROPIATIVO")){
			mode = EXPROPIATIVO;
		}else{
			mode = NO_EXPROPIATIVO;		
		}
		
	}else{
		
		if(!strcmp(tokens[0], "nThreads")){

			nThreads = atoi(tokens[1]);
			free(tokens);
		}else{
			
			if(!strcmp(tokens[0], "quantum")){

				quantum = atoi(tokens[1]);
				free(tokens);
			}else{

				if(!strcmp(tokens[0], "workByThread")){
					workByT = splitString(tokens[1], ';');
				}else{
					ticketsByT = splitString(tokens[1], ';');
				}
			}
			
			
		}
		
	}
    }
	 
    startArrayWork(workByT);
    startArrayTickets(ticketsByT);

    free(line);
    fclose(fp);
}

int main(int argc, char * argv[]){
	
	readFileProperties(argv[1]);

	struct sched_t *sch = sched_ls_alloc(nThreads, mode); //creating scheduler	

	thread_init(quantum, nThreads, sch); //creating thread lib
	
	int j;

	for(j = 0; j < nThreads; j++){
		thread_create(j, ticketsByThread[j] , muchoTrabajo, j + 1);	//creating threads	
	}
	thread_join();
	//thread_yield();
	
	return 0;
}
