#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// Given a line, parses the whitespace separated
// parts and stores in buffer. The return value
// specifies how many parts are parsed.
// Buffer should be freed by caller.
int parseline(char* line, char*** buffer) {
  int count = 0;
  char* part;
  char** parts = NULL;
  // TODO: See strsep man for better way to handle.
  while (1) {
    // TODO: This mutates line. Should line be copied?
    part = strsep(&line, " \n\t");
    // End of line.
    if (part == NULL) break;
    // Empty part.
    if (*part == '\0') continue;
    // TODO: Is there a better ways besides realloc?
    parts = realloc(parts, (count+1) * sizeof(char*));
    if (parts == NULL) {
      perror("parseline - realloc");
      exit(1);
    }
    parts[count] = part;
    count++;
  }
  *buffer = parts;
  return count;
}

int main(int argc, char* argv[]) {
  // TODO: Handle batch
  char* line = NULL;
  size_t len = 0;
  ssize_t nread;
  char** partsbuffer = NULL;

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

    parseline(line, &partsbuffer);

    // If command 'exit', then exit.
    if (strcmp(partsbuffer[0], "exit") == 0) {
      break;
    }
  }

  free(line);
  free(partsbuffer);
  exit(0);
}

