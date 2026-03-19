#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/wait.h> 

int main (int argc, char *argv[]) {
    pid_t pid;

    if (argc < 2) {
        printf("Nessun argomento.\n");
        return EXIT_SUCCESS;
    }

    for (int i = 1; i < argc; i++) {
        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Fork Failed\n");
            return EXIT_FAILURE;
        } else if (pid == 0) {
            //PROCESSO FIGLIO
            //usiamo il PID del processo figlio per evitare che 
            //tutti i processi generino lo stesso numero casuale
            srand(getpid());
            sleep(rand() % 11); // rand() % max - min + 2
            printf("PID: %d, Messaggio : %s\n", getpid(), argv[i]);
            exit(EXIT_SUCCESS);
        }
    }
    //Il processo padre aspetta che tutti i processi child vengano terminati
    for (int i = 1; i < argc; i++)
        wait(NULL);
    return EXIT_SUCCESS;
}
