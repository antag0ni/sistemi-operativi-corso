/*
 * Programma che crea n thread, dove n è passato come parametro da riga di comando.
 * Ogni thread stampa il proprio indice progressivo da 0 a n-1.
 *
 * Compilazione: gcc nome_file.c -o nome_programma -lpthread
 * Esecuzione: ./nome_programma <numero_thread>
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *thread_func(void *arg) {
    int id = *(int*)arg;
    sleep(1);
    printf("Sono il thread [%d]\n", id);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <int>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);

    pthread_t tid[n];
    int arg[n];

    for (int i = 0; i < n; i++)
    {   
        arg[i] = i;
        if (pthread_create(&tid[i], NULL, thread_func, &arg[i]) != 0) {
            perror("Errore nella creazione del thread\n");
            return EXIT_FAILURE;
        }    
    }

    for (int i = 0; i < n; i++)
        pthread_join(tid[i], NULL);

    return EXIT_SUCCESS;
}