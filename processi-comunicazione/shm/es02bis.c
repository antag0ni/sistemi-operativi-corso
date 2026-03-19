#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define NUM_FLOAT 50
#define HALF_SIZE (NUM_FLOAT/2)

int child_process (int segment_id, int index) {
  //attacco la memoria
  float *p = (float *)shmat(segment_id, NULL, 0);
  int start = HALF_SIZE*index;
  int end = start + HALF_SIZE;
  for (int i = start; i < end; i++) {
    p[i] = 1.0/(i+1);
  }
  sleep(10);
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
  //CHILD 1
  pid = fork();
  if (pid < 0) {
    perror("Errore nella fork\n");
    return EXIT_FAILURE;
  } else if(pid == 0) {
    child_process(segment_id, 0);
    exit(EXIT_SUCCESS);
  }
  //CHILD 2
  pid = fork();
  if (pid < 0) {
    perror("Errore nella fork\n");
    return EXIT_FAILURE;
  } else if(pid == 0) {
    child_process(segment_id, 1);
    exit(EXIT_SUCCESS);
  }
  wait(NULL);
  wait(NULL);
  //attacco la memoria
  float *array = (float *)shmat(segment_id, NULL, 0);
  float sum = 0.0;
  for (int i = 0; i < NUM_FLOAT; i++) {
    sum += array[i];
  }
  printf("SUM: %f\n", sum);

  shmdt(array);
  shmctl(segment_id, IPC_RMID, NULL);

  return EXIT_SUCCESS;
} 
