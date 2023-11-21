#include "wav.h"
#include <stdio.h>
#include <string.h>

// Convert 32-bit unsigned little-endian value to big-endian from byte array
static inline uint32_t little2big_u32(uint8_t const* data) {
  return data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
}

// Convert 16-bit unsigned little-endian value to big-endian from byte array
static inline uint16_t little2big_u16(uint8_t const* data) {
  return data[0] | (data[1] << 8);
}

// Copy n bytes from source to destination and terminate the destination with
// null character. Destination must be at least (amount + 1) bytes big to
// account for null character.
static inline void bytes_to_string(uint8_t const* source,
                                   char* destination,
                                   size_t amount) {
  memcpy(destination, source, amount);
  destination[amount] = '\0';
}

// Parse the header of WAV file and return WAVFile structure with header and
// pointer to data
WAVFile WAV_ParseFileData(uint8_t const* data) {
  WAVFile file;
  uint8_t const* data_ptr = data;

  bytes_to_string(data_ptr, file.header.file_id, 4);
  data_ptr += 4;

  file.header.file_size = little2big_u32(data_ptr);
  data_ptr += 4;

  bytes_to_string(data_ptr, file.header.format, 4);
  data_ptr += 4;

  bytes_to_string(data_ptr, file.header.subchunk_id, 4);
  data_ptr += 4;

  file.header.subchunk_size = little2big_u32(data_ptr);
  data_ptr += 4;

  file.header.audio_format = little2big_u16(data_ptr);
  data_ptr += 2;

  file.header.number_of_channels = little2big_u16(data_ptr);
  data_ptr += 2;

  file.header.sample_rate = little2big_u32(data_ptr);
  data_ptr += 4;

  file.header.byte_rate = little2big_u32(data_ptr);
  data_ptr += 4;

  file.header.block_align = little2big_u16(data_ptr);
  data_ptr += 2;

  file.header.bits_per_sample = little2big_u16(data_ptr);
  data_ptr += 2;

  bytes_to_string(data_ptr, file.header.data_id, 4);
  data_ptr += 4;

  file.header.data_size = little2big_u32(data_ptr);
  data_ptr += 4;

  file.data = data_ptr;
  file.data_length = file.header.data_size;

  return file;
}