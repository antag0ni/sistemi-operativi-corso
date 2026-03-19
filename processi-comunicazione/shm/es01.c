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
  //controllo della shmget()
  if (segment_id < 0) {
    perror("Errore nella shmget()\n");
    shmctl(segment_id, IPC_RMID, NULL);
    return EXIT_FAILURE;
  }
  pid = fork();
  //attacco la memoria
  char *p = (char *)shmat(segment_id, NULL, 0);
  if (p == (char *)-1) {
    perror("Errore nella shmat()\n");
    return EXIT_FAILURE;
  }
  if (pid < 0) {
    perror("Errore nella fork\n");
    return EXIT_FAILURE;
  } else if(pid == 0) {
    //leggo da tastiera
    printf("Inserire stringa: ");
    fgets(p, MAX_SIZE, stdin);
    shmdt(p);
  } else {
    wait(NULL);
    printf("%s\n", p);
  }
  //dealloco la memoria
  shmdt(p);
  shmctl(segment_id, IPC_RMID, NULL);
  return EXIT_SUCCESS;
} 
