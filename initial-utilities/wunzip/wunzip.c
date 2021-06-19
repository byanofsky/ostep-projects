#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("wunzip: file1 [file2 ...]\n");
    return 1;
  }

  // Each entry is 5 bytes: an int (4 bytes) + char (1 byte)
  void* buffer = malloc(5);
  for (int i = 1; i < argc; i++) {
    FILE* fp = fopen(argv[i], "r");
    if (fp == NULL) {
      perror("wunzip");
      return 1;
    }

    while (fread(buffer, 5, 1, fp) > 0) {
      if (ferror(fp) != 0) {
        perror("wunzip");
        return 1;
      }
      char* cp = buffer+4;
      int* ip = buffer;
      for (int i = 0; i < *ip; i++) {
        printf("%c", *cp);
      }
    }
    fclose(fp);
  }
  free(buffer);

  return 0;
}
