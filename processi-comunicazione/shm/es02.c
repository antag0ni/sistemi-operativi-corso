#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define NUM_FLOAT 100

int child_process (int segment_id) {
  //attacco la memoria
  float *p = (float *)shmat(segment_id, NULL, 0);
  if (p == (float *)-1) {
    perror("Errore nella shmat()\n");
    return EXIT_FAILURE;
  }
  for (int i = 1; i <= NUM_FLOAT; i++) {
    p[i-1] = 1.0/i; 
  }
  shmdt(p);
}

int main () {
  size_t dim = NUM_FLOAT * sizeof(float);
  pid_t pid;
  //creo memoria condivisa
  int segment_id = shmget(IPC_PRIVATE, dim, 0666);
  //controllo della shmget()
  if (segment_id < 0) {
    perror("Errore nella shmget()\n");
    shmctl(segment_id, IPC_RMID, NULL);
    return EXIT_FAILURE;
  }
  pid = fork();
  if (pid < 0) {
    perror("Errore nella fork\n");
    return EXIT_FAILURE;
  } else if(pid == 0) {
    child_process(segment_id);
    exit(EXIT_SUCCESS);
  } else {
    wait(NULL);
    //attacco la memoria
    float *array = (float *)shmat(segment_id, NULL, 0);
    float sum = 0.0;
    for (int i = 0; i < NUM_FLOAT; i++) {
      sum += array[i];
    }
    printf("SUM: %f\n", sum);
    shmdt(array);
  }
  shmctl(segment_id, IPC_RMID, NULL);
  return EXIT_SUCCESS;
} 
