/*
 * ESERCIZIO 01 - Programmazione C (Pthreads - Sincronizzazione Alternata)
 * 
 * Il candidato completi il programma fornito implementando le funzioni `thread_dispari` e `thread_pari`.
 * 
 * Specifiche:
 * 1. I due thread condividono uno stato definito dalla struttura `SharedState`.
 * 2. La variabile `contatore` parte da 1 e deve arrivare fino a `MAX_COUNT` (20).
 * 3. I thread devono collaborare per stampare i numeri in ordine strettamente crescente.
 * 4. Il `thread_dispari` deve stampare il valore di `contatore` SOLO se è un numero dispari,
 *    per poi incrementarlo di 1 e svegliare l'altro thread. Se è pari, deve bloccarsi in attesa.
 * 5. Il `thread_pari` deve stampare il valore di `contatore` SOLO se è un numero pari,
 *    per poi incrementarlo di 1 e svegliare l'altro thread. Se è dispari, deve bloccarsi in attesa.
 * 6. Entrambi i thread terminano quando il contatore supera `MAX_COUNT`.
 * 
 * Output atteso:
 * [DISPARI] 1
 * [PARI] 2
 * [DISPARI] 3
 * ... e così via fino a 20.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_COUNT 20

typedef struct {
    int contatore;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} SharedState;

void *thread_dispari(void *arg);
void *thread_pari(void *arg);

int main() {
    pthread_t tid_dispari, tid_pari;
    SharedState shared;

    shared.contatore = 1;
    pthread_mutex_init(&shared.mutex, NULL);
    pthread_cond_init(&shared.cond, NULL);

    pthread_create(&tid_dispari, NULL, thread_dispari, &shared);
    pthread_create(&tid_pari, NULL, thread_pari, &shared);

    pthread_join(tid_dispari, NULL);
    pthread_join(tid_pari, NULL);

    pthread_mutex_destroy(&shared.mutex);
    pthread_cond_destroy(&shared.cond);

    printf("Elaborazione completata.\n");
    return 0;
}

void *thread_dispari(void *arg) {
    SharedState *s = (SharedState *)arg;
    // INSERIRE QUI IL CODICE DEL THREAD DISPARI
    while(1) {
        pthread_mutex_lock(&s->mutex);
        
        if (s->contatore >= MAX_COUNT) {
            pthread_mutex_unlock(&s->mutex);
            pthread_cond_signal(&s->cond);
            return NULL;
        }

        while(s->contatore % 2 == 0)
            pthread_cond_wait(&s->cond, &s->mutex);
        
        printf("[DISPARI]: %d\n", s->contatore);
        s->contatore++;
        pthread_mutex_unlock(&s->mutex);
        pthread_cond_signal(&s->cond);
    }
}

void *thread_pari(void *arg) {
    SharedState *s = (SharedState *)arg;
    // INSERIRE QUI IL CODICE DEL THREAD PARI
    while(1) {
        pthread_mutex_lock(&s->mutex);
        
        if (s->contatore >= MAX_COUNT) {
            pthread_mutex_unlock(&s->mutex);
            pthread_cond_signal(&s->cond);
            return NULL;
        }

        while(s->contatore % 2 == 1)
            pthread_cond_wait(&s->cond, &s->mutex);
        
        printf("[PARI]: %d\n", s->contatore);
        s->contatore++;
        pthread_mutex_unlock(&s->mutex);
        pthread_cond_signal(&s->cond);
    }
}