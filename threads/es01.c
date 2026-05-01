#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	float *array;
	int n;
} TParam;

void *calcola_radice(void *arg) {
	TParam *param = (TParam *)arg;

	for (int i = 0; i < param->n; i++) {
		param->array[i] = sqrt(param->array[i]);
	}
	return NULL;
}

int main (int argc, char *argv[]) {
	if (argc != 2) {
		printf("Devi specificare un numero.\n");
		return EXIT_FAILURE;
	}

	int n = atoi(argv[1]);

	float array[n];

	for (int i = 0; i < n; i++)
		array[i] = i + 1.0;
	

	pthread_t tid[2];

	TParam param[2];
	param[0].array = array;
	param[0].n = n/2;
	param[1].array = array + n/2;
	param[1].n = n-n/2;

	for (int i = 0; i < 2; i++) {
		if (pthread_create(&tid[i], NULL, calcola_radice, &param[i])) {
			printf("Errore nella creazione di un thread\n");
			return EXIT_FAILURE;
		}
	}

	for (int i = 0; i < 2; i++) {
		pthread_join(tid[i], NULL);
	}

	printf("Risultato: ");
	for (int i = 0; i < n; i++)
		printf("%.2f ", array[i]);
	printf("\n");

	return EXIT_SUCCESS;
} 
