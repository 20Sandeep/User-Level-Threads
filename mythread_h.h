#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <windows.h>

#define STACK_SIZE 9500
#define MAX_THREADS 10

typedef enum { READY, RUNNING, WAITING, SUSPENDED, TERMINATED } ThreadStatus;

typedef struct {
    int Thread_ID;                 
    HANDLE Thread_handle;    
    ThreadStatus status;     
    int waiting_Thread_ID;         
} MyThread;

void init_thread(int tid);
int mythread_create(void (*func)(void));
void mythread_yield(void);
void mythread_join(int tid);
void mythread_suspend(void);
void mythread_resume(int tid);
void mythread_terminate(void);

#endif 
