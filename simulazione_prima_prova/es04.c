/*****************************************************************
Il candidato completi il programma fornito, implementando il main.
Il programma crea un processo figlio; il processo figlio legge
dalla riga di comando una stringa di testo (una singola parola).
Il figlio trasmette al padre mediante una pipe i caratteri della stringa 
uno ad uno. Dopo aver inviato l'ultimo carattere utile, il figlio invia 
il carattere terminatore '\0' per segnalare la fine della stringa.
Il processo padre legge dalla pipe i caratteri trasmessi dal figlio
e li stampa a video finche' non riceve il carattere '\0'.
Infine, il padre attende la terminazione del figlio e stampa un
messaggio di chiusura.

Esempio: Lanciando il programma con la parola "Esame"
Sono il processo figlio. Trasmetto la parola: Esame
Sono il processo padre. Carattere ricevuto: E
Sono il processo padre. Carattere ricevuto: s
Sono il processo padre. Carattere ricevuto: a
Sono il processo padre. Carattere ricevuto: m
Sono il processo padre. Carattere ricevuto: e
Sono il processo padre. Il figlio ha terminato.
******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int fd[2];
    if (argc != 2) {
        printf("Parametro invalido!\n");
        return EXIT_FAILURE;
    }

    if (pipe(fd) < 0) {
        perror("Errore nella pipe\n");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Errore nella fork\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        close(fd[0]);
        printf("Sono il processo figlio. Trasmetto la parola: %s\n", argv[1]);

        int len = strlen(argv[1]);
        for (int i = 0; i < len + 1; i++)
            write(fd[1], &argv[1][i], sizeof(char));

        close(fd[1]);
        exit(EXIT_SUCCESS);
    } else {
        close(fd[1]);
        char c;
        while (read(fd[0], &c, sizeof(char)) > 0) {
            if (c == '\0')
                break;
            printf("Sono il processo padre. Carattere ricevuto: %c\n", c);
        }
        printf("\n");
        close(fd[0]);
        printf("Sono il processo padre. Il processo figlio ha terminato.\n");
    }

    return EXIT_SUCCESS;
}
