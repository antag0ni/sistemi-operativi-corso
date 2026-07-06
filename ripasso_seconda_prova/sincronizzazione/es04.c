/*
 * Si realizzi un programma in C che crea due thread secondari:
 *
 * - Il primo thread incrementa una variabile intera "conteggio" ogni
 *   3 secondi, stampando il suo valore ad ogni incremento.
 *
 * - Il secondo thread aspetta che la variabile "conteggio" raggiunga
 *   il valore 10; quando ciò accade, la rimette a 0 e stampa un
 *   messaggio; dopodiché ricomincia da capo.
 *
 * - Il thread principale resta in attesa (infinita) che i due thread
 *   secondari terminino.
 *
 * NOTA BENE: questo non è un problema produttore/consumatore; ma
 * anche in questo caso abbiamo:
 *   - l'accesso a una risorsa condivisa (la variabile "conteggio")
 *     da parte dei due thread
 *   - l'attesa di una condizione che dipende dal valore della
 *     risorsa condivisa, da parte del secondo thread
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define THREADS 2
#define SLEEP 3

typedef struct {
    int conteggio;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} TBuffer;

void * thread_incremento(void * arg) {
    TBuffer * buffer = (TBuffer *)arg;
    while (1) {
        pthread_mutex_lock(&buffer->mutex);

        buffer->conteggio++;
        printf("THREAD 1: conteggio = %d\n", buffer->conteggio);

        if (buffer->conteggio == 10)
            pthread_cond_signal(&buffer->cond);

        pthread_mutex_unlock(&buffer->mutex);
        sleep(SLEEP);
    }
    return NULL;
}

void * thread_reset(void * arg) {
    TBuffer * buffer = (TBuffer *)arg;
    while (1) {
        pthread_mutex_lock(&buffer->mutex);

        while (buffer->conteggio < 10) {
            pthread_cond_wait(&buffer->cond, &buffer->mutex);
        }
        buffer->conteggio = 0;
        printf("THREAD 2: conteggio resettato\n");

        pthread_mutex_unlock(&buffer->mutex);
    }
    return NULL;
}

int main (int argc, char *argv[])
{
    TBuffer buffer;
    pthread_mutex_init(&buffer.mutex, NULL);
    pthread_cond_init(&buffer.cond, NULL);

    buffer.conteggio = 0;

    pthread_t tid[THREADS];

    if (pthread_create(&tid[0], NULL, thread_incremento, &buffer) != 0 ||
        pthread_create(&tid[1], NULL, thread_reset, &buffer) != 0 ) {
            perror("Errore nella creazione di un thread");
            return EXIT_FAILURE;
    }
    
    for (int i = 0; i < THREADS; i++)
        pthread_join(tid[i], NULL); 

    pthread_mutex_destroy(&buffer.mutex);
    pthread_cond_destroy(&buffer.cond);
    return EXIT_SUCCESS;
}