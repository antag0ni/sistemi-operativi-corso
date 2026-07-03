/*
 * Per ogni numero naturale n si definisce n-mo numero armonico alternato la somma
 * h_n = 1 - 1/2 + 1/3 - 1/4 + ... + (-1)^(n+1) * 1/n. La funzione HarmonicNumber
 * calcola il numero armonico alternato corrispondente al numero naturale n.
 * Il main crea due thread, che eseguono le due funzioni producer_thread e
 * consumer_thread. I due thread condividono un buffer, definito attraverso la
 * struttura TBuffer, che può contenere un numero oppure essere vuoto.
 *
 * Il thread producer ripete le seguenti operazioni:
 *   1. Aspetta che il buffer sia vuoto
 *   2. Legge un numero naturale da standard input inserendolo nel buffer
 *   3. Rende il buffer non vuoto
 *   4. Ricomincia da 1
 *
 * Il thread consumer ripete le seguenti operazioni:
 *   1. Aspetta che il buffer sia non vuoto
 *   2. Calcola e stampa il numero armonico del numero contenuto nel buffer
 *   3. Rende il buffer vuoto
 *   4. Ricomincia da 1
 *
 * L'inserimento di un numero minore o uguale a 0 termina il programma.
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

double HarmonicNumber(int n);

int main (int argc, char *argv[]) {
    return EXIT_SUCCESS;
}

double HarmonicNumber(int n) {
    double h_n = 0.0;
    int sign = 1;
    for (int i = 1; i <= n; i++) {
        h_n += sign * (1.0 / i);
        sign = -sign; // Alterna il segno
    }
    return h_n;
}