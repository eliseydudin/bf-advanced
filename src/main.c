#include <llvm-c/Core.h>

#include "state.h"

int main(int argc, const char *argv[]) {
  struct fkys_state state = fkys_state_new();
  fkys_state_finish(&state);

  return 0;
}
