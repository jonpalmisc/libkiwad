#ifndef KIWAD_KIWAD_H
#define KIWAD_KIWAD_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KW_MAGIC "KIWAD"

#define WE_SUCCESS 0
#define WE_IO_FAILURE 1
#define WE_INVALID_FORMAT 2

// An entry inside a WAD archive.
typedef struct {
  int32_t offset;
  int32_t size;
  int32_t zippedSize;
  char zipped;
  int32_t checksum;
  int32_t nameLength;
  char *name;
} wad_entry;

// A whole WAD archive.
typedef struct {
  FILE *file;
  int32_t version;
  int32_t entryCount;
  wad_entry *entries;
} wad_archive;

int wad_archive_init(wad_archive *ar, char *filename);

#endif
