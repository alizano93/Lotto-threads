
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <assert.h>
#include <signal.h>
#include <math.h>
#include "Structures.h"
#include "myThread.h"
#include "task_t.h"
#include "gtk_ui.h"

int nThreads;
int mode;
int *ticketsByThread;
int *workByThread;
int quantum;


extern int totalProcess;
extern int actualNumberOfProcess;

double arcsin(double x, int n)
{
    //Agregado por Andres
    int n_torelease= n * quantum / 100;
    //Fin
    double fact,sum;
    fact=sum=1.0;
    int i;
    for(i=1;i<=n;i++)
    {
    if(mode != EXPROPIATIVO && i % n_torelease == 0){
        raise(SIGPROF);
    }
	fact *= (2 * (double)i - 1)/(2 * (double)i );
	double po = pow(x,(2*(double)i+1));
	double factwo = po / (2*(double)i+1);
	sum += fact * (pow(x,(2*(double)i+1)) / (2*(double)i+1));

	//Agregado por Daniel
	float percent = (float)i / (float)n;
//	updateThread(percent, sum);
	//Fin
    }
    return sum;
}

static void trabajo(int n){
    printf("resultado final = 2 arcsin (1) = %lf \n", 2*arcsin(1.0,n*50));
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
	
	int size = sizeof(tokens) / sizeof(*tokens); //This to let receive parameters in any order.
	//workByThread = (int*)malloc(sizeof(int) * nThreads);
	workByThread = (int*)malloc(sizeof(int) * size);
	int i;
        for (i = 0; i < nThreads; i++){
		
            workByThread[i] = atoi(tokens[i]);
        }
	free(tokens);
}

void startArrayTickets(char **tokens){
		
	int size = sizeof(tokens) / sizeof(*tokens);//This to let receive parameters in any order.
	
	//ticketsByThread = (int*)malloc(sizeof(int) * nThreads);
	ticketsByThread = (int*)malloc(sizeof(int) * size);
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

	gtk_init(NULL, NULL);

	struct sched_t *sch = sched_ls_alloc(nThreads, mode); //creating scheduler	

	thread_init(quantum, nThreads, sch); //creating thread lib

		

//	create_UI(nThreads);
//	show_ui();
	
	
	int j;

	for(j = 0; j < nThreads; j++){
		int t_id = j + 1;
		thread_create(t_id, ticketsByThread[j] , trabajo, workByThread[j]);	//creating threads
//		add_row(t_id);
	}

	//show_ui();
	

	thread_join();

//	gtk_main();// LOOP CONFLICT WITH JOIN????

	
	//thread_yield();
	
	return 0;
}
