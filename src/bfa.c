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

void bfa_state_dealloc(struct bfa_state *state) {
  LLVMDisposeBuilder(state->builder);
  LLVMDisposeModule(state->module);
  LLVMContextDispose(state->context);

  free(state);
}

void bfa_dump_module(struct bfa_state *state) {
  LLVMDumpModule(state->module);
}

// values stuff
void setup_values(struct bfa_state *state, struct bfa_values *values) {
  LLVMTypeRef i32 = LLVMInt32TypeInContext(state->context);
  LLVMTypeRef i32_array = LLVMArrayType(i32, 10000);
  LLVMTypeRef i32_ptr = LLVMPointerType(i32, 0);

  LLVMValueRef zero = LLVMConstInt(i32, 0, 0);

  values->global_array = LLVMAddGlobal(state->module, i32_array, "array");
  values->ptr = LLVMBuildGEP2(
      state->builder,
      i32_ptr,
      values->global_array,
      &zero,
      1,
      "pointer"
  );
}

struct bfa_values *bfa_values(struct bfa_state *state) {
  struct bfa_values *values = malloc(sizeof(bfa_values));
  setup_values(state, values);

  return values;
}

void bfa_values_dealloc(struct bfa_values *values) {
  free(values);
}
