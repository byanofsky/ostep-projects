#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char* argv[]) {
  // TODO: Handle batch
  char* line = NULL;
  size_t len = 0;
  ssize_t nread;

  while (1) {
    printf("wish> ");
    nread = getline(&line, &len, stdin);

    if (nread < 0) {
      // TODO: errno is 0 when getline receives EOF.
      // Instead, can this be intercepted?
      if (errno == 0) {
        // Print line break for cleaner UI.
        printf("\n");
        break;
      }
      perror("getline");
      exit(1);
    }

    // TODO: Do we need to define line break?
    if (strcmp(line, "exit\n") == 0) {
      break;
    }
  }

  free(line);
  exit(0);
}
