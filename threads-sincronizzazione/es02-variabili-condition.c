#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define PROD_THREADS 1
#define CONS_THREADS 2

#define BUF_SIZE 4

#define MAIN_SLEEP 60

typedef struct {
    int elem[BUF_SIZE];         /* Elementi del buffer */
    int primo;                  /* Indice del primo elemento */
    int ultimo;                 /* Indice successivo all'ultimo elemento */
    int cont;                   /* Numero di elementi nel buffer */
    pthread_mutex_t mutex;
    pthread_cond_t non_pieno;
    pthread_cond_t non_vuoto;
} TBuffer;

typedef struct {
    TBuffer *buffer;            /* Puntatore al buffer condiviso */
    int id;                     /* Identificativo del produttore/consumatore */
    int ritardo;                /* Durata del ritardo in secondi */
} TParam;

void *produttore(void *arg);
void *consumatore(void *arg);

int main() {
    TBuffer buffer;
    buffer.primo = 0;
    buffer.ultimo = 0;
    buffer.cont = 0;
    
    pthread_mutex_init(&buffer.mutex, NULL);
    pthread_cond_init(&buffer.non_pieno, NULL);
    pthread_cond_init(&buffer.non_vuoto, NULL);

    int i;
    TParam prod_param[PROD_THREADS];
    for (i = 0; i < PROD_THREADS; i++) {
        prod_param[i].id = 1 + i;
        prod_param[i].ritardo = 2;
        prod_param[i].buffer = &buffer;
    }
    
    TParam cons_param[CONS_THREADS];
    for (i = 0; i < CONS_THREADS; i++) {
        cons_param[i].id = 1 + i;
        cons_param[i].ritardo = 5 + i * 3;
        cons_param[i].buffer = &buffer;
    }

    pthread_t prod_tid[PROD_THREADS];
    pthread_t cons_tid[CONS_THREADS];

    for (i = 0; i < PROD_THREADS; i++) {
        if (pthread_create(&prod_tid[i], NULL, produttore, &prod_param[i]) != 0) {
            printf("Errore nella creazione thread\n");
            return 1;
        }
    }
    
    for (i = 0; i < CONS_THREADS; i++) {
        if (pthread_create(&cons_tid[i], NULL, consumatore, &cons_param[i]) != 0) {
            printf("Errore nella creazione thread\n");
            return 1;
        }
    }

    sleep(MAIN_SLEEP);
    return 0;
}

void *produttore(void *arg) {
    TParam *p = (TParam *)arg;
    TBuffer *buf = p->buffer;
    int numero = 0;
    
    while (1) {
        numero++; /* Genera il prossimo numero */
        
        /* Entra nella regione critica */
        pthread_mutex_lock(&buf->mutex);
        
        /* Aspetta che il buffer sia non pieno */
        while (buf->cont == BUF_SIZE) {
            pthread_cond_wait(&buf->non_pieno, &buf->mutex);
        }
            
        buf->elem[buf->ultimo] = numero;
        buf->ultimo = (buf->ultimo + 1) % BUF_SIZE;
        buf->cont++;
        
        pthread_cond_signal(&buf->non_vuoto);
        
        /* Esce dalla regione critica */
        pthread_mutex_unlock(&buf->mutex);
        
        printf("Sono il produttore %d e ho prodotto: %d\n", p->id, numero);
        sleep(p->ritardo);
    }
    
    return NULL;
}

void *consumatore(void *arg) {
    TParam *p = (TParam *)arg;
    TBuffer *buf = p->buffer;
    int numero;
    
    while (1) {
        /* Entra nella regione critica */
        pthread_mutex_lock(&buf->mutex);
        
        /* Aspetta che il buffer sia non vuoto */
        while (buf->cont == 0) {
            pthread_cond_wait(&buf->non_vuoto, &buf->mutex);
        }
            
        numero = buf->elem[buf->primo];
        buf->primo = (buf->primo + 1) % BUF_SIZE;
        buf->cont--;
        
        pthread_cond_signal(&buf->non_pieno);
        
        /* Esce dalla regione critica */
        pthread_mutex_unlock(&buf->mutex);
        
        printf("Sono il consumatore %d e ho consumato: %d\n", p->id, numero);
        sleep(p->ritardo);
    }
    
    return NULL;
}
