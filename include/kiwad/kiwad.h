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
#define WE_EMPTY_FILE 3
#define WE_UNIMPLEMENTED 99

#define WS_OK 0
#define WS_EMPTY 1

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
int wad_archive_read(wad_archive *ar, wad_entry *ent, unsigned char **buf);
int wad_archive_stat(wad_archive *ar, wad_entry *ent);
int wad_archive_deinit(wad_archive *ar);

#endif
