#include <kiwad/kiwad.h>

int wad_archive_init(wad_archive *ar, char *filename) {

  // Attempt to open the archive.
  ar->file = fopen(filename, "rb");
  if (ar->file == NULL) {
    return WE_IO_FAILURE;
  }

  // Read the file magic (first 5 bytes, should be 'KIWAD').
  char *magic = malloc(5 * sizeof(char));
  fread(magic, sizeof(char), 5, ar->file);

  // Verify that we have received a WAD file.
  if (strcmp(KW_MAGIC, magic) != 0) {
    return WE_INVALID_FORMAT;
  }

  fread(&ar->version, sizeof(int32_t), 1, ar->file);
  fread(&ar->entryCount, sizeof(int32_t), 1, ar->file);

  // Skip the spacer byte if the archive is version 2 or greater.
  if (ar->version >= 2) {
    fseek(ar->file, 1, SEEK_CUR);
  }

  // Allocate memory for the entry list.
  ar->entries = malloc(ar->entryCount * sizeof(wad_entry));

  // Build our list of entries.
  for (int32_t i = 0; i < ar->entryCount; i++) {
    wad_entry *ce = &ar->entries[i];

    fread(&ce->offset, sizeof(int32_t), 1, ar->file);
    fread(&ce->size, sizeof(int32_t), 1, ar->file);
    fread(&ce->zippedSize, sizeof(int32_t), 1, ar->file);
    fread(&ce->zipped, sizeof(char), 1, ar->file);
    fread(&ce->checksum, sizeof(int32_t), 1, ar->file);
    fread(&ce->nameLength, sizeof(int32_t), 1, ar->file);

    // The nameLength specified in the archive includes the null-terminator.
    ce->name = malloc(ce->nameLength * sizeof(char));
    fread(ce->name, ce->nameLength, 1, ar->file);
  }

  return WE_SUCCESS;
}

int wad_archive_read(wad_archive *ar, wad_entry *ent, unsigned char **buf) {

  // TODO: Add support for compressed files.
  if (ent->zipped) {
    return WE_UNIMPLEMENTED;
  }

  // Jump to the offset where the file's data begins.
  fseek(ar->file, ent->offset, SEEK_SET);

  // Read the file's contents into a buffer.
  *buf = calloc(ent->size, 1);
  size_t read = fread(*buf, 1, ent->size, ar->file);

  return WE_SUCCESS;
}

int wad_archive_stat(wad_archive *ar, wad_entry *ent) {
  fseek(ar->file, ent->offset, SEEK_SET);

  // Read the first 4 bytes of the file to determine if it is empty.
  int32_t check = 0;
  fread(&check, sizeof(int32_t), 1, ar->file);
  if (check == 0) {
    return WS_EMPTY;
  }

  return WS_OK;
}

// Deinitalize a WAD archive by freeing all entries and the input stream.
int wad_archive_deinit(wad_archive *ar) {
  free(ar->entries);
  fclose(ar->file);

  return WE_SUCCESS;
}