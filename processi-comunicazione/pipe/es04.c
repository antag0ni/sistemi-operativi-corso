#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 5

int main(int argc, char *argv[]) {
  int fd[2];
  pid_t pid;

  if (pipe(fd) < 0) {
    printf("Errore nella pipe\n");
    return EXIT_FAILURE;
  }

  pid = fork();

  if (pid < 0) {
    printf("Erore nella fork\n");
    return(EXIT_FAILURE);
  } else if (pid == 0) {
    //CHILD
    close(fd[0]);
    
    int array[N];
    size_t byte_inviati = 0;

    for (int i = 0; i < N; i++) {
      printf("Inserire un intero: ");
      scanf("%d", &array[i]);
    }
    
    byte_inviati += write(fd[1], &array[0], sizeof(int));
    byte_inviati += write(fd[1], &array[1], sizeof(int));
    sleep(1);
    for(int i = 2; i < N; i++)
      byte_inviati += write(fd[1], &array[i], sizeof(int));
    
    if (byte_inviati != sizeof(array)) {
      printf("Errore nell'invio\n");
      return(EXIT_FAILURE);
    }

    close(fd[1]);
  } else {
    //PADRE
    close(fd[1]);
    int letti[N];
    size_t byte_ricevuti = 0;
    for(int i = 0; i < N; i++) {
      byte_ricevuti += read(fd[0], &letti[i], sizeof(int));
      printf("Ricevuto [%d]\n", letti[i]);
    }
    if (byte_ricevuti != sizeof(letti)) {
      printf("Errore nella read\n");
      return(EXIT_FAILURE);
    }
    close(fd[0]);
  }

  return EXIT_SUCCESS;
}
