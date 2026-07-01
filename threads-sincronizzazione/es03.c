#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREADS 2

typedef struct {
    int value;
    int is_empty; // 1 se vuoto, 0 se pieno
    pthread_mutex_t mutex;
    pthread_cond_t cond_producer; // Per far aspettare il produttore
    pthread_cond_t cond_consumer; // Per far aspettare il consumatore
} TBuffer;

TBuffer buffer;

double HarmonicNumber(int n);
void *producer_thread(void *arg);
void *consumer_thread(void *arg);

int main() {
    //Inizializzo il buffer
    buffer.is_empty = 1;
    pthread_mutex_init(&buffer.mutex, NULL);
    pthread_cond_init(&buffer.cond_producer, NULL);
    pthread_cond_init(&buffer.cond_consumer, NULL);

    pthread_t tid[THREADS];

    //Creazione dei due thread
    if (pthread_create(&tid[0], NULL, producer_thread, NULL) != 0) {
        perror("Errore nella creazione del thread produttore\n");
        return EXIT_FAILURE;
    } 

    if (pthread_create(&tid[1], NULL, consumer_thread, NULL) != 0) {
        perror("Errore nella creazione del thread consumatore\n");
        return EXIT_FAILURE;
    }

    //Attesa della terminazione
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    //Pulizia
    pthread_mutex_destroy(&buffer.mutex);
    pthread_cond_destroy(&buffer.cond_producer);
    pthread_cond_destroy(&buffer.cond_consumer);

    printf("PROGRAMMA TERMIATO.\n");

    return EXIT_SUCCESS;
}

void *producer_thread(void *arg) {
    int input_num;
    while (1) {
        //Blocco il mutex
        pthread_mutex_lock(&buffer.mutex);

        // 1.
        while (!buffer.is_empty) {
            pthread_cond_wait(&buffer.cond_producer, &buffer.mutex);
        }

        // 2.
        printf("Inserisci un numero naturale (<= 0 per terminare): ");
        scanf("%d", &input_num);

        buffer.value = input_num;

        // 3.
        buffer.is_empty = 0;

        pthread_cond_signal(&buffer.cond_consumer);
        
        //Rilascia il mutex
        pthread_mutex_unlock(&buffer.mutex);

        if (input_num <= 0) {
            break;
        }
    }
    return NULL;
}

void *consumer_thread(void *arg) {
    while (1) {
        pthread_mutex_lock(&buffer.mutex);

        while (buffer.is_empty) {
            pthread_cond_wait(&buffer.cond_consumer, &buffer.mutex);
        }

        int n = buffer.value;
        if (n <= 0) {
            pthread_mutex_unlock(&buffer.mutex);
            break;
        }

        double h_n = HarmonicNumber(n);

        printf("Numero armonico alternato per %d: %lf\n", n, h_n);

        buffer.is_empty = 1;

        pthread_cond_signal(&buffer.cond_producer);

        pthread_mutex_unlock(&buffer.mutex);
    }
    return NULL;
}

double HarmonicNumber(int n) {
    double h_n = 0.0;
    int sign = 1;
    for (int i = 1; i <= n; i++) {
        h_n += sign * (1.0 / i);
        sign = -sign; //Alterna il segno
    }
    return h_n;
}
