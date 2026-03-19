#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  int fd[2];
  char *messaggio;

  if (argc < 2) {
    printf("Parametro non valido!\n");
    return EXIT_SUCCESS;
  }
  if (pipe(fd) < 0) {
    perror("Errore nella pipe\n");
    return EXIT_FAILURE;
  }
  pid_t pid = fork();
  if (pid < 0) {
    perror("Errore nella nella fork\n");
    return EXIT_FAILURE;
  } else if (pid == 0) {
    close(fd[0]);
    int inviati;
    for (int i = 1; i < argc; i++) {
      inviati = write(fd[1], argv[i], sizeof(argv[i]));
    }
    if (inviati != sizeof(argv)) {
      printf("Errore nell'invio!\n");
      return EXIT_FAILURE;
    }
    close(fd[1]);
    exit(EXIT_SUCCESS);
  } else {
    close(fd[1]);
    if (inviati != sizeof(argv)) {
      printf("Errore nell'invio!\n");
      return EXIT_FAILURE;
    }
    for (int i = 1; i < argc; i++) {
      //printf("%s", argv[i]);
      printf("\n");
    }
  }
  return EXIT_SUCCESS;
}

