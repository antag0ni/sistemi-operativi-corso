/*
 * Per ogni numero naturale n si definisce n-mo numero armonico alternato la somma
 * h_n = 1 - 1/2 + 1/3 - 1/4 + ... + (-1)^(n+1) * 1/n. La funzione HarmonicNumber
 * calcola il numero armonico alternato corrispondente al numero naturale n.
 * Il main crea due thread, che eseguono le due funzioni producer_thread e
 * consumer_thread. I due thread condividono un buffer, definito attraverso la
 * struttura TBuffer, che può contenere un numero oppure essere vuoto.
 *
 * Il thread producer ripete le seguenti operazioni:
 *   1. Aspetta che il buffer sia vuoto
 *   2. Legge un numero naturale da standard input inserendolo nel buffer
 *   3. Rende il buffer non vuoto
 *   4. Ricomincia da 1
 *
 * Il thread consumer ripete le seguenti operazioni:
 *   1. Aspetta che il buffer sia non vuoto
 *   2. Calcola e stampa il numero armonico del numero contenuto nel buffer
 *   3. Rende il buffer vuoto
 *   4. Ricomincia da 1
 *
 * L'inserimento di un numero minore o uguale a 0 termina il programma.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
    int n;
    int is_empty;
    pthread_mutex_t mutex;
    pthread_cond_t non_vuoto;
    pthread_cond_t non_pieno;
}TBuffer;


double HarmonicNumber(int n);

void* producer(void* arg);
void* consumer(void* arg);

int main (int argc, char *argv[]) {
    TBuffer buffer;
    buffer.is_empty = 1;
    pthread_mutex_init(&buffer.mutex, NULL);
    pthread_cond_init(&buffer.non_pieno, NULL);
    pthread_cond_init(&buffer.non_vuoto, NULL);

    pthread_t tid[2];

    if (pthread_create(&tid[0], NULL, producer, &buffer) != 0 ||
        pthread_create(&tid[1], NULL, consumer, &buffer) != 0 ) {
            perror("Errore nella creazione di un thread");
            return EXIT_FAILURE;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    pthread_mutex_destroy(&buffer.mutex);
    pthread_cond_destroy(&buffer.non_pieno);
    pthread_cond_destroy(&buffer.non_vuoto);

    return EXIT_SUCCESS;
}

double HarmonicNumber(int n) {
    double h_n = 0.0;
    int sign = 1;
    for (int i = 1; i <= n; i++) {
        h_n += sign * (1.0 / i);
        sign = -sign; // Alterna il segno
    }
    return h_n;
}

void* producer(void* arg) {
    TBuffer *p = (TBuffer *)arg;
    while (1) {
        pthread_mutex_lock(&p->mutex);
        while(p->is_empty == 0)
            pthread_cond_wait(&p->non_pieno, &p->mutex);
        printf("Inserire un numero: ");
        scanf("%d", &p->n);        
        if (p->n <= 0) {
            p->is_empty = 0;
            pthread_cond_signal(&p->non_vuoto);
            pthread_mutex_unlock(&p->mutex);
            return NULL;
        }
        p->is_empty = 0;
        pthread_cond_signal(&p->non_vuoto);
        pthread_mutex_unlock(&p->mutex);
    }
    return NULL;
}

void* consumer(void* arg) {
    TBuffer *p = (TBuffer *)arg;
    while (1) {
        pthread_mutex_lock(&p->mutex);
         while(p->is_empty)
            pthread_cond_wait(&p->non_vuoto, &p->mutex);
        if (p->n <= 0) {
            pthread_mutex_unlock(&p->mutex);
            return NULL;
        }
        printf("h_n: %.2f\n", HarmonicNumber(p->n));
        p->is_empty = 1;
        pthread_cond_signal(&p->non_pieno);
        pthread_mutex_unlock(&p->mutex);
    }
    return NULL;
}