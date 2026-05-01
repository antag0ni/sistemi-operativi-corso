#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define SIZE 200

typedef struct {
  char str[SIZE];
  int palindrome;
}shared_data;

void string_reverse(char *str) {
  int len = strlen(str);
  for (int i = 0; i < len / 2; i++) {
    char temp =  str[i];
    str[i] = str[len - 1 - i];
    str[len - 1 - i] = temp;
  }
}

int is_palindrome(char *str) {
  char string[SIZE];
  strcpy(string, str);
  string_reverse(string);
  if (strcmp(str, string))
    return 0;
  else
    return 1;
}

int main(int argc, char **argv) {
  
  if (argc != 2) {
    printf("Parametri non validi!");
    return EXIT_FAILURE;
  }

  pid_t pid;
  //creo memoria condivisa
  int segment_id = shmget(IPC_PRIVATE, sizeof(shared_data), 0666);
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
      shared_data *p = (shared_data *)shmat(segment_id, NULL, 0);
      strcpy(p->str, argv[1]);
      //reverse
      string_reverse(p->str);
      pid_t pid2 = fork();
      if (pid2 < 0) {
        return 1;
      } else if (pid2 == 0) {
        int flag = is_palindrome(argv[1]);
        p->palindrome = flag;
        shmdt(p);
        exit(EXIT_SUCCESS);
      } else {
        wait(NULL);
      }
      shmdt(p);
      exit(EXIT_SUCCESS);
  } else {
    wait(NULL);
    shared_data *p = (shared_data *)shmat(segment_id, NULL, 0);
    printf("%s\n", p->str);
    if (p->palindrome == 1) {
      printf("La stringa è palindroma.\n");
    } else {
      printf("La stringa non è palindroma.\n");
    }
    shmdt(p);
  }

  shmctl(segment_id, IPC_RMID, NULL);

  return EXIT_SUCCESS;
}
