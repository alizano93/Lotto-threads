

#ifndef __SCHED_H__
#define __SCHED_H__
struct task_t;
/* scheduler struct */
struct sched_t {
    struct task_t* (*nextTask) (void);
    void (*addTask)(struct task_t*);
    void (*removeTask)(int);
    
};

/* scheduler alloc functions */
struct sched_t *sched_ls_alloc();
struct sched_t *sched_fifo_alloc();

#endif// __CO_SCHED_H__
