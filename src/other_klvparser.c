#include <stdio.h>
#include <stdlib.h>

#include "include/Config.h"
#include "libklv/libklv.h"
#include "libklv/list.h"

uint8_t *read_data(FILE *in, size_t size);

int main(int argc, char **argv) {
  uint8_t *binary = NULL;
  size_t data_size = 0;

  if (argc > 1) {
    // The input file has been passed in the command line.
    // Read the data from it.
    FILE *in_file = fopen(argv[1], "rb");
    fseek(in_file, 0, SEEK_END);
    data_size = ftell(in_file);
    rewind(in_file);
    if (in_file) {
      binary = read_data(in_file, data_size);
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
    binary = read_data(stdin, data_size);
  }
  if (binary) {
    klv_ctx_t *context = libklv_init();
    context->buffer = binary;
    context->buffer_size = data_size;
    context->buf_ptr = binary;
    context->buf_end = &binary[data_size];

    int result = libklv_parse_data(binary, data_size, context);

    // Free the KLV Data
    libklv_cleanup(context);

    // Free the packet data
    free(binary);

    return result;
  }

  return EXIT_SUCCESS;
}

uint8_t *read_data(FILE *in, size_t size) {
  if (in) {

    uint8_t *buffer = malloc((sizeof(char)) * size);
    for (size_t i = 0; i < size; i++) {
      fread(buffer + i, 1, 1, in);
    }
    fprintf(stderr, "Binary data loaded with size %ld\n", size);
    return buffer;
  }

  return NULL;
}