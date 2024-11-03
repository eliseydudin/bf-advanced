#ifndef __STATE_H__
#define __STATE_H__

#include <llvm-c/Core.h>

struct fkys_state {
  LLVMContextRef ctx;
  LLVMModuleRef module;
  LLVMBuilderRef builder;
};

struct fkys_state fkys_state_new() {
  LLVMContextRef ctx = LLVMContextCreate();
  LLVMModuleRef module = LLVMModuleCreateWithNameInContext("fkys", ctx);
  LLVMBuilderRef builder = LLVMCreateBuilderInContext(ctx);

  struct fkys_state new = {.ctx = ctx, .module = module, .builder = builder};
  return new;
}

void fkys_state_finish(struct fkys_state *state) {
  LLVMDisposeBuilder(state->builder);
  LLVMDisposeModule(state->module);
  LLVMContextDispose(state->ctx);
}

#endif
