#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  char* line = NULL;
  size_t len = 0;
  ssize_t nread;

  while (1) {
    printf("wish> ");
    nread = getline(&line, &len, stdin);

    if (nread < 0) {
      perror("getline");
      exit(1);
    }
  }

  free(line);
}
