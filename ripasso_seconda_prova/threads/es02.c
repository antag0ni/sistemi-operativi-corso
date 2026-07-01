/*
 * Scrivere in C un programma che legge da tastiera due numeri interi: n e k.
 * * Successivamente il processo crea 3 thread secondari che provvedono a
 * calcolare indipendentemente il fattoriale di n, quello di k e quello di (n-k)
 * mediante una funzione factorial_thread che richiama un'altra funzione
 * per il calcolo vero e proprio del fattoriale.
 * * Una volta terminati i thread secondari, il thread principale stampa i risultati
 * di ognuno e calcola e stampa il coefficiente binomiale (n su k) = n! / (k!(n - k)!).
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define THREADS 3

typedef struct {
    int num;
    unsigned long long fact;
} TParam;

unsigned long long factorial(int n) {
    unsigned long long fact = 1;
    for (int i = 1; i <= n; i++)
        fact = fact * i;
    return fact;
}

void *thread_func(void *arg) {
    TParam *p = (TParam *)arg;
    p->fact = factorial(p->num);
    return NULL;
}

int main() {

    int n, k;
    
    printf("Inserire N: ");
    if(scanf("%d", &n) != 1) return EXIT_FAILURE;
    printf("Inserire K: ");
    if(scanf("%d", &k) != 1) return EXIT_FAILURE;

    pthread_t tid[THREADS];
    TParam param[THREADS];

    param[0].num = n;
    param[1].num = k;
    param[2].num = n-k;

    for (int i = 0; i < THREADS ; i++) {
        if (pthread_create(&tid[i], NULL, thread_func, &param[i]) != 0) {
            perror("[!] Errore nella creazione del thread\n");
            return EXIT_FAILURE;
        } 
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(tid[i], NULL);
    }

    unsigned long long coeff = (param[0].fact) / ((param[1].fact)*(param[2].fact));

    printf("Coefficiente binomiale [n = %d, k = %d]: %llu\n", n, k, coeff);
    return EXIT_SUCCESS;
}