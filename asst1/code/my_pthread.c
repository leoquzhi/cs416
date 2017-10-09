// File:	my_pthread.c
// Author:	Yujie REN
// Date:	09/23/2017

// name:
// username of iLab:
// iLab Server:

#include "my_pthread_t.h"

struct thread_node currentNode;
struct queue *waitingQueue;
struct queue *runningQueue[3] = {NULL, NULL, NULL};

void init()
{

	currentNode = (struct thread_node *)malloc(sizeof(struct thread_node));
	waitingQueue = (struct queue *)malloc(sizeof(struct queue));
 	for (i = 0; i < 3; i++)
    {
      runningQueue[i] = (struct queue *)malloc(sizeof(struct queue));
      runningQueue[i]->head = NULL;
    }


}


/* create a new thread */
int my_pthread_create(my_pthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg) {
	
//	Iterate through the running queue and stop when we reach a NULL value
	thread_node* ptr = scheduler->running_queue;
	while (ptr != NULL) {
		ptr = ptr->next;
	}
	
//	Malloc some space and create a new thread at the end of the running queue.
	ptr = malloc(sizeof(my_pthread));
	getcontext(&(ptr->thread.context));
	ptr->thread.context.uc_link = 0;
	
//	Which signals do we want to block?
//	ptr->context.uc_sigmask = 

//	Initializes a stack for the new thread with size 64000 bytes
	ptr->thread.context.uc_stack.ss_sp=malloc(64000);
	ptr->thread.context.uc_stack.ss_size=64000;
	ptr->thread.context.uc_stack.ss_flags=0;
	
//	Sets the pid of the new thread to be the first argument given
	ptr->thread.pid = *thread;
	
//	Make a new context. We assume the function has 0 arguments.
	makecontext(&(ptr->thread.context), function, 0);
	ptr->thread.priority_level = 1;
	ptr->thread.priority = 1000;
	struct itimerval* timer = malloc(sizeof(struct itimerval));
	getitimer(ITIMER_REAL, timer);
	if (timer->it_interval != 25) {
		timer->it_interval = 25;
		timer->it_value = 25;
		setitimer(ITIMER_REAL, timer, NULL);
	}
	return 0;
};

int execute() {
	if (scheduler->current_thread.priority_level == 1) {
		scheduler->current_thread.priority_level = 2;
	} else if (scheduler->current_thread.priority_level == 2) {
		
	}
	return 0;
}

/* give CPU pocession to other user level threads voluntarily */
int my_pthread_yield() {
	//Get current_thread ID because you can only yield if you're the currently running thread
	//Move from the running queue to the waiting queue
	
	return 0;
};

/* terminate a thread */
void my_pthread_exit(void *value_ptr) {
	//Same as yield, except end the thread
};

/* wait for thread termination */
int my_pthread_join(my_pthread_t thread, void **value_ptr) {
	//Same as yield, just also check to see if the other thread has finished yet
	/*
	my_pthread* ptr = scheduler.running_queue;
	my_pthread* prev = NULL;
	//Remove the thread from the running queue
	while (ptr->thread.pid != scheduler.current_thread && ptr != NULL) {
		prev = ptr;
		ptr = ptr->next;
	}
	if (prev == NULL) {
		scheduler.running_queue = NULL;
	} else {
		prev->next = ptr->next;
	}
	*/
	
	//Remove from the running queue
	my_pthread_yield();
	
	//Wait for the other thread to finish executing
	finished_executing = 0;
	while (finished_executing == 0) {
		my_pthread* ptr = scheduler.running_queue;
		while (ptr->next != NULL) {
			if (ptr->thread.pid == thread) {
				my_pthread_exit(NULL);
			}
		}
	}
	return 0;
};

/* initial the mutex lock */
int my_pthread_mutex_init(my_pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr) {
	mutex->mutex_lock = malloc(sizeof(lock));
	mutex->mutex_lock->value = 0;
	return 0;
};

/* aquire the mutex lock */
int my_pthread_mutex_lock(my_pthread_mutex_t *mutex) {
	while (1) {
		if (mutex->mutex_lock->value == 0) {
			mutex->mutex_lock->value = 1;
			mutex->pid = scheduler->current_thread;
			return 0;
		}
	}
	return 0;
};

/* release the mutex lock */
int my_pthread_mutex_unlock(my_pthread_mutex_t *mutex) {
	if (scheduler->current_thread == mutex->mutex_lock->pid) {
		mutex->mutex_lock->value = 0;
		mutex->pid = -1;
	}
	return 0;
};

/* destroy the mutex */
int my_pthread_mutex_destroy(my_pthread_mutex_t *mutex) {
	while (1) {
		if (mutex->mutex_lock->value == 0) {
			free(mutex->mutex_lock);
			return 0;
		}
	}
	return 0;
};

