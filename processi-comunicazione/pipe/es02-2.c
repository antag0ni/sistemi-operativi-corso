#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  int fd[2], total_len = 0;

  if (argc < 2) {
    printf("Parametro non valido!\n");
    return EXIT_SUCCESS;
  }
  for (int i = 1; i < argc; i++) {
    total_len += strlen(argv[i]) + 1;
  }
  char *messaggio = (char *)malloc(total_len);
  if (messaggio == NULL) {
    printf("Errore nella malloc!\n");
    return EXIT_FAILURE;
  }
  if (pipe(fd) < 0) {
    printf("Errore nella pipe\n");
    free(messaggio);
    return EXIT_FAILURE;
  }
  pid_t pid = fork();
  if (pid < 0) {
    printf("Errore nella nella fork\n");
    free(messaggio);
    return EXIT_FAILURE;
  } else if (pid == 0) {
      close(fd[0]);
      int byte_inviati = 0;
      for (int i = 1; i < argc; i++) {
        int len_parola = strlen(argv[i]);
        byte_inviati += write(fd[1], argv[i], len_parola);
        if (i < argc - 1) {
          byte_inviati += write(fd[1], " ", 1);
        } else {
          byte_inviati += write(fd[1], "\0", 1);
        }
      }
      if (byte_inviati != total_len) {
        printf("Errore nell'invio!\n");
        free(messaggio);
        return EXIT_FAILURE;
      }
      close(fd[1]);
      free(messaggio);
      exit(EXIT_SUCCESS);
  } else {
      close(fd[1]);
      int byte_ricevuti = read(fd[0], messaggio, total_len);
      if (byte_ricevuti != total_len) {
        printf("Errore nella read!\n");
        return EXIT_FAILURE;
      }
      close(fd[0]);
      printf("%s\n", messaggio);
      wait(NULL);
      free(messaggio);
  }
  return EXIT_SUCCESS;
}

