#ifndef __STATE_H__
#define __STATE_H__

#include <llvm-c/Core.h>
#include <stddef.h>

struct fkys_values {
  LLVMValueRef array, index;
};

struct fkys_constants {
  LLVMValueRef zero, one;
};

struct fkys_types {
  LLVMTypeRef i32;
  LLVMTypeRef main;
};

struct fkys_main {
  LLVMBasicBlockRef entry;
  LLVMValueRef main_fn;
};

struct fkys_state {
  LLVMContextRef ctx;
  LLVMModuleRef module;
  LLVMBuilderRef builder;

  struct fkys_values values;
  struct fkys_constants constants;
  struct fkys_types types;
  struct fkys_main main;
};

void fkys_state_new(struct fkys_state *state) {
  state->ctx = LLVMContextCreate();
  state->module = LLVMModuleCreateWithNameInContext("fkys", state->ctx);
  state->builder = LLVMCreateBuilderInContext(state->ctx);

  state->types.i32 = LLVMInt32TypeInContext(state->ctx);
  state->types.main = LLVMFunctionType(state->types.i32, NULL, 0, 0);

  state->constants.zero = LLVMConstInt(state->types.i32, 0, 0);
  state->constants.one = LLVMConstInt(state->types.i32, 1, 0);

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

#endif
