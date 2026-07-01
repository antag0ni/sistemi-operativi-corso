/*****************************************************************
Il candidato completi il programma fornito, implementando
il main.
Il programma crea un processo figlio; il processo figlio legge
da stringa comando un numero intero N >= 0, e trasmette al processo padre
mediante una pipe i valori N, N-1, N-2, N-3, ..., 0 (incluso).
Il processo padre legge dalla pipe i valori trasmessi dal processo figlio
e li stampa, finche' non riceve il valore 0; dopodiche' il processo
padre attende la terminazione del processo figlio e termina.

Esempio:
Sono il processo figlio. Letto 4
Sono il processo padre. Ho ricevuto: 4
Sono il processo padre. Ho ricevuto: 3
Sono il processo padre. Ho ricevuto: 2
Sono il processo padre. Ho ricevuto: 1
Sono il processo padre. Ho ricevuto: 0
Sono il processo padre. Il figlio ha terminato.

The candidate should complete the program provided, implementing
the main.
The program creates a child process; the child process reads
from the command line arguments an integer N >= 0, and transmits the values
N, N-1, N-2, N-3, ..., 0 (inclusive) to the parent process via a pipe.
The father process reads from the pipe the values transmitted by the child process
and prints them, until it receives the value 0; then the father process
process waits for the termination of the child process and terminates.

Example:
I am the child process. Read: 4
I am the father process. I have received: 4
I am the father process. I have received: 3
I am the father process. I have received: 2
I am the father process. I have received: 1
I am the father process. I have received: 0
I am the father process. The son has finished.

******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    int fd[2];
    if (pipe(fd) < 0) {
        perror("Error in pipe()\n");
        return EXIT_FAILURE;
    }

    if (argc != 2 || atoi(argv[1]) < 0) {
        fprintf(stderr, "Usage: %s <int> (>=0)", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);

    pid_t pid = fork();
    if (pid < 0) {
        perror("Error in fork()\n");
        return EXIT_FAILURE;
    } else if (pid == 0) {
        // CHILD
        printf("Sono il processo figlio. LETTO: %d.\n", n);
        close(fd[0]);

        int byte_sended = 0;
        for (int i = n; i >= 0; i--) {
            byte_sended += write(fd[1], &i, sizeof(int));
            sleep(1);
        }

        if (byte_sended != (n + 1) * sizeof(int)) {
            printf("Messaggio inviato parzialmente.\n");
            exit(EXIT_FAILURE);
        }

        close(fd[1]);

        exit(EXIT_SUCCESS);
    } else {
        // FATHER
        close(fd[1]);

        int num;

        while (read(fd[0], &num, sizeof(int)) > 0) {
            printf("Sono il processo padre. RICEVUTO: %d.\n", num);
            if (num == 0)
                break;
        } 
        
        //wait(NULL);

        printf("Sono il processo padre. Il figlio ha terminato.\n");
        close(fd[0]);
    }

    return EXIT_SUCCESS;
}
