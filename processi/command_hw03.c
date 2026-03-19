#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {
  pid_t pid;
  if (argc != 2) {
    printf("Invalid argument!\n");
    return EXIT_SUCCESS;
  }
  pid = fork();
  if (pid < 0) {
    fprintf(stderr, "Fork Failded\n");
    return EXIT_FAILURE;
  } else if (pid == 0) {
    //child process
    execlp(argv[1], argv[1], NULL);
    fprintf(stderr, "Comando non trovato\n");
    exit(EXIT_FAILURE);
  } else {
    int status;
    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
      printf("Comando eseguito!\n");
  }
  return EXIT_SUCCESS;
}
