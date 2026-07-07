/*
 * ESERCIZIO 01 - Programmazione C (Pthreads - Bounded Buffer)
 * 
 * Il candidato completi il programma fornito implementando le funzioni `producer_thread` e `consumer_thread`. 
 * 
 * Specifiche:
 * 1. I due thread condividono un buffer circolare, definito dalla struttura `SharedBuffer`, 
 *    capace di contenere fino a 5 interi alla volta.
 * 2. Questa volta, il produttore inserisce un elemento alla volta e il consumatore estrae un elemento alla volta. 
 * 3. Il thread producer deve generare TOTAL_ITEMS (15) numeri casuali e inserirli nel buffer. 
 *    - Se il buffer è pieno (count == BUFFER_SIZE), il produttore deve bloccarsi in attesa.
 *    - Dopo aver inserito un elemento nella posizione `in`, deve aggiornare gli indici, incrementare 
 *      il conteggio e segnalare al consumatore che c'è un nuovo dato.
 * 4. Il thread consumer deve estrarre TOTAL_ITEMS (15) numeri dal buffer.
 *    - Se il buffer è vuoto (count == 0), il consumatore deve bloccarsi in attesa.
 *    - Dopo aver letto l'elemento dalla posizione `out`, deve aggiornare gli indici, decrementare 
 *      il conteggio, stamparne il valore e segnalare al produttore che si è liberato spazio.
 * 5. È richiesta la corretta gestione degli indici circolari (`in` e `out`) utilizzando l'operatore modulo (%).
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define TOTAL_ITEMS 15

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;       // Indice di inserimento
    int out;      // Indice di estrazione
    int count;    // Numero di elementi attualmente nel buffer
    pthread_mutex_t mutex;
    pthread_cond_t cond_full;  // Segnala che c'è spazio (non pieno)
    pthread_cond_t cond_empty; // Segnala che c'è un dato (non vuoto)
} SharedBuffer;

void *producer_thread(void *arg);
void *consumer_thread(void *arg);

int main() {
    pthread_t prod_tid, cons_tid;
    SharedBuffer shared;

    shared.in = 0;
    shared.out = 0;
    shared.count = 0;
    pthread_mutex_init(&shared.mutex, NULL);
    pthread_cond_init(&shared.cond_full, NULL);
    pthread_cond_init(&shared.cond_empty, NULL);

    pthread_create(&prod_tid, NULL, producer_thread, &shared);
    pthread_create(&cons_tid, NULL, consumer_thread, &shared);

    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid, NULL);

    pthread_mutex_destroy(&shared.mutex);
    pthread_cond_destroy(&shared.cond_full);
    pthread_cond_destroy(&shared.cond_empty);

    return 0;
}

void *producer_thread(void *arg) {
    SharedBuffer *s = (SharedBuffer *)arg;
    // INSERIRE QUI IL CODICE DEL PRODUTTORE
    for(int i = 0; i < TOTAL_ITEMS; i++) {
        pthread_mutex_lock(&s->mutex);
        while (s->count == BUFFER_SIZE)
            pthread_cond_wait(&s->cond_empty, &s->mutex);

        s->buffer[s->in] = rand() % 100;
        printf("[PRODUCER] Inserito: %d\n", s->buffer[s->in]);
        s->count++;
        s->in = (s->in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&s->mutex);
        pthread_cond_signal(&s->cond_full);
        sleep(1);
    }
    return NULL;
}

void *consumer_thread(void *arg) {
    SharedBuffer *s = (SharedBuffer *)arg;
    // INSERIRE QUI IL CODICE DEL CONSUMATORE
    for(int i = 0; i < TOTAL_ITEMS; i++) {
        pthread_mutex_lock(&s->mutex);
        while (s->count == 0)
            pthread_cond_wait(&s->cond_full, &s->mutex);

        int elem = s->buffer[s->out];
        printf("[CONSUMER] Elemento estratto: %d\n", elem);
        s->out = (s->out + 1) % BUFFER_SIZE;
        s->count--;

        pthread_mutex_unlock(&s->mutex);
        pthread_cond_signal(&s->cond_empty);
        sleep(3);
    }
    return NULL;
}
