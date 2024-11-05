#include <bfa/bfa.h>

int main(int argc, const char *argv[]) {
  BFA_CLEANUP struct bfa_state *state = bfa_state();
  bfa_dump_module(state);

  return 0;
}
