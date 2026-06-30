/*****************************************************************
Il candidato completi il programma fornito, implementando il main.
Il programma definisce una struct 'Esame' contenente una matricola 
(stringa) e un voto (intero).

Il programma ottiene un intero N > 0 da riga di comando e crea un 
processo figlio. 
Il processo figlio deve:
1. Allocare un'area di memoria condivisa sufficientemente grande
   per contenere N struct di tipo 'Esame' (NON un array di interi!).
2. Popolare l'area di memoria condivisa con N esami. La matricola 
   di ogni esame deve essere la stringa "VR" concatenata all'indice 
   corrente (es. "VR0", "VR1", "VR2"... usa la funzione sprintf). 
   Il voto deve essere calcolato come (18 + i).
3. Terminare.

Il processo padre deve:
1. Attendere la terminazione del figlio (qui la wait serve per 
   sincronizzare la memoria condivisa!).
2. Leggere l'array di struct dalla memoria condivisa.
3. Stampare a video la matricola e il voto di tutti gli esami 
   ricevuti.
4. Rimuovere l'area di memoria condivisa e terminare.

Esempio con N = 3:
Padre: L'esame dello studente VR0 ha preso 18
Padre: L'esame dello studente VR1 ha preso 19
Padre: L'esame dello studente VR2 ha preso 20
******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

typedef struct {
    char matricola[20];
    int voto;
} Esame;

int main(int argc, char *argv[]) {

    if (argc != 2 || atoi(argv[1]) <= 0) {
        fprintf(stderr, "Usage: %s N\t(N > 0)\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);

    int segment_id = shmget(IPC_PRIVATE, n * sizeof(Esame), S_IRUSR | S_IWUSR);
    if (segment_id < 0) {
        perror("Errore nella shmget\n");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Errore nella fork\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        //CHILD
        
        Esame * p = (Esame *)shmat(segment_id, NULL, 0);
        if (p == (Esame *)-1) {
            perror("Errore nella shmat\n");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < n; i++) {
            snprintf(p[i].matricola, sizeof(p[i].matricola), "%s%d", "VR", i);
            p[i].voto = 18 + i;
        }
        
        shmdt(p);

        exit(EXIT_SUCCESS);
    } else {
        // FATHER
        wait(NULL);

        Esame * p = (Esame *)shmat(segment_id, NULL, 0);
        if (p == (Esame *)-1) {
            perror("Errore nella shmat\n");
            exit(EXIT_FAILURE);
        }
        
        for (int i = 0; i < n; i++) {
            printf("Padre: L'esame dello studente %s ha preso %d\n", p[i].matricola, p[i].voto);
        }

        shmdt(p);
    }
    
    shmctl(segment_id, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}
