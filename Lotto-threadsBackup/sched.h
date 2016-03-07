

#ifndef __SCHED_H__
#define __SCHED_H__

#include "Structures.h"
enum{EXPROPIATIVO, NO_EXPROPIATIVO};
/* scheduler struct */
struct sched_t {
    void (*addTask)(struct task_t*);
    void (*removeTask)(int);
    void (*manageTimer)();
    void (*init)(int, int);
    
};

/* scheduler alloc functions */
struct sched_t *sched_ls_alloc();
struct sched_t *sched_fifo_alloc();

#endif// __CO_SCHED_H__
