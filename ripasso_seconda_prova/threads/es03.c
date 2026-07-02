/*
 * Scrivere in C un programma che prenda come argomenti una lista di
 * interi positivi e crea NTHREADS thread, dove NTHREADS è una costante.
 * Gli argomenti sono divisi in NTHREADS parti, ognuna delle quali
 * passata come input ad un thread.
 *
 * Ogni thread calcola la funzione fibonacci per ognuno degli interi in
 * input.
 *
 * Il main stampa a video la lista ordinata (successione di fibonacci)
 * contenente i risultati calcolati dai thread.
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NTHREADS 3

typedef struct{
    int *array;
    int size;
}TParam;

void print_array(int * array, int n);
int fib(int n);
void *thread_func(void *arg);
int comp_int(const void *a, const void *b);

int main (int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Provide list\n");
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        if (atoi(argv[i]) < 0) {
            fprintf(stderr, "Provide only positive integers\n");
            return EXIT_FAILURE;    
        }
    }

    int n = argc - 1;

    int * array = (int*)malloc(n * sizeof(int));
    if (array == NULL) {
        perror("ERROR IN MALLOC");
        return EXIT_FAILURE;
    }

    //PARAMETRI
    TParam param[NTHREADS];

    // DISTRIBUIRE IL CARICO
    int size = n / NTHREADS;
    param[0].size = size + (n % NTHREADS);
    for (int i = 1; i < NTHREADS; i++)
        param[i].size = size;     

    // POPOLARE ARRAY THREADS
    int offset = 0;
    for (int i = 0; i < NTHREADS; i++) {   

        param[i].array = (int*)malloc(param[i].size * sizeof(int));
        if (param[i].array == NULL) {
            perror("Errore nella malloc (thread array)");
            return EXIT_FAILURE;
        }

        for (int j = 0; j < param[i].size; j++) {
                param[i].array[j] = atoi(argv[j+1+offset]);
        }
        offset += param[i].size;
    }

    //THREADS
    pthread_t tid[NTHREADS];
    for (int i = 0; i < NTHREADS; i++)
    {
        if (pthread_create(&tid[i], NULL, thread_func, &param[i]) != 0) {
            perror("Errore nella creazione del thread");
            return EXIT_FAILURE;
        }
    }
    for (int i = 0; i < NTHREADS; i++)
        pthread_join(tid[i], NULL);

    // RECUPERO RISULTATI DAI THREAD
    offset = 0;
    for (int i = 0; i < NTHREADS; i++) {
        for (int j = 0; j < param[i].size; j++) {
            array[j+offset] = param[i].array[j];
        }
        offset += param[i].size;
    }

    qsort(array, n, sizeof(int), comp_int);
   
    print_array(array, n);

    for (int i = 0; i < NTHREADS; i++)
        free(param[i].array);
    
    free(array);
    return EXIT_SUCCESS;
}

void *thread_func(void *arg) {
    TParam *p = (TParam *)arg;
    int size = p->size;
    for (int i = 0; i < size; i++)
        p->array[i] = fib(p->array[i]);
    return NULL;
}

void print_array(int * array, int n) {
    printf("Array: [");
    for (int i = 0; i < n - 1; i++) {
        printf("%d, ", array[i]);
    }
    printf("%d]\n", array[n-1]);
}

int fib(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    int prev1 = 0, prev2 = 1, current;
    for (int i = 2; i <= n; i++) {
            current = prev1 + prev2;
            prev1 = prev2;
            prev2 = current;
    }
    return current;
}

int comp_int(const void *a, const void *b) {
    return *(int*)a - *(int*)b;
}