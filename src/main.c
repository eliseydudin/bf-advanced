#include <bfa/bfa.h>

int main(int argc, const char *argv[]) {
  struct bfa_state *state = bfa_state();
  struct bfa_values *values = bfa_values(state);
  struct bfa_putchar *put = bfa_putchar(state);

  bfa_values_incr(state, values);
  bfa_values_incr_array(state, values);
  LLVMValueRef tmp = bfa_values_load_ptr(state, values);
  bfa_putchar_call(put, state, tmp);
  bfa_exit(state);
  bfa_dump_module(state);

  bfa_putchar_dealloc(put);
  bfa_values_dealloc(values);
  bfa_state_dealloc(state);

  return 0;
}
