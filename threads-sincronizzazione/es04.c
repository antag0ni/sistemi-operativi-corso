#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREADS 2
#define TIME_SLEEP 1
#define LIMIT 3

typedef struct {
    int conteggio;
    pthread_mutex_t mutex;
    pthread_cond_t limite_raggiunto;
} TBuffer;

TBuffer buffer;

void *incremento_thread(void *arg);
void *attesa_thread(void *arg);

int main () {
    //Inizializzo il buffer
    buffer.conteggio = 0;
    pthread_mutex_init(&buffer.mutex, NULL);
    pthread_cond_init(&buffer.limite_raggiunto, NULL);

    pthread_t tid[THREADS];

    //Creazione dei due thread
    if (pthread_create(&tid[0], NULL, incremento_thread, NULL) != 0) {
        perror("Errore nella creazione del thread produttore\n");
        return EXIT_FAILURE;
    }

    if (pthread_create(&tid[1], NULL, attesa_thread, NULL) != 0) {
        perror("Errore nella creazione del thread consumatore\n");
        return EXIT_FAILURE;
    }

    //Attesa della terminazione
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    //Pulizia
    pthread_mutex_destroy(&buffer.mutex);
    pthread_cond_destroy(&buffer.limite_raggiunto);

    return EXIT_SUCCESS;
}

void *incremento_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&buffer.mutex);
    
        buffer.conteggio++;
        printf("Sono il thread 1, conteggio = %d.\n", buffer.conteggio);

        pthread_cond_signal(&buffer.limite_raggiunto);

        pthread_mutex_unlock(&buffer.mutex);

        sleep(TIME_SLEEP);
    }
    return NULL;
}

void *attesa_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&buffer.mutex);
        
        while (buffer.conteggio < LIMIT) {
            pthread_cond_wait(&buffer.limite_raggiunto, &buffer.mutex);
        }

        buffer.conteggio = 0;
        printf("[*] Sono il thread 2 ho resettato il conteggio a 0.\n");

        pthread_mutex_unlock(&buffer.mutex);
    }
    return NULL;
}
