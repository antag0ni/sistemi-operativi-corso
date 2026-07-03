#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int saldo;
    pthread_mutex_t mutex;
    pthread_cond_t non_vuoto;
    pthread_cond_t non_pieno;
} TSalvadanaio;

void* risparmiatore(void* arg) {
    TSalvadanaio *p = (TSalvadanaio *)arg;
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&p->mutex);

        while(p->saldo >= 100)
            pthread_cond_wait(&p->non_pieno, &p->mutex);

        p->saldo = p->saldo + 10; 
        printf("Risparmiatore: ho aggiunto 10. Saldo attuale: %d\n", p->saldo);
        pthread_cond_signal(&p->non_vuoto);
        pthread_mutex_unlock(&p->mutex);
        sleep(1);
    }
    return NULL;
}

void* spender(void* arg) {
    TSalvadanaio *p = (TSalvadanaio *)arg;
    pthread_mutex_lock(&p->mutex);

    while(p->saldo < 50) {
        pthread_cond_wait(&p->non_vuoto, &p->mutex);
    }

    p->saldo = p->saldo - 50;
    printf("Spender: ho sottratto 50. Saldo attuale: %d\n", p->saldo);

    pthread_cond_signal(&p->non_pieno);
    pthread_mutex_unlock(&p->mutex);

    return NULL;
}

int main() {
    TSalvadanaio salvadanaio;
    salvadanaio.saldo = 0;
    pthread_mutex_init(&salvadanaio.mutex, NULL);
    pthread_cond_init(&salvadanaio.non_vuoto, NULL);
    pthread_cond_init(&salvadanaio.non_pieno, NULL);

    pthread_t tid[3];

    if (pthread_create(&tid[0], NULL, risparmiatore, &salvadanaio) != 0 ||
        pthread_create(&tid[1], NULL, risparmiatore, &salvadanaio) != 0 ||
        pthread_create(&tid[2], NULL, spender, &salvadanaio) != 0) {
            perror("Errore nella creazione di un thread");
            return EXIT_FAILURE;
    }

    for (int i = 0; i < 3; i++) {
        pthread_join(tid[i], NULL);
    }

    pthread_mutex_destroy(&salvadanaio.mutex);
    pthread_cond_destroy(&salvadanaio.non_vuoto);
    pthread_cond_destroy(&salvadanaio.non_pieno);

    return EXIT_SUCCESS;
}