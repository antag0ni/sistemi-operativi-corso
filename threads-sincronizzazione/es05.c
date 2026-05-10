#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SIZE 100

typedef struct {
    char stringa[MAX_SIZE];
    bool is_empty;
    pthread_mutex_t mutex;
    pthread_cond_t cond_producer;
    pthread_cond_t cond_consumer;
} TBuffer;

void *produttore(void *arg);

TBuffer buffer;

int main() {

    pthread_mutex_init(&buffer.mutex, NULL);
    pthread_cond_init(&buffer.cond_producer, NULL);
    pthread_cond_init(&buffer.cond_consumer, NULL);
    
    buffer.is_empty = true;

    pthread_t tid;

    if (pthread_create(&tid, NULL, produttore, NULL) != 0) {
        perror("Errore nella creazione di un thread.\n");
        return EXIT_FAILURE;
    }

    //pthread_join(tid, NULL);

    while(1) {
        pthread_mutex_lock(&buffer.mutex);
        
        while (buffer.is_empty) {
            pthread_cond_wait(&buffer.cond_consumer, &buffer.mutex);
        }
        
        int len = strlen(buffer.stringa) - 1;

        if (len == 0) {
            pthread_mutex_unlock(&buffer.mutex);
            break;
        }

        printf("[CONSUMATORE] La lunghezza della stringa è: %d\n", len);

        buffer.is_empty = true;

        pthread_cond_signal(&buffer.cond_producer);

        pthread_mutex_unlock(&buffer.mutex);
    }
    return EXIT_SUCCESS;
}

void *produttore(void *arg) {
    while(1) {

        pthread_mutex_lock(&buffer.mutex);

        while (!buffer.is_empty) {
            pthread_cond_wait(&buffer.cond_producer, &buffer.mutex);
        }

        printf("[PRODUTTORE] Inserire stringa (Invio per uscire): ");
        fgets(buffer.stringa, MAX_SIZE, stdin);

        buffer.is_empty = false;

        pthread_cond_signal(&buffer.cond_consumer);

        pthread_mutex_unlock(&buffer.mutex);
    }
    return NULL;
}
