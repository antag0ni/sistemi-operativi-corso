#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 2
#define START1 1
#define END1 50
#define START2 51
#define END2 100

typedef struct {
	int start;
	int end;
	int *psomma;
	pthread_mutex_t *mutex;
} TParam;

void *calcola_somma(void *arg);

int main () {
	int somma = 0;
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);
	pthread_t tid[THREADS];
	TParam param[THREADS];
	param[0].start = START1;
	param[0].end = END1;
	param[0].psomma = &somma;
	param[0].mutex = &mutex;
	param[1].start = START2;
	param[1].end = END2;
	param[1].psomma = &somma;
	param[1].mutex = &mutex;

	for (int i = 0; i < THREADS; i++) {
		if (pthread_create(&tid[i], NULL, calcola_somma, &param[i]) != 0) {
			printf("Errore nella creazione di un thread.\n");
			return EXIT_FAILURE;
		}
	}
	for (int i = 0; i < THREADS; i++)
		pthread_join(tid[i], NULL);

	printf("La somma è: %d\n", somma);

	pthread_mutex_destroy(&mutex);
	return EXIT_SUCCESS;
}

void *calcola_somma(void *arg) {
	TParam *p = (TParam *)arg;
	int s = 0;
	for (int i = p->start; i <= p->end; i++)
		s += i;
	pthread_mutex_lock(p->mutex);
	*p->psomma += s;
	pthread_mutex_unlock(p->mutex);
	return NULL;
}
