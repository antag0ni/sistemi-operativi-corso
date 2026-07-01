/*
 * 1. Scrivere in C un programma che prenda un numero intero n come parametro 
 *    sulla linea di comando, e costruisca un array di n float con i valori 1, 2,...,n.
 * 
 * 2. Il programma crea due thread secondari, a ciascuno dei quali passa metà 
 *    dell'array (il primo thread riceve gli elementi di indice da 0 a n/2 escluso, 
 *    e il secondo gli elementi di indice da n/2 a n escluso).
 * 
 * 3. Ciascun thread secondario calcola, per ciascun elemento dell'array che ha 
 *    ricevuto, la radice quadrata (usando sqrt) e la memorizza nello stesso 
 *    elemento dell'array.
 * 
 * 4. Il thread principale aspetta la terminazione dei due thread secondari e 
 *    stampa il valore modificato dell'array.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct {
    float *array;
    int n;
} TParam;

void print_array(float * array, int n) {
    printf("Array: ");
    for (int i = 0; i < n; i++)
        printf("%.2f\t", array[i]);
    printf("\n");
}

void *thread_func(void *arg) {
    TParam *p = (TParam *)arg;
    int n = p->n;

    float *array = p->array;  
    
    for (int i = 0; i < n; i++)
        array[i] = sqrt(array[i]);
    
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <int>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);

    float *float_array = (float*)malloc(n * sizeof(float));
    if (float_array == NULL) {
        perror("[!] Errore nella malloc\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < n; i++)
        float_array[i] = i + 1;

    print_array(float_array, n);

    pthread_t tid[2];

    TParam p0, p1;
    p0.array = float_array;
    p1.array = float_array + n/2;
    p0.n = n/2;
    p1.n = n - n/2;

    if (pthread_create(&tid[0], NULL, thread_func, &p0) != 0) {
        perror("[!] Errore nella creazione del thread\n");
        return EXIT_FAILURE;
    } 
    if (pthread_create(&tid[1], NULL, thread_func, &p1) != 0) {
        perror("[!] Errore nella creazione del thread\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(tid[i], NULL);
    }
    
    print_array(float_array, n);
    
    free(float_array);
    
    return EXIT_SUCCESS;
}