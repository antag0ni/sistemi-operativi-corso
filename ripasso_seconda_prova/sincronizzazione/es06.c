/*
 * Il programma calcola il numero di cifre necessarie per rappresentare un numero in diverse basi,
 * usando il modello producer-consumer basato su thread con sincronizzazione con mutex.
 * Il thread producer genera numeri casuali tra 0 e 100 e li inserisce in un buffer condiviso.
 * Il thread consumer legge numeri dal buffer e calcola il numero di cifre necessarie per rappresentarli
 * in base 2 (binario), base 8 (ottale), base 10 (decimale) e base 16 (esadecimale).
 * Il thread consumer ripete le seguenti operazioni:
 * 1. Aspetta che il buffer sia non vuoto
 * 2. Verifica se il numero è palindrome e stampa il risultato
 * 3. Rende il buffer vuoto
 * 4. Ricomincia da 1
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

#define MAX_STEPS 10

/* Prototipi delle funzioni */
void *producer_thread(void *);
void *consumer_thread(void *);

/* Definizione del tipo SharedBuffer */
typedef struct {
    int number;
    int full;      // Flag: buffer pieno o vuoto
    int steps;      // Contatore degli step
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} SharedBuffer;

int main() {
    pthread_t prod_thread, cons_thread;
    SharedBuffer sharedBuffer;

    sharedBuffer.full = 0;
    sharedBuffer.steps = 0;
    pthread_mutex_init(&sharedBuffer.mutex, NULL);
    pthread_cond_init(&sharedBuffer.cond, NULL);

    // Crea i thread produttore e consumatore
    pthread_create(&prod_thread, NULL, producer_thread, &sharedBuffer);
    pthread_create(&cons_thread, NULL, consumer_thread, &sharedBuffer);

    // Attende la terminazione dei thread
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // Distrugge mutex e variabile di condizione
    pthread_mutex_destroy(&sharedBuffer.mutex);
    pthread_cond_destroy(&sharedBuffer.cond);

    return 0;
}

void *producer_thread(void *arg) {
    // Inserire qui il codice
    SharedBuffer * s = (SharedBuffer *)arg;
    while (1) {
        pthread_mutex_lock(&s->mutex);

        if (s->steps == MAX_STEPS) {
            pthread_mutex_unlock(&s->mutex);
            pthread_cond_signal(&s->cond);
            return NULL;
        }

        while (s->full == 1)
            pthread_cond_wait(&s->cond, &s->mutex);
        
        s->number = rand() % 100 + 1;
        
        printf("[PRODUCER]: inserito %d\n", s->number);

        s->full = 1;
        s->steps++;

        pthread_mutex_unlock(&s->mutex);
        pthread_cond_signal(&s->cond);
    }
}

void *consumer_thread(void *arg) {
    // Inserire qui il codice
    SharedBuffer * s = (SharedBuffer *)arg;
    while (1) {
        pthread_mutex_lock(&s->mutex);


        while (s->full == 0)
            pthread_cond_wait(&s->cond, &s->mutex);

        int cifre = (int) floor(log(s->number) / log(2)) + 1;
        
        printf("[CONSUMER]: cifre per %d (base 2), %d\n", s->number, cifre);
        //dovrei implementare le altre ma non mi aggiunge nulla allo scopo della preparazione

        s->full = 0;

        if (s->steps == MAX_STEPS) {
            pthread_mutex_unlock(&s->mutex);
            pthread_cond_signal(&s->cond);
            return NULL;
        }

        pthread_mutex_unlock(&s->mutex);
        pthread_cond_signal(&s->cond);
    }
}