/*
 * Scrivere in C un programma che prenda come argomenti una lista di interi
 * e crea 3 thread. La lista viene passata in input ad ognuno dei thread.
 *
 * I thread calcolano rispettivamente il minimo, il massimo e la media degli
 * elementi della lista.
 *
 * Il main, infine, stampa a video i risultati dei 3 thread.
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NTHREADS 3

typedef struct{
    float *array;
    int size;
    float result;
}TParam;

void print_array(float * array, int n);

void *thread_min(void *arg) {
    TParam *p = (TParam *)arg;
    p->result = p->array[0];
    for (int i = 0; i < p->size; i++) {
        if (p->array[i] < p->result)
            p->result = p->array[i];
    }
    return NULL;
}

void *thread_max(void *arg) {
    TParam *p = (TParam *)arg;
    p->result = p->array[0];
    for (int i = 0; i < p->size; i++) {
        if (p->array[i] > p->result)
            p->result = p->array[i];
    }
    return NULL;
}

void *thread_avg(void *arg) {
    TParam *p = (TParam *)arg;
    float sum = 0;
    for (int i = 0; i < p->size; i++) {
        sum += p->array[i];
    }        
    p->result = sum / p->size;
    return NULL;
}


int main (int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Provide list\n");
        return EXIT_FAILURE;
    }

    int n = argc - 1;
    float * array = (float*)malloc(n * sizeof(float));
    if (array == NULL) {
        perror("Errore nella malloc");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++)
        array[i] = atof(argv[i+1]);

    pthread_t tid[NTHREADS];
    TParam param[NTHREADS];

    for (int i = 0; i < NTHREADS; i++) {
        param[i].size = n;
        param[i].array = array;
    }

    if (    pthread_create(&tid[0], NULL, thread_min, &param[0]) != 0 ||
            pthread_create(&tid[1], NULL, thread_max, &param[1]) != 0 ||
            pthread_create(&tid[2], NULL, thread_avg, &param[2]) != 0       ) {
        perror("Errore nella creazione di un thread");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < NTHREADS; i++)
        pthread_join(tid[i], NULL);

    print_array(array, n);

    printf("MIN: %.2f,\tMAX: %.2f,\t AVG: %.2f\n", param[0].result, param[1].result, param[2].result);
    
    free(array);
    return EXIT_SUCCESS;
}

void print_array(float * array, int n) {
    printf("Array: [");
    for (int i = 0; i < n - 1; i++) {
        printf("%.2f, ", array[i]);
    }
    printf("%.2f]\n", array[n-1]);
}