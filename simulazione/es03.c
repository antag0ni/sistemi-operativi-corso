/*****************************************************************
Il candidato completi il programma fornito, implementando il main.
Il programma ottiene un numero intero N > 0 dai parametri della linea
di comando, e crea un processo figlio. Il processo figlio genera 
un array contenente i primi N numeri pari (partendo da 2, es. 2, 4, 6...)
e li scrive all'interno di un'area di memoria condivisa.
Il processo padre attende la terminazione del processo figlio,
legge l'array dalla memoria condivisa e lo stampa a video in ordine
inverso (dall'ultimo numero pari generato fino al primo), per poi 
rimuovere la memoria condivisa.

Esempio: Specificando sulla linea di comando il parametro 5
l'output del programma sara':

Figlio: ho generato i 5 numeri pari in memoria condivisa.
Padre: i numeri pari in ordine inverso sono: 10 8 6 4 2 
------------------------------------------------------------------
The candidate should complete the program provided, implementing
the main.
The program obtains an integer N > 0 from the command line parameters,
and creates a child process. The child process generates an array 
containing the first N even numbers (starting from 2, e.g., 2, 4, 6...) 
and writes them into a shared memory area.
The parent process waits for the termination of the child process,
reads the array from the shared memory and prints it to the screen in 
reverse order (from the last generated even number to the first), and 
then removes the shared memory.

Example: If you specify the parameter 5 on the command line
the output of the program will be:

Child: I generated the 5 even numbers in shared memory.
Parent: the even numbers in reverse order are: 10 8 6 4 2 

******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

void * generate_even_array(int * array, int n) {
    int k = 2;
    for (int i = 0; i < n; i++) {
        array[i] = k;
        k = k + 2;
    }
}

void print_reverse(int * array, int k) {
    for (int i = k - 1; i >= 0; i--) {
        printf("%d ", array[i]);
    }
}

int main(int argc, char *argv[]) {

    if (argc != 2 || atoi(argv[1]) <= 0) {
        fprintf(stderr, "Usage: %s N\t(N must be > 0)\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);

    int segment_id = shmget(IPC_PRIVATE, n * sizeof(int), S_IRUSR | S_IWUSR);
    if (segment_id < 0) {
        perror("Errore nella shmget\n");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Errore nella fork\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // FIGLIO
        int * p = shmat(segment_id, NULL, 0);
        if (p == (int *)-1) {
            perror("Errore nella shmat");
            exit(EXIT_FAILURE);
        }

        generate_even_array(p, n);
        printf("FIGLIO: array di %d numeri pari generato.\n", n);

        shmdt(p);
        exit(EXIT_SUCCESS);
    } else {
        // PADRE
        wait(NULL);

        int * p = shmat(segment_id, NULL, 0);
        if (p == (int *)-1) {
            perror("Errore nella shmat");
            exit(EXIT_FAILURE);
        }

        printf("PADRE: ");
        print_reverse(p, n);
        printf("\n");

        shmdt(p);
    }

    shmctl(segment_id, IPC_RMID, NULL);
    
    return EXIT_SUCCESS;
}
