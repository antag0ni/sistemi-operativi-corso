/* 
    Scrivere un programma C concorrente che generi un un processo figlio, il
    quale a sua volta generi un ulteriore processo figlio.
        ๏ Quanti processi verranno creati?
        ๏ Disegnare l’albero dei processi. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {
    pid_t pid;
    // tempi di esecuzione
    unsigned int tP = 2;
    unsigned int tC = 5;
    unsigned int tP_2 = 1;
    unsigned int tC_2 = 2;

    printf("Main: ");
    printf("PID=%d; My parent PID=%d\n", getpid(), getppid());

    // create the child process
    pid = fork();
    if (pid < 0) {
        // error occurred
        fprintf(stderr, "Fork Failed\n");
        return 1;
    } else if (pid == 0) {
        // child process
        sleep(tC);
        printf("Child: PID returned=%d ", pid);
        printf("Child PID = %d; My parent PID=%d\n", getpid(), getppid());

        //new child process    
        pid = fork();
        if (pid < 0){ 
            // error occurred
            fprintf(stderr, "Fork Failed\n");
            return 1;
        }else if(pid == 0){
            //child_2 process
            sleep(tC_2);
            printf("Child_2: PID returned=%d ", pid);
            printf("Child_2 PID = %d; My parent PID=%d\n", getpid(), getppid());
        } else {
            //parent_2 process
            wait(NULL);
            sleep(tP_2);
            printf("Father_2: PID returned=%d ", pid);
            printf("Father_2 PID=%d; My parent PID=%d\n", getpid(), getppid());
        }

    } else {
        // parent process
        wait(NULL); //sync
        sleep(tP);
        printf("Father: PID returned=%d ", pid);
        printf("Father PID=%d; My parent PID=%d\n", getpid(), getppid());
    }
   
    return EXIT_SUCCESS; 
}
