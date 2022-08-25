/**
 * other_klvparser.c: C-based KLV to JSON converter
 * @author Kongsberg Geospatial Ltd.
 * @author www.kongsberggeospatial.com
 * @copyright 2022 Kongsberg Geospatial Ltd.
 */

#include <stdio.h>
#include <stdlib.h>

#include "include/Config.h"
#include "libklv/libklv.h"
#include "libklv/list.h"

const size_t BYTES_IN_A_MEGABYTE = 1048576;

int read_data(uint8_t *buffer, FILE *in, size_t *size);

int main(int argc, char **argv) {
  uint8_t *binary = NULL;
  size_t data_size = UINT32_MAX;

  if (argc > 1) {
    // The input file has been passed in the command line.
    // Read the data from it.
    FILE *in_file = fopen(argv[1], "rb");
    if (in_file) {
      fseek(in_file, 0, SEEK_END);
      data_size = ftell(in_file);
      rewind(in_file);
      binary = (uint8_t *)malloc((sizeof(char)) * data_size);
      read_data(binary, in_file, &data_size);
      fclose(in_file);
    } else {
      // Deal with error condition
    }
  } else {
    // No input file has been passed in the command line.
    // Read the data from stdin (std::cin).
    binary = (uint8_t *)malloc((sizeof(uint8_t)) * BYTES_IN_A_MEGABYTE);
    read_data(binary, stdin, &data_size);
  }
  if (binary) {
    klv_ctx_t *context = libklv_init();
    context->buffer = binary;
    context->buffer_size = data_size;
    context->buf_ptr = binary;
    context->buf_end = &binary[data_size];

    libklv_parse_data(context);

    // Free the KLV Data
    libklv_cleanup(context);

    // Free the packet data
    free(binary);
  }

  return EXIT_SUCCESS;
}

int read_data(uint8_t *buffer, FILE *in, size_t *size) {
  uint64_t result = 0;
  size_t bytes_to_read = *size == UINT64_MAX ? BYTES_IN_A_MEGABYTE : *size;
  if (in) {
    for (size_t i = 0; i < bytes_to_read; i++) {
      uint64_t old_result = result;
      result += fread(buffer + i, 1, 1, in);
      if (result == old_result) {
        break;
      }
    }
    if (result < bytes_to_read) {
      fprintf(stderr, "Only able to load %ld of %ld bytes\n", result, *size);
      *size = result;
      return -1;
    } else {
      fprintf(stderr, "Binary data loaded with size %ld\n", *size);
    }
  }

  return 0;
}