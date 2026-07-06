/*
 * Si realizzi un programma in C che crea un thread secondario che
 * funge da produttore, mentre il main funge da consumatore.
 *
 * - Il buffer condiviso consente di memorizzare una sola stringa.
 *
 * - Il produttore legge una parola da tastiera e la inserisce nel
 *   buffer, poi ricomincia da capo.
 *
 * - Il main, in un ciclo infinito, preleva la parola dal buffer e
 *   ne stampa la lunghezza.
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct {
    char * string;
    int is_changed;
    char * exit_str;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} TBuffer;

void * thread_fun(void * arg) {
    TBuffer * buffer = (TBuffer *)arg;
    while(1) {
        pthread_mutex_lock(&buffer->mutex);
        while (buffer->is_changed == 1)
            pthread_cond_wait(&buffer->cond, &buffer->mutex);
        printf("Insert string: ");
        fgets(buffer->string, MAX_SIZE, stdin);
        if (strcmp(buffer->string, buffer->exit_str) == 0) {
            buffer->is_changed = 1;
            pthread_mutex_unlock(&buffer->mutex);
            pthread_cond_signal(&buffer->cond);
            return NULL;
        }
        buffer->is_changed = 1;
        pthread_mutex_unlock(&buffer->mutex);
        pthread_cond_signal(&buffer->cond);
    }
    return NULL;
}

int main () {
    TBuffer buffer;
    pthread_mutex_init(&buffer.mutex, NULL);
    pthread_cond_init(&buffer.cond, NULL);

    buffer.string = (char*)malloc(MAX_SIZE * sizeof(char));
    if(buffer.string == NULL) {
       perror("Errore nella malloc");
       return EXIT_FAILURE; 
    }
    buffer.is_changed = 0;

    buffer.exit_str = (char*)malloc(6 * sizeof(char));
    if (buffer.exit_str == NULL) {
       perror("Errore nella malloc");
       return EXIT_FAILURE; 
    }
    strcpy(buffer.exit_str, "exit\n");

    pthread_t tid;

    if ( pthread_create(&tid, NULL, thread_fun, &buffer) != 0 ) {
        perror("Errore nella creazione di un thread");
        return EXIT_FAILURE;
    }

    int n;

    while(1) {
        pthread_mutex_lock(&buffer.mutex);
        while (buffer.is_changed == 0)
            pthread_cond_wait(&buffer.cond, &buffer.mutex);
        
        if (strcmp(buffer.string, buffer.exit_str) == 0) {
            pthread_mutex_unlock(&buffer.mutex);
            pthread_cond_signal(&buffer.cond);
            break;
        }
        n = strlen(buffer.string) - 1;
        printf("len = %d\n", n);
        buffer.is_changed = 0;
        pthread_mutex_unlock(&buffer.mutex);
        pthread_cond_signal(&buffer.cond);
    }
    
    pthread_join(tid, NULL); 

    pthread_mutex_destroy(&buffer.mutex);
    pthread_cond_destroy(&buffer.cond);
    free(buffer.string);
    free(buffer.exit_str);
    return EXIT_SUCCESS;
}