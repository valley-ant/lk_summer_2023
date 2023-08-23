#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#include "cond.h"
#include "futex.h"
#include "mutex.h"


#define N_THREADS		3

mutex_t lock;

static void thread_create_with_prio(int prio);

static void mutex_init_wrap(mutex_t *mu) {
	muattr_t muattr;
	muattr_setprotocol(&muattr, PRIO_INHERIT);
	mutex_init(mu, &muattr);
}

int main(void)
{
	pthread_attr_t attr;
	pthread_t thread_table[N_THREADS];

	pthread_attr_init (&attr);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO); 
	pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED);   

	mutex_init_wrap(&lock);

	return EXIT_SUCCESS;
}
