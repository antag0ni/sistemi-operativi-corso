#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREADS 3

typedef struct {
	int number;
	int result;
} TParam;

int factorial(int n) {
	if (n<=1)
		return 1;
	return n * factorial(n-1);
}

void *factorial_thread(void *arg) {
	TParam *p = (TParam *)arg;
	p->result = factorial(p->number);
	return NULL;
}

int main (int argc, char *argv[]) {
	if (argc != 3) {
		printf("Devi specificare due numeri.\n");
		return EXIT_FAILURE;
	}

	int n = atoi(argv[1]);
	int k = atoi(argv[2]);

	pthread_t tid[THREADS];
	TParam param[THREADS];
	param[0].number = n;
	param[1].number = k;
	param[2].number = n-k;

	for (int i = 0; i < THREADS; i++) {
		if (pthread_create(&tid[i], NULL, factorial_thread, &param[i])) {
			printf("Errore nella creazione di un thread\n");
			return EXIT_FAILURE;
		}
	}

	for (int i = 0; i < THREADS; i++) {
		pthread_join(tid[i], NULL);
	}

	int n_fact = param[0].result;
	int k_fact = param[1].result;
	int n_minus_k_fact = param[2].result;

	int binomial_coeff = n_fact / (k_fact * n_minus_k_fact);

	printf("Il coefficiente binomiale C(%d, %d) e': %d\n", n, k, binomial_coeff);

	return EXIT_SUCCESS;
} 
