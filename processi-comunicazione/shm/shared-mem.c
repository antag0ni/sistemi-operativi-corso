/*
* Scrivere un programma che crei un un’area di memoria condivisa e
* successivamente crei un processo figlio. Il processo figlio inserisce
* nell’area condivisa un’informazione rappresentata dalla stringa "Hello
* World!" che verrà poi letta dal processo padre.
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define MAX_SIZE 200

int main () {
  pid_t pid;
  //creo memoria condivisa
  int segment_id = shmget(IPC_PRIVATE, MAX_SIZE, 0666);
  if (segment_id < 0) {
    perror("Errore in shmget");
    return EXIT_FAILURE;
  }
  //attacco memoria condivisa
  char *p  = (char *)shmat(segment_id, NULL, 0);
  if (p == (char *)-1) {
    perror("Errore in shmat");
  }

  pid = fork();
  if (pid < 0) {
    perror("Errore nella fork\n");
    return EXIT_FAILURE;
  } 
  else if (pid == 0) {
    strcpy(p, "Hello World!");
    exit(EXIT_SUCCESS);
  }
  else {
    wait(NULL);
    printf("%s\n", p);
  }
  //Dealloco la memoria
  shmdt(p);
  shmctl(segment_id, IPC_RMID, NULL);
  return EXIT_SUCCESS;
} 
