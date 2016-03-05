#include <stdio.h>
#include <signal.h>
#include "Structures.h"
#include "myThread.h"

extern int totalProcess;
extern int actualNumberOfProcess;

static void muchoTrabajo(int n){
    printf("imprimo parametro=%d", n);

}


int main(int argc, char * argv[]){

	totalProcess = 0;
	int param = 5;
	//printf("%d",totalProcess);
	thread_init(1000000, 3);
	thread_t it;
	thread_create(&it, muchoTrabajo,param);

	raise(SIGPROF);
	printf("actual %d",actualNumberOfProcess);

	return 0;
}
