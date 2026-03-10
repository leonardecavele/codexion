#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define THREADS	2

typedef struct s_safe_thread
{
	pthread_t		thread;
	int				*n;
	pthread_mutex_t	*mutex;
}	t_safe_thread;

void	*func(void *arg)
{
	t_safe_thread	*t = (t_safe_thread *)arg;

	pthread_mutex_lock(t->mutex);
	printf("receive: %d\n", *t->n);
	*t->n = (rand() % 42) + 1;
	printf("changed to: %d\n", *t->n);
	pthread_mutex_unlock(t->mutex);

	return (NULL);
}

int	main(void)
{
	t_safe_thread		threads[THREADS];
	pthread_mutex_t		mutex;
	int					n;

	srand(time(NULL));

	n = 2;
	pthread_mutex_init(&mutex, NULL);

	printf("before n: %d\n", n);

	for (int i = 0; i < THREADS; i++)
	{
		threads[i].n = &n;
		threads[i].mutex = &mutex;
	}
	for (int i = 0; i < THREADS; i++)
		pthread_create(&threads[i].thread, NULL, func, &threads[i]);

	for (int i = 0; i < THREADS; i++)
		pthread_join(threads[i].thread, NULL);
	pthread_mutex_destroy(&mutex);

	printf("after n: %d\n", n);

	return (0);
}
