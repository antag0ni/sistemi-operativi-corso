/*
* Il candidato completi il programma fornito, implementando le funzioni producer_thread e
* consumer_thread.
* Il main crea due thread, che eseguono le due funzioni citate.
* I due thread condividono un buffer, definito attraverso la struttura
* SharedBuffer, che può contenere una array di 10 elementi oppure essere vuoto.
* Il buffer è protetto da un mutex e da una variabile di condizione.
*
* Il thread producer ripete le seguenti operazioni per MAX_STEPS volte:
* 1. Aspetta che il buffer sia vuoto
* 2. Genera un array di 10 elementi inserendoli nel buffer
* 3. Rende il buffer non vuoto
* 4. Aspetta 2 secondi
* 4. Ricomincia da 1
*
* Il thread consumer ripete le seguenti operazioni per MAX_STEPS volte:
* 1. Aspetta che il buffer sia non vuoto
* 2. Calcola e stampa la media e la deviazione standard degli elementi dell'array
* 3. Rende il buffer vuoto
* 4. Aspetta 1 secondo
* 4. Ricomincia da 1
*
* I due thread devono gestire in maniera appropriata la sincronizzazione
* nell'accesso al buffer.
*
* SI USI l'istruzione rand() % 100 per generate un numero intero tra 1 e 100.
*
* Esempio:
*
* Producer generated array: 7 49 73 58 30 72 44 78 23 9
* Consumer calculated mean: 44.30, stddev: 24.96
* Producer generated array: 40 65 92 42 87 3 27 29 40 12
* Consumer calculated mean: 43.70, stddev: 28.01
* Producer generated array: 3 69 9 57 60 33 99 78 16 35
* Consumer calculated mean: 45.90, stddev: 30.14
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define MAX_STEPS 3

/* Prototipi delle funzioni */
void *producer_thread(void *);
void *consumer_thread(void *);

/* Definizione del tipo SharedBuffer */
typedef struct {
    int buffer[BUFFER_SIZE];
    int count;
    int steps; // Contatore degli step
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} SharedBuffer;

int main() {
    pthread_t prod_thread, cons_thread;
    SharedBuffer sharedBuffer;

    sharedBuffer.count = 0;
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

void* producer_thread(void* arg) {
    // Inserire qui il codice
    SharedBuffer *buffer = (SharedBuffer *)arg;
    
    for (int i = 0; i < MAX_STEPS; i++) {
        pthread_mutex_lock(&buffer->mutex);
        
        while (buffer->count == 1) 
            pthread_cond_wait(&buffer->cond, &buffer->mutex);
        
        printf("Array: ");
        for (int j = 0; j < BUFFER_SIZE; j++) {
            buffer->buffer[j] = rand() % 100;
            printf("%d ", buffer->buffer[j]);    
        }    
        printf("\n");
        
        buffer->count = 1;
        pthread_mutex_unlock(&buffer->mutex);
        pthread_cond_signal(&buffer->cond);
        
        sleep(2);
    }
    
    return NULL;
}

void* consumer_thread(void* arg) {
    // Inserire qui il codice
    SharedBuffer *buffer = (SharedBuffer *)arg;
    
    for (int i = 0; i < MAX_STEPS; i++) {
        pthread_mutex_lock(&buffer->mutex);

        while (buffer->count == 0) 
            pthread_cond_wait(&buffer->cond, &buffer->mutex);
        
        float sum = 0;
        for (int j = 0; j < BUFFER_SIZE; j++)
            sum += buffer->buffer[j];
        
        float mean = sum / BUFFER_SIZE;
        
        sum = 0;
        for (int j = 0; j < BUFFER_SIZE; j++)
            sum += pow(buffer->buffer[j] - mean, 2);
        
        sum = sum / BUFFER_SIZE;
        float stddev = sqrt(sum);

        printf("calculated mean: %.2f, stddev: %.2f\n", mean, stddev);    
        
        buffer->count = 0;
        pthread_mutex_unlock(&buffer->mutex);
        pthread_cond_signal(&buffer->cond);
        
        sleep(1);
    }
    
    return NULL;
}