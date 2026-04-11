/*****************************************************************
Il candidato completi il programma fornito, implementando il main.
Il programma ottiene un numero intero N dai parametri della linea
di comando, e crea un processo figlio che, utilizzando la funzione
'generate_array', genera un array contenente i primi N numeri di Fibonacci.
Successivamente, il processo figlio estrae i valori dispari dalla
successione di Fibonacci creata e li passa al processo padre tramite
memoria condivisa.
Il processo padre attende la terminazione del processo figlio, e
stampa a video i valori ricevuti dal processo figlio.

Esempio: Specificando sulla linea di comando il parametro 10
l'output del programma sara':

Generated Fibonacci sequence 1 1 2 3 5 8 13 21 34 55
Odd Fibonacci numbers received: 1 1 3 5 13 21 55
------------------------------------------------------------------
The candidate should complete the program provided, implementing
the main.
Il programma obtains an integer N from the command line parameters,
and creates a child process that, using the function 'generate_array',
generates an array containing the first N Fibonacci numbers.
Subsequently, the child process extracts the odd values from the
Fibonacci sequence created and passes them to the parent process via
shared memory.
The parent process waits for the termination of the child process
and prints the values received from the child process.

Example: If you specify the parameter 10 on the command line
the output of the program will be:

Generated Fibonacci sequence 1 1 2 3 5 8 13 21 34 55
Odd Fibonacci numbers received: 1 1 3 5 13 21 55

******************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>

// Computes a Fibonacci number (n starts from 0)
int fibo(int n) {
    if (n <= 1)
        return 1;
    else
        return fibo(n - 1) + fibo(n - 2);
}

// Generates the first k Fibonacci numbers and stores them in an array
void generate_array(int *array, int k) {
    for (int i = 0; i < k; i++)
        array[i] = fibo(i);
}

// Extracts odd numbers from the Fibonacci array
int extract_odd_numbers(int *array, int k, int *odd_array) {
    int count = 0;
    for (int i = 0; i < k; i++) {
        if (array[i] % 2 != 0) {
            odd_array[count++] = array[i];
        }
    }
    return count;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <integer>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);

    if (n <= 0) {
        fprintf(stderr, "Please provide a positive integer.\n");
        return (EXIT_FAILURE); 
    }

    int segment_id = shmget(IPC_PRIVATE, (n + 1) * sizeof(int), S_IRUSR | S_IWUSR);
    if (segment_id < 0) {
        perror("Error in shmget()\n");
        //shmctl(segment_id, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Error in fork()\n");
        shmctl(segment_id, IPC_RMID, NULL);
        return EXIT_FAILURE;
    } else if (pid == 0) {
        // CHILD
        int * array = (int *)malloc( n * sizeof(int) );
        int * odd = (int *)malloc( n * sizeof(int) );

        if (array == NULL || odd == NULL) {
            perror("Error in malloc()\n");
            return EXIT_FAILURE;
        }

        generate_array(array, n);

        printf("Generated Fibonacci sequence: ");
        for (int i = 0; i < n; i++)
            printf("%d ", array[i]);
        printf("\n");

        int n_odd = extract_odd_numbers(array, n, odd);

        int *p = (int *)shmat(segment_id, NULL, 0);
        if (p == (int *)-1) {
            perror("Error in shmat()\n");
            exit(EXIT_FAILURE);
        }
        
        for (int i = 0; i < n_odd; i++)
            p[i + 1] = odd[i];
        p[0] = n_odd;

        shmdt(p);
        free(array);
        free(odd);

        exit(EXIT_SUCCESS);
    } else {
        // FATHER
        wait(NULL);
        
        int *p = (int *)shmat(segment_id, NULL, 0);
        if (p == (int *)-1) {
            perror("Error in shmat() [FATHER PROCESS]\n");
            exit(EXIT_FAILURE);
        }
        
        printf("Odd Fibonacci numbers received: ");
        for (int i = 1; i <= p[0]; i++)
            printf("%d ", p[i]);
        printf("\n");

        shmdt(p);
    }
    shmctl(segment_id, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}
