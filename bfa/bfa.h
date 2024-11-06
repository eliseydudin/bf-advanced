#ifndef __BFA_H__
#define __BFA_H__

#include <llvm-c/Core.h>

struct bfa_state {
  LLVMContextRef context;
  LLVMModuleRef module;
  LLVMBuilderRef builder;
};

struct bfa_values {
  LLVMValueRef ptr;
  LLVMValueRef global_array;
};

struct bfa_values *bfa_values(struct bfa_state *state);
void bfa_values_dealloc(struct bfa_values *);
LLVMValueRef
bfa_values_load_ptr(struct bfa_state *state, struct bfa_values *values);

struct bfa_state *bfa_state();
void bfa_state_dealloc(struct bfa_state *);
void bfa_exit(struct bfa_state *);

void bfa_dump_module(struct bfa_state *);

#endif
