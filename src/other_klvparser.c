#include <stdio.h>
#include <stdlib.h>

#include "include/Config.h"
#include "libklv/libklv.h"
#include "libklv/list.h"

int read_data(uint8_t *buffer, FILE *in, size_t size);

int main(int argc, char **argv) {
  uint8_t *binary = NULL;
  size_t data_size = 0;

  if (argc > 1) {
    // The input file has been passed in the command line.
    // Read the data from it.
    FILE *in_file = fopen(argv[1], "rb");
    if (in_file) {
      fseek(in_file, 0, SEEK_END);
      data_size = ftell(in_file);
      rewind(in_file);
      binary = (uint8_t *)malloc((sizeof(char)) * data_size);
      read_data(binary, in_file, data_size);
      fclose(in_file);
    } else {
      // Deal with error condition
    }
  } else {
    // No input file has been passed in the command line.
    // Read the data from stdin (std::cin).
    fseek(stdin, 0, SEEK_END);
    data_size = ftell(stdin);
    rewind(stdin);
    binary = (uint8_t *)malloc((sizeof(char)) * data_size);
    read_data(binary, stdin, data_size);
  }
  if (binary) {
    klv_ctx_t *context = libklv_init();
    context->buffer = binary;
    context->buffer_size = data_size;
    context->buf_ptr = binary;
    context->buf_end = &binary[data_size];

    int result = libklv_parse_data(context);

    // Free the KLV Data
    libklv_cleanup(context);

    // Free the packet data
    free(binary);

    return result;
  }

  return EXIT_SUCCESS;
}

int read_data(uint8_t *buffer, FILE *in, size_t size) {
  uint64_t result = 0;
  if (in) {
    for (size_t i = 0; i < size; i++) {
      result += fread(buffer + i, 1, 1, in);
    }
    if (result < size) {
      fprintf(stderr, "Only able to load %ld of %ld bytes\n", result, size);
      return -1;
    } else {

      fprintf(stderr, "Binary data loaded with size %ld\n", size);
    }
  }

  return 0;
}