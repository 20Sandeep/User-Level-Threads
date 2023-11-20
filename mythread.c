#include <stdio.h>
#include <windows.h>

#define STACK_SIZE 9500
#define MAX_THREADS 10

typedef enum { READY, RUNNING, WAITING, SUSPENDED, terminated } ThreadStatus;

typedef struct {
    int Thread_ID;                 
    HANDLE Thread_handle;    
    ThreadStatus status;      
    int waiting_Thread_ID;          
} MyThread;

MyThread threads[MAX_THREADS];
int current_thread = -1;      
DWORD WINAPI example_function(LPVOID lpParam) {
    for (int i = 0; i < 5; i++) {
        printf("Thread %d: %d\n", GetCurrentThreadId(), i);
        Sleep(100);  
    }
    return 0;
}

void init_thread(int Thread_ID) {
    threads[Thread_ID].Thread_ID = Thread_ID;
    threads[Thread_ID].status = READY;
    threads[Thread_ID].waiting_Thread_ID = -1;
    threads[Thread_ID].Thread_handle = NULL;
}

int mythread_create(void (*func)(void)) {
    for (int i = 0; i < MAX_THREADS; i++) {
        if (threads[i].status == terminated) {
            init_thread(i);
            threads[i].Thread_handle = CreateThread(NULL, STACK_SIZE, (LPTHREAD_START_ROUTINE)func, NULL, 0, NULL);
            return i;
        }
    }
    return -1;  
}

void mythread_yield(void) {
    int next_thread = (current_thread + 1) % MAX_THREADS;

    while (threads[next_thread].status != READY) {
        next_thread = (next_thread + 1) % MAX_THREADS;
    }

    if (current_thread != -1) {
        threads[current_thread].status = READY;
        threads[current_thread].waiting_Thread_ID = -1;
    }

    threads[next_thread].status = RUNNING;
    current_thread = next_thread;

    WaitForSingleObject(threads[current_thread].Thread_handle, INFINITE);
}

void mythread_join(int Thread_ID) {
    if (threads[Thread_ID].status != terminated) {
        threads[current_thread].status = WAITING;
        threads[current_thread].waiting_Thread_ID = Thread_ID;
        mythread_yield();
    }
}

void mythread_suspend(void) {
    threads[current_thread].status = SUSPENDED;
    mythread_yield();
}

void mythread_resume(int Thread_ID) {
    if (threads[Thread_ID].status == SUSPENDED) {
        threads[Thread_ID].status = READY;
    }
}

void mythread_terminate(void) {
    threads[current_thread].status = terminated;
    mythread_yield();
}

int main() {
    int Thread_ID1 = mythread_create((void (*)(void))example_function);
    int Thread_ID2 = mythread_create((void (*)(void))example_function);

    WaitForSingleObject(threads[Thread_ID1].Thread_handle, INFINITE);
    WaitForSingleObject(threads[Thread_ID2].Thread_handle, INFINITE);

    printf("All threads ended.\n");

    return 0;
}
