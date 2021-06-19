#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE (512)

int main(int argc, char** argv) {
  if (argc == 1) {
    printf("wgrep: searchterm [file ...]\n");
    return 1;
  }

  char* term = argv[1];

  // No files, read from stdin
  if (argc == 2) {
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, stdin) != NULL) {
      if (strstr(buffer, term) != NULL) {
        printf("%s", buffer);
      }
    }
    return 0;
  }

  // File arguments start at pos 2
  for (int i = 2; i < argc; i++) {
    FILE* fp = fopen(argv[i], "r");
    if (fp == NULL) {
      printf("wgrep: cannot open file\n");
      return 1;
    }
    char* line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, fp)) > 0) {
      if (strstr(line, term) != NULL) {
        printf("%s", line);
      }
    }
    fclose(fp);
  }

  return 0;
}
