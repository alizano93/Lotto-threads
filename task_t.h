#ifndef __UTASK__
#define __UTASK__
#include <ucontext.h>
struct task_t{

	int id;
	int tickets;
	int lowerLimit;
	int upperLimit;
	ucontext_t context;
	int finish;
	
};

#endif//__UTASK__
