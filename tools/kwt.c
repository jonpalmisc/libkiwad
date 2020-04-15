#include <stdlib.h>
#include <string.h>

#include <kiwad/kiwad.h>

#define KWT_OPT_HELP 0
#define KWT_OPT_CSV 1

int match_opt(char *cmd) {
  if (strcmp(cmd, "-c") == 0) {
    return KWT_OPT_CSV;
  }

  return KWT_OPT_HELP;
}

void print_usage() {
  printf("%s\n", "usage: kwt [-c] <input>");
}

int main(int argc, char *argv[]) {

  // Check for task and input.
  if (argc < 3) {
    print_usage();
    return EXIT_FAILURE;
  }

  // Attempt to match the option we have been given.
  int opt = match_opt(argv[1]);
  if (opt == KWT_OPT_HELP) {
    print_usage();
    return EXIT_FAILURE;
  }

  // Now that we know we have a valid option, initialize an archive.
  wad_archive ar;
  int error = wad_archive_init(&ar, argv[2]);
  if (error != WE_SUCCESS) {
    printf("error: couldn't initialize archive struct (E%02d)\n", error);
    return EXIT_FAILURE;
  }

  // Create a CSV summary of an archive.
  if (opt == KWT_OPT_CSV) {
    FILE *csv_out = fopen(strcat(argv[2], ".csv"), "w");

    // Start writing CSV if opening the output file was successful.
    if (csv_out != NULL) {
      fprintf(csv_out, "index,offset,bytes,name\n");

      for (int i = 0; i < ar.entryCount; i++) {
        wad_entry *ce = &ar.entries[i];
        fprintf(csv_out, "%d,%d,%d,%s\n", i, ce->offset, ce->size, ce->name);
      }

      fclose(csv_out);
    } else {
      printf("error: couldn't write CSV output");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}