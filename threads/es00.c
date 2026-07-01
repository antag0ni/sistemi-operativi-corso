#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *print_thread_id(void *arg) {
	int index = *(int *)arg;
	sleep(1);
	printf("Sono il thread di indice %d\n", index);
	return NULL;
}

int main (int argc, char *argv[]) {
	if (argc != 2) {
		printf("Devi specificare un numero.\n");
		return EXIT_FAILURE;
	}

	int n = atoi(argv[1]);
		
	pthread_t tid[n]; //creo n thread
	int arg[n];
	
	for (int i = 0; i < n; i++) {
		arg[i] = i;
		if(pthread_create(&tid[i], NULL, print_thread_id, &arg[i])) {
			printf("Errore nella creazione di un thread\n");
			return EXIT_FAILURE;
		}
	}

	for (int i = 0; i < n; i++)
		pthread_join(tid[i], NULL);

	return EXIT_SUCCESS;
} 
