#include <bfa/bfa.h>

int main(int argc, const char *argv[]) {
  struct bfa_state *state = bfa_state();
  struct bfa_values *values = bfa_values(state);
  bfa_dump_module(state);

  bfa_values_dealloc(values);
  bfa_state_dealloc(state);

  return 0;
}
