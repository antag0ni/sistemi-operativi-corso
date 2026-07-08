/*
 * Esercizio 3 - Threads
 *
 * > Si realizzi un programma in C che crea 100 thread figli, a cui deve passare come
 *   parametri un intero da 1 a 100 (diverso per ogni thread figlio) e un buffer che può
 *   contenere un singolo numero intero.
 *
 * > Una volta che i thread figli sono stati creati, il main deve ripetere le seguenti
 *   operazioni:
 *   1. Ripeti per 100 volte i passi da 2 a 4.
 *   2. Aspetta che il buffer sia pieno.
 *   3. Preleva il valore contenuto nel buffer e stampalo a video.
 *   4. Rendi il buffer vuoto.
 *
 * > I thread figli, che eseguono la funzione worker_thread, devono svolgere le seguenti
 *   operazioni:
 *   1. Aspetta 3 secondi.
 *   2. Aspetta che il buffer sia vuoto.
 *   3. Inserisci il numero ricevuto come parametro nel buffer.
 *   4. Rendi il buffer pieno.
 *
 * > I thread devono gestire in maniera appropriata la sincronizzazione nell'accesso al
 *   buffer.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 100

typedef struct {
    int elem;
    int is_empty;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} TBuffer;

typedef struct {
    int num;
    int id;
    TBuffer * buffer;
} TParam;

void * worker_thread (void * arg) {
    TParam * p = (TParam *)arg;
    sleep(3);
    pthread_mutex_lock(&p->buffer->mutex);
    while (p->buffer->is_empty == 0)
        pthread_cond_wait(&p->buffer->cond, &p->buffer->mutex);

    p->buffer->elem = p->num; 
    printf("[THREAD %d] inserito elem: %d\n", p->id, p->buffer->elem);

    p->buffer->is_empty = 0;
    pthread_cond_signal(&p->buffer->cond);
    pthread_mutex_unlock(&p->buffer->mutex);
    return NULL;    
}

int main() {
    
    pthread_t tid[NUM_THREADS];
    TParam param[NUM_THREADS];

    TBuffer * buffer = malloc(sizeof(TBuffer));
    if (buffer == NULL) {
        perror("errore malloc");
        return EXIT_FAILURE;
    }

    buffer->elem = 0;
    buffer->is_empty = 1;
    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->cond, NULL);

    for (int i = 0; i < NUM_THREADS; i++)
    {   
        param[i].id = i;
        param[i].num = i + 1;
        param[i].buffer = buffer;
        if(pthread_create(&tid[i], NULL, worker_thread, &param[i]) != 0) {
            perror("errore nella creazione del thread");
            return EXIT_FAILURE;
        }
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_mutex_lock(&buffer->mutex);
        while (buffer->is_empty == 1)
            pthread_cond_wait(&buffer->cond, &buffer->mutex);

        printf("[MAIN]: prelevato num: %d\n", buffer->elem);

        buffer->is_empty = 1;
        pthread_cond_broadcast(&buffer->cond);
        pthread_mutex_unlock(&buffer->mutex);
    }
    
    
    for (int i = 0; i < NUM_THREADS; i++)
        pthread_join(tid[i], NULL);

    pthread_mutex_destroy(&buffer->mutex);
    pthread_cond_destroy(&buffer->cond);

    free(buffer);

    return EXIT_SUCCESS;
}