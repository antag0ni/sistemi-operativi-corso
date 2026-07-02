/*
 * Si realizzi un programma in C che implementi il problema
 * produttore/consumatore con i threads sincronizzati mediante mutex e
 * variabili condition.
 *
 * - Produttori e consumatori hanno un proprio numero identificativo,
 *   assegnato in fase di creazione.
 * - Le informazioni scambiate tra produttori e consumatori sono numeri
 *   interi.
 *
 * Il thread principale crea 1 produttore e 2 consumatori.
 *
 * - Il produttore produce numeri progressivi (1, 2, ...), aspettando un
 *   tempo prestabilito tra un numero e l'altro.
 * - Ciascun consumatore preleva i numeri dal buffer, li stampa a video e
 *   aspetta un tempo prestabilito tra un numero e l'altro.
 *
 * Il massimo numero di informazioni da mantenere nel buffer è 4.
 *
 * Il thread principale termina dopo 60 secondi.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define PROD_THREADS 1
#define CONS_THREADS 2

#define BUFFER_SIZE 4

#define SLEEP 60

typedef struct {
    int elem[BUFFER_SIZE];
    int primo;
    int ultimo;
    int cont;
    pthread_mutex_t mutex;
    pthread_cond_t non_pieno;
    pthread_cond_t non_vuoto;
} TBuffer;

typedef struct {
    int id;
    TBuffer *buffer;
    int ritardo;    
} TParam;


void *produttore(void *arg);
void *consumatore(void *arg);

int main(int argc, char *argv[]) {

    TBuffer buffer;
    buffer.primo = 0;
    buffer.ultimo = 0;
    buffer.cont = 0;
    pthread_mutex_init(&buffer.mutex, NULL);
    pthread_cond_init(&buffer.non_pieno, NULL);
    pthread_cond_init(&buffer.non_vuoto, NULL);

    TParam prod_param[PROD_THREADS];
    for (int i = 0; i < PROD_THREADS; i++) {
        prod_param[i].id = 1 + i;
        prod_param[i].ritardo = 2;
        prod_param[i].buffer = &buffer;
    }
    
    TParam cons_param[CONS_THREADS];
    for (int i = 0; i < CONS_THREADS; i++) {
        cons_param[i].id = 1 + i;
        cons_param[i].ritardo = 5+i*3;
        cons_param[i].buffer = &buffer;
    }

    pthread_t prod_tid[PROD_THREADS];
    pthread_t cons_tid[CONS_THREADS];

    for (int i = 0; i < PROD_THREADS; i++) {
        if (pthread_create(&prod_tid[i], NULL, produttore, &prod_param[i])) {
            perror("Errore nella creazione del thread");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < CONS_THREADS; i++) {
        if (pthread_create(&cons_tid[i], NULL, consumatore, &cons_param[i])) {
            perror("Errore nella creazione del thread");
            return EXIT_FAILURE;
        }
    }

    sleep(SLEEP);
    return EXIT_SUCCESS;
}

void *produttore(void *arg) {
    TParam *p = (TParam *)arg;
    TBuffer *buf = p->buffer;

    int num = 0;

    while (1) {
        num++;
        pthread_mutex_lock(&buf->mutex);
        while (buf->cont == BUFFER_SIZE)
            pthread_cond_wait(&buf->non_pieno, &buf->mutex);
        buf->elem[buf->ultimo] = num;
        buf->ultimo = (buf->ultimo+1)%BUFFER_SIZE;       
        buf->cont++;
        pthread_cond_signal(&buf->non_vuoto);
        pthread_mutex_unlock(&buf->mutex);
        printf("[PRODUCER %d] produced: %d\n", p->id, num);
        sleep(p->ritardo);
    }
    return NULL;
}

void *consumatore(void *arg) {
    TParam *p = (TParam *)arg;
    TBuffer *buf = p->buffer;
    int num;
    while (1)
    {
        pthread_mutex_lock(&buf->mutex);
        while (buf->cont == 0)
            pthread_cond_wait(&buf->non_vuoto, &buf->mutex);
        num = buf->elem[buf->primo];
        buf->primo = (buf->primo+1)%BUFFER_SIZE;
        buf->cont--;
        pthread_cond_signal(&buf->non_pieno);
        pthread_mutex_unlock(&buf->mutex);
        printf("[CONSUMER %d] consumed: %d\n", p->id, num);
        sleep(p->ritardo);
    }
    return NULL;
}