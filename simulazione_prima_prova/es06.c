/*****************************************************************
Il candidato completi il programma fornito, implementando il main.
Il programma definisce una struttura 'Prodotto' contenente il 
nome del prodotto (stringa) e la sua quantita' (intero).

Il programma riceve una stringa da riga di comando (es. "Maglietta").
Il processo padre crea una SINGOLA pipe e genera DUE processi figli 
(uno dopo l'altro).
- Il processo PADRE chiude i propri descrittori della pipe e 
  termina SENZA usare la wait.
  
- Il PRIMO FIGLIO (Scrittore) genera 3 istanze della struct Prodotto. 
  Per ogni prodotto, imposta il nome concatenando la stringa ricevuta 
  da argv con un numero progressivo (es. "Maglietta_1", "Maglietta_2"...). 
  La quantita' sara' un numero a piacere (es. 10, 20, 30). 
  Invia le 3 struct sulla pipe e termina.
  
- Il SECONDO FIGLIO (Lettore) legge le struct dalla pipe finche' 
  la read non restituisce 0 (ovvero quando lo scrittore chiude 
  la pipe). Per ogni struct ricevuta, ne stampa il contenuto. 

Esempio con parametro "Libro":
Scrittore: Sto inviando i 3 prodotti sulla pipe...
Lettore: Ricevuto prodotto 'Libro_1', quantita': 10
Lettore: Ricevuto prodotto 'Libro_2', quantita': 20
Lettore: Ricevuto prodotto 'Libro_3', quantita': 30
******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N 3

typedef struct {
    char nome[50];
    int quantita;
} Prodotto;

int main(int argc, char *argv[]) {
    
    int fd[2];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <string>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (pipe(fd) < 0) {
        perror("Errore nella pipe\n");
        return EXIT_FAILURE;
    }

    pid_t pid_1 = fork();
    if (pid_1 < 0) {
        perror("Errore nella fork 1\n");
        exit(EXIT_FAILURE);
    } else if (pid_1 == 0) {
        //CHILD 1 
        close(fd[0]);
        int byte_inviati = 0;
        Prodotto * array_prodotti = (Prodotto *)malloc(N * sizeof(Prodotto));
        if (array_prodotti == NULL) {
            perror("Errore nella malloc\n");
            exit(EXIT_FAILURE);
        }
        printf("Scrittore: Sto inviando i %d prodotti sulla pipe...\n", N);
        for (int i = 0; i < N; i++) {
            snprintf(array_prodotti[i].nome, sizeof(array_prodotti[i].nome), "%s_%d", argv[1], i + 1);
            array_prodotti[i].quantita = 10 + i;
            byte_inviati += write(fd[1], &array_prodotti[i], sizeof(Prodotto));
        }
        if (byte_inviati != N * sizeof(Prodotto)) {
            perror("[!] MESSAGGIO INVIATO PARZIALMENTE \n");
        }
        free(array_prodotti);
        close(fd[1]);
        exit(EXIT_SUCCESS);
    }

    pid_t pid_2 = fork();
    if (pid_2 < 0) {
        perror("Errore nella fork 2\n");
        exit(EXIT_FAILURE);
    } else if (pid_2 == 0) {
        //CHILD 2
        close(fd[1]);
        Prodotto curr;
        while (read(fd[0], &curr, sizeof(Prodotto)) > 0) {
            printf("Lettore: Ricevuto prodotto '%s', quantita': %d\n", curr.nome, curr.quantita);
        }
        close(fd[0]);
        exit(EXIT_SUCCESS);
    }

    close(fd[0]);
    close(fd[1]);

    return EXIT_SUCCESS;
}
