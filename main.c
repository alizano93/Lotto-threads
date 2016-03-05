#include <stdio.h>
#include "Structures.h"
#include "myThread.h"

extern int totalProcess;

int main(int argc, char * argv[]){

	totalProcess = 0;
	printf("%d",totalProcess);
	thread_init(10000, 2);
	printf("%d",totalProcess);

	return 0;
}
