#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#include "cond.h"
#include "futex.h"
#include "mutex.h"


#define N_THREADS		3


mutex_t lock;

void task_high() {
	printf("high (before lock)\n");
	mutex_lock(&lock);
	printf("high\n");
	mutex_unlock(&lock);

	printf("end of high\n");
}

void task_med() {
	printf("med\n");

	sleep(2);
	printf("end of med\n");
}

void task_low() {
	printf("low\n");
	mutex_lock(&lock);
	
	sleep(1);
	printf("low goes on\n");
	mutex_unlock(&lock);

	printf("end of low\n");
}

static void mutex_init_wrap(mutex_t *mu) {
	muattr_t muattr;
	muattr_setprotocol(&muattr, PRIO_INHERIT);
	mutex_init(mu, &muattr);
}

void (*tasks[])() = {
	task_high,
	task_med,
	task_low
};

int main(void)
{
	printf("start\n");
	int state;
	struct sched_param param;
	pthread_attr_t attr;
	pthread_t ids[N_THREADS];

	pthread_attr_init(&attr);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	pthread_attr_setinheritsched (&attr, PTHREAD_EXPLICIT_SCHED);   

	mutex_init_wrap(&lock);
	
	for (int i = N_THREADS - 1; i >= 0; --i) {
		printf("%d\n", i);
		param.sched_priority = (N_THREADS - i) * 10;
		pthread_attr_setschedparam(&attr, &param);
		state = pthread_create(&ids[i], &attr, (void *)tasks[i], (void *)NULL);

		if (state)
			printf("thread %d create failed. Error: %d\n", i, state);
		
	}

	for (int i = 0; i < N_THREADS; ++i) {
		pthread_join(ids[i], NULL);
	}



	return EXIT_SUCCESS;
}
