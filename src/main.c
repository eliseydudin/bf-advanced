#include <bfa/bfa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_file(FILE *input) {
  char *array = malloc(4);
  char ch;
  size_t max_size = 4, size = 0;

  while ((ch = fgetc(input)) && ch != EOF) {
    if (size == max_size) {
      max_size *= 1.6;
      array = realloc(array, max_size);
    }

    array[size++] = ch;
  }

  array[size] = '\0';

  return array;
}

int main(int argc, const char *argv[]) {
  if (argc == 1) {
    fprintf(stderr, "No file specified\n");
    fprintf(stderr, "usage: bfa <file>\n");
    return 1;
  }

  char *filename;
  int mode = 0; // 0 - normal, 1 - emit llvm
  for (int i = 1; i < argc; i++) {
    const char *str = argv[i];

    if (str[0] != '-') {
      filename = (char *) str;
    } else if (!strcmp(str, "-emit-llvm")) {
      mode = 1;
    }
  }

  FILE *input = fopen(filename, "r");
  if (input == NULL) {
    fprintf(stderr, "Given file doesn't exist\n");
    fprintf(stderr, "usage: bfa <file>\n");
    return 1;
  }

  struct bfa_state *state = bfa_state();
  struct bfa_values *values = bfa_values(state);
  struct bfa_putchar *put = bfa_putchar(state);

  char *source = read_file(input);
  bfa_state_interpret(source, state, values, put);
  free(source);

  switch (mode) {
    case 0:
      bfa_state_compile(state);
      break;
    case 1:
      bfa_dump_module(state);
      break;
  }

  bfa_putchar_dealloc(put);
  bfa_values_dealloc(values);
  bfa_state_dealloc(state);

  fclose(input);

  return 0;
}
