#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <assert.h>
#include "task_t.h"
#include "sched.h"

int nThreads;
int mode;
int *ticketsByThread;
int *workByThread;
int quantum;

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}


void startArrayWork(char **tokens){
	workByThread = (int*)malloc(sizeof(int) * nThreads);
	int i;
        for (i = 0; i < nThreads; i++){
            workByThread[i] = atoi(tokens[i]);
            free(*(tokens + i));
        }
}

void startArrayTickets(char **tokens){
	ticketsByThread = (int*)malloc(sizeof(int) * nThreads);
	int i;
        for (i = 0; i < nThreads; i++){
            ticketsByThread[i] = atoi(tokens[i]);
            free(*(tokens + i));
        }
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
	
        char **tokens = str_split(line, '=');
	
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

				if(!strcmp(tokens[0], "workByThreads")){

					workByT = str_split(tokens[1], ';');
				}else{
					ticketsByT = str_split(tokens[1], ';');
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

void rutina(void){

}

int main(int argc, char * argv[]){

	readFileProperties(argv[1]);
	
	struct sched_t *sch = sched_ls_alloc();
	struct task_t *t = (struct task_t*)malloc(sizeof(struct task_t));
	t->id = 1;
	t->tickets = 100;
	t->lowerLimit = 0;
	t->upperLimit = 100;
	ucontext_t context;
	getcontext(&context);
        context.uc_stack.ss_sp = malloc(16);
        context.uc_stack.ss_size = 16;
        context.uc_stack.ss_flags = 0;
        makecontext(&(context),rutina, 0);
	sch->init(1, 1);
	sch->addTask(t);
	struct task_t *next = sch->nextTask();
	
	int finish;

	printf("Next process to do: %d\n", next->id);
	
	return 0;
}
