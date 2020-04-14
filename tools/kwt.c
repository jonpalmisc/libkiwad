#include <stdlib.h>
#include <string.h>

#include <kiwad/kiwad.h>

#define KWT_TASK_LIST "ls"

void printError(char *error) { printf("error: %s.\n", error); }

int main(int argc, char *argv[]) {

  // Check for task and input.
  if (argc < 3) {
    printf("%s\n", "usage: kwt <task> <input>");
    return EXIT_FAILURE;
  }

  wad_archive ar;
  int error = wad_archive_init(&ar, argv[2]);
  if (error != WE_SUCCESS) {
    printf("Couldn't initialize archive struct. (Error %d)\n", error);
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], KWT_TASK_LIST) == 0) {
    printf("%s\n", "Offset   Size     Name");

    for (int i = 0; i < ar.entryCount; i++) {
      wad_entry *ce = &ar.entries[i];
      
      char compressed = ce->zipped != 0 ? '*' : '-';
      printf("%08x %06x %c %s\n",  ce->offset, ce->size, compressed, ce->name);
    }
  } else {
    printError("unknown command");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}