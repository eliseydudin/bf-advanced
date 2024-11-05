#ifndef __STATE_H__
#define __STATE_H__

#include <fkys/fkys.h>

void fkys_state_new(struct fkys_state *state) {
  state->ctx = LLVMContextCreate();
  state->module = LLVMModuleCreateWithNameInContext("fkys", state->ctx);
  state->builder = LLVMCreateBuilderInContext(state->ctx);

  state->types.i32 = LLVMInt32TypeInContext(state->ctx);
  state->types.main = LLVMFunctionType(state->types.i32, NULL, 0, 0);
  state->types.putchar_type =
      LLVMFunctionType(state->types.i32, &state->types.i32, 1, 0);

  state->constants.zero = LLVMConstInt(state->types.i32, 0, 0);
  state->constants.one = LLVMConstInt(state->types.i32, 1, 0);
  state->constants.putchar =
      LLVMAddFunction(state->module, "putchar", state->types.putchar_type);

  state->main.main_fn =
      LLVMAddFunction(state->module, "main", state->types.main);
  state->main.entry =
      LLVMAppendBasicBlockInContext(state->ctx, state->main.main_fn, "entry");
  LLVMPositionBuilderAtEnd(state->builder, state->main.entry);

  state->values.index =
      LLVMBuildAlloca(state->builder, state->types.i32, "index");
}

void fkys_dump_to_cli(struct fkys_state *state) {
  LLVMDumpModule(state->module);
}

void fkys_state_finish(struct fkys_state *state) {
  LLVMDisposeBuilder(state->builder);
  LLVMDisposeModule(state->module);
  LLVMContextDispose(state->ctx);
}

// operations on values
void fkys_return_zero(struct fkys_state *state) {
  LLVMBuildRet(state->builder, state->constants.zero);
}

LLVMValueRef fkys_get_index(struct fkys_state *state) {
  return LLVMBuildLoad2(
      state->builder,
      state->types.i32,
      state->values.index,
      "tmpindex"
  );
}

void fkys_set_index(struct fkys_state *state, LLVMValueRef val) {
  LLVMBuildStore(state->builder, val, state->values.index);
}

void fkys_incr(struct fkys_state *state, LLVMValueRef *value) {
  *value =
      LLVMBuildAdd(state->builder, *value, state->constants.one, "tmpindex");
}

#endif
