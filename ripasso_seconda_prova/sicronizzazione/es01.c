/*
 * Realizzare un programma che crea due thread secondari. Il primo thread
 * secondario deve calcolare la somma dei numeri interi da 1 a 50, e il
 * secondo la somma dei numeri interi da 51 a 100. Entrambi i thread
 * secondari devono aggiungere la somma calcolata a una variabile somma
 * (inizializzata a 0) fornita dal thread principale.
 *
 * Il thread principale aspetta la terminazione dei due thread secondari
 * e stampa il valore della variabile somma.
 *
 * I thread secondari devono gestire correttamente la sincronizzazione
 * nell'accedere alla variabile condivisa somma.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREADS 2

#define START1 0
#define END1 50
#define START2 51
#define END2 100

typedef struct {
    int start;
    int end;
    int *psomma;
    pthread_mutex_t *mutex;
} TParam;


void *thread_func(void *arg) {
    TParam *p = (TParam *)arg;
    int tmp = 0;
    for (int i = p->start; i <= p->end; i++)
        tmp += i;
    pthread_mutex_lock(p->mutex);
    *(p->psomma) += tmp;
    pthread_mutex_unlock(p->mutex);
    
    return NULL;
}

int main (int argc, char** argv) {

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

    for (int i = 0; i < THREADS; i++)
    {
        if (pthread_create(&tid[i], NULL, thread_func, &param[i]) != 0) {
            perror("Errore nella creazione del thread\n");
            return EXIT_FAILURE;
        }
    }
    for (int i = 0; i < THREADS; i++) 
        pthread_join(tid[i], NULL);

    printf("La somma è: %d\n", somma);

    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}