#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("wzip: file1 [file2 ...]\n");
    return 1;
  }

  // A null terminator means we aren't tracking a character count yet
  char last_char = '\0';
  int count = 0;

  for (int i = 1; i < argc; i++) {
    FILE* fp = fopen(argv[i], "r");
    if (fp == NULL) {
      perror("wzip");
      return 1;
    }

    char* line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, fp)) > 0) {
      for (int i = 0; i < linelen; i++) {
        if (line[i] == last_char) {
          count++;
        } else {
          // Only write out if tracking a character count
          if (last_char != '\0') {
            fwrite(&count, sizeof(count), 1, stdout);
            fwrite(&last_char, sizeof(last_char), 1, stdout);
          }
          count = 1;
          last_char = line[i];
        }
      }
    }
    if (ferror(fp)) {
      perror("wzip");
      fclose(fp);
      return 1; 
    }
    fclose(fp);
  }
  // Write out any remaining character count
  fwrite(&count, sizeof(count), 1, stdout);
  fwrite(&last_char, sizeof(last_char), 1, stdout);
  return 0;
}
