#include <fkys/fkys.h>

int main(int argc, const char *argv[]) {
  struct fkys_state state;

  fkys_state_new(&state);

  LLVMValueRef tmp = fkys_array_get(&state);
  for (int i = 0; i < 10; i++) {
    fkys_incr(&state, &tmp);
  }
  fkys_set_index(&state, tmp);

  tmp = fkys_array_get(&state);
  fkys_putchar(&state, &tmp);

  fkys_return_zero(&state);

  fkys_dump_to_cli(&state);
  fkys_state_finish(&state);

  return 0;
}
