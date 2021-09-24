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

void executecmd(int argc, char* argv[], 
    char* path[], int path_size) {
    char* cmd = argv[0];
  for (int i = 0; i < path_size; i++) {
    // Add 2 for string terminator and slash before cmd.
    char* cmdpath = malloc(strlen(cmd) + strlen(path[i]) + 2);
    if (cmdpath == NULL) {
      perror("executecmd");
      exit(1);
    }
    strcpy(cmdpath, path[i]);
    strcat(cmdpath, "/");
    strcat(cmdpath, cmd);
    if (access(cmdpath, X_OK) == 0) {
      printf("Success: %s\n", cmdpath);
      return;
    } else if (errno == ENOTDIR || errno == ENOENT) {
      // Could not find in path.
    } else {
      // actual error, so exit.
      perror("access");
      exit(1);
    }
  }
  // TODO: Handle if unable to find command.
}

int main(int argc, char* argv[]) {
  // TODO: Handle batch
  char* line = NULL;
  size_t len = 0;
  ssize_t nread;
  char** partsbuffer = NULL;
  // Null terminated list of paths.
  char** path = NULL;
  int path_size = 0;

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

    int nparts = parseline(line, &partsbuffer);

    // Builtins
    if (strcmp(partsbuffer[0], "exit") == 0) {
      exit(0);
    } else if (strcmp(partsbuffer[0], "cd") == 0) {
      chdir(partsbuffer[1]);
    } else if (strcmp(partsbuffer[0], "path") == 0) {
      path_size = nparts - 1;  // Don't count command.
      path = realloc(path, path_size * sizeof(char*)); 
      if (path == NULL) {
        perror("path - realloc");
        exit(1);
      }
      for (int i = 1; i < nparts; i++) {
        // TODO: Should this be a string copy?
        path[i-1] = partsbuffer[i];
      }
    } else {
      executecmd(nparts, partsbuffer, path, path_size);
    }

  }
  // Included for good form, but likely not required.
  free(line);
  free(partsbuffer);
  exit(0);
}

