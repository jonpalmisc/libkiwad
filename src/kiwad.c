#include <kiwad/kiwad.h>

// See SPEC.txt for information regarding the KIWAD file format.

int wad_archive_init(wad_archive *ar, char *filename) {
  ar->version = -1;

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
  
    // The nameLength specified in the archive includes the null-terminatior.
    ce->name = malloc(ce->nameLength * sizeof(char));
    fread(ce->name, ce->nameLength, 1, ar->file);
  }

  return WE_SUCCESS;
}
