#include <assert.h>
#include <bfa/bfa.h>
#include <stdlib.h>

struct bfa_state *bfa_state() {
  struct bfa_state *state = malloc(sizeof(struct bfa_state));

  assert(state);

  state->context = LLVMContextCreate();
  state->module = LLVMModuleCreateWithNameInContext("bfa", state->context);
  state->builder = LLVMCreateBuilderInContext(state->context);

  assert(state->context);
  assert(state->module);
  assert(state->builder);

  return state;
}

void bfa_state_dealloc(struct bfa_state **state) {
  LLVMDisposeBuilder((*state)->builder);
  LLVMDisposeModule((*state)->module);
  LLVMContextDispose((*state)->context);

  free(*state);
}

void bfa_dump_module(struct bfa_state *state) {
  LLVMDumpModule(state->module);
}
