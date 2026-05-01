#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N_STRUCT 50

typedef struct {
  float x;
  float y;
  float z;
} T_cord;

int main(int argc, char *argv[]) {
  int fd[2];
  pid_t pid;
  
  if (pipe(fd) < 0) {
    printf("Errore nella pipe\n");
    return EXIT_FAILURE;
  }

  pid = fork();

  if (pid < 0) {
    printf("Erore nela fork\n");
    return(EXIT_FAILURE);
  } else if (pid == 0) {
    close(fd[1]);
    //CHILD
    T_cord cord;
    int byte_ricevuti;

    while((byte_ricevuti = read(fd[0], &cord, sizeof(cord))) > 0 ) {
      if (byte_ricevuti != sizeof(cord)) {
        printf("Errore nella read\n");
        return(EXIT_FAILURE);
      }

      printf("Figlio riceve: x=%.1f, y=%.1f, z=%.1f\n", cord.x, cord.y, cord.z);
    }

    close(fd[0]);
  } else {
    //PADRE
    close(fd[0]);
    for (int i = 0; i < N_STRUCT; i++) {
      T_cord cord;
      cord.x = (float)i;
      cord.y = (float)(i*2);
      cord.z = (float)(3.0*i) / 2.0f;

      int byte_inviati = write(fd[1], &cord, sizeof(cord));

      if (byte_inviati != sizeof(cord)) {
        printf("Errore nella write\n");
        return(EXIT_FAILURE);
      }

    }
    close(fd[1]);
  }

  return EXIT_SUCCESS;
}
