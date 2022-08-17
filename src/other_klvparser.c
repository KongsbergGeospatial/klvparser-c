#include <stdio.h>
#include <stdlib.h>

#include "include/Config.h"
#include "libklv/libklv.h"

uint8_t *readData(FILE *in, size_t size);

int main(int argc, char **argv) {
  uint8_t *binary = NULL;
  size_t data_size = 0;
  printf("Hello, world: %d.%d\n", KlvParser_VERSION_MAJOR, KlvParser_VERSION_MINOR);

  if (argc > 1) {
    // The input file has been passed in the command line.
    // Read the data from it.
    FILE *in_file = fopen(argv[1], "rb");
    fseek(in_file, 0, SEEK_END);
    data_size = ftell(in_file);
    rewind(in_file);
    if (in_file) {
      binary = readData(in_file, data_size);
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
    binary = readData(stdin, data_size);
  }
  klv_ctx_t *context = libklv_init();
  context->buffer = binary;
  context->buffer_size = data_size;
  context->buf_ptr = binary;
  context->buf_end = &binary[data_size];

  int result = libklv_parse_data(binary, data_size, context);

  printf("Done!\n");

  libklv_cleanup(context);

  return 0;
}

uint8_t *readData(FILE *in, size_t size) {
  if (in) {

    char *buffer = malloc((sizeof(char)) * size);
    for (size_t i = 0; i < size; i++) {
      fread(buffer + i, 1, 1, in);
    }
    printf("Binary data loaded with size %ld\n", size);
    return buffer;
  }

  return NULL;
}